// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_EXAMPLE_H_
#define FINALPROJECT_MYLIBRARY_EXAMPLE_H_

#include <mylibrary/direction.h>
#include <mylibrary/location.h>

#include <string>

namespace mylibrary {

const int kWidth = 800;
const int kHeight = 800;
const int ktile_size = 50;

class Crosser{
 private:
  Location location_;
  int score_;
  std::string name_;
 public:
  Crosser(std::string name);
  Location GetLocation();
  void SetLocation(Location loc);
  bool DoesIntersect(int x_one, int y_one, int x_two, int y_two);
  void Move(Direction dir);
  bool IsInWinningPosition();
  void CalculateScore(int speed);
  void SetScore(int score);
  size_t GetScore();
  std::string GetName();
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_EXAMPLE_H_
