/*hash.cpp*/

//
// Nishant Chudasama
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 6 Part2: Hashing DIVVY data
// Definitions for the hash functions
// 

#include "hash.h"

using namespace std;

//
// isNumeric (from Prof. Hummel's Solution for Lab 11)
//
// Returns true if the given string is numeric (all digits), false
// if not.  If the string is empty, false is returned since there 
// are no digits present.
//
bool isNumeric(string s)
{
	//
	// A string is numeric if it contains 1 or more digits, so let's
	// use a regular expression to check that.
	//
	// we are using POSIX basic notation for regular expressions, see
	//   https://en.cppreference.com/w/cpp/regex/ecmascript
	// where [:d:] means digit, [[:d:]] means match a single digit.
	// The + means match the preceding sub-expression 1 or more times.
	//
	regex pattern("[[:d:]]+", regex::ECMAScript);

	smatch matchResults;  // provides more details on the match

	if (regex_match(s, matchResults, pattern))
		return true;
	else
		return false;
}

//
// HashbyID 
//
// Hashes by the station ID to return an index from 0....N-1
// Uses the mid-square hashing algorithm
//
int HashbyID(int stationID, int N)
{
	return stationID % N;
}

//
// HashbyAbbrev
//
// Hashes by the station abbreviation to return an index from 0....N-1
// Uses the Dan Bernstein multiplicative string algorithm
//
int HashbyAbbrev(string abbrev, int N)
{
	unsigned long long hash = 5381;

	for (char& c : abbrev) {
		hash = ((hash << 5) + hash) + c;  // << 5 + hash=> * 33
	}

	return hash % N;
}

//
// HashbyTripId
//
// Function to return an index from 0...N-1 based on the tripId
// Valid tripIds must start with "Tr" followed by digits
// 
int HashbyTripId(string tripId, int N)
{
	if (tripId.substr(0, 2) != "Tr") {
		return -1;
	}

	// Remove "Tr"
	tripId.erase(0, 2);

	// Rest of the string must consist of all digits
	if (!isNumeric(tripId)) {
		return -1;
	}

	// simple % function is z
	int index = stoi(tripId);

	return index % N;
}

//
// HashbyBikeId
//
// Function to return an index from 0...N-1 based on the bikeId
// Valid bikeIds must start with "B" followed by digits
//
int HashbyBikeId(string bikeId, int N)
{
	if (bikeId[0] != 'B') {
		return -1;
	}

	// Remove "B"
	bikeId.erase(0, 1);

	// Rest of the string must consist entirely of digits
	if (!isNumeric(bikeId)) {
		return -1;
	}

	int index = stoi(bikeId);
	return index % N;
}
