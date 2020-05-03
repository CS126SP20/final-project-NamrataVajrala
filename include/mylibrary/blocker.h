//
// Created by Namrata Vajrala on 4/18/20.
//

#ifndef FINALPROJECT_BLOCKER_H
#define FINALPROJECT_BLOCKER_H
//#include <mylibrary/location.h>

#include "location.h"

namespace mylibrary {

const int kcharacter_size = 50;

class Blocker {
 private:
  int x_loc_;
  int y_loc_;
  int speed_blocker_;
  int width_blocker;
//  int height_;
  Location location_;
 public:
  Blocker(int x, int y, int speed, int w);
  Location GetLocation();
  Location GetCenterLocation();
  void SetLocation(Location loc);
  void MoveBlocker();
  };
}
#endif  // FINALPROJECT_BLOCKER_H
