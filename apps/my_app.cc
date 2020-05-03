// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>

#include <cinder/Font.h>
#include <cinder/Text.h>
#include <cinder/Vector.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
//#include <gflags/gflags.h>
#include "imgui.h"
#include "mylibrary/person.h"
#include <gflags/gflags.h>


using cinder::Color;
using cinder::ColorA;
using cinder::TextBox;

#include <mylibrary/direction.h>
#include <mylibrary/location.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
#include <vector>

#include "cinder/audio/SamplePlayerNode.h"
#include "cinder/audio/Source.h"
#include "cinder/audio/audio.h"
#include "cinder/gl/Texture.h"
#include "mylibrary/lane.h"
namespace myapp {

DECLARE_uint32(speed);
DECLARE_string(name);
cinder::gl::Texture2dRef mTex;
cinder::gl::Texture2dRef mTexBack;
cinder::gl::Texture2dRef mTexBlocker;
cinder::gl::Texture2dRef mTexLog;
cinder::gl::Texture2dRef mTexWin;
cinder::gl::Texture2dRef mTexWinTwo;
cinder::gl::Texture2dRef mTexlose;
cinder::gl::Texture2dRef mTexloseTwo;

const char kDbPath[] = "identifier.sqlite";


using cinder::app::KeyEvent;
//: scoreboard_(cinder::app::getAssetPath(kDbPath).string())
MyApp::MyApp() : scoreboard_(cinder::app::getAssetPath(kDbPath).string()),
                 name_{FLAGS_name},
                 speed_factor_{FLAGS_speed}
{

}

void MyApp::setup() {
  isWinner_ = false;
  isGameOver_ = false;
  safe_ = false;
  score_ = 0;
  //name_ = "nim";
  //screen size extensible
  int num_of_obstacles[knumber_lanes] = {0, 2, 3, 4, 3, 1, 5, 2, 3, 2, 4, 3, 2, 5, 1, 0 };
  //int num_of_obstacles[knumber_lanes] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 3, 2, 5, 1, 0 };
  //int num_of_obstacles[knumber_lanes] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  int width[knumber_lanes] = {0, 200, 100, 50, 100, 300, 50, 200, 100, 200, 100, 300, 200, 50, 300, 0 };
  size_t speed[knumber_lanes] = {0 + speed_factor_ , -2 - speed_factor_, 3 + speed_factor_,
                              -5 - speed_factor_, 3 + speed_factor_, -6 - speed_factor_,
                              5 + speed_factor_, -2 - speed_factor_, 10 + speed_factor_,
                              -10 - speed_factor_, 4 + speed_factor_, -3 - speed_factor_,
                              9 + speed_factor_, -5 - speed_factor_, 10 + speed_factor_, 0 + speed_factor_};

  num_obstacles_ = 8;
  for (int i = 0; i < knumber_lanes; i++) {
    mylibrary::Lane lane(num_of_obstacles[i], width[i], i + 1, speed[i], blockers_vector_);
    lanes_.push_back(lane);
  }
}

void MyApp::update() {
  cinder::gl::clear();
  draw();

}

void MyApp::draw() {

  winners_.clear();
  if (isGameOver_) {
    if (winners_.empty()) {
      scoreboard_.AddScore(myLibrary::Person(name_,static_cast<size_t>( score_)));
      winners_ = scoreboard_.RetrieveHighScores(3);
      assert(!winners_.empty());
    }
    if (isWinner_) {
      drawWinScreen();
      //score_ = 0;
      return;
    } else {
//      for(int i = 0; i < winners_.size(); i++) {
//        std::cout << winners_[i].name;
//        std::cout << winners_[i].score;
//      }
      drawLoseScreen();
      //score_ = 0;
      return;
    }
  }

  //draw background
  auto img_crosser_back = loadImage(cinder::app::loadAsset( "2ec01f5f9ef54422276d913e6cb4e8f9.jpg" ) );
  mTexBack = cinder::gl::Texture2d::create( img_crosser_back );
  cinder::gl::draw( mTexBack, cinder::Rectf(0, 0, 800, 800));

  //draw all blockers
  drawBlocker();

  //draw crosser
  drawCrosser();



}

//problem: pause and reset game functionality
void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP:
    case KeyEvent::KEY_k:
    case KeyEvent::KEY_w: {
      crosser_.Move(mylibrary::Direction::kUp);
      break;
    }
    case KeyEvent::KEY_DOWN:
    case KeyEvent::KEY_j:
    case KeyEvent::KEY_s: {
      crosser_.Move(mylibrary::Direction::kDown);
      break;
    }
    case KeyEvent::KEY_LEFT:
    case KeyEvent::KEY_h:
    case KeyEvent::KEY_a: {
      crosser_.Move(mylibrary::Direction::kLeft);
      break;
    }
    case KeyEvent::KEY_RIGHT:
    case KeyEvent::KEY_l:
    case KeyEvent::KEY_d: {
      crosser_.Move(mylibrary::Direction::kRight);
      break;
    }
    case KeyEvent::KEY_q: {
      isGameOver_ = false;
      isWinner_ = false;
      score_=0;
      winners_.clear();
      cinder::gl::clear();
      Reset();
    }
  }
}

