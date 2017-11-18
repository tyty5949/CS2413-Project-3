/*
 * HashTableBucket.h, the header only class for hash table buckets.
 *
 * Class: CS2413
 * Author: Tyler Hunt (113401590)
 * Date: 11/17/17
 * Assignment: Project #3
 */
#ifndef HASHTABLEBUCKET_H_
#define HASHTABLEBUCKET_H_

/**
 * The class for encapsulated needed data for hash table buckets of any arbitrary type.
 * Note: Most commonly of type HashTableIte<T1,T2> to arbitrarily encapsulate some type
 * 	T1 and type T2.
 */
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

/**
 * Default constructor;
 */
template <typename T>
HashTableBucket<T>::HashTableBucket() {
	_emptySinceBeginning = true;
	_value = 0;
}

/**
 * Checks whether or not this bucket as been empty since its creation.
 * Note. Buckets are re-created during every re-hash to reset this value.
 */
template <typename T>
bool HashTableBucket<T>::isEmptySinceBeginning() {
	return _emptySinceBeginning;
}

/**
 * Checks whether or not this bucket currently has an arbitrary type stored in it.
 */
template <typename T>
bool HashTableBucket<T>::isEmpty() {
	return (_value == 0);
}

/**
 * Gets the pointer to the arbitrary value of type T currently stored in this
 * bucket.
 * Note. The pointer to its original creation, no copies.
 */
template <typename T>
T* HashTableBucket<T>::getValue() {
	return _value;
}

/**
 * Sets the pointer to the arbitrary value of type T to be stored in this bucket.
 * Note. The pointer to its original creation, no copies.
 */
template <typename T>
void HashTableBucket<T>::setValue(T* value) {
	_value = value;
	_emptySinceBeginning = false;
}

/**
 * Destructor.
 */
template <typename T>
HashTableBucket<T>::~HashTableBucket() {

}

#endif /* HASHTABLEBUCKET_H_ */
