/*
 * EclipseR.h, The header only linked hash table class.
 *
 * Class: CS2413
 * Author: Tyler Hunt (113401590)
 * Date: 11/17/17
 * Assignment: Project #3
 */
#ifndef LINKEDHASHTABLE_H_
#define LINKEDHASHTABLE_H_

#include <string>
#include <iostream>
#include "LinkedList.h"
#include "HashTableBucket.h"
#include "HashTableItem.h"

/**
 * Class which functions as a linked hash table data structure.
 * A linked hash table has a linked list stored alongside a hash table
 * so it can properly define an iterating order. The linked list order
 * is determined by the compFunc passed into the constructor.
 *
 * This hash table implements double hashing as its collision resolution
 * method. This means that two hashing functions primaryHashFunc and
 * secondaryHashFunc must be passed as parameters into the constructor.
 */
template<typename T1, typename T2>
class LinkedHashTable {
public:
	LinkedHashTable<T1, T2>(int (*compFunc)(T1 obj1, T1 obj2),
			int (*primaryHashFunc)(T1 obj1, int tableSize),
			int (*secondaryHashFunc)(T1 obj1, int tableSize));
	LinkedHashTable<T1, T2>(LinkedList<T1>* keyList, LinkedList<T2>* valueList,
			int (*compFunc)(T1 obj1, T1 obj2),
			int (*primaryHashFunc)(T1 obj1, int tableSize),
			int (*secondaryHashFunc)(T1 obj1, int tableSize));
	~LinkedHashTable<T1, T2>();
	void reHash();
	void merge(LinkedList<T1>* keyList, LinkedList<T2>* valueList);
	void purge(LinkedList<T1>* keyList);
	void add(T1 key, T2* value);
	void remove(T1 key);
	T2& get(T1 key);
	ResizableArray<T2>* toResizableArray();
	LinkedList<T2>* toLinkedList();
	void print();

private:
	int (*_compFunc)(T1 obj1, T1 obj2);
	int (*_primaryHashFunc)(T1 obj1, int tableSize);
	int (*_secondaryHashFunc)(T1 obj1, int tabelSize);
	double LOAD_FACTOR = .75;
	LinkedList<HashTableItem<T1, T2>>* _linkedList;
	HashTableBucket<HashTableItem<T1, T2>>* _hashArray;
	int _hashArraySize;

};

/**
 * Default constructor to initialize with compare, primary hash, and secondary
 * hash functions. No data.
 *
 * @param compFunc -
 * 				The key comparison function.
 * @param primaryHashFunc -
 * 				The primary hashing function for the keys.
 * @param secondaryHashFunc -
 * 				The secondary hashing function for the keys.
 */
template<typename T1, typename T2>
LinkedHashTable<T1, T2>::LinkedHashTable(int (*compFunc)(T1 obj1, T1 obj2),
		int (*primaryHashFunc)(T1 obj1, int tableSize),
		int (*secondaryHashFunc)(T1 obj1, int tableSize)) {
	_compFunc = compFunc;
	_primaryHashFunc = primaryHashFunc;
	_secondaryHashFunc = secondaryHashFunc;
	_linkedList = new LinkedList<HashTableItem<T1, T2>>();
	_hashArraySize = (int) (5.0 * (1.0 + LOAD_FACTOR));
	_hashArray = new HashTableBucket<HashTableItem<T1, T2>> [_hashArraySize];
}

/**
 * Constructor to initialize with compare, primary hash, and secondary
 * hash functions. Also initializes with a preset list of data.
 *
 * @param keyList -
 * 				A linked list of the keys to be added. Indexed same as valueList.
 * @param valueList -
 * 				A linked list of the values to be added. Indexed same as keyList.
 * @param compFunc -
 * 				The key comparison function.
 * @param primaryHashFunc -
 * 				The primary hashing function for the keys.
 * @param secondaryHashFunc -
 * 				The secondary hashing function for the keys.
 */
template<typename T1, typename T2>
LinkedHashTable<T1, T2>::LinkedHashTable(LinkedList<T1>* keyList,
		LinkedList<T2>* valueList, int (*compFunc)(T1 obj1, T1 obj2),
		int (*primaryHashFunc)(T1 obj1, int tableSize),
		int (*secondaryHashFunc)(T1 obj1, int tableSize)) {
	_compFunc = compFunc;
	_primaryHashFunc = primaryHashFunc;
	_secondaryHashFunc = secondaryHashFunc;
	_linkedList = new LinkedList<HashTableItem<T1, T2>>();
	_hashArray = 0;
	_hashArraySize = 0;

	// Fill linked list and rehash
	merge(keyList, valueList);
}

/**
 * Function which populates the hash array with the hash table items from
 * the internal linked list. Uses double hashing as its collision resolution
 * strategy.
 * Note. Is executed on every merge, purge, and table resize.
 */
