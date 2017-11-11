/**
 * Header only class which encapsulates a dynamic array for any
 * type of data.
 *
 * Class: CS2414
 * Author: Tyler Hunt (113401590)
 * Date: 9/29/17
 * Assignment: Lab #4
 */
// TODO Javadoc
#ifndef RESIZABLEARRAY_H_
#define RESIZABLEARRAY_H_

#include "Eclipse.h"

/**
 * Helper function which resizes the size of any type of dynamic array.
 *
 * @param array -
 * 			The dynamic array to be resized.
 * @param oldSize -
 * 			The current size of the array.
 * @param newSize -
 * 			The size of the array after resizing.
 */
template<typename T>
void resizeArray(T *&array, int oldSize, int newSize) {
	T *newArray = new T[newSize];

	for (int i = 0; i < oldSize; i++) {
		*(newArray + i) = *(array + i);
	}

	delete[] array;
	array = newArray;
}

/**
 * Templated class to encapsulate a dynamic array.
 */
template<typename T>
class ResizableArray {
	template<typename T1>
	friend std::ostream& operator<<(std::ostream& s, ResizableArray<T1>& array);
public:
	ResizableArray<T>();
	ResizableArray<T>(int initialSize);
	ResizableArray<T>(ResizableArray &array);
	void add(T object);
	void addAt(T object, int index);
	void replaceAt(T object, int index);
	void removeAt(int index);
	int size();
	T& get(int index);
	void sort(int (*compFunc)(T obj1, T obj2));
	std::string toString();
	~ResizableArray();

private:
	int partition(T *array, int left, int right,
			int (*compFunc)(T obj1, T obj2));
	void quickSort(T *array, int left, int right, int (*compFunc)(T obj1, T obj2));
	int _arraySize;
	int _dataAmount;
	T *_array;
};

/**
 * Overloaded << operator to print out list of objects in array.
 * Note: Prints in reverse order.
 */
// TODO - Should this be in this class or the Eclipse class?
/*
 std::ostream& operator<<(std::ostream& s, ResizableArray<Eclipse>& array) {
 for (int i = array._dataAmount - 1; i >= 0; i--) {
 s << array.get(i).toString() << std::endl;
 }
 return s;
 }
 */

template<typename T>
std::ostream& operator<<(std::ostream& s, ResizableArray<Eclipse>& array) {
	for (int i = array._dataAmount - 1; i >= 0; i--) {
		s << array.get(i).toString() << std::endl;
	}
	return s;
}

template<typename T>
std::string ResizableArray<T>::toString() {
	std::string s;
	for (int i = 0; i < _dataAmount; i++) {
		s += _array[i].toString() + "\n";
	}
	return s;
}

/**
 * Default constructor which initializes size of the array to 10.
 */
template<typename T>
ResizableArray<T>::ResizableArray() {
	_arraySize = 10;
	_array = new T[_arraySize];
	_dataAmount = 0;
}

/**
 * Initializes array to given size.
 *
 * @param initialSize -
 * 			The size of the array.
 */
template<typename T>
ResizableArray<T>::ResizableArray(int initialSize) {
	_arraySize = initialSize;
	_array = new T[_arraySize];
	_dataAmount = 0;
}

/**
 * Copy constructor.
 *
 * @param array -
 * 			The array to be copied.
 */
template<typename T>
ResizableArray<T>::ResizableArray(ResizableArray &array) {
	_arraySize = array._arraySize;
	_array = new T[_arraySize];
	_dataAmount = array._dataAmount;
	for (int i = 0; i < _dataAmount; i++) {
		*(_array + i) = *(array._array + i);
	}
}

/**
 * Function which adds a templated type object to the end of the array.
 * Note: Size of array is doubled if necessary.
 *
 * @param object -
 * 			The object of templated type to be added.
 */
template<typename T>
void ResizableArray<T>::add(T object) {
	if (_dataAmount + 1 > _arraySize) {
		resizeArray(_array, _arraySize, _arraySize * 2);
		_arraySize *= 2;
	}
	*(_array + (_dataAmount)) = object;
	_dataAmount++;
}

/**
 * Function which adds a templated type object at a given position
 * in the array. Shifts all other objects right 1 index.
 * Note: Size of array is doubled if necessary.
 *
 * @param object -
 * 			The object of templated type to be added.
 * @param index -
 * 			The index to add the object.
 */
template<typename T>
void ResizableArray<T>::addAt(T object, int index) {
	if (index < 0 || index >= _dataAmount) {
		throw -1;
	}
	if (_dataAmount + 1 > _arraySize) {
		resizeArray(_array, _arraySize, _arraySize * 2);
		_arraySize *= 2;
	}
	for (int i = _dataAmount - 1; i >= index; i--) {
		*(_array + i + 1) = *(_array + i);
	}
	*(_array + index) = object;
	_dataAmount++;
}

/**
 * Function which replaces an object at the specified index
 * with a given object.
 *
 * @param object -
 * 			The object of templated type to be added.
 * @param index -
 * 			The index in array to be replaced.
 */
template<typename T>
void ResizableArray<T>::replaceAt(T object, int index) {
	if (index < 0 || index >= _dataAmount) {
		throw -2;
	}
	*(_array + index) = object;
}

/**
 * Function to remove an object in array at the given index. All
 * other objects are shifted left by index - 1.
 * Note: Array size is halved if necessary.
 *
 * @param index -
 * 			The index in the arrray to be removed.
 */
template<typename T>
void ResizableArray<T>::removeAt(int index) {
	if (index < 0 || index >= _dataAmount) {
		throw -3;
	}
	for (int i = index; i < _dataAmount - 1; i++) {
		*(_array + i) = *(_array + i - 1);
	}
	if (_dataAmount - 1 <= _arraySize / 2) {
		resizeArray(_array, _arraySize, _arraySize / 2);
		_arraySize /= 2;
	}
	_dataAmount--;
}

/**
 * Gets the number of objects stored in the array.
 *
 * @return -
 * 			The number of objects stored in the array.
 */
template<typename T>
int ResizableArray<T>::size() {
	return _dataAmount;
}

/**
 * Gets an object of templated type at given index in the array.
 *
 * @param index -
 * 			The index in array.
 * @return -
 * 			The object at the given index.
 */
template<typename T>
T& ResizableArray<T>::get(int index) {
	if (index < 0 || index >= _dataAmount) {
		throw -4;
	}
	return *(_array + index);
}

template<typename T>
void ResizableArray<T>::sort(int (*compFunc)(T obj1, T obj2)) {
	quickSort(_array, 0, _dataAmount - 1, compFunc);
}


template<typename T>
void ResizableArray<T>::quickSort(T *array, int left, int right, int (*compFunc)(T obj1, T obj2)) {
	T pivot = array[(left + right) / 2];
	int i = left;
	int j = right;

	while (i <= j) {
		while (compFunc(array[i], pivot) == -1)	// array[i] < pivot
			i++;
		while (compFunc(array[j], pivot) == 1)	// array[j] > pivot
			j--;
		if (i <= j) {
			T tmp = array[i];
			array[i] = array[j];
			array[j] = tmp;
			i++;
			j--;
		}
	};

	if (left < j)
		quickSort(array, left, j, compFunc);
	if (i < right)
		quickSort(array, i, right, compFunc);
}

/**
 * Destructor
 */
template<typename T>
ResizableArray<T>::~ResizableArray() {
	// De-allocate the memory of the array.
	delete[] _array;
}

#endif /* RESIZABLEARRAY_H_ */
