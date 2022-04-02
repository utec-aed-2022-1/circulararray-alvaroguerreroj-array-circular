#ifndef CIRCULARARRAY_ITERATOR_H_
#define CIRCULARARRAY_ITERATOR_H_

#include <iterator>
#include <stdexcept>

template <class T> class CircularArray;

// With help from https://stackoverflow.com/a/22801650
template <class T> class CircularArrayIterator
{
private:
    CircularArray<T> const& m_carr;
    T** m_ptr;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

public:
    CircularArrayIterator(CircularArray<T> const& carr, T** ptr = nullptr)
        : m_carr(carr),
          m_ptr(ptr)
    {
    }

    CircularArrayIterator(const CircularArrayIterator<T>& it)
        : m_carr(it.m_carr),
          m_ptr(it.m_ptr)
    {
    }

    CircularArrayIterator<T>& operator=(const CircularArrayIterator<T>& it)
    {
        m_ptr = it.m_ptr;

        return *this;
    }

    operator bool() const
    {
        return m_ptr != nullptr && m_ptr != m_carr.m_array_end;
        if (m_ptr)
            return true;
        else
            return false;
    }

    bool operator==(const CircularArrayIterator<T>& it) const
    {
        return m_ptr == it.m_ptr && &m_carr == &it.m_carr;
    }

    bool operator!=(const CircularArrayIterator<T>& it) const
    {
        return !(*this == it);
    }

    CircularArrayIterator<T>& operator+=(const difference_type& movement)
    {
        m_ptr = m_carr.iterator_at(this->index() + movement).m_ptr;
        return *this;
    }

    CircularArrayIterator<T>& operator-=(const difference_type& movement)
    {
        *this += -movement;
        return *this;
    }

    CircularArrayIterator<T>& operator++()
    {
        *this += 1;
        return *this;
    }

    CircularArrayIterator<T>& operator--()
    {
        *this -= 1;
        return *this;
    }

    CircularArrayIterator<T> operator++(int)
    {
        auto ret = *this;
        ++*this;
        return ret;
    }

    CircularArrayIterator<T> operator--(int)
    {
        auto ret = *this;
        --*this;
        return ret;
    }

    CircularArrayIterator<T> operator+(const difference_type& movement)
    {
        auto ret = *this;
        ret += movement;
        return ret;
    }

    CircularArrayIterator<T> operator-(const difference_type& movement)
    {
        auto ret = *this;
        ret -= movement;
        return ret;
    }

    difference_type operator-(const CircularArrayIterator<T>& it)
    {
        if (m_carr.is_parted())
        {
            return this->index() - it.index();
        }
        else
        {
            return std::distance(m_ptr, it.m_ptr);
        }
    }

    /**
     * Returns the index of the iterator on the circular array.
     *
     * This is **not** the index of the iterator on the underlying array.
     *
     * @return The index of the iterator within the circular array
     */
    difference_type index() const
    {
        if (m_ptr == m_carr.m_array_end)
        {
            return m_carr.m_size;
        }
        else if (m_carr.m_front <= m_ptr)
        {
            return std::distance(m_carr.m_front, m_ptr);
        }
        else
        {
            return m_carr.m_capacity - std::distance(m_ptr, m_carr.m_front);
        }
    }

    bool operator<(CircularArrayIterator<T> const& other)
    {
        if (!m_carr.is_parted())
        {
            return this->m_ptr < other.m_ptr;
        }
        else if (this->between_front_and_array_end())
        {
            if (other.between_front_and_array_end())
            {
                return this->m_ptr < other.m_ptr;
            }
            else
            {
                return true;
            }
        }
        else if (other.between_front_and_array_end())
        {
            return false;
        }
        else
        {
            return this->m_ptr < other.m_ptr;
        }
    }

    /**
     * Returns true if the iterator is between the front (included) and the
     * array end (not included) in the underlying array.
     *
     * @return A boolean
     */
    bool between_front_and_array_end() const
    {
        return (m_carr.m_front <= m_ptr) && (m_ptr < m_carr.m_array_end);
    }

    T& operator*()
    {
        return **m_ptr;
    }

    const T& operator*() const
    {
        return **m_ptr;
    }

    T* operator->()
    {
        return *m_ptr;
    }

    T* getPtr() const
    {
        return m_ptr;
    }

    const T* getConstPtr() const
    {
        return m_ptr;
    }
};

#endif // CIRCULARARRAY_ITERATOR_H_
