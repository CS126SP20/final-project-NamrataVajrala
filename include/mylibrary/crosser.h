// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_EXAMPLE_H_
#define FINALPROJECT_MYLIBRARY_EXAMPLE_H_

#include <mylibrary/location.h>
#include <mylibrary/direction.h>


namespace mylibrary {

const int kWidth = 800;
const int kHeight = 800;
const int ktile_size = 50;

class Crosser{
 private:
  Location location_;
 public:
  Crosser();
  Location GetLocation();
  void Move(Direction dir);
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_EXAMPLE_H_
