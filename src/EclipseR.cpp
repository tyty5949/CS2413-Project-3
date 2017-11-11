/*
 * EclipseR.cpp, The driver class.
 *
 * Class: CS2413
 * Author: Tyler Hunt (113401590)
 * Date: 11/2/17
 * Assignment: Project #2
 */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "EclipseR.h"

using namespace std;

/**
 * Main function.
 */
int main() {
	dataList = new LinkedList<Eclipse>();

	// Initial data load
	string input;
	bool foundFile = false;
	ifstream stream;
	while (!foundFile) {
		cout << "Enter the data file: ";
		getline(cin, input);

		// If only enter is hit
		if (input == "") {
			break;
		}

		// Check if file exists
		stream.open(input, ios_base::in);
		if (stream.good()) {
			foundFile = true;
			// Load data from stream
			mergeDataFromStream(stream);
		} else {
			cerr << "File is not available." << endl;
		}
	}

	// Main Loop
	do {
		cout << "MPFOCSQ? ";
		getline(cin, input);

		// Merge
		if (input == "M") {
			cout << "Enter the data file: ";
			getline(cin, input);

			if (input != "") {
				stream.open(input, ios_base::in);
				if (stream.good()) {
					mergeDataFromStream(stream);
				} else {
					cerr << "File is not available." << endl;
				}
			}
		}
		// Purge
		else if (input == "P") {
			cout << "Enter the data file: ";
			getline(cin, input);

			if (input != "") {
				stream.open(input, ios_base::in);
				if (stream.good()) {
					purgeDataFromStream(stream);
				} else {
					cerr << "File is not available." << endl;
				}
			}
		}
		// Find
		else if (input == "F") {
			cout << "Data field (1-18)? ";
			getline(cin, input);
			if (isWholeNumber(input)) {
				int col = stoi(input);
				findLoop(col - 1);
			}
		}
		// Output
		else if (input == "O") {
			cout << "File name?";
			getline(cin, input);
			// Output to cout
			if (input == "") {
				cout << header << endl;
				for (int i = 0; i < sortedArray->size(); i++) {
					cout << sortedArray->get(i).getDataString() << endl;
				}
				cout << "Data lines read: " << dataLinesAttempted
						<< "; Valid eclipses read: " << dataLinesValid
						<< "; Eclipses in memory: " << dataList->size() << endl;
			} else {
				// Output to file
				ofstream stream;
				stream.open(input, ios_base::out);
				if (!stream.good()) {
					cerr << "File is not available to write to." << endl;
					continue;
				} else {
					stream << header << endl;
					for (int i = 0; i < sortedArray->size(); i++) {
						stream << sortedArray->get(i).getDataString() << endl;
					}
					stream << "Data lines read: " << dataLinesAttempted
							<< "; Valid eclipses read: " << dataLinesValid
							<< "; Eclipses in memory: " << dataList->size()
							<< endl;
					stream.close();
				}
			}
		}
		// Print out by catalog number, traverse linked list
		else if (input == "C") {
			cout << header << endl;
			if (dataList->size() != 0) {
				LinkedList<Eclipse> *node = dataList;
				while (node->next() != 0) {
					cout << node->info().getDataString() << endl;
					node = node->next();
				}
				cout << node->info().getDataString() << endl;
			}
			cout << "Data lines read: " << dataLinesAttempted
					<< "; Valid eclipses read: " << dataLinesValid
					<< "; Eclipses in memory: " << dataList->size() << endl;
		}
		// Sort
		else if (input == "S") {
			cout << "Data field (1-18)?";
			getline(cin, input);
			if (isWholeNumber(input)) {
				int col = stoi(input);
				if (col >= 1 && col <= 18) {
					compareColumn = col - 1;
					sortedArray->sort(*compareEclipsesByColumn);// Pass function as parameter
				}
			}
		}
	} while (input != "Q");

	cout << "Friendly parting message." << endl;
	return 0;
}

