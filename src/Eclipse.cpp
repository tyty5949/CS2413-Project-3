/**
 * The Eclipse class which encapsulates the data of an
 * individual eclipse.
 *
 * Class: CS2414
 * Author: Tyler Hunt (113401590)
 * Date: 9/29/17
 * Assignment: Lab #4
 */
#include <iostream>
#include "Eclipse.h"

using namespace std;

/**
 * Default constructor to initialize eclipse with no data.
 */
Eclipse::Eclipse() {
	_numCols = 0;
	_dataString = "NaN";
}

/**
 * Copy constructor.
 *
 * @param eclipse -
 * 			The eclipse to copy.
 */
Eclipse::Eclipse(const Eclipse& eclipse) {
	_numCols = eclipse._numCols;
	_dataString = eclipse._dataString;
	for (int i = 0; i < _numCols; i++) {
		_cols[i] = eclipse._cols[i];
	}
}

/**
 * Function to parse a string formatted as 'col1','col2',...'coln'.
 * The input must be of length 16 or 18 as that is required for
 * and eclipse to be an eclipse.
 *
 * @param eclipseData -
 * 			The formatted string to parse.
 */
// TODO - Figure out how to have multiple single argument constructors
void Eclipse::parseString(string eclipseData) {
	// TODO - Verify format, right now it verifies in EclipseR.cpp
	int index = 0;
	for (unsigned int i = 0; i < eclipseData.length(); i++) {
		if (eclipseData.at(i) == ',') {
			index++;
		} else {
			_cols[index] += eclipseData.at(i);
		}
	}
	_numCols = index + 1;
}

/**
 * Destructor
 */
Eclipse::~Eclipse() {
	// Empty
}

/**
 * Function to get the column data.
 * INDEXED AT 0
 *
 * @param col -
 * 			The column to be accessed, INDEXED AT 1.
 * @return -
 * 			The column data.
 */
string Eclipse::getCol(int col) {
	if (col < 0 || col > _numCols - 1) {
		throw -10;
	}
	return _cols[col];
}

/**
 * Function to get the number of column data in this eclipse.
 *
 * @return -
 * 			The number of columns.
 */
int Eclipse::numCols() {
	return _numCols;
}

/**
 * Converts the eclipse to a properly formatted csv string.
 *
 * @return -
 * 			The csv string of this eclipse.
 */
string Eclipse::toString() {
	string s = getCol(0);
	for (int i = 1; i < numCols(); i++) {
		s += "," + getCol(i);
	}
	return s;
}

void Eclipse::setDataString(string dataString) {
	_dataString = dataString;
}

// TODO - Keeps return null ????
string Eclipse::getDataString() {
	return _dataString;
}

/**
 * Function which overloads the << operator.
 */
std::ostream& operator<<(std::ostream& s, Eclipse& eclipse) {
	s << eclipse.toString();
	return s;
}

/**
 * Function which overloads the = (assignment) operator.
 */
Eclipse& Eclipse::operator=(const Eclipse& rhs) {
	if (this != &rhs) {
		for (int i = 0; i < rhs._numCols; i++) {
			_cols[i] = rhs._cols[i];
		}
		_numCols = rhs._numCols;
		_dataString = rhs._dataString;
	}
	return *this;
}
