#ifndef QUEUE_H_
#define QUEUE_H_

#include "circulararray.hpp"

// Inheritance should be private because we don't intend the sub classes (QueueArray and
// StackArray) to be able to substitute an object of the superclass CircularArray, but we are
// only inheriting for code reuse.
//
// An alternative would be to do composition and have QueueArray (or StackArray) hold an
// CircularArray as one of its members.

template <typename T>
class QueueArray : private CircularArray<T>
{
    void enqueue(T data)
    {
        this->push_back(data);
    }

    T dequeue()
    {
        return this->pop_front();
    }

    using CircularArray<T>::begin;
    using CircularArray<T>::end;

    using CircularArray<T>::to_string;
};

#endif /* QUEUE_H_ */
