//
// Created by Namrata Vajrala on 4/18/20.
//

#include "mylibrary/blocker.h"

namespace mylibrary {

Blocker::Blocker(int x, int y, int speed, int w) : location_{Location(x, y)} {
  x_loc_ = x;
  y_loc_ = y;
  speed_blocker_ = speed;
  width__blocker = w;
}

Location Blocker::GetLocation() { return location_; }

void Blocker::SetLocation(Location loc) {
  location_ = loc;
}

void Blocker::MoveBlocker() {
    x_loc_ = x_loc_ + speed_blocker_;

    if (x_loc_ > width__blocker+800) {
      x_loc_ = -width__blocker;
    }

    location_ = Location(x_loc_,y_loc_);
}

}  // namespace mylibrary