/**
 * Function which will take a file stream of an eclipse data file, and will
 * merge those eclipses into the database.
 * NOTE: Duplicate catalog numbers will be overwritten
 * NOTE: Will be sorted by catalog numbers
 * NOTE: Resets sorting
 *
 * @param stream
 * 				The file stream.
 */
void mergeDataFromStream(ifstream &stream) {
	string line = "";
	header = "";
	for (int i = 0; i < 10; i++) {
		getline(stream, line);
		if (i == 9) {
			header += line;
		} else {
			header += line + "\n";
		}
	}
	getline(stream, line);
	while (line != "") {
		dataLinesAttempted++;
		fileLinesRead++;
		// Remove spaces and separate with commas, CSV format
		string origLine = line;
		line = formatLine(line);
		// Verify data lines as given in lab 1 - 4
		if (verifyLine(line)) {
			dataLinesValid++;

			// Create new eclipse
			Eclipse *eclipse = new Eclipse();
			eclipse->parseString(line);
			eclipse->setDataString(origLine);

			// Move it into list
			// TODO - More efficient list traversal
			bool inserted = false;
			for (int i = 0; i < dataList->size(); i++) {
				try {
					// TODO - Fix duplicate problem
					// Replace duplicate
					if (stoi(dataList->infoAt(i).getCol(0))
							== stoi(eclipse->getCol(0))) {
						dataList->addAt(*eclipse, i);
						dataList->removeAt(i);
						inserted = true;
						break;
					}
					// Just add before greater id's
					else if (stoi(dataList->infoAt(i).getCol(0))
							> stoi(eclipse->getCol(0))) {
						dataList->addAt(*eclipse, i);
						inserted = true;
						break;
					}
				} catch (string& s) {
					cout << "Exception in mergeDataFromStream(): " << s << endl;
				}
			}
			// List is empty
			if (dataList->size() == 0) {
				dataList->add(*eclipse);
			}
			// Add to end
			else if (!inserted) {
				try {
					dataList->addAt(*eclipse, dataList->size());
				} catch (string &s) {
					cout << s << endl;
				}
			}
		}
		getline(stream, line);
	}
	stream.close();

	// Replace sorted array
	delete sortedArray;
	sortedArray = dataList->toResizableArray();
}

/**
 * Function which will take a file stream of an eclipse data file, and will
 * purge those eclipses from the database.
 * NOTE: If a catalog number is not found, a error sent to cerr is presented
 * NOTE: Resets sorting to catalog numbers
 *
 * @param stream
 * 				The file stream.
 */
void purgeDataFromStream(ifstream &stream) {
	string line;
	for (int i = 0; i < 10; i++) {
		getline(stream, line);
	}
	getline(stream, line);
	while (line != "") {
		// Remove spaces and separate with commas, CSV format
		string origLine = line;
		line = formatLine(line);
		// Verify data lines as given in lab 1 - 4
		if (verifyLine(line)) {
			Eclipse *eclipse = new Eclipse();
			eclipse->parseString(line);
			eclipse->setDataString(origLine);

			// Remove it from the list
			bool removed = false;
			LinkedList<Eclipse> *node = dataList;
			try {
				while (node->next() != 0) {
					if (stoi(node->info().getCol(0))
							== stoi(eclipse->getCol(0))) {
						node->remove();
						removed = true;
						break;
					}
					node = node->next();
				}
				if (stoi(node->info().getCol(0)) == stoi(eclipse->getCol(0))) {
					node->remove();
					removed = true;
				}
			} catch (string &s) {
				cout << "Exception in purgeDataFromStream(): " << s << endl;
			}

			// If not found in list
			if (!removed) {
				cout << "The entry with catalog number " << eclipse->getCol(0)
						<< " was not found." << endl;
			}
		}
		getline(stream, line);
	}
	stream.close();

	// Replace sorted array
	delete sortedArray;
	sortedArray = dataList->toResizableArray();
}

