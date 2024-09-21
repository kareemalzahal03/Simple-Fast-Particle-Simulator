#pragma once
#include <iostream>
#include <stdexcept>

// A simple index struct for representing an index in an
// array / vector. By default, idx is invalid and does not
// have a number. Idx can be implicitly interpreted as a bool
// to check if it has a value, and implicityly interpreted
// as a size_t (aka unsigned long) for direct use in a vector's
// operator[].

struct idx {

    // Constructors
    idx();
    idx(std::size_t value);
    idx(const idx& other);

    // Assignment operators
    idx& operator=(const idx& other);
    idx& operator=(std::size_t new_value);

    // Increment operators
    idx& operator++();        // Pre-increment
    idx operator++(int);      // Post-increment

    // Conversion operators
    operator std::size_t() const;
    operator bool() const;

    // Equality operator
    bool operator==(const idx& other) const;
    bool operator!=(const idx& other) const;
    bool operator==(const std::size_t& other) const;

    // Friend function for output
    friend std::ostream& operator<<(std::ostream& os, const idx& index);

private:
    std::size_t value;
    bool valid;
};
