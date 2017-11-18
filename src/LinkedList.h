/**
 * Header only class which implements a linked list of any arbitrary type.
 *
 * Class: CS2413
 * Author: Tyler Hunt (113401590)
 * Date: 11/2/17
 * Assignment: Project #2
 */
#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <iostream>

/**
 * Class object which will act as both a linked list and linked list item.
 */
template<typename T>
class LinkedList {
	template<typename T1>
	friend std::ostream& operator<<(std::ostream& s, LinkedList<T1>& array);
public:
	LinkedList<T>();
	~LinkedList<T>();
	LinkedList<T>(const LinkedList<T>& linkedList);
	LinkedList<T>(T& object);
	LinkedList<T>(T& object, LinkedList<T>* linkedList);
	bool isEmpty();
	T* info();
	void add(T& object);
	LinkedList<T>& find(T& key);
	LinkedList<T>* next();
	int size();
	T& infoAt(int index);
	void addAt(T& object, int index);
	T remove();
	T removeAt(int index);
	ResizableArray<T>* toResizableArray();

protected:
	T* _info;
	LinkedList<T>* _next;
};

/**
 * Overloaded << operator to print out list of objects in array.
 */
template<typename T>
std::ostream& operator<<(std::ostream& s, LinkedList<T>& list) {
	if (list._info != 0) {
		LinkedList<T> currentItem = list;
		while (currentItem._next != 0) {
			s << currentItem << std::endl;
			currentItem = *currentItem._next;
		}
		s << currentItem << std::endl;
	}
	return s;
}

/**
 * Destructor.
 */
template<typename T>
LinkedList<T>::~LinkedList() {
	delete _next;
}

/**
 * Default constructor. Initializes info and next as null.
 */
template<typename T>
LinkedList<T>::LinkedList() {
	_info = NULL;
	_next = 0;
}

/**
 * Copy constructor.
 */
template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& linkedList) {
	_info = linkedList._info;
	_next = linkedList._next;
}

/**
 * Constructor to initialize with an info node and an null next node.
 */
template<typename T>
LinkedList<T>::LinkedList(T& object) {
	_info = &object;
	_next = 0;
}

/**
 * Constructor to initialize with an info node and a next node.
 */
template<typename T>
LinkedList<T>::LinkedList(T& object, LinkedList<T> *linkedList) {
	_info = &object;
	_next = linkedList;
}

/**
 * Determines whether or not the list is empty.
 *
 * @return
 * 				Whether or not the list is empty.
 */
template<typename T>
bool LinkedList<T>::isEmpty() {
	return (_info == 0);
}

/**
 * Gets the info object at this node.
 *
 * @return
 * 				The info node.
 *
 * @throws
 * 				If the list is empty.
 */
template<typename T>
T* LinkedList<T>::info() {
	if (_info == 0) {
		throw std::string("info(), LinkedList is empty, cannot get info!");
	}
	return _info;
}

/**
 * Adds a new node to the front of the list.
 *
 * @param object
 * 				The info to add to the new node.
 */
template<typename T>
void LinkedList<T>::add(T& object) {
	if (_info == 0) {
		_info = &object;
	} else {
		LinkedList<T> *temp = new LinkedList<T>(*_info, _next);
		_next = temp;
		_info = &object;
	}
}

/**
 *	Method which locates an object based on a given key and gives
 *	the corresponding LinkedList node.
 *	NOTE: Searches linearly
 *	NOTE: The == operator must be overridden by the data type.
 *
 *	@param key
 *				The key to search for.
 *	@return
 *				The node which matches the key.
 */
template<typename T>
LinkedList<T>& LinkedList<T>::find(T& key) {
	if (_info == 0) {
		return 0;
	} else if (key == _info) {
		return this;
	} else if (_next == 0) {
		return 0;
	} else {
		return _next->find(key);
	}
}

/**
 * Gets the pointer to the next node from this node. null if this is the last node
 * in the list.
 *
 * @return
 * 				The next node.
 */
