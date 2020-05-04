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
#include "cinder/audio/Voice.h"

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
cinder::gl::Texture2dRef mTexTwo;
cinder::gl::Texture2dRef mTexBack;
cinder::gl::Texture2dRef mTexBlocker;
cinder::gl::Texture2dRef mTexLog;
cinder::gl::Texture2dRef mTexWin;
cinder::gl::Texture2dRef mTexWinTwo;
cinder::gl::Texture2dRef mTexlose;
cinder::gl::Texture2dRef mTexloseTwo;
cinder::audio::VoiceRef mVoice;

const char kDbPath[] = "identifier.sqlite";


using cinder::app::KeyEvent;
MyApp::MyApp() : scoreboard_(cinder::app::getAssetPath(kDbPath).string()),
                 //name_{FLAGS_name},
                 speed_factor_{FLAGS_speed},
                 crosser_{FLAGS_name},
                 crosser_two_{"other"}
{

}

void MyApp::setup() {
  isWinner_ = false;
  isGameOver_ = false;
  safe_ = false;
  //score_ = 0;
  unsigned random_number = ( rand() % (3 - 1 + 1) ) + 1;
  //screen size extensible
  int num_of_obstacles[knumber_lanes] = {0, 2, 3, 3, 2, 1, 3, 2, 3, 2, 4, 3, 2, 5, 1, 0 };
//  for (int i = 0; i < knumber_lanes; i++) {
//    if (i == 0 || i == (knumber_lanes - 1)) {
//      num_of_obstacles[i] = 0;
//    } else {
//      num_of_obstacles[i] = random_number;
//    }
//  }
  //int num_of_obstacles[knumber_lanes] = {0, 0, 0, 0, 0, 0, 2, 1, 4, 2, 4, 3, 2, 5, 1, 0 };
  //int num_of_obstacles[knumber_lanes] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  int width[knumber_lanes] = {0, 100, 100, 50, 200, 150, 50, 30, 100, 100, 140, 150, 70, 50, 150, 0 };
  int speed[knumber_lanes] = {0, -1, 2, -2, 1, -3, 4, -1, 1, -2, 1, -1, 3, -1, 4, 0};
  for (int i = 0; i < knumber_lanes; i++) {
    if (speed[i] > 0) {
      speed[i] += speed_factor_;
    } else {
      speed[i] -= speed_factor_;
    }
  }

  cinder::audio::SourceFileRef sourceFile = cinder::audio::load(cinder::app::loadAsset( "Phineas and Ferb Soundtrack Intro Instrumental.wav" ) );
  mVoice = cinder::audio::Voice::create( sourceFile );

  // Start playing audio from the voice:
//  if (!mVoice->isPlaying()) {
//    mVoice->start();
//  }

  num_obstacles_ = 8;
  for (int i = 0; i < knumber_lanes; i++) {
    mylibrary::Lane lane(num_of_obstacles[i], width[i], i + 1, speed[i], blockers_vector_);
    lanes_.push_back(lane);
  }

  //load background image
  auto img_crosser_back = loadImage(cinder::app::loadAsset( "2ec01f5f9ef54422276d913e6cb4e8f9.jpg" ) );
  mTexBack = cinder::gl::Texture2d::create( img_crosser_back );

  //set an image for crosser
  auto img_crosser = loadImage(cinder::app::loadAsset( "f3a453e988c557182b5494a3ac794d92.png" ) );
  mTex = cinder::gl::Texture2d::create( img_crosser );

  //set an image for crosser two
  auto img_crosser_two = loadImage(cinder::app::loadAsset( "perry.jpg" ) );
  mTexTwo = cinder::gl::Texture2d::create( img_crosser_two );

  //set lose screen images
//  auto img_lose_two = loadImage(cinder::app::loadAsset("Screen Shot 2020-04-25 at 9.54.20 PM.png") );
//  mTexloseTwo = cinder::gl::Texture2d::create( img_lose_two );
  auto img_lose_two = loadImage(cinder::app::loadAsset("Screen Shot 2020-04-25 at 9.54.20 PM.png") );
  mTexloseTwo = cinder::gl::Texture2d::create( img_lose_two );
  auto img_lose = loadImage(cinder::app::loadAsset("26-512.png") );
  mTexlose = cinder::gl::Texture2d::create( img_lose );

  //set win screen images
  auto img_win_two = loadImage(cinder::app::loadAsset("Screen Shot 2020-04-25 at 9.54.20 PM.png") );
  mTexWinTwo = cinder::gl::Texture2d::create( img_win_two );
  auto img_win = loadImage(cinder::app::loadAsset("winners-clipart-17.png") );
  mTexWin = cinder::gl::Texture2d::create( img_win );

  //set images for blocker
  auto img_blocker = loadImage(cinder::app::loadAsset("volleyball-game-sports-court-play-512.png") );
  mTexBlocker = cinder::gl::Texture2d::create( img_blocker );
  //set images for log
  auto img_log = loadImage(cinder::app::loadAsset("rectangle-shape-clipart-28.png") );
  mTexLog = cinder::gl::Texture2d::create( img_log );
}