void MyApp::drawCrosser() {
  safe_ = false;
  //set an image for crosser
  auto img_crosser = loadImage(cinder::app::loadAsset( "f3a453e988c557182b5494a3ac794d92.png" ) );
  mTex = cinder::gl::Texture2d::create( img_crosser );

  std::vector<mylibrary::Blocker *> blockers_vect;
  int count = 0;
  //check if crosser hits any of the blocks
  for (mylibrary::Lane l : lanes_) {
//    safe_ = false;
    blockers_vect = l.GetBlockersVector();
    if(count < num_obstacles_) {
      for (int i = 0; i < l.GetNumBlockers(); i++) {
        mylibrary::Location loc = (blockers_vect.at(i))->GetLocation();
        if (crosser_.DoesIntersect(loc.Row(), loc.Col(),
                                   loc.Row() + l.GetWidth(),
                                   loc.Col() + ktile_size)) {
//          score_ = score_ + 5;
          isWinner_ = false;
          isGameOver_ = true;
          score_ = crosser_.CalculateScore(speed_factor_);
          //score_ = score_ + (kboard_size - crosser_.GetLocation().Col()) - ktile_size*2;
          Reset();
        }
      }
    }
    //score_ = score_ + count*10;
//    if(crosser_.IsCrosserUpARow()) {
//      score_ = score_ + 5;
//    }
//    if (isGameOver_ == false) {
//      score_ = score_ + 5;
//    }
    blockers_vect.clear();
    if (crosser_.IsInWinningPosition()) {
      isWinner_ = true;
      isGameOver_ = true;
      score_ = crosser_.CalculateScore(speed_factor_);
      //score_ = score_ + (kboard_size - crosser_.GetLocation().Col());
      Reset();
    }
    count++;
  }

//  else {
//    double m = crosser_.GetLocation().Col();
//    double n = (kboard_size - (ktile_size * num_obstacles_));
//    bool x = m < n;
//    if (x) {
////        safe_ = false;
//      for (int i = 0; i < l.GetNumBlockers(); i++) {
//        mylibrary::Location loc = (blockers_vect.at(i))->GetLocation();
//        if (crosser_.DoesIntersect(loc.Row(), loc.Col(),
//                                   loc.Row() + l.GetWidth(),
//                                   loc.Col() + ktile_size)) {
//          safe_  = true;
//          //blockers_vect.at(i)->GetCenterLocation()
//          crosser_.SetLocation(loc);
//        }
//      }
//      if (safe_ == false) {
//        isWinner_ = false;
//        isGameOver_ = true;
//        safe_ = false;
//        Reset();
//      }
//    }
//  }


  int c = 0;
  safe_ = false;
  if (crosser_.GetLocation().Col() < (kboard_size - (ktile_size * num_obstacles_))) {
    for (mylibrary::Lane l : lanes_) {
      blockers_vect = l.GetBlockersVector();
      if (c >= num_obstacles_) {
        for (int i = 0; i < l.GetNumBlockers(); i++) {
          mylibrary::Location loc = (blockers_vect.at(i))->GetLocation();
          if (crosser_.DoesIntersect(loc.Row(), loc.Col(),
                                     loc.Row() + l.GetWidth(),
                                     loc.Col() + ktile_size)) {
            //score_ = score_ + 5;
            safe_ = true;
            crosser_.SetLocation(blockers_vect.at(i)->GetCenterLocation());
          }
        }
      }
      c++;
    }
    if (safe_ == false) {
      isWinner_ = false;
      isGameOver_ = true;
      safe_ = false;
      score_ = crosser_.CalculateScore(speed_factor_);
      //score_ = score_ + (kboard_size - crosser_.GetLocation().Col());
      Reset();
    }

  }

  //set crossers location and draw
  mylibrary::Location loc = crosser_.GetLocation();
  cinder::gl::draw( mTex, cinder::Rectf(loc.Row(), loc.Col(),
      loc.Row() + ktile_size,
      loc.Col() + ktile_size));
}

