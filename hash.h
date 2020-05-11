/*hash.h*/

//
// Nishant Chudasama
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Prototypes for the hash functions
// 

#include <string>
#include <regex>   /*regular expressions*/

using namespace std;

bool isNumeric(string s);
int HashbyID(int stationID, int N);
int HashbyAbbrev(string abbrev, int N);
int HashbyTripId(string tripId, int N);
int HashbyBikeId(string bikeId, int N);