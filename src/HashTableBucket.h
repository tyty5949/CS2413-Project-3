/*
 * HashTableItem.h
 *
 *  Created on: Nov 17, 2017
 *      Author: Tyler
 */

#ifndef HASHTABLEBUCKET_H_
#define HASHTABLEBUCKET_H_

template <typename T>
class HashTableBucket {
public:
	HashTableBucket<T>();
	~HashTableBucket<T>();
	bool isEmptySinceBeginning();
	bool isEmpty();
	T* getValue();
	void setValue(T* value);

private:
	bool _emptySinceBeginning;
	T* _value;
};

template <typename T>
HashTableBucket<T>::HashTableBucket() {
	_emptySinceBeginning = true;
	_value = 0;
}

template <typename T>
bool HashTableBucket<T>::isEmptySinceBeginning() {
	return _emptySinceBeginning;
}

template <typename T>
bool HashTableBucket<T>::isEmpty() {
	return (_value == 0);
}

template <typename T>
T* HashTableBucket<T>::getValue() {
	return _value;
}

template <typename T>
void HashTableBucket<T>::setValue(T* value) {
	_value = value;
	_emptySinceBeginning = false;
}

template <typename T>
HashTableBucket<T>::~HashTableBucket() {

}

#endif /* HASHTABLEBUCKET_H_ */
