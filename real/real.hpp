#pragma once

#include <boost/multiprecision/cpp_dec_float.hpp>

#include <cmath>
#include <compare>
#include <cstddef>
#include <iomanip>
#include <iosfwd>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>

namespace real {

/**
 * @brief Arbitrary/high precision decimal real number.
 *
 * Backend:
 *     boost::multiprecision::cpp_dec_float_100
 *
 * Precision:
 *     100 decimal digits.
 */
class Real {
public:
    using backend_type = boost::multiprecision::cpp_dec_float_100;

public:
    // ---------------------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------------------

    constexpr Real() noexcept = default;

    Real(const Real&) = default;
    Real(Real&&) noexcept = default;

    Real& operator=(const Real&) = default;
    Real& operator=(Real&&) noexcept = default;

    Real(const backend_type& value);
    Real(backend_type&& value);

    Real(const char* value);
    Real(const std::string& value);
    Real(std::string_view value);

    template <
        typename T,
        typename = std::enable_if_t<
            std::is_arithmetic_v<T> &&
            !std::is_same_v<std::remove_cv_t<T>, bool>
        >
    >
    Real(T value)
        : value_(value)
    {
    }

    // ---------------------------------------------------------------------
    // Assignment
    // ---------------------------------------------------------------------

    Real& operator=(const backend_type& value);
    Real& operator=(backend_type&& value);

    Real& operator=(const char* value);
    Real& operator=(const std::string& value);
    Real& operator=(std::string_view value);

    template <
        typename T,
        typename = std::enable_if_t<
            std::is_arithmetic_v<T> &&
            !std::is_same_v<std::remove_cv_t<T>, bool>
        >
    >
    Real& operator=(T value)
    {
        value_ = value;
        return *this;
    }

    // ---------------------------------------------------------------------
    // Conversion
    // ---------------------------------------------------------------------

    /**
     * @brief Explicit conversion to arithmetic types.
     */
    template <
        typename T,
        typename = std::enable_if_t<std::is_arithmetic_v<T>>
    >
    explicit operator T() const
    {
        return value_.convert_to<T>();
    }

    /**
     * @brief Explicit boolean conversion.
     */
    explicit operator bool() const noexcept;

    /**
     * @brief Access the underlying Boost backend.
     */
    const backend_type& backend() const noexcept;
    backend_type& backend() noexcept;

    // ---------------------------------------------------------------------
    // String
    // ---------------------------------------------------------------------

    /**
     * @brief Convert to string.
     *
     * @param precision Number of decimal digits.
     *                  -1 = let cpp_dec_float decide.
     * @param fixed Use fixed-point notation.
     */
    std::string to_string(
        int precision = -1,
        bool fixed = false
    ) const;

    // ---------------------------------------------------------------------
    // Rounding
    // ---------------------------------------------------------------------

    /**
     * @brief Round to a number of digits after decimal point.
     *
     * round()      -> nearest integer
     * round(2)     -> 2 digits after decimal point
     * round(-2)    -> nearest hundred
     *
     * Half values are rounded away from zero.
     */
    Real round(int digits = 0) const;

    // ---------------------------------------------------------------------
    // Arithmetic operators
    // ---------------------------------------------------------------------

    Real& operator+=(const Real& rhs);
    Real& operator-=(const Real& rhs);
    Real& operator*=(const Real& rhs);
    Real& operator/=(const Real& rhs);
    Real& operator%=(const Real& rhs);