template<typename T>
LinkedList<T>* LinkedList<T>::next() {
	return _next;
}

/**
 * Gets the number of nodes in the list.
 * NOTE: Time complexity is O(n) as it must linearly count up.
 *
 * @return
 * 				The number of nodes in the list.
 */
template<typename T>
int LinkedList<T>::size() {
	if (_next == 0) {
		if (_info == 0) {
			return 0;
		} else {
			return 1;
		}
	} else {
		return 1 + _next->size();
	}
}

/**
 * Gets the info at a given node in the array.
 * NOTE: Must count up linearly to given index.
 *
 * @param index
 * 				The index of the node info to get, indexed at 0.
 * @return
 * 				The info of the node at the given index.
 *
 * @throws
 * 				If the list is empty or if the index is out of bounds;
 */
template<typename T>
T& LinkedList<T>::infoAt(int index) {
	if (_info == 0) {
		throw std::string("infoAt(), LinkedList is empty, cannot get info!");
	} else if (index == 0) {
		return *_info;
	} else if (_next == 0) {
		throw std::string("infoAt(), Index out of bounds!");
	} else {
		return _next->infoAt(index - 1);
	}
}

/**
 * Inserts a new node at the given index moving all other nodes to the right.
 *
 * @param object
 * 				The info the the new node.
 * @param index
 * 				The index where the new node should be inserted, indexed at 0.
 *
 * @throws
 * 				If the given index is out of bounds.
 */
template<typename T>
void LinkedList<T>::addAt(T& object, int index) {
	if (index == 0) {
		add(object);
	} else if (_next == 0 && index > 1) {
		throw std::string("addAt(), Index out of bounds! " + index);
	} else if (_next == 0 && index == 1) {
		LinkedList<T> *temp = new LinkedList<T>(object);
		_next = temp;
	} else {
		_next->addAt(object, index - 1);
	}
}

/**
 * Removes and returns the info of this node.
 *
 * @return
 * 				The info of this node.
 *
 * @throws
 * 				If the list is empty.
 */
template<typename T>
T LinkedList<T>::remove() {
	if (_info == 0) {
		throw std::string("remove(), LinkedList is empty, cannot remove!");
	}
	T temp = *_info;
	delete _info;
	_info = 0;
	if (_next == 0) {
		_info = 0;
	} else {
		LinkedList<T> *tempNext = _next;
		_info = _next->_info;
		_next = _next->_next;
		tempNext->_next = 0;
		tempNext->_info = 0;
	}
	return temp;
}

/**
 * Removes the returns the info of a node at a given index.
 *
 * @param index
 * 				The index of the node to remove.
 *
 * @throws
 * 				If the list is empty or if the given index is out of bounds.
 */
template<typename T>
T LinkedList<T>::removeAt(int index) {
	if (_info == 0) {
		throw std::string("removeAt(), LinkedList is empty, cannot remove!");
	} else if (index == 0) {
		return remove();
	} else if (_next == 0) {
		throw std::string("removeAt(), Index out of bounds!");
	} else {
		return removeAt(index - 1);
	}
}

/**
 * Method which converts a LinkedList object to a ResizableArray object.
 * NOTE: The 0th index in the array is the front of the list.
 *
 * @return
 * 				The resizable array.
 *
 * @throws
 * 				If the list is empty.
 */
template<typename T>
ResizableArray<T>* LinkedList<T>::toResizableArray() {
	if (_info == 0) {
		throw std::string(
				"toResizableArray(), LinkedList is empty, cannot convert!");
	}
	ResizableArray<T> *array = new ResizableArray<T>(size());
	LinkedList<T> *currentItem = this;
	while (currentItem->_next != 0) {
		array->add(*(currentItem->_info));
		currentItem = currentItem->_next;
	}
	array->add(*(currentItem->_info));
	return array;
}

#endif /* LINKEDLIST_H_ */
