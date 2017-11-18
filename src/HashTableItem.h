/*
 * HashTableItem.cpp, The header only class for hash table items.
 *
 * Class: CS2413
 * Author: Tyler Hunt (113401590)
 * Date: 11/17/17
 * Assignment: Project #3
 */
#ifndef HASHTABLEITEM_H_
#define HASHTABLEITEM_H_

/**
 * The HashTableItem class encapsulates an arbitrary key and value type
 * for use with hash tables.
 */
template<typename T1, typename T2>
class HashTableItem {
public:
	HashTableItem<T1, T2>();
	HashTableItem<T1, T2>(T1& key, T2* value);
	~HashTableItem<T1, T2>();
	T1 getKey();
	T2* getValue();

private:
	T1 _key;
	T2* _value;
};

/**
 * Default constructor;
 */
template<typename T1, typename T2>
HashTableItem<T1, T2>::HashTableItem() {
	_key = -1;
	_value = 0;
}

/**
 * Constructor that initializes the key and value.
 *
 * @param key-
 * 				A reference to an arbitrary key for this item.
 * @param value-
 * 				A pointer to an arbitrary value for this item.
 */
template<typename T1, typename T2>
HashTableItem<T1, T2>::HashTableItem(T1& key, T2* value) {
	_key = key;
	_value = value;
}

/**
 * Gets the key of this item of the arbitrary key type.
 *
 * @return -
 * 				The key for this item.
 */
template<typename T1, typename T2>
T1 HashTableItem<T1, T2>::getKey() {
	return _key;
}

/**
 * Gets the value of this item of the arbitrary value type.
 * Note: Returns the pointer to the original constructed item, no copies.
 *
 * @return -
 * 				The pointer to the value for this item.
 */
template<typename T1, typename T2>
T2* HashTableItem<T1, T2>::getValue() {
	return _value;
}

/*
 * Destructor
 */
template<typename T1, typename T2>
HashTableItem<T1, T2>::~HashTableItem() {

}

#endif /* HASHTABLEITEM_H_ */
