//
// Created by Namrata Vajrala on 4/19/20.
//

#ifndef FINALPROJECT_LOCATION_H
#define FINALPROJECT_LOCATION_H

#include <iosfwd>

namespace mylibrary {

class Location {
 public:
  /**
   * Location constructor with row and col
   * @param row int row loc
   * @param col int col loc
   */
  Location(int row, int col);

  /**
   * checks if locations are equal
   * @param rhs other location
   * @return bool if equal
   */
  bool operator==(const Location& rhs) const;

  /**
   * addition operator
   * @param rhs second location
   * @return location addition
   */
  Location operator+(const Location& rhs) const;

  /**
   * subtraction operator
   * @param rhs second location
   * @return subtracted location
   */
  Location operator-(const Location& rhs) const;

  /**
   * += operator to add locations
   * @param rhs sceond location to add
   * @return returns sum of locations
   */
  Location& operator+=(const Location& rhs);

  /**
   * minus operator
   * @return negative location
   */
  Location operator-() const;

  // Accessors.
  /**
   * gets the location row
   * @return int row val
   */
  int Row() const;

  /**
   * gets the location col
   * @return int col val
   */
  int Col() const;

 private:
  int row_;
  int col_;
};

}  // namespace mylibrary

#endif  // FINALPROJECT_LOCATION_H
