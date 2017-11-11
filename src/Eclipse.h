/**
 * Header file for the Eclipse class.
 *
 * Class: CS2414
 * Author: Tyler Hunt (113401590)
 * Date: 9/29/17
 * Assignment: Lab #4
 */
#ifndef ECLIPSE_H_
#define ECLIPSE_H_

#include <string>

// Defining the Eclipse class
class Eclipse {
	friend std::ostream& operator<<(std::ostream& s, const Eclipse& eclipse);
	// TODO - Is assignment operator able to be a friend?
public:
	Eclipse();
	Eclipse(const Eclipse& eclipse);
	~Eclipse();
	std::string getCol(int col);
	int numCols();
	std::string toString();
	Eclipse& operator=(const Eclipse& rhs); // Only way I could make it work.
	// TODO - Figure out how to have multiple single argument constructors
	void parseString(std::string eclipseData);
	void setDataString(std::string dataString);
	std::string getDataString();

private:
	std::string _cols[18];
	std::string _dataString;
	int _numCols;
};

#endif /* ECLIPSE_H_ */
