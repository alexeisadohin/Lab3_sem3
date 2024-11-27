#ifndef INTERVAL_H
#define INTERVAL_H

#include <iostream>
#include <string>
#include <functional>

template <typename T, typename Comparator = std::less_equal<T>>
class Interval {
public:
    T start;
    T end;
    Comparator comp;

    Interval(T start, T end, Comparator comp = Comparator()) : start(start), end(end), comp(comp) {}
    Interval() : start(T()), end(T()) {}

    bool Contains(T value) const {
        return comp(start, value) && comp(value, end);
    }

    std::string toString() const {
        return "[" + std::to_string(start) + ", " + std::to_string(end) + ")";
    }

    bool operator<(const Interval<T, Comparator>& other) const {
        return start < other.start || (start == other.start && end < other.end);
    }

    bool operator==(const Interval& other) const {
        return start == other.start && end == other.end;
    }
};

namespace std {
    template <typename T, typename Comparator>
    struct hash<Interval<T, Comparator>> {
        size_t operator()(const Interval<T, Comparator>& interval) const {
            size_t h1 = hash<T>()(interval.start);
            size_t h2 = hash<T>()(interval.end);
            return h1 ^ (h2 << 1);
        }
    };
}

#endif // INTERVAL_H