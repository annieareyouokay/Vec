#include <iostream>

#include "DynamicArray.h"

DynamicArray<int> func(DynamicArray<int> d) {
	return d;
}

int main()
{
	
	DynamicArray<int> arr, arr2;
	DynamicArray<int>::size_type size = arr.size();
	size = 12;
	for (size_t i = 0; i < size; i++)
	{	
		arr.push_back(i);
	}
	
//	arr2 = func(DynamicArray<int>());
	arr2 = std::move(arr);
	std::cout << arr2.size();
	return 0;
}