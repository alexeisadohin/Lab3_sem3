#ifndef ARRAYSEQUENCE_H
#define ARRAYSEQUENCE_H

#include "Sequence.h"
#include "DynamicArray.h"
#include <iostream>

template <typename T>
class ArraySequence : public Sequence<T> {
private:
    ShrdPtr<DynamicArray<T>> array;

public:
    ArraySequence() : array(new DynamicArray<T>(0)) {}
    ArraySequence(ShrdPtr<DynamicArray<T>>&& arr) : array(std::move(arr)) {}
    ArraySequence(T* items, int count) : array(new DynamicArray<T>(items, count)) {}
    ArraySequence(const ArraySequence<T>& arraySequence) : array(new DynamicArray<T>(*arraySequence.array)) {}
    ArraySequence(const ShrdPtr<DynamicArray<T>>& otherArray) : array(otherArray) {}


    ShrdPtr<Sequence<T>>  Append(const T& item) const override {
        ShrdPtr<DynamicArray<T>> newArray(new DynamicArray<T>(this->array->GetSize() + 1));
        for (int i = 0; i < this->array->GetSize(); ++i) {
            newArray->Set(i, this->array->Get(i));
        }
        newArray->Set(newArray->GetSize() - 1, item);
        return ShrdPtr<Sequence<T>>(new ArraySequence<T>(newArray));
    }

    ShrdPtr<Sequence<T>> Prepend(const T& item) const override {
        ShrdPtr<DynamicArray<T>> newArray(new DynamicArray<T>(this->array->GetSize() + 1));
        newArray->Set(0, item);
        for (int i = 0; i < this->array->GetSize(); ++i) {
            newArray->Set(i + 1, this->array->Get(i));
        }
        return ShrdPtr<Sequence<T>>(new ArraySequence<T>(std::move(newArray)));
    }

    ShrdPtr<Sequence<T>> InsertAt(const T& item, int index) const override {
        ShrdPtr<DynamicArray<T>> newArray(new DynamicArray<T>(this->array->GetSize() + 1));
        for (int i = 0; i < index; ++i) {
            newArray->Set(i, this->array->Get(i));
        }
        newArray->Set(index, item);
        for (int i = index; i < this->array->GetSize(); ++i) {
            newArray->Set(i + 1, this->array->Get(i));
        }
        return ShrdPtr<Sequence<T>>(new ArraySequence<T>(std::move(newArray)));
    }

    ShrdPtr<Sequence<T>> GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || startIndex >= array->GetSize() || endIndex < 0 || endIndex >= array->GetSize() || startIndex > endIndex) {
            throw std::out_of_range("IndexOutOfRange");
        }
        ShrdPtr<DynamicArray<T>> newArray(new DynamicArray<T>(endIndex - startIndex + 1));
        for (int i = startIndex; i <= endIndex; i++) {
            newArray->Set(i - startIndex, array->Get(i));
        }
        return ShrdPtr<Sequence<T>>(new ArraySequence<T>(std::move(newArray)));
    }

    T GetFirst() const override {
        if (array->GetSize() == 0) {
            throw std::out_of_range("IndexOutOfRange");
        }
        return array->Get(0);
    }

    T GetLast() const override {
        if (array->GetSize() == 0) {
            throw std::out_of_range("IndexOutOfRange");
        }
        return array->Get(array->GetSize() - 1);
    }

    const T& Get(int index) const override {
        if (index < 0 || index >= array->GetSize()) {
            std::cerr << "Error in get: Index " << index << " out of range [0, " << array->GetSize() - 1 << "]" << std::endl;
            throw std::out_of_range("Index out of range");
        }
        return array->Get(index);
    }
    T& Get(int index) override {
        if (index < 0 || index >= array->GetSize()) {
            std::cerr << "Error in get: Index " << index << " out of range [0, " << array->GetSize() - 1 << "]" << std::endl;
            throw std::out_of_range("Index out of range");
        }
        return array->Get(index);
    }

    void Set(int index, const T& value) override {
        if (index < 0 || index >= array->GetSize()) {
            std::cerr << "Error in set: Index " << index << " out of range [0, " << array->GetSize() - 1 << "]" << std::endl;
            throw std::out_of_range("Index out of range");
        }
        array->Set(index, value);
    }

    int GetLength() const override {
        return array->GetSize();
    }

    ShrdPtr<Sequence<T>> Copy() const override {
        return ShrdPtr<Sequence<T>>(new ArraySequence<T>(this->array));
    }

    ~ArraySequence() = default;
};

#endif // ARRAYSEQUENCE_H