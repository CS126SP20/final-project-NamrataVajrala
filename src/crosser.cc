// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <mylibrary/crosser.h>

namespace mylibrary {

Crosser::Crosser() : location_{Location(375, 750)} {}

Location Crosser::GetLocation() { return location_; }

//problem: isn't binded to board size fix later
void Crosser::Move(Direction dir) {
  Location other_vertical = Location(0,ktile_size);
  Location other_horiz = Location(ktile_size,0);

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
  if (!(location_.Row() > 0 && location_.Row() < kWidth) ||
      !(location_.Col() > 0 && location_.Col() < kHeight)) {
    location_ = prev_location;
  }
//  if (!(location_ > left_top_bound) || !(location_ < right_bottom_bound)) {
//    location_ = prev_location;
//  }
}

}  // namespace mylibrary
