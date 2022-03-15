#pragma once

#include <memory>
#include <cmath>
#include<algorithm>

template<typename T> class DynamicArray
{
public:
	//class interface
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef T value_type;
	typedef size_t size_type;
	typedef T& reference;
	typedef const T& const_reference;

	//constructor, destructor
	DynamicArray(): data(0), limit(0), avail(0) {}
	~DynamicArray() { clear(); }
	explicit DynamicArray(size_type n, const T& val = T()) { create(n, val); }
	//copy ctor 
	DynamicArray(const DynamicArray& v) { create(v.begin(), v.end()); }
	//copy assignment
	DynamicArray& operator=(const DynamicArray&);
	//move ctor
	DynamicArray(DynamicArray&& rval)
	: data(std::exchange(rval.data, nullptr)), limit(std::exchange(rval.limit, nullptr)), avail(std::exchange(rval.avail, nullptr))
	{ std::cout << "move constr" << std::endl; }
	//move assignment 
	DynamicArray& operator=(DynamicArray&& rval) {
		std::cout << "move assignment" << std::endl;
		std::swap(data, rval.data);
		std::swap(limit, rval.limit);
		std::swap(avail, rval.avail);

		return *this;
	}

	size_type size() const { return avail - data; }
	T& operator[](size_type i) { return data[i]; }
	const T& operator[](size_type i) const { return data[i]; }
	iterator begin() { return data; }
	const_iterator begin() const { return data; }
	iterator end() { return avail; }
	const_iterator end() const { return avail; }
	void push_back(const T& val) {
		if (avail == limit)
			grow();
		unchecked_append(val);
	}

	T* erase(iterator);

private:
	//class realization
	iterator data; //first element of array
	iterator limit; //element next for the last available element of array
	iterator avail; //element next for tha last created element of array
	std::allocator<T> alloc;

	//functions for placing array in memory and initializing it
	void create(size_type, const_reference);
	void create(const_iterator, const_iterator);

	//functions for destroying objects and releasing memory
	void clear();

	//push_back functions
	void grow();
	void unchecked_append(const_reference);
};

template<class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& rhs) {
	if (&rhs != this)
	{
		//clear array on the left hand side
		clear();
		//copy rhs to lhs
		create(rhs.begin(), rhs.end());
	}
	return *this;
}

template<class T> 
void DynamicArray<T>::create(size_type n, const_reference val) {
	data = alloc.allocate(n);
	limit = avail = data + n;
	std::uninitialized_fill(data, limit, val);
}

template<class T>
void DynamicArray<T>::create(const_iterator b, const_iterator e) {
	data = alloc.allocate(e - b);
	limit = avail = std::uninitialized_copy(b, e, data);
}

template<class T>
void DynamicArray<T>::clear() {
	if (data)
	{
		iterator it = avail;
		while (it >= data)
		{
			alloc.destroy(--it);
		}
		alloc.deallocate(data, limit - data);
	}
	data = avail = limit = 0;
}

template<class T> 
void DynamicArray<T>::grow() {
	//calculate new size
	size_type new_size = std::max(2 * (limit - data), ptrdiff_t(1));

	//reserve more memory and copy existing objects to it
	iterator new_data = alloc.allocate(new_size);
	iterator new_avail = std::uninitialized_copy(data, avail, new_data);

	//release old memory 
	clear();

	//set new memory area
	data = new_data;
	avail = new_avail;
	limit = data + new_size;
}

template<class T>
void DynamicArray<T>::unchecked_append(const_reference val) {
	//create new object next to last created
	alloc.construct(avail++, val);
}

template<class T>
T* DynamicArray<T>::erase(iterator it) {
	iterator ret = it;
	if (it >= data || it < avail)
	{		
		while (it < avail)
		{
			T* p_it = it++;
			*p_it = *it;
		}
		avail = --it;
	}
	return ret;
}