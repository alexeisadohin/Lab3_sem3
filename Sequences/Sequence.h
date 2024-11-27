#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "UnqPtr.h"
#include "ShrdPtr.h"

template <typename T>
class Sequence {
public:
    virtual ShrdPtr<Sequence<T>>  Append(const T& item) const = 0;
    virtual ShrdPtr<Sequence<T>> Prepend(const T& item) const = 0;
    virtual ShrdPtr<Sequence<T>> GetSubsequence(int startIndex, int endIndex) const  = 0;
    virtual ShrdPtr<Sequence<T>> InsertAt(const T& item, int index) const = 0;
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual const T& Get(int index) const = 0;
    virtual T& Get(int index) = 0;
    virtual int GetLength() const = 0;
    virtual ShrdPtr<Sequence<T>> Copy() const = 0;
    virtual ~Sequence() = default;
    virtual void Set(int index, const T& value) = 0;
};

#endif //SEQUENCE_H
