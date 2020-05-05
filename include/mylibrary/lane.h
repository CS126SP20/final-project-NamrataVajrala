//
// Created by Namrata Vajrala on 4/20/20.
//

#ifndef FINALPROJECT_LANE_H
#define FINALPROJECT_LANE_H

#include <vector>

#include "blocker.h"

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
  std::vector<mylibrary::Blocker *> blockers_vector_;
  int height_;

 public:
  /**
   * lane object constuctor
   * @param blockers_num number of blockers per lane
   * @param width width of blockers per lane
   * @param lane_num lane num of blockers in lane
   * @param lane_speed speed of lane
   * @param blockers blockers vector
   */
  Lane(int blockers_num, int width, int lane_num,
      int lane_speed, std::vector<mylibrary::Blocker *>& blockers);
  /**
   * gets the vector of blockers
   * @return blocker's vector
   */
  std::vector<mylibrary::Blocker *>& GetBlockersVector();
  /**
   * gets the width of the blocker
   * @return int width
   */
  int GetWidth() const;
  /**
   * returns the speed of the lane
   * @return int speed
   */
  int GetSpeed() const;
  //void MoveLane();
  /**
   * gets the number of blockers
   * @return int num of blockers
   */
  int GetNumBlockers() const;
  /**
   * gets the height of the blockers
   * @return int height
   */
  int GetHeight() const;
};

}
#endif  // FINALPROJECT_LANE_H