/**
 * Function which handles the find implementation. It error checks the users input
 * and then decided whether or not to use a binary or linear search. The function
 * will decide binary if the list is already sorted by the column to be searched through
 * and will opt for a linear search for the other times.
 *
 * @param column
 * 			The column to search through, indexed at 0.
 */
// TODO - Template and move to ResizableArray.h
void findLoop(int column) {
	cout << "Value to search?";
	string input;
	getline(cin, input);
	// Just hits enter
	if (input == "") {
		return;
	}
	// columns 0, 1, 2, 4, 6, 7, 8, 14, 15 (integers)
	if ((column >= 0 && column <= 2) || column == 4
			|| (column >= 6 && column <= 8) || (column >= 14 && column <= 15)) {
		if (!isWholeNumber(input)) {
			return;
		}
	}
	// columns 10, 11 (doubles)
	if (column == 10 || column == 11) {
		if (!isDecimalNumber(input)) {
			return;
		}
	}
	// column 3 (months)
	if (column == 3) {
		bool valid = false;
		for (int i = 0; i < 12; i++) {
			if (input == months[i]) {
				valid = true;
			}
		}
		if (!valid) {
			return;
		}
	}

	// Binary search
	LinkedList<Eclipse> foundList;
	if (column == compareColumn) {
		int firstBound = binarySearch(column, input, 0,
				sortedArray->size() - 1);
		// Find the upper bound with the same column value
		int upperBound = firstBound;
		while (upperBound < sortedArray->size()
				&& sortedArray->get(upperBound).getCol(column) == input) {
			upperBound++;
		}
		// Find the lower bound with the same column value
		int lowerBound = firstBound;
		while (lowerBound >= 0
				&& sortedArray->get(lowerBound).getCol(column) == input) {
			lowerBound--;
		}
		for (int i = lowerBound; i <= upperBound; i++) {
			foundList.addAt(sortedArray->get(i), i);
		}
	}
	// Linear search
	else {
		cout << sortedArray->size() << endl;
		try {
			// TODO - Implement more efficient traverse
			for (int i = 0; i < sortedArray->size(); i++) {
				if (sortedArray->get(i).getCol(column) == input) {
					foundList.addAt(sortedArray->get(i), foundList.size());
				}
			}
		} catch (string &s) {
			cout << "Exception in linearSearch(): " << s << endl;
		}
	}
	cout << header << endl;
	if (foundList.size() != 0) {
		LinkedList<Eclipse> *node = &foundList;
		while (node->next() != 0) {
			cout << node->info().getDataString() << endl;
			node = node->next();
		}
		cout << node->info().getDataString() << endl;
	}
	cout << "Eclipses found: " << foundList.size() << endl;
}

/**
 * Recursive function which works as a binary search. Works like the standard
 * binary serch algorithm.
 * NOTE: This function provides no error checking.
 *
 * @parm col
 * 				The column to search through.
 * @param val
 * 				The value within the column to search through.
 * @param left
 * 				The left edge of the array to search through.
 * @param right
 * 				The right edge of the array to search through.
 */
// TODO - Verify it works
int binarySearch(int col, string val, int left, int right) {
	int mid = left + right / 2;
	if (sortedArray->get(mid).getCol(col) == val) {
		return mid;
	} else if (sortedArray->get(mid).getCol(col) > val) {
		return binarySearch(col, val, mid + 1, right);
	} else {
		return binarySearch(col, val, left, mid - 1);
	}
}

/**
 * Function which works exactly like a java compare function. It takes two eclipses
 * and determines which is greater, less than, or equal. If obj1 > obj2, the function
 * will return 1. If obj1 < obj2, the function will return -1. If obj1 == obj2, the
 * function will return 0. It knows which column to compare against because it was chosen
 * and assigned to a variable earlier when the sort was commenced. This function is passed
 * to the quickSort() function as a parameter so it can effectively conduct the quicksort.
 * NOTE: Columns are indexed by 0
 */
