#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

#include "circulararray_iterator.h"

template <class T>
class CircularArray
{
private:
    friend class CircularArrayIterator<T>;

    T** m_array;
    T** m_array_end;

    size_t m_capacity;
    size_t m_size;

    // These pointers point to the the location where an element added to their
    // respective direction would be stored.
    T** m_back;
    T** m_front;

public:
    CircularArray();
    CircularArray(size_t capacity);

    virtual ~CircularArray();

    void push_front(T data);
    void push_back(T data);
    void insert(T data, size_t pos);

    T pop_front();
    T pop_back();
    CircularArrayIterator<T> iterator_at(size_t index) const;
    T& operator[](typename CircularArrayIterator<T>::difference_type index);

    CircularArrayIterator<T> begin() const;
    CircularArrayIterator<T> end() const;

    void enlarge();
    void sort();
    void clear();
    void reverse();

    bool is_full();
    bool is_empty();
    bool is_sorted();
    bool is_parted() const;

    size_t size();
    size_t capacity();

    std::string to_string(std::string sep = " ");

private:
    T** next(T** p);
    T** prev(T** p);
};

template <class T>
CircularArray<T>::CircularArray()
{
    CircularArray(0);
}

template <class T>
CircularArray<T>::CircularArray(size_t capacity)
{
    m_array = new T*[capacity];
    m_array_end = m_array + capacity;

    m_capacity = capacity;
    m_size = 0;

    m_front = nullptr;
    m_back = nullptr;
}

template <class T>
CircularArray<T>::~CircularArray()
{
    this->clear();

    delete[] m_array;
}

template <class T>
void CircularArray<T>::push_front(T data)
{
    if (m_size >= m_capacity)
    {
        enlarge();
    }

    if (m_front == nullptr)
    {
        m_front = m_array;
        m_back = m_array;
    }
    else
    {
        m_front = prev(m_front);
    }

    *m_front = new T(data);
    m_size++;
}

template <class T>
void CircularArray<T>::push_back(T data)
{
    if (m_size >= m_capacity)
    {
        enlarge();
    }

    if (m_front == nullptr)
    {
        m_front = m_array;
        m_back = m_array;
    }
    else
    {
        m_back = next(m_back);
    }

    *m_back = new T(data);
    m_size++;
}

template <class T>
void CircularArray<T>::insert(T data, size_t pos)
{
    if (pos > m_size)
    {
        throw std::runtime_error("Index out of range");
    }
    else if (m_size == 0)
    {
        this->push_back(data);
    }
    else if (pos == 0)
    {
        this->push_front(data);
    }
    else
    {
        // This could be optimized by checking whether the index to insert is closer
        // to the front or back and moving the smallest range.

        push_back(T()); // Only to add one element

        auto it_trailing = m_back;
        delete *it_trailing;
        auto it = this->prev(m_back);

        *it_trailing = *it;

        while (pos != 0)
        {
            it_trailing = it;
            it = this->prev(it);

            *it_trailing = *it;

            pos--;
        }

        *it = new T(data);
    }
}

template <class T>
T CircularArray<T>::pop_front()
{
    if (m_size == 0)
    {
        throw std::runtime_error("Array is empty");
    }

    T ret = **m_front;
    delete *m_front;

    if (m_size == 1)
    {
        m_front = nullptr;
        m_back = nullptr;
    }
    else
    {
        m_front = next(m_front);
    }

    m_size--;
    return ret;
}

template <class T>
T CircularArray<T>::pop_back()
{
    if (m_size == 0)
    {
        throw std::runtime_error("Array is empty");
    }

    T ret = **m_back;
    delete *m_back;

    if (m_size == 1)
    {
        m_front = nullptr;
        m_back = nullptr;
    }
    else
    {
        m_back = prev(m_back);
    }

    m_size--;
    return ret;
}

template <class T>
CircularArrayIterator<T> CircularArray<T>::iterator_at(size_t index) const
{
    if (index == m_size)
    {
        return CircularArrayIterator<T>(*this, m_array_end);
    }
    else if (!is_parted())
    {
        return CircularArrayIterator<T>(*this, m_front + index);
    }
    else
    {
        size_t positions_remaining = std::distance(m_front, m_array_end) - 1;

        if (positions_remaining >= index)
        {
            return CircularArrayIterator<T>(*this, m_front + index);
        }
        else
        {
            size_t index_remaining = index - positions_remaining - 1;
            return CircularArrayIterator<T>(*this, m_array + index_remaining);
        }
    }
}

template <class T>
T& CircularArray<T>::operator[](typename CircularArrayIterator<T>::difference_type index)
{
    return *iterator_at(index);
}

template <class T>
CircularArrayIterator<T> CircularArray<T>::begin() const
{
    return CircularArrayIterator<T>(*this, m_front);
}

template <class T>
CircularArrayIterator<T> CircularArray<T>::end() const
{
    return CircularArrayIterator<T>(*this, m_array_end);
}

template <class T>
void CircularArray<T>::enlarge()
{
    size_t new_capacity = std::max((size_t)10, m_capacity << 1);

    T** new_array = new T*[new_capacity];
    T** next_to_add_new_array = new_array;

    T** it = m_front;
    for (size_t i = 0; i < m_size; i++)
    {
        *next_to_add_new_array = *it;

        next_to_add_new_array++;
        it = this->next(it);
    }

    delete[] m_array;

    m_array = new_array;
    m_array_end = new_array + new_capacity;
    m_capacity = new_capacity;

    if (m_size == 0)
    {
        m_front = nullptr;
        m_back = nullptr;
    }
    else
    {
        m_front = m_array;
        m_back = m_array + m_size - 1;
    }
}

template <class T>
void CircularArray<T>::reverse()
{
    std::reverse(this->begin(), this->end());
}

template <class T>
void CircularArray<T>::sort()
{
    std::sort(this->begin(), this->end());
}

template <class T>
void CircularArray<T>::clear()
{
    for (auto it = begin(); it != end(); it++)
    {
        delete (*it.getPtr());
    }

    m_size = 0;
    m_front = nullptr;
    m_back = nullptr;
}

template <class T>
T** CircularArray<T>::prev(T** p)
{
    if (p == m_array)
    {
        return std::prev(m_array_end);
    }
    else
    {
        return std::prev(p);
    }
}

template <class T>
T** CircularArray<T>::next(T** p)
{
    if (p == std::prev(m_array_end))
    {
        return m_array;
    }
    else
    {
        return std::next(p);
    }
}

template <class T>
bool CircularArray<T>::is_full()
{
    return m_size == m_capacity;
}

template <class T>
bool CircularArray<T>::is_empty()
{
    return m_size == 0;
}

template <class T>
bool CircularArray<T>::is_sorted()
{
    return std::is_sorted(this->begin(), this->end());
}

template <class T>
bool CircularArray<T>::is_parted() const
{
    return m_front != nullptr && m_front > m_back;
}

template <class T>
size_t CircularArray<T>::capacity()
{
    return m_capacity;
}

template <class T>
size_t CircularArray<T>::size()
{
    return m_size;
}

template <class T>
std::string CircularArray<T>::to_string(std::string sep)
{
    std::string result = "";

    for (auto it = this->begin(); it != this->end(); it++)
    {
        result += std::to_string(*it) + sep;
    }

    return result;
}
