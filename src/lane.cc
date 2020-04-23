//
// Created by Namrata Vajrala on 4/20/20.
//

#include "mylibrary/lane.h"

#include <utility>
#include "mylibrary/blocker.h"


namespace mylibrary {

Lane::Lane(int blockers_num, int width, int lane_num, int lane_speed, std::vector<mylibrary::Blocker *>& blockers) {
  num_blockers_ = blockers_num;
  width_ = width;
  lane_number_ = lane_num;
  speed_ = lane_speed;
  blockers_vector_ = blockers;
//  FillBlockersVector();

}

//void Lane::FillBlockersVector() {
//
//}

std::vector<mylibrary::Blocker *>& Lane::GetBlockersVector() {
  int x = 0;
  int y = kHeight_screen - (ktile_width * lane_number_);
  for (int i = 0; i < num_blockers_; i++) {
    Blocker * blocker = new Blocker(x, y, speed_, width_);
    blockers_vector_.push_back(blocker);
    x = x + (int)(width_ + ((kWidth_screen - (width_*num_blockers_))/ (double)(num_blockers_ - 1)));
  }
  return blockers_vector_;
}

int Lane::GetWidth() {
  return width_;
}

int Lane::GetSpeed() {
  return speed_;
}

int Lane::GetNumBlockers() { return num_blockers_;
}

void Lane::MoveLane() {
  //std::vector<mylibrary::Blocker> blockers_list = GetBlockersVector();
  //for (Blocker blocker: blockers_list) {
  //blocker.MoveBlocker();
  // }
  for (int i = 0; i < num_blockers_; i++) {
    blockers_vector_.at(i)->MoveBlocker();
  }
}



}