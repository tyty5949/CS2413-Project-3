/*
 * LinkedHashTable.h
 *
 *  Created on: Nov 17, 2017
 *      Author: Tyler
 */

#ifndef LINKEDHASHTABLE_H_
#define LINKEDHASHTABLE_H_

#include <string>
#include "LinkedList.h"
#include "HashTableBucket.h"
#include "HashTableItem.h"

template<typename T1, typename T2>
class LinkedHashTable {
public:
	LinkedHashTable<T1, T2>(int (*compFunc)(T1 obj1, T1 obj2),
			int (*primaryHashFunc)(T1 obj1), int (*secondaryHashFunc)(T1 obj1));
	LinkedHashTable<T1, T2>(LinkedList<T1>* keyList, LinkedList<T2>* valueList,
			int (*compFunc)(T1 obj1, T1 obj2), int (*primaryHashFunc)(T1 obj1),
			int (*secondaryHashFunc)(T1 obj1));
	~LinkedHashTable<T1, T2>();
	void reHash();
	void merge(LinkedList<T1>* keyList, LinkedList<T2>* valueList);
	void purge(LinkedList<T1>* keyList);
	void add(T1 key, T2* value);
	void remove(T1 key);
	T2* get(T1 key);

private:
	int (*_compFunc)(T1 obj1, T1 obj2);
	int (*_primaryHashFunc)(T1 obj1);
	int (*_secondaryHashFunc)(T1 obj1);
	double LOAD_FACTOR = .75;
	LinkedList<HashTableItem<T1, T2>>* _linkedList;
	HashTableBucket<HashTableItem<T1, T2>> _hashArray[1];

};

template<typename T1, typename T2>
LinkedHashTable<T1, T2>::LinkedHashTable(int (*compFunc)(T1 obj1, T1 obj2),
		int (*primaryHashFunc)(T1 obj1), int (*secondaryHashFunc)(T1 obj1)) {
	_compFunc = compFunc;
	_primaryHashFunc = primaryHashFunc;
	_secondaryHashFunc = secondaryHashFunc;
	_linkedList = new LinkedList<HashTableItem<T1, T2>>();
	_hashArray = HashTableBucket<HashTableItem<T1, T2>> [(int) (10
			* (1.0 + LOAD_FACTOR))];
}

template<typename T1, typename T2>
LinkedHashTable<T1, T2>::LinkedHashTable(LinkedList<T1>* keyList,
		LinkedList<T2>* valueList, int (*compFunc)(T1 obj1, T1 obj2),
		int (*primaryHashFunc)(T1 obj1), int (*secondaryHashFunc)(T1 obj1)) {
	_compFunc = compFunc;
	_primaryHashFunc = primaryHashFunc;
	_secondaryHashFunc = secondaryHashFunc;
	_linkedList = new LinkedList<HashTableItem<T1, T2>>();

	// Fill linked list and rehash
	merge(keyList, valueList);
}

template<typename T1, typename T2>
void LinkedHashTable<T1, T2>::reHash() {
	int size = _linkedList->size();
	_hashArray = HashTableBucket<T1, T2> [(int) (size * (1.0 + LOAD_FACTOR))];

}

template<typename T1, typename T2>
void LinkedHashTable<T1, T2>::merge(LinkedList<T1>* keyList,
		LinkedList<T2>* valueList) {
	// If table is empty
	if (_linkedList->size() == 0) {
		_linkedList->add(
				new HashTableItem(*(keyList->info()), valueList->info()));
	}

	// Traverse key list
	LinkedList<T1> *keyItem = keyList->next();
	LinkedList<T2> *valueItem = valueList->next();
	while (keyItem != 0) {
		// Traverse linked list, looking for where to insert
		LinkedList<HashTableItem<T1, T2>> *item = _linkedList;
		bool foundLoc = false;
		while (foundLoc) {
			// Compare keyItem and item.key
			int comp = _comFunc(*(keyItem->info()), *(item->info()->getKey()));
			if (comp < 0) {				//keyItem < item.key
				// Add before
				item->add(new HashTableItem(*keyItem, valueItem));
				foundLoc = true;
			} else if (comp == 0) {		//keyItem == item.key
				// Add after
				item->addAt(new HashTableItem(*keyItem, valueItem), 1);
				item->_info = valueItem;
				foundLoc = true;
			} else {					//keyItem > item.key
				// If last item in linked list
				if (item->next() == 0) {
					// Add after
					item->add(new HashTableItem(*keyItem, valueItem));
					foundLoc = true;
				}
			}

			// Increment item
			item = item->next();
		}

		// Increment items
		keyItem = keyList->next();
		valueItem = valueList->next();
	}
}

template<typename T1, typename T2>
void LinkedHashTable<T1, T2>::purge(LinkedList<T1>* keyList) {
	// If table is empty
	if (_linkedList->info() == 0) {
		return;
	}

	// Traverse key list
	LinkedList<T1> *keyItem = keyList;
	while (keyItem != 0) {

	}
}

#endif /* LINKEDHASHTABLE_H_ */
