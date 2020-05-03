// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <mylibrary/blocker.h>
#include <mylibrary/crosser.h>

#include "mylibrary/ScoreBoard.h"
#include "mylibrary/lane.h"
#include "mylibrary/person.h"
//#include "mylibrary/location.h"

namespace myapp {

const int kboard_size = 800;
const int ktile_size = 50;
const int knumber_lanes = 16;

class MyApp : public cinder::app::App {
 private:
  mylibrary::Crosser crosser_;
  std::vector<mylibrary::Lane> lanes_;
  std::vector<mylibrary::Blocker *> blockers_vector_;
  bool isWinner_;
  bool isGameOver_;
  std::vector<myLibrary::Person> winners_;
  int score_;
  ScoreBoard scoreboard_;
  std::string name_;
  int num_obstacles_;
  bool safe_ = true;
  size_t speed_factor_;
  //mylibrary::Location prev_loc_;

 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
  void drawCrosser();
  void drawBlocker();
  void Reset();
  void drawWinScreen();
  void drawLoseScreen();
  void PrintText(const std::string& text, const cinder::ivec2& size,
                        const cinder::vec2& loc);
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
