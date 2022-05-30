#ifndef CIRCULARARRAY_H_
#define CIRCULARARRAY_H_

#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

#include "circulararray_iterator.hpp"

template<class T>
class CircularArray
{
private:
    friend class CircularArrayIterator<T>;

    T* m_array;
    T* m_array_end;

    size_t m_capacity;
    size_t m_size{0};

    T* m_back{nullptr};
    T* m_front{nullptr};

public:
    CircularArray(size_t capacity = 0)
        : m_array{new T[capacity]},
          m_array_end{m_array + capacity},
          m_capacity{capacity}
    {
    }

    virtual ~CircularArray()
    {
        this->clear();
    }

    void reserve(size_t size)
    {
        if (m_capacity > size)
        {
            return;
        }

        T* new_array = new T[size];
        T* next_to_add_new_array = new_array;

        for (auto it = this->begin(); it != this->end(); ++it)
        {
            *next_to_add_new_array = std::move((it));
            ++next_to_add_new_array;
        }

        delete[] m_array;
        m_array = new_array;

        m_array_end = new_array + size;
        m_capacity = size;

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

    void push_front(T data)
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

        *m_front = data;
        m_size++;
    }

    void push_back(T data)
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

        *m_back = data;
        m_size++;
    }

    void insert(T data, size_t pos)
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
            // This could be optimized by checking whether the index to insert is closer to the
            // front or back and moving the smallest range.

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

            *it = data;
        }
    }

    auto front() -> T&
    {
        return *m_front;
    }

    auto back() -> T&
    {
        return *m_back;
    }

    void pop_front()
    {
        if (m_size == 0)
        {
            throw std::runtime_error("Array is empty");
        }

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
    }

    void pop_back()
    {
        if (m_size == 0)
        {
            throw std::runtime_error("Array is empty");
        }

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
    }

    auto iterator_at(size_t index) const -> CircularArrayIterator<T>
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

    auto operator[](typename CircularArrayIterator<T>::difference_type index) -> T&
    {
        return *iterator_at(index);
    }

    auto begin() const -> CircularArrayIterator<T>
    {
        return CircularArrayIterator<T>(*this, m_front);
    }

    auto end() const -> CircularArrayIterator<T>
    {
        return CircularArrayIterator<T>(*this, m_array_end);
    }

    void enlarge()
    {
        size_t new_capacity = std::max((size_t)10, m_capacity << 1);
        this->reserve(new_capacity);
    }

    void sort()
    {
        std::sort(this->begin(), this->end());
    }

    void clear()
    {
        delete[] m_array;

        m_capacity = 0;
        m_size = 0;
        m_front = nullptr;
        m_back = nullptr;
    }

    void reverse()
    {
        std::reverse(this->begin(), this->end());
    }

    auto is_full() -> bool
    {
        return m_size == m_capacity;
    }

    auto is_empty() -> bool
    {
        return m_size == 0;
    }

    auto is_sorted() -> bool
    {
        return std::is_sorted(this->begin(), this->end());
    }

    [[nodiscard]] auto is_parted() const -> bool
    {
        return m_front != nullptr && m_front > m_back;
    }

    auto size() -> size_t
    {
        return m_size;
    }

    auto capacity() -> size_t
    {
        return m_capacity;
    }

    auto to_string(std::string sep = " ") -> std::string
    {
        std::string result = "";

        for (auto it = this->begin(); it != this->end(); it++)
        {
            result += std::to_string(*it) + sep;
        }

        return result;
    }

private:
    T* next(T* p)
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

    T* prev(T* p)
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
};

#endif /* CIRCULARARRAY_H_ */
