#include "idx.hpp"

// Default constructor
idx::idx() : value(0), valid(false) {}

// Constructor for valid index
idx::idx(std::size_t value) : value(value), valid(true) {}

// Copy constructor
idx::idx(const idx& other) : value(other.value), valid(other.valid) {}

// Copy assignment operator
idx& idx::operator=(const idx& other) {
    if (this != &other) {
        value = other.value;
        valid = other.valid;
    }
    return *this;
}

// Assignment operator for std::size_t
idx& idx::operator=(std::size_t new_value) {
    value = new_value;
    valid = true;
    return *this;
}

// Pre-increment operator (++idx)
idx& idx::operator++() {
    if (!valid) throw std::runtime_error("Incrementing invalid index");
    ++value;
    return *this;
}

// Post-increment operator (idx++)
idx idx::operator++(int) {
    if (!valid) throw std::runtime_error("Incrementing invalid index");
    idx temp = *this;
    ++value;
    return temp;
}

// Conversion to size_t
idx::operator std::size_t() const {
    if (!valid) throw std::runtime_error("Invalid index accessed");
    return value;
}

// Conversion to bool (check if valid)
idx::operator bool() const {
    return valid;
}

// Equality operator
bool idx::operator==(const idx& other) const {
    return (!valid && !other.valid) || 
        (valid == other.valid && value == other.value);
}

bool idx::operator!=(const idx& other) const {
    return !this->operator==(other);
}

bool idx::operator==(const std::size_t& otherValue) const {
    return (valid && value == otherValue);
}

// Overload for << operator
std::ostream& operator<<(std::ostream& os, const idx& index) {
    if (index.valid) {
        os << index.value;
    } else {
        os << "Invalid";
    }
    return os;
}
