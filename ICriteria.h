#ifndef ICRITERIA_H
#define ICRITERIA_H
#include "ShrdPtr.h"
#include "Sequence.h"
#include "Interval.h"

template <typename T>
class ICriteria {
public:
    virtual ~ICriteria() = default;

    virtual ShrdPtr<Sequence<Interval<T>>> generateIntervals(const T& min, const T& max, int numIntervals) const = 0;
};

#endif //ICRITERIA_H
