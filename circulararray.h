#include <iostream>
using namespace std;

template <class T>
class CircularArray
{
private:
    T* m_array;
    size_t m_capacity;
    size_t m_back;
    size_t m_front;

public:
    CircularArray();
    CircularArray(size_t _capacity);
    virtual ~CircularArray();
    void push_front(T data);
    void push_back(T data);
    void insert(T data, size_t pos);
    T pop_front();
    T pop_back();
    bool is_full();
    bool is_empty();
    size_t size();
    void clear();
    T& operator[](size_t index);
    void sort();
    bool is_sorted();
    void reverse();
    string to_string(string sep = " ");

private:
    size_t next(size_t index);
    size_t prev(size_t index);
};

template <class T>
CircularArray<T>::CircularArray()
{
    CircularArray(0);
}

template <class T>
CircularArray<T>::CircularArray(size_t _capacity)
{
    this->m_array = new T[_capacity];
    this->m_capacity = _capacity;

    this->m_front = -1;
    this->m_back = -1;
}

template <class T>
CircularArray<T>::~CircularArray()
{
    delete[] m_array;
}

template <class T>
size_t CircularArray<T>::prev(size_t index)
{
    return (index == 0) ? m_capacity - 1 : index - 1;
}

template <class T>
size_t CircularArray<T>::next(size_t index)
{
    return (index + 1) % m_capacity;
}

template <class T>
string CircularArray<T>::to_string(string sep)
{
    string result = "";
    for (size_t i = 0; i < size(); i++)
        result += std::to_string((*this)[i]) + sep;
    return result;
}