template<typename T1, typename T2>
void LinkedHashTable<T1, T2>::reHash() {
	// Re-initialize hash array based on linked list size and load factor
	_hashArraySize =
			(int) ((double) (_linkedList->size()) * (1.0 + LOAD_FACTOR));
	delete[] _hashArray;
	_hashArray = new HashTableBucket<HashTableItem<T1, T2>> [_hashArraySize];

	// If linked list is empty
	if (_linkedList->size() == 0) {
		_hashArraySize = (int) (5.0 * (1.0 + LOAD_FACTOR));
		_hashArray =
				new HashTableBucket<HashTableItem<T1, T2>> [_hashArraySize];
		return;
	}

	// Traverse linked list
	LinkedList<HashTableItem<T1, T2>> *item = _linkedList;
	HashTableBucket<HashTableItem<T1, T2>> *bucket;
	while (item != 0) {
		int hashVal = _primaryHashFunc(item->info()->getKey(), _hashArraySize);
		bucket = (_hashArray + hashVal);

		// If bucket is empty
		if (bucket->getValue() == 0) {
			bucket->setValue(item->info());
		}

		// If bucket is not empty
		else {
			// Calculate secondary hash
			int secondaryHash = _secondaryHashFunc(item->info()->getKey(),
					_hashArraySize);

			// Find place to put the item
			bool found = false;
			while (!found) {
				// Update primary hash offset
				hashVal = (hashVal + secondaryHash) % _hashArraySize;

				// TODO - Fix infinite loop bug

				// If bucket is empty
				if (bucket->getValue() == 0) {
					// Set bucket equal to value
					bucket->setValue(item->info());
					found = true;
				}
			}
		}

		// Increment item
		item = item->next();
	}
}

/**
 * Function to purge a linked list of keys and values into the map.
 * Note. Duplicates are overwritten.
 * Note. Map is re-hashed.
 *
 * @param keyList -
 * 				A linked list of the keys to be added. Indexed same as valueList.
 * @param valueList -
 * 				A linked list of the values to be added. Indexed same as keyList.
 */
template<typename T1, typename T2>
void LinkedHashTable<T1, T2>::merge(LinkedList<T1>* keyList,
		LinkedList<T2>* valueList) {
	// Traverse key list
	LinkedList<T1> *keyItem = keyList;
	LinkedList<T2> *valueItem = valueList;
	while (keyItem != 0) {
		// Add to linked list
		add(*(keyItem->info()), valueItem->info());

		// Increment key and value
		keyItem = keyItem->next();
		valueItem = valueItem->next();
	}

	// Re-hash map
	reHash();
}

/**
 * Function to purge a linked list of keys from the map.
 * Note. Map is re-hashed.
 *
 * @param keyList -
 * 				A linked list of the keys to be removed.
 */
template<typename T1, typename T2>
void LinkedHashTable<T1, T2>::purge(LinkedList<T1>* keyList) {
	// If table is empty
	if (_linkedList->info() == 0) {
		return;
	}

	// Traverse key list
	LinkedList<T1> *keyItem = keyList;
	while (keyItem != 0) {
		// Try to remove key
		remove(*(keyItem->info()));

		// Increment key
		keyItem = keyItem->next();
	}

	reHash();
}

// TODO - Make map automatically resize when it hits some threshold.
/**
 * Function to add a singular key, value pair to the map.
 * Note. Map can resize, which leads to re-sizing.
 *
 * @param key -
 * 				The key to be added to the map.
 * @param value -
 * 				The keys corresponding value to be added to the map.
 */
template<typename T1, typename T2>
void LinkedHashTable<T1, T2>::add(T1 key, T2* value) {
	// If list is empty
	if (_linkedList->size() == 0) {
		_linkedList = new LinkedList<HashTableItem<T1, T2>>(
				*(new HashTableItem<T1, T2>(key, value)));
		return;
	}

	// Traverse linked list
	LinkedList<HashTableItem<T1, T2>> *item = _linkedList;
	LinkedList<HashTableItem<T1, T2>> *previousItem = _linkedList;
	bool found = false;
	while (!found && item != 0) {
		// Compare keyItem and item.key
		int comp = _compFunc(key, item->info()->getKey());

		// Insert according to comparison
		if (comp < 0) {				//keyItem < item.key
			// Add before
			item->add(*(new HashTableItem<T1, T2>(key, value)));
			found = true;
		} else if (comp == 0) {		//keyItem == item.key
			// Add after
			item->addAt(*(new HashTableItem<T1, T2>(key, value)), 1);
			found = true;
		}

		// Increment item
		previousItem = item;
		item = item->next();
	}

	// Should go at end of list
	if (!found) {
		previousItem->addAt(*(new HashTableItem<T1, T2>(key, value)), 1);
	}
}