// TODO - Cleanup, like a lot
int compareEclipsesByColumn(Eclipse obj1, Eclipse obj2) {
	// columns 0, 1, 2, 4, 6, 7, 8, 14, 15 (integers)
	if ((compareColumn >= 0 && compareColumn <= 2) || compareColumn == 4
			|| (compareColumn >= 6 && compareColumn <= 8)
			|| (compareColumn >= 14 && compareColumn <= 15)) {
		int i1 = stoi(obj1.getCol(compareColumn));
		int i2 = stoi(obj2.getCol(compareColumn));
		if (i1 > i2) {
			return 1;
		} else if (i2 > i1) {
			return -1;
		} else {
			return 0;
		}
	}
	// column 3 (months)
	else if (compareColumn == 3) {
		int month1;
		int month2;
		for (int i = 0; i < 12; i++) {
			if (months[i] == obj1.getCol(3)) {
				month1 = i;
			}
			if (months[i] == obj2.getCol(3)) {
				month2 = i;
			}
		}
		if (month1 > month2) {
			return 1;
		} else if (month2 > month1) {
			return -1;
		} else {
			return 0;
		}
	}
	// column 5
	else if (compareColumn == 5) {
		string s1 = obj1.getCol(5);
		string s2 = obj2.getCol(5);
		string test1 = s1.substr(0, 2) + s1.substr(3, 2) + s1.substr(6, 2);
		int i1 = stoi(test1);
		string test2 = s2.substr(0, 2) + s2.substr(3, 2) + s2.substr(6, 2);
		int i2 = stoi(test2);
		if (i1 > i2) {
			return 1;
		} else if (i2 > i1) {
			return -1;
		} else {
			return 0;
		}

	}
	// columns 10 & 11
	else if (compareColumn == 10 || compareColumn == 11) {
		double d1 = stod(obj1.getCol(compareColumn));
		double d2 = stod(obj2.getCol(compareColumn));
		if (d1 > d2) {
			return 1;
		} else if (d2 > d1) {
			return -1;
		} else {
			return 0;
		}
	}
	// Columns 12 & 13
	// TODO - Do N/S or E/W properly
	else if (compareColumn == 12 || compareColumn == 13) {
		double d1 = stod(
				obj1.getCol(compareColumn).substr(0,
						obj1.getCol(compareColumn).length() - 2));
		double d2 = stod(
				obj2.getCol(compareColumn).substr(0,
						obj2.getCol(compareColumn).length() - 2));
		if (d1 > d2) {
			return 1;
		} else if (d2 > d1) {
			return -1;
		} else {
			return 0;
		}
	}
	// Columns 16 & 17
	// TODO - Do time properly
	else if (compareColumn == 16 || compareColumn == 17) {
		if (obj1.numCols() == 16 && obj2.numCols() == 16) {	// Both objects missing columns, keep natural order?
			return 0;
		} else if (obj1.numCols() == 16) {// Object 1 is missing columns, obj1 automatically greater than obj2
			return 1;
		} else if (obj2.numCols() == 16) {// object 2 is missing columns, obj1 automatically less than obj2
			return -1;
		} else {							// both have 18 columns
			if (obj1.getCol(compareColumn) == obj2.getCol(compareColumn)) {
				return 0;
			} else if (obj1.getCol(compareColumn)
					> obj2.getCol(compareColumn)) {
				return 1;
			} else {
				return -1;
			}
		}
	}
	// column 9
	else {
		if (obj1.getCol(compareColumn) == obj2.getCol(compareColumn)) {
			return 0;
		} else if (obj1.getCol(compareColumn) > obj2.getCol(compareColumn)) {
			return 1;
		} else {
			return -1;
		}
	}
	return 0;
}

