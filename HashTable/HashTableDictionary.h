#ifndef HASHTABLEDICTIONARY_H
#define HASHTABLEDICTIONARY_H

#include "IDictionary.h"
#include "HashTable.h"
#include <utility>

template <typename TKey, typename TElement>
class HashTableDictionary : public IDictionary<TKey, TElement> {
private:
    HashTable<TKey, TElement> hashTable;

public:
    explicit HashTableDictionary(int initialCapacity = 25) : hashTable(initialCapacity) {}

    int GetCount() const override { return hashTable.GetCount(); }
    int GetCapacity() const override { return hashTable.GetCapacity(); }

    TElement Get(const TKey& key) const override { return hashTable.Get(key); }

    void Add(const TKey& key, const TElement& element) override {
        hashTable.Add(key, element);
    }

    ShrdPtr<Sequence<std::pair<TKey, TElement>>> GetAllItems() const override {return hashTable.GetAllItems();}

    void Remove(const TKey& key) override { hashTable.Remove(key); }

    bool ContainsKey(const TKey& key) const override { return hashTable.ContainsKey(key); }

    class Iterator : public IDictionary<TKey, TElement>::Iterator {
    private:
        typename HashTable<TKey, TElement>::Iterator *innerIterator;

    public:
        Iterator(typename HashTable<TKey, TElement>::Iterator* it) : innerIterator(it) {}

        ~Iterator() override {
            delete innerIterator;
        }

        bool HasNext() override {
            return innerIterator->HasNext();
        }

        std::pair<TKey, TElement> Next() override {
            return innerIterator->Next();
        }

    };

    typename IDictionary<TKey, TElement>::Iterator* begin() override {
        return new Iterator(hashTable.begin());
    }

    typename IDictionary<TKey, TElement>::Iterator* end() override {
        return new Iterator(hashTable.end());
    }
};


#endif //HASHTABLEDICTIONARY_H