void MyApp::update() {
  if (!mVoice->isPlaying()) {
    mVoice->start();
  }
  cinder::gl::clear();
  draw();
}

void MyApp::draw() {

  //Game over functionality
  if (isGameOver_) {
    if (winners_.empty()) {
      //scoreboard_.AddScore(myLibrary::Person(name_,static_cast<size_t>( crosser_.GetScore())));
      scoreboard_.AddScore(myLibrary::Person(crosser_.GetName(),static_cast<size_t>( crosser_.GetScore())));
      scoreboard_.AddScore(myLibrary::Person(crosser_two_.GetName(),static_cast<size_t>( crosser_two_.GetScore())));
      winners_ = scoreboard_.RetrieveHighScores(3);
      assert(!winners_.empty());
    }
    if (isWinner_) {
      drawWinScreen();
      return;
    } else {
      drawLoseScreen();
      return;
    }
  }

  //draw background
  cinder::gl::draw( mTexBack, cinder::Rectf(0, 0, kboard_size, kboard_size));

  //draw all blockers
  drawBlocker();

  //draw crosser
  drawCrosser();
  //drawCrosser(crosser_two_, mTexTwo);
}

//problem: pause and reset game functionality
void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP:
//    case KeyEvent::KEY_k:
    case KeyEvent::KEY_w: {
      crosser_.Move(mylibrary::Direction::kUp);
      break;
    }
    case KeyEvent::KEY_DOWN:
//    case KeyEvent::KEY_j:
    case KeyEvent::KEY_s: {
      crosser_.Move(mylibrary::Direction::kDown);
      break;
    }
    case KeyEvent::KEY_LEFT:
//    case KeyEvent::KEY_h:
    case KeyEvent::KEY_a: {
      crosser_.Move(mylibrary::Direction::kLeft);
      break;
    }
    case KeyEvent::KEY_RIGHT:
//    case KeyEvent::KEY_l:
    case KeyEvent::KEY_d: {
      crosser_.Move(mylibrary::Direction::kRight);
      break;
    }
    case KeyEvent::KEY_k: {
      crosser_two_.Move(mylibrary::Direction::kUp);
    }
    case KeyEvent::KEY_j: {
      crosser_two_.Move(mylibrary::Direction::kDown);
    }
    case KeyEvent::KEY_h: {
      crosser_two_.Move(mylibrary::Direction::kLeft);
    }
    case KeyEvent::KEY_l: {
      crosser_two_.Move(mylibrary::Direction::kRight);
    }
    case KeyEvent::KEY_q: {
      isGameOver_ = false;
      isWinner_ = false;
      //score_=0;
      crosser_.SetScore(0);
      crosser_two_.SetScore(0);
      winners_.clear();
      cinder::gl::clear();
      Reset();
    }
  }
}

