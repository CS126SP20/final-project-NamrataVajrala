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
  /**
   * crosser constructor, intitializes name
   * @param name name of crosser
   */
  Crosser(std::string name);
  /**
   * gets the location of crosser
   * @return location
   */
  Location GetLocation();
  /**
   * sets the crosser's location
   * @param loc location to set to
   */
  void SetLocation(Location loc);
  /**
   * checks if crosser hits the blocker
   * @param x_one first x of blocker
   * @param y_one first y of blocker
   * @param x_two second x of blocker
   * @param y_two second y of blocker
   * @return bool if it interects
   */
  bool DoesIntersect(int x_one, int y_one, int x_two, int y_two);
  /**
   * moves the crosser based on the dir
   * @param dir direction to move in
   */
  void Move(Direction dir);
  /**
   * checks if crosser is at the winning pos
   * @return bool if in pos
   */
  bool IsInWinningPosition();
  /**
   * calculates the score based on the speed
   * @param speed speed factor of crosser
   */
  void CalculateScore(int speed);
  /**
   * sets the score for the crosser
   * @param score score to set to
   */
  void SetScore(int score);
  /**
   * gets the score for the crosser
   * @return crosser's score
   */
  size_t GetScore();
  /**
   * gets the name of the crosser
   * @return name of crrosser
   */
  std::string GetName();
};

}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_EXAMPLE_H_
