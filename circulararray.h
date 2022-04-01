#include <iostream>
#include <iterator>
#include <string>

template <class T>
class CircularArray
{
private:
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
    T& operator[](size_t index);

    void sort();
    void clear();
    void reverse();

    bool is_full();
    bool is_empty();
    bool is_sorted();
    bool is_parted();

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

    m_front = m_array;
    m_back = m_array;
}

template <class T>
CircularArray<T>::~CircularArray()
{
    delete[] m_array;
}

template <class T>
T** CircularArray<T>::prev(T** p)
{
    if (p == m_array)
    {
        return m_array_end;
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
bool CircularArray<T>::is_parted()
{
    return m_front != nullptr && m_front > m_back;
}

template <class T>
size_t CircularArray<T>::capacity()
{
    return m_capacity;
}
template <class T>
std::string CircularArray<T>::to_string(std::string sep)
{
    std::string result = "";
    for (size_t i = 0; i < size(); i++)
        result += std::to_string((*this)[i]) + sep;
    return result;
}
