#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <vector>
#include <numeric>

// MathUtils
// ----------------------------------------------------------------
// Small, genuinely reused FUNCTION TEMPLATES (as opposed to the
// ObjectManager CLASS TEMPLATE). Kept free-standing since they are
// generic numeric helpers, not tied to any one class's responsibility.
// ----------------------------------------------------------------

// Clamps 'value' into the inclusive range [minVal, maxVal].
// Used e.g. to keep a friction coefficient within a sane [0,1] range,
// and could equally be used for any other bounded numeric quantity.
template <typename T>
T clampValue(T value, T minVal, T maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

// Computes the arithmetic mean of a collection of numeric values.
// Used e.g. to report the average speed of all objects in the world.
template <typename T>
double average(const std::vector<T>& values) {
    if (values.empty()) return 0.0;
    T sum = std::accumulate(values.begin(), values.end(), T());
    return static_cast<double>(sum) / values.size();
}

#endif // MATH_UTILS_H