// TODO - Make map automatically resize when it hits some threshold.
/**
 * Function to remove a key from the map.
 * Note. Can resize the map, which leads to re-hashing.
 *
 * @param key -
 * 				The key to be removed from the map.
 */
template<typename T1, typename T2>
void LinkedHashTable<T1, T2>::remove(T1 key) {
	// Traverse linked list, looking for item to remove
	LinkedList<HashTableItem<T1, T2>> *item = _linkedList;
	bool found = false;
	while (!found && item != 0) {
		// Compare keyItem and item.key
		int comp = _compFunc(key, item->info()->getKey());

		//keyItem == item.key
		if (comp == 0) {
			item->remove();
			found = true;
		}

		// Increment item
		item = item->next();
	}

	// If not found in list
	if (!found) {
		std::cout << "The entry with key " << key << " was not found."
				<< std::endl;
	}
}

/**
 * Function to get a value from the map based on a key.
 *
 * @param key -
 * 				The key to search for.
 *
 * @return -
 * 				The value that corresponds to the given key.
 */
template<typename T1, typename T2>
T2& LinkedHashTable<T1, T2>::get(T1 key) {
	// Calculate primary hash
	int hashVal = _primaryHashFunc(key, _hashArraySize);
	HashTableBucket<HashTableItem<T1, T2>> bucket = _hashArray[hashVal];

	// If key in primary hash bucket == key
	if (_compFunc(bucket._value->getKey(), key) == 0) {

	} else {
		// Calculate secondary hash
		int secondaryHash = _secondaryHashFunc(key, _hashArraySize);

		bool foundEmptySinceBeginning = false;
		while (!foundEmptySinceBeginning) {
			// Update primary hash offset
			hashVal = (hashVal + secondaryHash) % _hashArraySize;

			// Update bucket
			bucket = _hashArray[hashVal];

			// If bucket is not empty
			if (bucket.getValue() != 0) {
				// If bucket key equals the desired key
				if (_compFunc(bucket.getValue()->getKey(), key) == 0) {
					return bucket.getValue()->getValue();
				}
			} else {
				// If bucket is empty and was since beginning
				if (bucket.isEmptySinceBeginning()) {
					foundEmptySinceBeginning = true;
				}
			}
		}
	}

	return 0;
}

/**
 * Converts the internal linked list to a resizable array.
 * Note. Ordered by key order.
 *
 * @return -
 * 					The ResizableArray object of the values.
 */
template<typename T1, typename T2>
ResizableArray<T2>* LinkedHashTable<T1, T2>::toResizableArray() {
	ResizableArray<T2> *array = new ResizableArray<T2>(_linkedList->size());

	// Empty table
	if (_linkedList->size() == 0) {
		return array;
	}

	// Iterate over linked list
	LinkedList<HashTableItem<T1, T2>> *item = _linkedList;
	while (item != 0) {
		// Add item to end of array
		try {
			array->add(*(item->info()->getValue()));
		} catch (int c) {
			std::cerr << "LinkedHashTable:285 threw code " << c << std::endl;
		}

		// Increment item
		item = item->next();
	}

	return array;
}

/**
 * Function which converts the internal linked list, to a linked list
 * of values.
 * Note. Ordered by key order.
 *
 * @return -
 * 					The LinkedList object of the values.
 */
template<typename T1, typename T2>
LinkedList<T2>* LinkedHashTable<T1, T2>::toLinkedList() {
	LinkedList<T2> *list = new LinkedList<T2>();

	// Empty table
	if (_linkedList->size() == 0) {
		return list;
	}

	// Iterate over linked list
	LinkedList<HashTableItem<T1, T2>> *item = _linkedList;
	int count = 0;
	while (item != 0) {
		// Add item to end of list
		list->addAt(*(item->info()->getValue()), count);

		// Increment item
		item = item->next();
		count++;
	}

	return list;
}

/**
 * Function to print the hash map to the standart output.
 * Note. Useful for debugging.
 */
template<typename T1, typename T2>
void LinkedHashTable<T1, T2>::print() {
	// Traverse array
	for (int i = 0; i < _hashArraySize; i++) {
		if (_hashArray[i].getValue() == 0) {
			std::cout << std::endl << i << "\t" << "NULL" << std::endl;
		} else {
			Eclipse e = *((_hashArray + i)->getValue()->getValue());
			std::cout << std::endl << i << "\t" << e.toString() << std::endl;
		}
	}
	std::cout << std::endl;
}

/**
 * Destructor
 */
template<typename T1, typename T2>
LinkedHashTable<T1, T2>::~LinkedHashTable() {
	// Delete all items in linked list
	delete _linkedList;
	delete[] _hashArray;
}

#endif /* LINKEDHASHTABLE_H_ */