void MyApp::drawCrosser() {
  safe_ = false;

  //Implementation for the blockers that hit the crosser
  std::vector<mylibrary::Blocker *> blockers_vect;
  int count = 0;
  //check if crosser hits any of the blocks
  for (mylibrary::Lane l : lanes_) {
    blockers_vect = l.GetBlockersVector();
    if(count < num_obstacles_) {
      for (int i = 0; i < l.GetNumBlockers(); i++) {
        mylibrary::Location loc = (blockers_vect.at(i))->GetLocation();
        if (crosser_.DoesIntersect(loc.Row(), loc.Col(),
                                   loc.Row() + l.GetWidth(),
                                   loc.Col() + ktile_size) ) {
          isWinner_ = false;
          isGameOver_ = true;
          //score_ = crosser_.CalculateScore(speed_factor_);
          crosser_.CalculateScore(speed_factor_);
//          crosser_two_.CalculateScore(speed_factor_);
          Reset();
        }
      }
    }
    //check if crosser is in the winning position
    blockers_vect.clear();
    if (crosser_.IsInWinningPosition()) {
      isWinner_ = true;
      isGameOver_ = true;
      //score_ = crosser_.CalculateScore(speed_factor_);
      crosser_.CalculateScore(speed_factor_);
//      crosser_two_.CalculateScore(speed_factor_);
      Reset();
    }
    count++;
  }

  //Implementation for the blockers that the crosser jumps onto
  safe_ = false;
  if (crosser_.GetLocation().Col() < (kboard_size - (ktile_size * num_obstacles_))) {
    for (mylibrary::Lane l : lanes_) {
      blockers_vect = l.GetBlockersVector();
      if (count >= num_obstacles_) {
        for (int i = 0; i < l.GetNumBlockers(); i++) {
          mylibrary::Location loc = (blockers_vect.at(i))->GetLocation();
          if (crosser_.DoesIntersect(loc.Row(), loc.Col(),
                                     loc.Row() + l.GetWidth(),
                                     loc.Col() + ktile_size)) {
            safe_ = true;
            crosser_.SetLocation(blockers_vect.at(i)->GetCenterLocation());
//            crosser_two_.SetLocation(blockers_vect.at(i)->GetCenterLocation());
          }
        }
      }
      count++;
    }
    if (safe_ == false) {
      isWinner_ = false;
      isGameOver_ = true;
      safe_ = false;
      //score_ = crosser_.CalculateScore(speed_factor_);
      crosser_.CalculateScore(speed_factor_);
//      crosser_two_.CalculateScore(speed_factor_);
      Reset();
    }
  }

  //set crossers location and draw
  mylibrary::Location loc = crosser_.GetLocation();
//  mylibrary::Location loc_two = crosser_two_.GetLocation();
  cinder::gl::draw( mTex, cinder::Rectf(loc.Row(), loc.Col(),
      loc.Row() + ktile_size,
      loc.Col() + ktile_size));
//  cinder::gl::draw( mTexTwo, cinder::Rectf(loc_two.Row(), loc_two.Col(),
//                                           loc_two.Row() + ktile_size,
//                                           loc_two.Col() + ktile_size));
}

void MyApp::drawBlocker() {
  //intialize vector
  std::vector<std::string> blocker_images;

  for (int i = 0; i < lanes_.size(); i++) {
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

  //draw screen
  cinder::gl::draw( mTexWinTwo, cinder::Rectf(0, 0,
                                               kboard_size,
                                               kboard_size));

  cinder::gl::draw( mTexWin, cinder::Rectf(ktile_size*3, ktile_size*3,
                                            kboard_size - ktile_size*3,
                                            ktile_size*5.5));

  //print score information
  const cinder::vec2 center = {400, 450};
  const cinder::ivec2 size = {500, 50};

  std::stringstream ss;
  //ss << "Your score: " << name_ << " - " << score_;
//  ss << "Your score: " << name_ << " - " << crosser_.GetScore();
  ss << "Your score: " << crosser_.GetName() << " - " << crosser_.GetScore();
  PrintText(ss.str(), size, {400,
                             350});

  PrintText("Press key Q to go back to the game", size, {400, 400});
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

  //draw screen
  cinder::gl::draw( mTexloseTwo, cinder::Rectf(0, 0,
                                            kboard_size,
                                            kboard_size));

  cinder::gl::draw( mTexlose, cinder::Rectf(250, 150,
                                            550,
                                            450));

  //print score information
  const cinder::ivec2 size = {500, 50};
  std::stringstream ss;
  //ss << "Your score: " << name_ << " - " << score_;
  //ss << "Your score: " << name_ << " - " << crosser_.GetScore();
  ss << "Yours: " << crosser_.GetName() << " - " << crosser_.GetScore();
  PrintText(ss.str(), size, {400,
                             500});
//  ss << "second: " << crosser_two_.GetName() << " - " << crosser_two_.GetScore();
//  PrintText(ss.str(), size, {400,
//                             520});

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
  crosser_.SetLocation(mylibrary::Location(kboard_size/2 - ktile_size/2, kboard_size - ktile_size));
  crosser_two_.SetLocation(mylibrary::Location(kboard_size/2 - ktile_size/2, kboard_size - ktile_size));
  winners_.clear();
}

}  // namespace myapp
