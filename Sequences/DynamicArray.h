#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <stdexcept>

template <typename T>
class DynamicArray {
private:
    T* items;
    int size;

public:

    DynamicArray(int size) : size(size), items(new T[size]) {}

    DynamicArray(T* items, int count) : size(count), items(new T[count]) {
        for (int i = 0; i < count; ++i) {
            this->items[i] = items[i];
        }
    }

    DynamicArray(const DynamicArray<T>& dynamicArray) : size(dynamicArray.size), items(new T[dynamicArray.size]) {
        for (int i = 0; i < size; ++i) {
            items[i] = dynamicArray.items[i];
        }
    }

    ~DynamicArray() {
        delete[] items;
    }


    int GetSize() const {
        return size;
    }

    const T& Get(int index) const {
        if (index < 0 || index >= size) throw std::out_of_range("out_of_range");
        return items[index];
    }

    T& Get(int index) {
        if (index < 0 || index >= size) throw std::out_of_range("out_of_range");
        return items[index];
    }

    void Set(int index, T value) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("IndexOutOfRange");
        }
        items[index] = value;
    }

    void Resize(int newSize) {
        if (newSize <= 0) {
            throw std::length_error("Invalid new size");
        }
        if (newSize == size) {
            return;
        }
        T* newItems = new T[newSize];
        int minSize = (newSize > size) ? size : newSize;
        for (int i = 0; i < minSize; ++i) {
            newItems[i] = items[i];
        }
        delete[] items;
        items = newItems;
        size = newSize;
    }

    T &operator [] (int index) {
        if (index < 0 || index >= size)
            throw std::out_of_range("IndexOutOfRange");
        return items[index];
    }
};

#endif // DYNAMICARRAY_H
