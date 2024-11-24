#ifndef UNIFORMCRITERIA_H
#define UNIFORMCRITERIA_H

#include "ICriteria.h"
#include "ArraySequence.h"

template <typename T>
class UniformCriteria : public ICriteria<T> {
public:
    ShrdPtr<Sequence<Interval<T>>> generateIntervals(const T& min, const T& max, int numIntervals) const override {
        T step = (max - min) / static_cast<T>(numIntervals);
        ShrdPtr<Sequence<Interval<T>>> intervals(new ArraySequence<Interval<T>>());

        for (int i = 0; i < numIntervals; i++) {
            Interval<T> interval(min + i * step, min + (i + 1) * step);
            intervals = intervals->Append(interval);
        }

        return intervals;
    }
};

#endif // UNIFORMCRITERIA_H
