#include "../real/real.hpp"

#include <boost/multiprecision/cpp_dec_float.hpp>

#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace real {

// ============================================================================
// Constructors
// ============================================================================

Real::Real(const backend_type& value)
    : value_(value)
{
}

Real::Real(backend_type&& value)
    : value_(std::move(value))
{
}

Real::Real(const char* value)
    : value_(value)
{
}

Real::Real(const std::string& value)
    : value_(value)
{
}

Real::Real(std::string_view value)
    : value_(std::string(value))
{
}

// ============================================================================
// Assignment
// ============================================================================

Real& Real::operator=(const backend_type& value)
{
    value_ = value;
    return *this;
}

Real& Real::operator=(backend_type&& value)
{
    value_ = std::move(value);
    return *this;
}

Real& Real::operator=(const char* value)
{
    value_ = backend_type(value);
    return *this;
}

Real& Real::operator=(const std::string& value)
{
    value_ = backend_type(value);
    return *this;
}

Real& Real::operator=(std::string_view value)
{
    value_ = backend_type(std::string(value));
    return *this;
}

// ============================================================================
// Conversion
// ============================================================================

Real::operator bool() const noexcept
{
    return value_ != 0;
}

const Real::backend_type& Real::backend() const noexcept
{
    return value_;
}

Real::backend_type& Real::backend() noexcept
{
    return value_;
}

// ============================================================================
// String
// ============================================================================

std::string Real::to_string(int precision, bool fixed) const
{
    if (precision < 0) {
        return value_.str(
            0,
            fixed
                ? std::ios_base::fixed
                : std::ios_base::fmtflags(0)
        );
    }

    return value_.str(
        precision,
        fixed
            ? std::ios_base::fixed
            : std::ios_base::fmtflags(0)
    );
}

// ============================================================================
// Round
// ============================================================================

Real Real::round(int digits) const
{
    using boost::multiprecision::ceil;
    using boost::multiprecision::floor;
    using boost::multiprecision::pow;

    backend_type scale;

    if (digits >= 0) {
        scale = pow(backend_type(10), digits);
    } else {
        scale = pow(backend_type(10), -digits);
    }

    backend_type scaled;

    if (digits >= 0) {
        scaled = value_ * scale;
    } else {
        scaled = value_ / scale;
    }

    backend_type rounded;

    // Round half away from zero.
    if (scaled >= 0) {
        rounded = floor(scaled + backend_type("0.5"));
    } else {
        rounded = ceil(scaled - backend_type("0.5"));
    }

    if (digits >= 0) {
        return Real(rounded / scale);
    }

    return Real(rounded * scale);
}

// ============================================================================
// Arithmetic
// ============================================================================

Real& Real::operator+=(const Real& rhs)
{
    value_ += rhs.value_;
    return *this;
}

Real& Real::operator-=(const Real& rhs)
{
    value_ -= rhs.value_;
    return *this;
}

Real& Real::operator*=(const Real& rhs)
{
    value_ *= rhs.value_;
    return *this;
}

Real& Real::operator/=(const Real& rhs)
{
    value_ /= rhs.value_;
    return *this;
}

Real& Real::operator%=(const Real& rhs)
{
    if (rhs.value_ == 0) {
        throw std::domain_error(
            "real::Real: modulo by zero"
        );
    }

    using boost::multiprecision::ceil;
    using boost::multiprecision::floor;

    const backend_type quotient =
        value_ / rhs.value_;

    backend_type truncated;

    if (quotient >= 0) {
        truncated = floor(quotient);
    } else {
        truncated = ceil(quotient);
    }

    value_ -= truncated * rhs.value_;

    return *this;
}

// ============================================================================
// Unary
// ============================================================================

Real Real::operator+() const
{
    return *this;
}

Real Real::operator-() const
{
    return Real(-value_);
}

// ============================================================================
// Increment / decrement
// ============================================================================

Real& Real::operator++()
{
    value_ += 1;
    return *this;
}

Real Real::operator++(int)
{
    Real old(*this);
    ++(*this);
    return old;
}

Real& Real::operator--()
{
    value_ -= 1;
    return *this;
}

Real Real::operator--(int)
{
    Real old(*this);
    --(*this);
    return old;
}

// ============================================================================
// Streams
// ============================================================================

std::ostream& operator<<(std::ostream& os, const Real& value)
{
    os << value.backend();
    return os;
}

std::istream& operator>>(std::istream& is, Real& value)
{
    Real::backend_type tmp;

    if (is >> tmp) {
        value = std::move(tmp);
    }

    return is;
}

} // namespace real
