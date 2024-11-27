#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <iostream>
#include "IDictionary.h"
#include "HashTableDictionary.h"
#include "Interval.h"
#include "ShrdPtr.h"
#include <numeric>
#include "Sequence.h"

template <typename T>
class Histogram {
private:
    ShrdPtr<Sequence<Interval<T>>> intervals;
    ShrdPtr<IDictionary<Interval<T>, int>> data;

public:
    explicit Histogram(ShrdPtr<Sequence<Interval<T>>> intervals)
        : intervals(intervals),
          data(new HashTableDictionary<Interval<T>, int>()) {}

    void add(ShrdPtr<Sequence<T>> values) {
        for (int i = 0; i < values->GetLength(); ++i) {
            T value = values->Get(i);
            for (int j = 0; j < intervals->GetLength(); ++j) {
                if (intervals->Get(j).Contains(value)) {
                    Interval<T> interval = intervals->Get(j);
                    int currentCount = data->ContainsKey(interval) ? data->Get(interval) : 0;
                    data->Add(interval, currentCount + 1);
                }
            }
        }
    }

    void printStatistics() const {
        for (int i = 0; i < intervals->GetLength(); ++i) {
            Interval<T> interval = intervals->Get(i);
            if (data->ContainsKey(interval)) {
                const auto& values = data->Get(interval);
                T sum = std::accumulate(values.begin(), values.end(), T(0));
                T average = values.empty() ? T(0) : sum / static_cast<T>(values.size());

                std::cout << interval.toString() << " -> "
                          << "Count: " << values.size()
                          << ", Average: " << average << std::endl;
            } else {
                std::cout << interval.toString() << " -> Count: 0" << std::endl;
            }
        }
    }

    const IDictionary<Interval<T>, int>& getData()  {
        return *data;
    }
};

#endif // HISTOGRAM_H
