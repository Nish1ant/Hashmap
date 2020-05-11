/*main.cpp*/

//
// Nishant Chudasama
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #06 Part 2: Hashing DIVVY data
// Date: 04/14/2020 (Late Submission) 
//
// Program to store and analyze data from DIVVY company using the hashmap class
// 

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>

#include "hash.h"
#include "hashmap.h"
#include "util.h"

using namespace std;

//
// stationData
// 
// Struct which contains the information for stations:
// Full name, Latitude, Longitude and the Online Date
// 
struct stationData
{
	int id;
	string abbrev;
	string fullName;
	double latitude;
	double longitude;
	int capacity;
	string date;

	stationData()
	{
		id = 0;
		abbrev = "";
		fullName = "";
		latitude = 0.0;
		longitude = 0.0;
		capacity = 0;
		date = "";
	}
};

//
// tripData
//
// Struct to stor information about trips:
// Contains tripId, starting time, ending time, bike ID, duration (in s),
// ID of starting station, ID of ending station.
// Also contains 2 optional fields for identifying rider: gender and birth year
//
struct tripData
{
	string tripId;
	string startTime;
	string endTime;
	string bikeId;
	int duration;
	int from;
	int to;
	string gender;
	string birthYear;

	tripData()
	{
		tripId = "";
		startTime = "";
		endTime = "";
	    bikeId = "";
		duration = 0;
		from = 0;
		to = 0;
		gender = "";
		birthYear = "";
	}
};

//
// readStationsFile
// 
// given a file name for the stations,
// read the file and input data into hash table
//  
bool readStationsFile(string stationsFile, int &numStations,
			   hashmap<int, stationData>& stationsById, 
			   hashmap<string, int>& stationsByAbbrev)
{
	ifstream  infile(stationsFile);

	cout << "Reading " << stationsFile << endl;

	if (!infile.good())
	{
		cout << "**Error: unable to open '" << stationsFile << "'..." << endl;
		return false;
	}

	// file is open, start processing:
	string line;

	getline(infile, line);  // input and discard first row --- header row

	int stationID;
	string ID, abbrev, fullName, latitude, longitude, capacity, date;
	while (getline(infile, line))
	{
		stringstream s(line);

		//
		// format: id, abbrev, fullname, latitude, longitude, capacity, online_date
		// 
		getline(s, ID, ',');        // serves as the key for stationsById
		getline(s, abbrev, ',');    // key for stationsByAbbrev
		getline(s, fullName, ',');
		getline(s, latitude, ',');
		getline(s, longitude, ',');
		getline(s, capacity, ',');
		getline(s, date, ',');

		stationID = stoi(ID);

		//
		// store into hash table
		// 
		stationData sd;

		sd.id = stationID;
		sd.abbrev = abbrev;
		sd.fullName = fullName;
		sd.latitude = stod(latitude);
		sd.longitude = stod(longitude);
		sd.capacity = stoi(capacity);
		sd.date = date;
		

		if (stationsById.insert(stationID, sd, HashbyID)
			&& stationsByAbbrev.insert(abbrev, stationID, HashbyAbbrev)) {
			numStations++;
		}
			
	}
 
	// If get here, all lines were succesfully read:
	return true;  
}


//
// readTripsFile
// 
// given a file name for the trips,
// read the file and input data into hash table
// 
bool readTripsFile(string tripsFile, int& numTrips, int &numBikes,
	hashmap<string, tripData>& tripsById,
	hashmap<string, int>& bikesById)
{
	ifstream  infile(tripsFile);

	cout << "Reading " << tripsFile << endl;

	if (!infile.good())
	{
		cout << "**Error: unable to open '" << tripsFile << "'..." << endl;
		return false;
	}

	// file is open, start processing:
	string line;

	getline(infile, line);  // input and discard first row --- header row

	string tripID, startTime, endTime, bikeID, duration, from, to, identifies, birthyear;
	while (getline(infile, line))
	{
		stringstream s(line);

		//
		// format: tripID, startTime, endTime, bikeID, time, from, to, gender, year
		// 
		getline(s, tripID, ',');        // serves as key for tripsById
		getline(s, startTime, ',');     
		getline(s, endTime, ',');
		getline(s, bikeID, ',');        // key for bikesById
		getline(s, duration, ',');
		getline(s, from, ',');
		getline(s, to, ',');
		getline(s, identifies, ',');    // optional
		getline(s, birthyear, ',');     // optional


		//
		// create tripData object
		// 
		tripData td;

		td.tripId = tripID;
		td.startTime = startTime;
		td.endTime = endTime;
		td.bikeId = bikeID;
		td.duration = stoi(duration);
		td.from = stoi(from);
		td.to = stoi(to);
		td.gender = identifies;
		td.birthYear = birthyear;

		// Insert trip into hashmap
		if (tripsById.insert(tripID, td, HashbyTripId)) {
			numTrips++;

			// Check if bike data exists; if yes, increment usage (else insert)
			int usage = 1;
			if (!bikesById.search(bikeID, usage, HashbyBikeId)) {
				numBikes++;
				bikesById.insert(bikeID, usage, HashbyBikeId);
			}
			else {
				bikesById.insert(bikeID, ++usage, HashbyBikeId); // overwrite with (usage+1)
			}
		}


	}

	// If get here, all lines were read:
	return true;  
}

