#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdexcept>
#include <utility>
#include <memory>

template <typename TKey, typename TElement, typename Hash = std::hash<TKey>>
class HashTable {
private:
    struct Entry {
        TKey key;
        TElement element;
        bool occupied;

        Entry() : occupied(false) {}
    };

    Entry* table;
    int count;
    int capacity;
    Hash hash;

    int hashKey(const TKey& key) const {
        return hash(key) % capacity;
    }

    void resizeTable(int newCapacity) {
        Entry* oldTable = table;
        int oldCapacity = capacity;

        capacity = newCapacity;
        count = 0;
        table = new Entry[capacity];

        for (int i = 0; i < oldCapacity; ++i) {
            if (oldTable[i].occupied) {
                Add(oldTable[i].key, oldTable[i].element);
            }
        }

        delete[] oldTable;
    }

    int findNode(const TKey& key) const {
        int index = hashKey(key);
        int originalIndex = index;

        while (table[index].occupied) {
            if (table[index].key == key) {
                return index;
            }
            index = (index + 1) % capacity;
            if (index == originalIndex) {
                break;
            }
        }
        return -1;
    }

public:
    explicit HashTable(int initialCapacity = 25)
        : capacity(initialCapacity), count(0) {
        table = new Entry[capacity];
    }

    ~HashTable() {
        delete[] table;
    }

    int GetCount() const { return count; }
    int GetCapacity() const { return capacity; }

    TElement Get(const TKey& key) const {
        int index = findNode(key);
        if (index == -1) {
            throw std::runtime_error("Element not found");
        }
        return table[index].element;
    }


    void Add(const TKey& key, const TElement& element) {
        if (count >= capacity * 0.75) {
            resizeTable(capacity * 2);
        }

        int index = hashKey(key);
        int originalIndex = index;

        while (table[index].occupied) {
            if (table[index].key == key) {
                table[index].element = element;
                return;
            }
            index = (index + 1) % capacity;
            if (index == originalIndex) {
                throw std::runtime_error("Hash table is full");
            }
        }

        table[index].key = key;
        table[index].element = element;
        table[index].occupied = true;
        ++count;
    }

    void Remove(const TKey& key) {
        int index = findNode(key);
        if (index == -1) {
            throw std::runtime_error("Element not found");
        }

        table[index].occupied = false;
        --count;
    }

    bool ContainsKey(const TKey& key) const {
        return findNode(key) != -1;
    }

    class Iterator {
    private:
        const HashTable* hashTable;
        int currentIndex;

        void moveToNextValid() {
            while (currentIndex < hashTable->capacity &&
                   !hashTable->table[currentIndex].occupied) {
                ++currentIndex;
            }
        }

    public:
        Iterator(const HashTable* ht, int start)
            : hashTable(ht), currentIndex(start) {
            moveToNextValid();
        }

        Iterator& operator++() {
            ++currentIndex;
            moveToNextValid();
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return hashTable == other.hashTable && currentIndex == other.currentIndex;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        std::pair<TKey, TElement> operator*() const {
            if (currentIndex >= hashTable->capacity) {
                throw std::runtime_error("Iterator out of bounds");
            }
            return {hashTable->table[currentIndex].key, hashTable->table[currentIndex].element};
        }
    };

    Iterator begin() const {
        return Iterator(this, 0);
    }

    Iterator end() const {
        return Iterator(this, capacity);
    }

    ShrdPtr<Sequence<std::pair<TKey, TElement>>> GetAllItems() const {
        auto items = ShrdPtr<Sequence<std::pair<TKey, TElement>>>(new ArraySequence<std::pair<TKey, TElement>>());
        for (const auto& item : *this) {
            items = items->Append(item);
        }
        return items;
    }

};

#endif // HASHTABLE_H
