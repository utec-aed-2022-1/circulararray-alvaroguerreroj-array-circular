#pragma once

#include <iterator>
#include <stdexcept>

template<class T>
class CircularArray;

// With help from https://stackoverflow.com/a/22801650
template<class T>
class CircularArrayIterator
{
private:
    CircularArray<T> const& m_carr;
    T* m_ptr;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

public:
    CircularArrayIterator(CircularArray<T> const& carr, T* ptr = nullptr)
        : m_carr(carr),
          m_ptr(ptr)
    {
    }

    CircularArrayIterator(CircularArrayIterator<T> const& it)
        : m_carr(it.m_carr),
          m_ptr(it.m_ptr)
    {
    }

    auto operator=(CircularArrayIterator<T> const& it) -> CircularArrayIterator<T>&
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

    auto operator==(CircularArrayIterator<T> const& it) const -> bool
    {
        return m_ptr == it.m_ptr && &m_carr == &it.m_carr;
    }

    auto operator!=(CircularArrayIterator<T> const& it) const -> bool
    {
        return !(*this == it);
    }

    auto operator+=(difference_type const& movement) -> CircularArrayIterator<T>&
    {
        m_ptr = m_carr.iterator_at(this->index() + movement).m_ptr;
        return *this;
    }

    auto operator-=(difference_type const& movement) -> CircularArrayIterator<T>&
    {
        *this += -movement;
        return *this;
    }

    auto operator++() -> CircularArrayIterator<T>&
    {
        *this += 1;
        return *this;
    }

    auto operator--() -> CircularArrayIterator<T>&
    {
        *this -= 1;
        return *this;
    }

    auto operator++(int) -> CircularArrayIterator<T>
    {
        auto ret = *this;
        ++*this;
        return ret;
    }

    auto operator--(int) -> CircularArrayIterator<T>
    {
        auto ret = *this;
        --*this;
        return ret;
    }

    auto operator+(difference_type const& movement) const -> CircularArrayIterator<T>
    {
        auto ret = *this;
        ret += movement;
        return ret;
    }

    auto operator-(difference_type const& movement) const-> CircularArrayIterator<T>
    {
        auto ret = *this;
        ret -= movement;
        return ret;
    }

    auto operator-(CircularArrayIterator<T> const& it) const -> difference_type
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
    auto index() const -> difference_type
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

    auto operator<(CircularArrayIterator<T> const& other) -> bool
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
    auto between_front_and_array_end() const -> bool
    {
        return (m_carr.m_front <= m_ptr) && (m_ptr < m_carr.m_array_end);
    }

    auto operator*() -> T&
    {
        return *m_ptr;
    }

    auto operator*() const -> const T&
    {
        return *m_ptr;
    }

    auto operator->() -> T*
    {
        return m_ptr;
    }

    auto getPtr() const -> T*
    {
        return m_ptr;
    }

    auto getConstPtr() const -> const T*
    {
        return m_ptr;
    }
};

template<typename T>
struct std::iterator_traits<CircularArrayIterator<T>>
{
    using difference_type = typename CircularArrayIterator<T>::difference_type;
    using value_type = typename CircularArrayIterator<T>::value_type;
    using pointer = typename CircularArrayIterator<T>::pointer;
    using reference = typename CircularArrayIterator<T>::reference;
    using iterator_category = typename CircularArrayIterator<T>::iterator_category;
};