//
// inputData
// 
// Given a filename denoting lottery winning numbers,
// inputs that data into the given hash table.
// 
bool inputData(string stationsFile, string tripsFile,
			   int &numStations, int &numTrips, int &numBikes,
			   hashmap<int, stationData> &stationsById, 
			   hashmap<string, int> &stationsByAbbrev,
			   hashmap<string, tripData> &tripsById,
			   hashmap<string, int> &bikesById)
{
	// Return true when both files have been succesfully read:
	if (readStationsFile(stationsFile, numStations, stationsById, stationsByAbbrev)
		&& readTripsFile(tripsFile, numTrips, numBikes, tripsById, bikesById)) {
		return true;
	}

	return false;
}

//
// Function to display all commands
//
void displayHelpMenu()
{
	cout << "Available commands:" << endl;
	cout << " Enter a station id (e.g. 341)" << endl;
	cout << " Enter a station abbreviation (e.g. Adler)" << endl;
	cout << " Enter a trip id (e.g. Tr10426561)" << endl;
	cout << " Enter a bike id (e.g. B5218)" << endl;
	cout << " Nearby stations (e.g. nearby 41.86 -87.62 0.5)" << endl;
	cout << " Similar trips (e.g. similar Tr10424639 0.3)" << endl;
}

//
// Function to display details for a given station
//
void printStationData(stationData& sd)
{
	cout << "Station:" << endl;
	cout << " ID: " << sd.id << endl;
	cout << " Abbrev: " << sd.abbrev << endl;
	cout << " Fullname: " << sd.fullName << endl;
	cout << " Location: (" << sd.latitude << ", " << sd.longitude << ")" << endl;
	cout << " Capacity: " << sd.capacity << endl;
	cout << " Online date: " << sd.date << endl;
}

//
// Function to convert duration to hrs,min,sec format
//
void displayDuration(int duration)
{
	int numHours = duration / 3600;
	int numMinutes = duration / 60;
	int numSeconds = duration - (numHours * 3600) - (numMinutes * 60);

	// display the duration:
	cout << " Duration: ";

	if (numHours > 0)
		cout << numHours << " hours, ";

	if (numMinutes > 0)
		cout << numMinutes << " minutes, ";

	cout << numSeconds << " seconds" << endl;
}

//
// display information about a given trip
//
void printTripData(tripData& td, hashmap<int, stationData> &stationsById)
{
	// Get station names for the fromStation and toStation:
	string fromStation, toStation;
	stationData sd;

	stationsById.search(td.from, sd, HashbyID);
	fromStation = sd.abbrev;

	stationsById.search(td.to, sd, HashbyID);
	toStation = sd.abbrev;

	cout << "Trip:" << endl;
	cout << " ID: " << td.tripId << endl;
	cout << " Starttime: " << td.startTime << endl;
	cout << " Bikeid: " << td.bikeId << endl;
	displayDuration(td.duration);
	cout << " From station: " << fromStation << " (" << td.from << ")" << endl;
	cout << " To station: " << toStation << " (" << td.to << ")" << endl;
	cout << " Rider identifies as: " << td.gender << endl;
	cout << " Birthyear: " << td.birthYear << endl;
}

// 
// display information about a given bike
//
void displayBikeData(string bikeId, int usage)
{
	cout << "Bike:" << endl;
	cout << " ID: " << bikeId << endl;
	cout << " Usage: " << usage << endl;
}

