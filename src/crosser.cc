// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <string>
#include <utility>

#include <mylibrary/crosser.h>

namespace mylibrary {

Crosser::Crosser(std::string name) :
location_{Location(kinit_x, kinit_y)} {
  score_ = 0;
  name_ = std::move(name);
}

Location Crosser::GetLocation() const {
  return location_;
}

void Crosser::SetLocation(Location loc) {
  location_= loc;
}

bool Crosser::DoesIntersect(int x_one, int y_one, int x_two, int y_two) const {
  int this_x_one = location_.Row();
  int this_y_one = location_.Col();
  int this_x_two = location_.Row() + ktile_size;
  int this_y_two = location_.Col() + ktile_size;
  return (!(this_x_one >= x_two || this_x_two <= x_one
  || this_y_one >= y_two || this_y_two <= y_one));
}

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
  if (!(location_.Row() > 0 &&
  location_.Row() < kWidth - ktile_size/ktwo_val) ||
  !(location_.Col() > -ktile_size && location_.Col() < kHeight)) {
    location_ = prev_location;
  }
}

void Crosser::CalculateScore(int speed) {
  if (location_.Col() == (kHeight - ktile_size)) {
    score_ = 0;
  } else {
    score_ = ((kHeight - location_.Col()) - ktile_size*ktwo_val)*(speed+1);
  }
}

void Crosser::SetScore(int score) {
  score_ = score;
}

size_t Crosser::GetScore() const {
  return score_;
}

std::string Crosser::GetName() const {
  return name_;
}

bool Crosser::IsInWinningPosition() const {
  return location_.Col() <= 0;
}

}  // namespace mylibrary
