// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <mylibrary/crosser.h>

namespace mylibrary {

Crosser::Crosser() : location_{Location(375, 750)} {}

Location Crosser::GetLocation() { return location_; }

void Crosser::Move(Direction dir) {
  Location other_vertical = Location(0,50);
  Location other_horiz = Location(50,0);
  Location left_top_bound = Location(0, 0);
  Location right_bottom_bound = Location(kWidth, kHeight);

  Location prev_location = location_;

  if (dir == Direction::kUp) {
    location_ = location_ - other_vertical;
  }
  if (dir == Direction::kDown) {
    location_ = location_ + other_vertical;
  }
  if (dir == Direction::kRight) {
    location_ = location_ + other_horiz;
  }
  if (dir == Direction::kLeft) {
    location_ = location_ - other_horiz;
  }
  if (!(location_ > left_top_bound) || !(location_ < right_bottom_bound)) {
    location_ = prev_location;
  }
}

}  // namespace mylibrary