//
// displayNearbyStations:
//
// Find stations that are at a distance <= maxDist from the given coordinates
// Uses the distBetween2Points function (written by Prof. Hummel)
//
void displayNearbyStations(hashmap<int, stationData> &stationsById,
						   string latitude,string longitude, string distance)
{
	vector<int> keys = stationsById.getKeys();  // Vector to store keys 
	vector<pair<int, double>> stationDistances; // Set of (ID, distance) pairs

	stationData val;
	double lat1 = stod(latitude);
	double long1 = stod(longitude);
	double maxDist = stod(distance);
	double lat2, long2;
	double dist;

	// Loop through the keys vector, calculating distances:
	for (auto& key : keys) {
		stationsById.search(key, val, HashbyID);
		lat2 = val.latitude;
		long2 = val.longitude;

		dist = distBetween2Points(lat1, long1, lat2, long2);

		if (dist < maxDist) {
			stationDistances.push_back(make_pair(val.id, dist));
			
		}
	}

	// Display stations:
	cout << "Stations within " << distance << " miles "
		 << "of (" << latitude << ", " << longitude << ")" << endl;

	if (stationDistances.empty()) {
		cout << " none found" << endl;
		return;
	}

	// Sort vector:
	sort(stationDistances.begin(), stationDistances.end(), Ordering());

	for (auto& elem : stationDistances) {
		cout << " station " << elem.first << ": " << elem.second << " miles" << endl;
	}

}

//
// Function which returns true when a given station is within range
// of a given point
//
bool stationWithinRange(double distance, stationData& sd,
						double latitude, double longitude)
{
	if (distBetween2Points(latitude, longitude, sd.latitude, sd.longitude) <= distance) {
		return true;  // Station in range
	}

	// Distance exceeded the max:
	return false;
}

// Function to print the elements of the nearbyStation sets
void printStationIds(set<int>& stationIds)
{
	size_t i = 0;
	auto itr = stationIds.begin();
	while ( i < stationIds.size() - 1) {
		cout <<  *itr << " ";
		++i;
		++itr;
	}

	cout << *itr << endl;
}

//
// getSimilarTrips:
//
// Function to display trips that are 'similar' to a given trip,
// i.e. the start and end stations are within a given range from the origin and
// destination of te trip.
// Also counts the actual # of trips that fall within the same bounds
//
// Complexity: O(S + T), where S = # of stations; T = # of trips
//
void getSimilarTrips(string tripId, string distance,
					 hashmap<string, tripData> &tripsById,
					 hashmap<int, stationData> &stationsById)
{
	cout << "Trips that follow a similar path "
		<< "(+/-" << distance << " miles) as " << tripId << endl;

	// Check if trip exists:
	tripData currentTrip;
	if (!tripsById.search(tripId, currentTrip, HashbyTripId)) {
		cout << " no such trip" << endl;
		return;
	}

	// Get all trips and stations 
	vector<string> trips = tripsById.getKeys();
	vector<int> stations = stationsById.getKeys();

	double maxDist = stod(distance);

	int fromId = currentTrip.from;
	int toId = currentTrip.to;
	stationData stationInfo;

	// Get the coordinates for S (start)
	stationsById.search(fromId, stationInfo, HashbyID);
	double Slat = stationInfo.latitude;
	double Slong = stationInfo.longitude;

	// Get coordinates for D (destination)
	stationsById.search(toId, stationInfo, HashbyID);
	double Dlat = stationInfo.latitude;
	double Dlong = stationInfo.longitude;


	// get stations near S and D:
	set<int> nearbyStart;       // Set to store station Ids which are nearby S (S')
	set<int> nearbyDestination; // Set to store station Ids which are nearby D (D')
	for (auto& stationId : stations)
	{
		stationsById.search(stationId, stationInfo, HashbyID);

		// Current station is near S:
		if (stationWithinRange(maxDist, stationInfo, Slat, Slong)) {
			nearbyStart.insert(stationId);
		}

		// Current station is near D:
		if (stationWithinRange(maxDist, stationInfo, Dlat, Dlong)) {
			nearbyDestination.insert(stationId);
		}

	}

	// Count the number of similar trips:
	int numTrips = 0;
	int nearS, nearD;
	for (auto& trip : trips)
	{
		// get the fromId and toId for the station
		tripsById.search(trip, currentTrip, HashbyTripId);  
		fromId = currentTrip.from;
		toId = currentTrip.to;

		// For the trip to count, "from" should be near S (in S')
		// and "to" should be near D (in D')
		nearS = nearbyStart.count(fromId);
		nearD = nearbyDestination.count(toId);
		if ((nearS > 0) && (nearD > 0)) {
			numTrips++;
		}
	}

	cout << " nearby starting points: ";
	printStationIds(nearbyStart);
	cout << " nearby ending points: ";
	printStationIds(nearbyDestination);
	cout << " trip count: " << numTrips << endl;
}

