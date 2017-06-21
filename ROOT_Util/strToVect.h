//
// strToVect.h
// ROOT_Util/
//
// Created by Johann Gan on 6/13/17
//
// Template function. Converts a string containing delimited (by a char) values into a vector.
// Default delimiting character is ','

/////////////////////////////////////////////////

#ifndef STRTOVECT_H_
#define STRTOVECT_H_

/////////////////////////////////////////////////

#include <vector>
#include <string>
#include <sstream>

#include <boost/lexical_cast.hpp>

/////////////////////////////////////////////////

using namespace std;


template <class T>
vector<T> strToVect(const string& s, char delim = ',')
{
	vector<T> v;
	stringstream ss(s);
	string ele;

	while(getline(ss, ele, ','))
	{
		v.push_back(boost::lexical_cast<T>(ele));
	}

	return v;
}


#endif /* STRTOVECT_H_ */