void MyApp::drawBlocker() {
  //intialize vector
  std::vector<std::string> blocker_images;

  for (int i = 0; i < lanes_.size(); i++) {
    //set images for blocker
    auto img_blocker = loadImage(cinder::app::loadAsset("volleyball-game-sports-court-play-512.png") );
    mTexBlocker = cinder::gl::Texture2d::create( img_blocker );
    //set images for log
    auto img_log = loadImage(cinder::app::loadAsset("rectangle-shape-clipart-28.png") );
    mTexLog = cinder::gl::Texture2d::create( img_log );

    blockers_vector_ = lanes_[i].GetBlockersVector();

    for (int j = 0; j < lanes_[i].GetNumBlockers(); j++) {
      blockers_vector_.at(j)->MoveBlocker();
      mylibrary::Location loc_top = blockers_vector_.at(j)->GetLocation();
      if(i < num_obstacles_) {
        cinder::gl::draw( mTexBlocker, cinder::Rectf(loc_top.Row(), loc_top.Col(),
                                                     loc_top.Row() + lanes_[i].GetWidth(),
                                                     loc_top.Col() + ktile_size));
      } else {
        //blocker = takes in img
        cinder::gl::draw( mTexLog, cinder::Rectf(loc_top.Row(), loc_top.Col(),
                                                     loc_top.Row() + lanes_[i].GetWidth(),
                                                     loc_top.Col() + ktile_size));
      }
    }
    blockers_vector_.clear();
  }
}

void MyApp::drawWinScreen() {
  cinder::gl::clear();
  auto img_win_two = loadImage(cinder::app::loadAsset("brewster-wallpaper-2679-002117-64_1000.jpg") );
  mTexWinTwo = cinder::gl::Texture2d::create( img_win_two );
  cinder::gl::draw( mTexWinTwo, cinder::Rectf(0, 0,
                                               kboard_size,
                                               kboard_size));

  auto img_win = loadImage(cinder::app::loadAsset("winners-clipart-17.png") );
  mTexWin = cinder::gl::Texture2d::create( img_win );
  cinder::gl::draw( mTexWin, cinder::Rectf(ktile_size*3, ktile_size*3,
                                            kboard_size - ktile_size*3,
                                            ktile_size*5.5));
  const cinder::vec2 center = {400, 400};
  const cinder::ivec2 size = {500, 50};

  std::stringstream ss;
  ss << "Your score: " << name_ << " - " << score_;
  PrintText(ss.str(), size, {400,
                             300});

  PrintText("Press key Q to go back to the game", size, {400, 350});
  PrintText("Overall Highest Scores", size, center);
  size_t row = 0;
  for (const myLibrary::Person& person : winners_) {
    std::stringstream ss;
    ss << person.name << " - " << person.score;
    PrintText(ss.str(), size, {center.x,
                                      center.y + (++row) * 50});
  }
}

void MyApp::drawLoseScreen() {
  cinder::gl::clear();
  auto img_lose_two = loadImage(cinder::app::loadAsset("Screen Shot 2020-04-25 at 9.54.20 PM.png") );
  mTexloseTwo = cinder::gl::Texture2d::create( img_lose_two );
  cinder::gl::draw( mTexloseTwo, cinder::Rectf(0, 0,
                                            kboard_size,
                                            kboard_size));

  auto img_lose = loadImage(cinder::app::loadAsset("26-512.png") );
  mTexlose = cinder::gl::Texture2d::create( img_lose );
  cinder::gl::draw( mTexlose, cinder::Rectf(250, 150,
                                            550,
                                            450));
//  ktile_size*6, ktile_size*6,
//      kboard_size - ktile_size*6,
//      kboard_size - ktile_size*9));
  const cinder::ivec2 size = {500, 50};
  std::stringstream ss;
  ss << "Your score: " << name_ << " - " << score_;
  PrintText(ss.str(), size, {400,
                             500});

  const cinder::vec2 center = {400, 600};

  PrintText("Press key Q to go back to the game", size, {400, 550});
  PrintText("Overall Highest Scores", size, center);

  size_t row = 0;
  for (const myLibrary::Person& person : winners_) {
    std::stringstream ss;
    ss << person.name << " - " << person.score;
    PrintText(ss.str(), size, {center.x,
                               center.y + (++row) * 50});
  }
  //score_ = 0;
}


void MyApp::PrintText(const std::string& text, const cinder::ivec2& size,
               const cinder::vec2& loc) {
  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font("Arial", 30))
      .size(size)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2,
                             loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}

void MyApp::Reset() {
  crosser_.SetLocation(mylibrary::Location(375, 750));
  //score_=0;
  winners_.clear();
}

}  // namespace myapp
