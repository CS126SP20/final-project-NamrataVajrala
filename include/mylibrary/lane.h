//
// Created by Namrata Vajrala on 4/20/20.
//

#ifndef FINALPROJECT_LANE_H
#define FINALPROJECT_LANE_H

#include <vector>
#include "mylibrary/blocker.h"

namespace mylibrary {

const int kWidth_screen = 800;
const int kHeight_screen = 800;
const int ktile_width = 50;

class Lane {
 private:
  int num_blockers_;
  int width_;
  int lane_number_;
  int speed_;

 public:
  Lane(int blockers_num, int width, int lane_num, int lane_speed);
  std::vector<mylibrary::Blocker> GetBlockersVector();
  int GetWidth();
  int GetSpeed();
  void MoveLane();
};

}
#endif  // FINALPROJECT_LANE_H
