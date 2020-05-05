//
// Created by Namrata Vajrala on 4/18/20.
//

#ifndef FINALPROJECT_BLOCKER_H
#define FINALPROJECT_BLOCKER_H

#include "location.h"

namespace mylibrary {

const int kcharacter_size = 50;

class Blocker {
 private:
  int x_loc_;
  int y_loc_;
  int speed_blocker_;
  int width_blocker;
  Location location_;
 public:
  /**
   * constructor initializing blocker object
   * @param x coordinate
   * @param y coordinate
   * @param speed blocker speed
   * @param w width of the blocker
   */
  Blocker(int x, int y, int speed, int w);
  /**
   * gets the blocker location
   * @return location
   */
  Location GetLocation() const;
  /**
   * Gets the location of the center of the blocker
   * @return location
   */
  Location GetCenterLocation() const;
  /**
   * sets the location of the blocker
   * @param loc location to set to
   */
  void SetLocation(Location loc);
  /**
   * moves the blocker forward/backward
   */
  void MoveBlocker();
  };
}
#endif  // FINALPROJECT_BLOCKER_H