    friend Real operator+(Real lhs, const Real& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    friend Real operator-(Real lhs, const Real& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    friend Real operator*(Real lhs, const Real& rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    friend Real operator/(Real lhs, const Real& rhs)
    {
        lhs /= rhs;
        return lhs;
    }

    friend Real operator%(Real lhs, const Real& rhs)
    {
        lhs %= rhs;
        return lhs;
    }

    // ---------------------------------------------------------------------
    // Unary operators
    // ---------------------------------------------------------------------

    Real operator+() const;
    Real operator-() const;

    // ---------------------------------------------------------------------
    // Increment / decrement
    // ---------------------------------------------------------------------

    Real& operator++();
    Real operator++(int);

    Real& operator--();
    Real operator--(int);

    // ---------------------------------------------------------------------
    // Comparison
    // ---------------------------------------------------------------------

    friend bool operator==(const Real& lhs, const Real& rhs) noexcept
    {
        return lhs.value_ == rhs.value_;
    }

    friend bool operator!=(const Real& lhs, const Real& rhs) noexcept
    {
        return lhs.value_ != rhs.value_;
    }

    friend bool operator<(const Real& lhs, const Real& rhs) noexcept
    {
        return lhs.value_ < rhs.value_;
    }

    friend bool operator<=(const Real& lhs, const Real& rhs) noexcept
    {
        return lhs.value_ <= rhs.value_;
    }

    friend bool operator>(const Real& lhs, const Real& rhs) noexcept
    {
        return lhs.value_ > rhs.value_;
    }

    friend bool operator>=(const Real& lhs, const Real& rhs) noexcept
    {
        return lhs.value_ >= rhs.value_;
    }

#if __cplusplus >= 202002L
    friend std::strong_ordering operator<=>(const Real& lhs, const Real& rhs) noexcept
    {
        if (lhs.value_ < rhs.value_)
            return std::strong_ordering::less;

        if (lhs.value_ > rhs.value_)
            return std::strong_ordering::greater;

        return std::strong_ordering::equal;
    }
#endif

private:
    backend_type value_{};
};

// ==========================================================================
// Arithmetic with built-in numeric types
// ==========================================================================

template <
    typename T,
    typename = std::enable_if_t<
        std::is_arithmetic_v<T> &&
        !std::is_same_v<std::remove_cv_t<T>, bool>
    >
>
inline Real operator+(Real lhs, T rhs)
{
    lhs += Real(rhs);
    return lhs;
}

template <
    typename T,
    typename = std::enable_if_t<
        std::is_arithmetic_v<T> &&
        !std::is_same_v<std::remove_cv_t<T>, bool>
    >
>
inline Real operator+(T lhs, Real rhs)
{
    rhs += Real(lhs);
    return rhs;
}

template <
    typename T,
    typename = std::enable_if_t<
        std::is_arithmetic_v<T> &&
        !std::is_same_v<std::remove_cv_t<T>, bool>
    >
>
inline Real operator-(Real lhs, T rhs)
{
    lhs -= Real(rhs);
    return lhs;
}

template <
    typename T,
    typename = std::enable_if_t<
        std::is_arithmetic_v<T> &&
        !std::is_same_v<std::remove_cv_t<T>, bool>
    >
>
inline Real operator-(T lhs, const Real& rhs)
{
    return Real(lhs) - rhs;
}

template <
    typename T,
    typename = std::enable_if_t<
        std::is_arithmetic_v<T> &&
        !std::is_same_v<std::remove_cv_t<T>, bool>
    >
>
inline Real operator*(Real lhs, T rhs)
{
    lhs *= Real(rhs);
    return lhs;
}

template <
    typename T,
    typename = std::enable_if_t<
        std::is_arithmetic_v<T> &&
        !std::is_same_v<std::remove_cv_t<T>, bool>
    >
>
inline Real operator*(T lhs, Real rhs)
{
    rhs *= Real(lhs);
    return rhs;
}

template <
    typename T,
    typename = std::enable_if_t<
        std::is_arithmetic_v<T> &&
        !std::is_same_v<std::remove_cv_t<T>, bool>
    >
>
inline Real operator/(Real lhs, T rhs)
{
    lhs /= Real(rhs);
    return lhs;
}

template <
    typename T,
    typename = std::enable_if_t<
        std::is_arithmetic_v<T> &&
        !std::is_same_v<std::remove_cv_t<T>, bool>
    >
>
inline Real operator/(T lhs, const Real& rhs)
{
    return Real(lhs) / rhs;
}

template <
    typename T,
    typename = std::enable_if_t<
        std::is_arithmetic_v<T> &&
        !std::is_same_v<std::remove_cv_t<T>, bool>
    >
>
inline Real operator%(Real lhs, T rhs)
{
    lhs %= Real(rhs);
    return lhs;
}

template <
    typename T,
    typename = std::enable_if_t<
        std::is_arithmetic_v<T> &&
        !std::is_same_v<std::remove_cv_t<T>, bool>
    >
>
inline Real operator%(T lhs, const Real& rhs)
{
    return Real(lhs) % rhs;
}

// ==========================================================================
// Comparisons with built-in numeric types
// ==========================================================================

#define REAL_DEFINE_COMPARISON(OP)                                      \
    template <typename T,                                             \
        typename = std::enable_if_t<                                  \
            std::is_arithmetic_v<T> &&                                \
            !std::is_same_v<std::remove_cv_t<T>, bool>>>              \
    inline bool operator OP(const Real& lhs, T rhs) noexcept           \
    {                                                                  \
        return lhs OP Real(rhs);                                       \
    }                                                                  \
                                                                        \
    template <typename T,                                             \
        typename = std::enable_if_t<                                  \
            std::is_arithmetic_v<T> &&                                \
            !std::is_same_v<std::remove_cv_t<T>, bool>>>              \
    inline bool operator OP(T lhs, const Real& rhs) noexcept           \
    {                                                                  \
        return Real(lhs) OP rhs;                                       \
    }

REAL_DEFINE_COMPARISON(==)
REAL_DEFINE_COMPARISON(!=)
REAL_DEFINE_COMPARISON(<)
REAL_DEFINE_COMPARISON(<=)
REAL_DEFINE_COMPARISON(>)
REAL_DEFINE_COMPARISON(>=)

#undef REAL_DEFINE_COMPARISON

// ==========================================================================
// Stream operators
// ==========================================================================

std::ostream& operator<<(std::ostream& os, const Real& value);
std::istream& operator>>(std::istream& is, Real& value);

} // namespace real

// ==========================================================================
// std::numeric_limits specialization
// ==========================================================================

