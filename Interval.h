#ifndef INTERVAL_H
#define INTERVAL_H

#include <iostream>
#include <string>
#include <functional>

template <typename T>
class Interval {
public:
    T start;
    T end;

    Interval(T start, T end) : start(start), end(end) {}
    Interval() : start(T()), end(T()) {}

    bool Contains(T value) const {
        return value >= start && value <= end;
    }

    std::string toString() const {
        return "[" + std::to_string(start) + ", " + std::to_string(end) + ")";
    }

    bool operator<(const Interval<T>& other) const {
        return start < other.start || (start == other.start && end < other.end);
    }

    bool operator==(const Interval& other) const {
        return start == other.start && end == other.end;
    }
};

namespace std {
    template <typename T>
    struct hash<Interval<T>> {
        size_t operator()(const Interval<T>& interval) const {
            size_t h1 = hash<T>()(interval.start);
            size_t h2 = hash<T>()(interval.end);
            return h1 ^ (h2 << 1);
        }
    };
}

#endif // INTERVAL_H