//
// Parse the input to get the latitude, longitude and radius for "nearby"
//
void parseNearby(string& command, string& latitude, string &longitude, string &radius)
{
	stringstream s(command);
	string nearby;

	// Parse input to get "nearby", latitude and longitude:
	getline(s, nearby, ' ');    // get "nearby"; do nothing
	getline(s, latitude, ' ');
	getline(s, longitude, ' ');
	getline(s, radius, ' ');
}

//
// Parse the input to get the tripId, and radius for "similar"
//
void parseSimilar(string& command, string &tripId, string &distance)
{
	stringstream s(command);
	string similar;
	
	// Parse input to get "similar", tripId and distance:
	getline(s, similar, ' ');    // get "similar"; do nothing
	getline(s, tripId, ' ');
	getline(s, distance, ' ');
}

// Function to get the user input for the command
void getInput(string& command)
{
	cout << endl;
	cout << "Please enter a command, help, or #> ";
	cin.sync();
	getline(cin, command);
}

int main()
{
	cout << "** DIVVY analysis program **" << endl;
	cout << endl;

	//
	// Create hashmaps:
	// 
	hashmap<int, stationData> stationsById(10000);     // Store (ID, stationData)
	hashmap<string, int> stationsByAbbrev(10000);      // Store (Abbrev, ID)
	hashmap<string, tripData> tripsById(2500000);      // Store (tripId, tripData)
	hashmap<string, int> bikesById(50000);             // Store (bikeId, #uses)
	//
	// get the file names for reading data:
	// 
	string stationsFile = "stations.csv";
	string tripsFile = "trips.csv";
	string filename;

	// get station file:
	cout << "Enter stations file> ";
	getline(cin, filename);

	if (filename != "")
		stationsFile = filename;

	// get trips file:
	cout << "Enter trips file> ";
	cin.sync();
	getline(cin, filename);

	if (filename != "")
		tripsFile = filename;

	cout << endl;

	// read in the files:
	int numStations = 0;
	int numTrips = 0;
	int numBikes = 0;

	bool success = inputData(stationsFile, tripsFile, numStations, numTrips, numBikes,
							 stationsById, stationsByAbbrev, tripsById, bikesById);

	//
	// did we input anything?
	// 
	if (!success)
	{
		cout << "No data, file not found?" << endl;
		return 0;
	}

	cout << endl;
	cout << "# of stations: " << numStations << endl;
	cout << "# of trips: " << numTrips << endl;
	cout << "# of bikes: " << numBikes << endl;


	/* ---- Menu for the program ---- */
	string command;
	getInput(command);
	
	stationData sd;
	tripData td;
	int val;
	
	while (command != "#")
	{
		// Display help menu:
		if (command == "help")
		{
			displayHelpMenu();
		}

		// Look up station by ID; display if found
		else if (isNumeric(command))
		{
			int id = stoi(command);

			if (stationsById.search(id, sd, HashbyID)) {
				printStationData(sd);
			}

			else {
				cout << "Station not found" << endl;
			}
			
		}

		// Display data for trip:
		else if (command.substr(0,2) == "Tr") 
		{
			if (tripsById.search(command, td, HashbyTripId)) {
				printTripData(td, stationsById);
			}

			else
				cout << "Trip not found" << endl;
		}

		// Display data for bike:
		else if (command[0] == 'B')
		{
			if (bikesById.search(command, val, HashbyBikeId)) {
				displayBikeData(command, val);
			}

			else
				cout << "Bike not found" << endl;
		}

		// Display station info if found when searching by Abbrev: 
		// Hash by abbreviation fist and then by the station ID
		else if (stationsByAbbrev.search(command, val, HashbyAbbrev))
		{
			stationsById.search(val, sd, HashbyID);
			printStationData(sd);
		}

		// Display nearby stations:
		else if (command.find("nearby") != string::npos) {
			stringstream s(command);
			string latitude, longitude, radius;

			// Parse input to get "nearby", latitude and longitude:
			parseNearby(command, latitude, longitude, radius);
			displayNearbyStations(stationsById, latitude, longitude, radius);
		}

		// Find stations that are near S(start point) and D(destination) 
		// and generate sets S' and D' respectively.
		// Also count how many trips (starting in S' and ending in D')
		// fall within the given distance
		else if (command.find("similar") != string::npos)
		{
			string similar, tripId, distance;

			// Parse input to get tripId and distance:
			parseSimilar(command, tripId, distance);
			getSimilarTrips(tripId, distance, tripsById, stationsById);
		}

		// Assume everything else is a station abbrev which was not found
		else {
			cout << "Station not found" << endl;
		}

		// get next input:
		getInput(command);
	}
	
	//
	// done!
	//
	return 0;
}
