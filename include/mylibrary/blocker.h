//
// Created by Namrata Vajrala on 4/18/20.
//

#ifndef FINALPROJECT_BLOCKER_H
#define FINALPROJECT_BLOCKER_H
#include <mylibrary/location.h>

namespace mylibrary {

class Blocker {
 private:
  int x_loc_;
  int y_loc_;
  int speed_blocker_;
  int width__blocker;
//  int height_;
  Location location_;
 public:
  Blocker(int x, int y, int speed, int w);
  Location GetLocation();
  void SetLocation(Location loc);
  void MoveBlocker();
  };
}
#endif  // FINALPROJECT_BLOCKER_H
