/*
 * EclipseR.h
 *
 * Class: CS2413
 * Author: Tyler Hunt (113401590)
 * Date: 11/2/17
 * Assignment: Project #2
 */
#ifndef ECLIPSER_H_
#define ECLIPSER_H_

#include "ResizableArray.h"
#include "Eclipse.h"
#include "LinkedHashTable.h"
#include <string>

LinkedHashTable<int,Eclipse> *hashTable;
ResizableArray<Eclipse> *sortedArray;
int dataLinesAttempted;
int dataLinesValid;
int fileLinesRead;
int compareColumn;
std::string header;
std::string months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
		"Sep", "Oct", "Nov", "Dec" };

bool isWholeNumber(std::string s);
bool isDecimalNumber(std::string s);
std::string formatLine(std::string line);
bool verifyLine(std::string line);
void mergeDataFromStream(std::ifstream &stream);
void purgeDataFromStream(std::ifstream &stream);
int compareEclipsesByColumn(Eclipse obj1, Eclipse obj2);
int binarySearch(int col, std::string val, int left, int right);
void findLoop(int column);
int compareKeys(int key1, int key2);
int primaryHash(int key, int tableSize);
int secondaryHash(int key, int tableSize);
int main();

#endif /* ECLIPSER_H_ */
