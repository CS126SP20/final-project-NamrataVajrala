// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <CinderImGui.h>
#include <cinder/app/App.h>
#include <mylibrary/blocker.h>
#include <mylibrary/crosser.h>
#include <cinder/gl/gl.h>

#include "mylibrary/ScoreBoard.h"
#include "mylibrary/lane.h"
#include "mylibrary/person.h"

namespace myapp {

const int kboard_size = 800;
const int ktile_size = 50;
const int knumber_lanes = 16;
const int knum_obstacles_ = 14;
const int kone = 1;
const int ktwo = 2;
const int kthree = 3;
const int kfour = 4;
const int kfive = 5;
const int khundred = 100;
const int kfifty = 50;

class MyApp : public cinder::app::App {
 private:
  mylibrary::Crosser crosser_;
  mylibrary::Crosser crosser_two_;
  std::vector<mylibrary::Lane> lanes_;
  std::vector<mylibrary::Blocker *> blockers_vector_;
  bool is_winner_;
  bool is_game_over_;
  std::vector<myLibrary::Person> winners_;
  ScoreBoard scoreboard_;
  bool safe_ = true;
  size_t speed_factor_;
  bool is_multiplayer_;

 public:
  /**
   * constrctor that intializes private variables
   */
  MyApp();
  /**
   * Loads images, audio, creates lane objects
   */
  void setup() override;
  /**
   * calls the draw method, plays sound
   */
  void update() override;
  /**
   * draws each object, checks for the game's state
   */
  void draw() override;
  /**
   * called when a key is pressed
   */
  void keyDown(cinder::app::KeyEvent) override;
  /**
   * draws the crosser object and checks for intersection
   */
  void drawCrosser(mylibrary::Crosser& crosser_obj,
      const cinder::gl::Texture2dRef& texture);
  /**
   * draws the blocker objects in each lane
   */
  void drawBlocker();
  /**
   * resets the position and winners
   */
  void Reset();
  /**
   * draws the losing screen
   */
  void drawEndGameScreen(const std::string& screen_type);
  /**
   * prints text in the attributes passed
   */
  void PrintText(const std::string& text, const cinder::ivec2& size,
                        const cinder::vec2& loc);
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
