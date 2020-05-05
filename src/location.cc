//
// Created by Namrata Vajrala on 4/19/20.
//

#include "../include/mylibrary/location.h"

namespace mylibrary {

Location::Location(int row, int col) : row_(row), col_(col) {}

Location Location::operator+(const Location& rhs) const {
  return { row_ + rhs.row_, col_ + rhs.col_ };
}

Location Location::operator-(const Location& rhs) const {
  return *this + (-rhs);
}

Location Location::operator-() const { return {-row_, -col_}; }

Location& Location::operator+=(const Location& rhs) {
  *this = *this + rhs;
  return *this;
}

int Location::Row() const { return row_; }

int Location::Col() const { return col_; }

}  // namespace mylibrary
