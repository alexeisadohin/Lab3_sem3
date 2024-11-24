#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdexcept>
#include "ArraySequence.h"

template <typename TKey, typename TElement>
class HashTable {
private:
    struct Entry {
        TKey key;
        TElement element;
        bool occupied;
        Entry* next;

        Entry() : occupied(false), next(nullptr) {}
    };
    Entry* table;
    int count;
    int capacity;
    enum CollisionResolutionMethod {
        OPEN_ADDRESSING,
        CHAINING
    };
    CollisionResolutionMethod resolutionMethod;

    int hash(const TKey& key) const {
        return std::hash<TKey>{}(key) % capacity;
    }

    int findIndex(const TKey& key) const {
        int index = hash(key);
        int originalIndex = index;

        while (table[index].occupied) {
            if (table[index].key == key) {
                return index;
            }
            index = (index + 1) % capacity;
            if (index == originalIndex) {
                return -1;
            }
        }
        return -1;
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
public:
    explicit HashTable(int initialCapacity = 25, CollisionResolutionMethod method = OPEN_ADDRESSING)
    : capacity(initialCapacity), count(0), resolutionMethod(method) {
        table = new Entry[capacity];
    }


    ~HashTable() {
        delete[] table;
    }

    int GetCount() const { return count; }
    int GetCapacity() const { return capacity; }

    TElement Get(const TKey& key) const {
        int index = hash(key);

        if (resolutionMethod == OPEN_ADDRESSING) {
            int originalIndex = index;

            while (table[index].occupied) {
                if (table[index].key == key) {
                    return table[index].element;
                }
                index = (index + 1) % capacity;
                if (index == originalIndex) {
                    break;
                }
            }
        } else if (resolutionMethod == CHAINING) {
            Entry* current = &table[index];
            while (current) {
                if (current->key == key) {
                    return current->element;
                }
                current = current->next;
            }
        }

        throw std::runtime_error("Element not found (get method in HT)");
    }

void Add(const TKey& key, const TElement& element) {
    int index = hash(key);

    if (resolutionMethod == OPEN_ADDRESSING) {

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

    } else if (resolutionMethod == CHAINING) {

        Entry* newNode = new Entry();
        newNode->key = key;
        newNode->element = element;
        newNode->occupied = true;
        newNode->next = nullptr;

        if (!table[index].occupied) {
            table[index] = *newNode;
        } else {

            Entry* current = &table[index];
            while (current->next) {

                if (current->key == key) {
                    current->element = element;
                    delete newNode;
                    return;
                }
                current = current->next;
            }

            current->next = newNode;
        }
    }

    count++;
}

void Remove(const TKey& key) {
    int index = hash(key);

    if (resolutionMethod == OPEN_ADDRESSING) {

        int foundIndex = findIndex(key);
        if (foundIndex != -1) {
            table[foundIndex].occupied = false;
            count--;
        } else {
            throw std::runtime_error("Element not found (remove method in HT)");
        }
    } else if (resolutionMethod == CHAINING) {
        Entry* current = &table[index];
        Entry* prev = nullptr;

        while (current) {
            if (current->key == key) {

                if (prev) {
                    prev->next = current->next;
                } else {

                    table[index] = *current->next;
                }
                delete current;
                count--;
                return;
            }
            prev = current;
            current = current->next;
        }
        throw std::runtime_error("Element not found (remove method in HT)");
    }
}


    bool ContainsKey(const TKey& key) const {
        return findIndex(key) != -1;
    }

    class Iterator {
    private:
        const HashTable* hashTable;
        int currentIndex;

    public:
        Iterator(const HashTable* ht, int start) : hashTable(ht), currentIndex(start) {}
        ~Iterator() = default;


        bool HasNext()  {
            while (currentIndex < hashTable->capacity && !hashTable->table[currentIndex].occupied) {
                currentIndex++;
            }
            return currentIndex < hashTable->capacity;
        }

        std::pair<TKey, TElement> Next() {
            if (!HasNext()) {
                throw std::runtime_error("No next element");
            }
            auto result = std::make_pair(hashTable->table[currentIndex].key, hashTable->table[currentIndex].element);
            currentIndex++;
            return result;
        }
    };

    Iterator* begin() {
        return new Iterator(this, 0);
    }

    Iterator* end() {
        return new Iterator(this, capacity);
    }

    ShrdPtr<Sequence<std::pair<TKey, TElement>>> GetAllItems() const {
        auto allItems = ShrdPtr<Sequence<std::pair<TKey, TElement>>>(new ArraySequence<std::pair<TKey, TElement>>());

        for (int i = 0; i < capacity; ++i) {
            if (table[i].occupied) {
                allItems = allItems->Append({table[i].key, table[i].element});
            }
            Entry* current = table[i].next;
            while (current) {
                allItems = allItems->Append({current->key, current->element});
                current = current->next;
            }
        }
        return allItems;
    }

};
#endif //HASHTABLE_H
