/*util.h*/

//
// Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #06 Part 2: Hashing DIVVY data
// 
// Contains prototype for distBetween2Points and functor definition for sort
// 

#include <iostream>
#include <cmath>

using namespace std;

double distBetween2Points(double lat1, double long1, double lat2, double long2);

class Ordering
{
public:
	// Order in ascending order of distances for the stationDistances vector
	// When distances are same, order in ascending order of 
	bool operator() (const pair<int, double>& p1, const pair<int, double>& p2)
	{
		if (p1.second < p2.second)
			return true;

		else if (p1.second == p2.second) {
			return p1.first < p2.first;
		}

		else
			return false;
	}
};