/**
 * Function which verifies the line based on the criterion specified in earlier labs.
 * NOTE: Duplicates are no longer checked as they are handled elsewhere
 * NOTE: The data line must be in CSV format, use formatLine()
 *
 * @param line
 * 				The CSV data line.
 * @return
 * 				Whether or not the line is valid.
 */
bool verifyLine(string line) {
	// Parse line into ResizableArray
	ResizableArray<string> lineData;
	while (line.find(",") != string::npos) {
		string word = line.substr(0, line.find(","));
		lineData.add(word);
		line = line.substr(line.find(",") + 1, line.length() - line.find(","));
	}
	lineData.add(line);

	// Perform column amount check
	if (lineData.get(9).at(0) == 'P' && lineData.size() != 16) {
		cerr << "Error in data row " << fileLinesRead << ": " << lineData.size()
				<< " columns found. Should be 16." << endl;
		return false;
	} else if (lineData.get(9).at(0) != 'P' && lineData.size() != 18) {
		cerr << "Error in data row " << fileLinesRead << ": " << lineData.size()
				<< " columns found. Should be 18." << endl;
		return false;
	}

	// Perform whole number checks
	int wholeIndicies[] = { 0, 1, 2, 4, 6, 7, 8, 14, 15 };
	for (int i = 0; i < 9; i++) {
		if (!isWholeNumber(lineData.get(wholeIndicies[i]))) {
			cerr << "Error in data row " << fileLinesRead << ": Column "
					<< wholeIndicies[i] + 1 << " is not a whole number."
					<< endl;
			return false;
		}
	}

	// Perform decimal number checks
	int decimalIndicies[] = { 10, 11 };
	for (int i = 0; i < 2; i++) {
		if (!isDecimalNumber(lineData.get(decimalIndicies[i]))) {
			cerr << "Error in data row " << fileLinesRead << ": Column "
					<< decimalIndicies[i] + 1 << " is not a decimal number."
					<< endl;
			return false;
		}
	}

	// Duplicate check
	/*
	 try {
	 for (int i = 0; i < dataList->size(); i++) {
	 if (stoi(dataList->infoAt(i).getCol(0)) == stoi(lineData.get(0))) {
	 cerr << "Error in data row " << fileLinesRead
	 << ": Duplicate catalog number "
	 << stoi(lineData.get(0)) << "." << endl;
	 return true;	// This time we want it to overwrite.
	 }
	 }
	 } catch (string& s) {
	 cout << "Exception in verifyLine(): " << s;
	 return false;
	 }
	 */

	return true;
}

/**
 * Function which formats the specified line (must be validated first),
 * to the correct output format with commas.
 *
 * @param line -
 * 			The line to be formatted.
 * @return -
 * 			The formatted string.
 */
string formatLine(string line) {
	string s = "";
	bool foundCol = false;
	bool firstCol = true;
	for (unsigned int i = 0; i < line.length(); i++) {
		if (line.at(i) != ' ') {
			if (!foundCol) {
				if (firstCol) {
					firstCol = false;
				} else {
					s.append(",");
				}
			}
			foundCol = true;
			s.append(line.substr(i, 1));
		} else {
			foundCol = false;
		}
	}
	return s;
}

/**
 * Returns whether or not the number could be a whole number.
 * Note: Parses to int and checks for exceptions.
 * Note: '-' now counts as a whole number.
 *
 * @return -
 * 			Whether or not the string is a whole number.
 */
bool isWholeNumber(string s) {
	if (s == "-") {
		return false;
	}
	try {
		stoi(s);
		return true;
	} catch (exception const &e) {
		return false;
	}
}

/**
 * Returns whether or not the number could be a decimal number.
 * Note: Parses to double and checks for exceptions.
 *
 * @return -
 * 			Whether or not the string is a decimal number.
 */
bool isDecimalNumber(string s) {
	try {
		stod(s);
		return true;
	} catch (exception const &e) {
		return false;
	}
}
