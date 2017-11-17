/*
 * HashTableItem.h
 *
 *  Created on: Nov 17, 2017
 *      Author: Tyler
 */

#ifndef HASHTABLEITEM_H_
#define HASHTABLEITEM_H_

template <typename T1, typename T2>
class HashTableItem {
public:
	HashTableItem<T1,T2>();
	HashTableItem<T1,T2>(T1 key, T2* value);
	~HashTableItem<T1,T2>();
	T1 getKey();
	T2* getValue();

private:
	T1 _key;
	T2* _value;
};

template <typename T1, typename T2>
HashTableItem<T1,T2>::HashTableItem() {
	_key = -1;
	_value = 0;
}

template <typename T1, typename T2>
HashTableItem<T1,T2>::HashTableItem(T1 key, T2* value) {
	_key = key;
	_value = value;
}

template <typename T1, typename T2>
T1 HashTableItem<T1,T2>::getKey() {
	return _key;
}

template <typename T1, typename T2>
T2* HashTableItem<T1,T2>::getValue() {
	return _value;
}

template <typename T1, typename T2>
HashTableItem<T1,T2>::~HashTableItem() {

}

#endif /* HASHTABLEITEM_H_ */
