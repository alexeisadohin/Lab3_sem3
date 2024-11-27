#ifndef UNIFORMCRITERIA_H
#define UNIFORMCRITERIA_H

#include "ICriteria.h"
#include "ArraySequence.h"

template <typename T>
class UniformCriteria : public ICriteria<T> {
public:
    ShrdPtr<Sequence<Interval<T>>> generateIntervals(const T& min, const T& max, int numIntervals) const override {
        ShrdPtr<Sequence<Interval<T>>> intervals(new ArraySequence<Interval<T>>());

        if (numIntervals <= 0) {
            return intervals;
        }

        if (numIntervals == 1) {
            intervals = intervals->Append(Interval<T>(min, max));
            return intervals;
        }

        T range = max - min;
        for (int i = 0; i < numIntervals; ++i) {
            T start = min + (range * i) / numIntervals;
            T end = min + (range * (i + 1)) / numIntervals;

            intervals = intervals->Append(Interval<T>(start, end));
        }

        return intervals;
    }
};


#endif // UNIFORMCRITERIA_H
