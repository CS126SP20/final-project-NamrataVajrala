//
// Created by Namrata Vajrala on 4/18/20.
//

#include "../include/mylibrary/blocker.h"

namespace mylibrary {

Blocker::Blocker(int x, int y, int speed, int w) : location_{Location(x, y)} {
  x_loc_ = x;
  y_loc_ = y;
  speed_blocker_ = speed;
  width_blocker = w;
}

Location Blocker::GetLocation() const {
  return location_;
}

void Blocker::SetLocation(Location loc) {
  location_ = loc;
}

void Blocker::MoveBlocker() {
    x_loc_ = x_loc_ + speed_blocker_;

    if ((speed_blocker_ > 0) && x_loc_ > width_blocker+800) {
      x_loc_ = -width_blocker;
    }

    if ((speed_blocker_ < 0) && x_loc_ < -width_blocker) {
      x_loc_ = width_blocker+800;
    }

    location_ = Location(x_loc_,y_loc_);
}

Location Blocker::GetCenterLocation() const {
  int x = x_loc_ + (int)(width_blocker/2) - (int)(kcharacter_size/2);
  return {x, y_loc_};
}

}  // namespace mylibrary
