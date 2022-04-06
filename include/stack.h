#ifndef STACK_H_
#define STACK_H_

#include "circulararray.h"

template <typename T>
class StackArray : private CircularArray<T>
{
    void push(T data)
    {
        this->push_back(data);
    }

    T pop()
    {
        return this->pop_back();
    }

    using CircularArray<T>::begin;
    using CircularArray<T>::end;

    using CircularArray<T>::to_string;
};

#endif /* STACK_H_ */
