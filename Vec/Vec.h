#pragma once

#include <memory>

template<class T> class Vec
{
public:
	//class interface
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef T value_type;
	typedef size_t size_type;
	typedef T& reference;
	typedef const T& const_reference;

	//constructor, destructor, copy, assignment
	Vec() { create(); }
	~Vec() { uncreate(); }
	Vec(const Vec& v) { create(v.begin(), v.end());	}
	explicit Vec(size_type n, const T& val = T()) { create(n, val); }
	Vec& operator=(const Vec&);

	size_type size() const { return avail - data; }
	reference operator[](size_type i) { return data[i]; }
	const_reference operator[](size_type i) const { return data[i]; }
	iterator begin() { return data; }
	const_iterator begin() const { return data; }
	iterator end() { return avail; }
	const_iterator end() const { return avail; }
	void push_back(const T& val) {
		if (avail = limit)
		{
			grow();
		}
		unchecked_append(val);
	}

private:
	//class realization
	iterator data; //first element of array
	iterator limit; //element next for the last available element of array
	iterator avail; //element next for tha last created element of array
	std::allocator<T> alloc;

	//functions for placing array in memory and initializing it
	void create();
	void create(size_type, const_reference);
	void create(const_iterator, const_iterator);

	//functions for destroying objects and releasing memory
	void uncreate();

	//push_back functions
	void grow();
	void unchecked_append(const_reference);
};

template<class T>
Vec<T>& Vec<T>::operator=(const Vec& rhs) {
	if (&rhs != this)
	{
		//clear array on the left side
		uncreate();
		//copy rhs to lhs
		create(rhs.begin(), rhs.end());
	}
	return *this;
}

template<class T>
void Vec<T>::create() {
	data = avail = limit = 0;
}

template<class T> 
void Vec<T>::create(size_type n, const_reference val) {
	data = alloc.allocate(n);
	limit = avail = data + n;
	std::uninitialized_fill(data, limit, val);
}

template<class T>
void Vec<T>::create(const_iterator b, const_iterator e) {
	data = alloc.allocate(e - b);
	limit = avail = std::uninitialized_copy(b, e, data);
}