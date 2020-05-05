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
DECLARE_string(othername);
DECLARE_bool(multiplayer);
cinder::gl::Texture2dRef mTex;
cinder::gl::Texture2dRef mTexTwo;
cinder::gl::Texture2dRef mTexBack;
cinder::gl::Texture2dRef mTexBlocker;
cinder::gl::Texture2dRef mTexLog;
cinder::gl::Texture2dRef mTexWin;
cinder::gl::Texture2dRef mTexlose;
cinder::gl::Texture2dRef mTexEndGameBack;
cinder::audio::VoiceRef mVoice;

const char kDbPath[] = "identifier.sqlite";


using cinder::app::KeyEvent;
MyApp::MyApp() : scoreboard_(cinder::app::getAssetPath(kDbPath).string()),
                 speed_factor_{FLAGS_speed},
                 crosser_{FLAGS_name},
                 crosser_two_{FLAGS_othername},
                 is_multiplayer_{FLAGS_multiplayer} {}

//sets up initial states
void MyApp::setup() {
  //initialize instance variables
  isWinner_ = false;
  isGameOver_ = false;
  safe_ = false;

  //initialize lanes vector
  //screen size extensible
  int num_of_obstacles[knumber_lanes] = {0, ktwo, kthree, kthree, ktwo, kone,
                                         kthree, ktwo, kthree, ktwo, kfour,
                                         kthree, ktwo, kfive, kone, 0 };
  //int num_of_obstacles[knumber_lanes] = {0, 0, 0, 0, 0, 0, 2, 1, 4, 2, 4, 3, 2, 5, 1, 0 };
  //int num_of_obstacles[knumber_lanes] = {0, 0, 0, 0, 1, 0, 0, 0,
  //                                       0, 0, 0, 0, 0, 0, 1, 0 };
  int width[knumber_lanes] = {0, khundred, khundred, kfifty, khundred*ktwo,
                              khundred+kfifty, kfifty, kfifty,
                              khundred, khundred, khundred+kfifty,
                              khundred+kfifty, khundred, kfifty,
                              khundred+kfifty, 0 };
  int speed[knumber_lanes] = {0, -kone, ktwo, -ktwo, kone, -kthree, kfour,
                              -kone, kone,
                              -ktwo, kone, -kone, kthree, -kone, kfour, 0};
  for (int i = 0; i < knumber_lanes; i++) {
    if (speed[i] > 0) {
      speed[i] += speed_factor_;
    } else {
      speed[i] -= speed_factor_;
    }
  }
  num_obstacles_ = 8;
  for (int i = 0; i < knumber_lanes; i++) {
    mylibrary::Lane lane(num_of_obstacles[i], width[i],
        i + 1, speed[i], blockers_vector_);
    lanes_.push_back(lane);
  }

  //load all external images/audio files
  //load sound file
  cinder::audio::SourceFileRef sourceFile = cinder::audio::load(
      cinder::app::loadAsset( "Phineas and"
                              " Ferb Soundtrack "
                              "Intro Instrumental.wav" ) );
  mVoice = cinder::audio::Voice::create( sourceFile );
  //load background image
  auto img_crosser_back = loadImage(cinder::app::loadAsset(
      "2ec01f5f9ef54422276d913e6cb4e8f9.jpg" ) );
  mTexBack = cinder::gl::Texture2d::create( img_crosser_back );
  //set an image for crosser
  auto img_crosser = loadImage(cinder::app::loadAsset(
      "f3a453e988c557182b5494a3ac794d92.png" ) );
  mTex = cinder::gl::Texture2d::create( img_crosser );
  //set an image for crosser two
  auto img_crosser_two = loadImage(cinder::app::loadAsset(
      "p6.png" ) );
  mTexTwo = cinder::gl::Texture2d::create( img_crosser_two );
  //set lose screen images
  auto img_end_back = loadImage(cinder::app::loadAsset(
      "Screen Shot 2020-04-25 at 9.54.20 PM.png") );
  mTexEndGameBack = cinder::gl::Texture2d::create( img_end_back );
  auto img_lose = loadImage(cinder::app::loadAsset(
      "26-512.png") );
  mTexlose = cinder::gl::Texture2d::create( img_lose );
  //set win screen images
  auto img_win = loadImage(cinder::app::loadAsset(
      "winners-clipart-17.png") );
  mTexWin = cinder::gl::Texture2d::create( img_win );
  //set images for blocker
  auto img_blocker = loadImage(cinder::app::loadAsset(
      "volleyball-game-sports-court-play-512.png") );
  mTexBlocker = cinder::gl::Texture2d::create( img_blocker );
  //set images for log
  auto img_log = loadImage(cinder::app::loadAsset(
      "rectangle-shape-clipart-28.png") );
  mTexLog = cinder::gl::Texture2d::create( img_log );
}

//calls draw and audio
void MyApp::update() {
  if (!mVoice->isPlaying()) {
    mVoice->start();
  }
  cinder::gl::clear();
  draw();
}

//calls diff draw components and handles game over
void MyApp::draw() {

  //Game over functionality
  if (isGameOver_) {
    //add scores to leaderboard
    if (winners_.empty()) {
      scoreboard_.AddScore(myLibrary::Person(crosser_.GetName(),
          static_cast<size_t>( crosser_.GetScore())));
      scoreboard_.AddScore(myLibrary::Person(crosser_two_.GetName(),
          static_cast<size_t>( crosser_two_.GetScore())));
      winners_ = scoreboard_.RetrieveHighScores(3);
      assert(!winners_.empty());
    }
    //draw end game screens
    if (isWinner_) {
      drawEndGameScreen("win");
      return;
    } else {
      drawEndGameScreen("lose");
      return;
    }
  }

  //draw background
  cinder::gl::draw( mTexBack, cinder::Rectf(0, 0,
      kboard_size, kboard_size));

  //draw all blockers
  drawBlocker();

  //draw crosser
  drawCrosser(crosser_, mTex);
  if (is_multiplayer_) {
    drawCrosser(crosser_two_, mTexTwo);
  }
}

//problem: pause and reset game functionality
void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP: {
      crosser_.Move(mylibrary::Direction::kUp);
      break;
    }
    case KeyEvent::KEY_DOWN: {
      crosser_.Move(mylibrary::Direction::kDown);
      break;
    }
    case KeyEvent::KEY_LEFT: {
      crosser_.Move(mylibrary::Direction::kLeft);
      break;
    }
    case KeyEvent::KEY_RIGHT: {
      crosser_.Move(mylibrary::Direction::kRight);
      break;
    }
    //handle key clicks for multiplayer
    case KeyEvent::KEY_w: {
      if (is_multiplayer_) {
        crosser_two_.Move(mylibrary::Direction::kUp);
      }
      break;
    }
    case KeyEvent::KEY_s: {
      if (is_multiplayer_) {
        crosser_two_.Move(mylibrary::Direction::kDown);
      }
      break;
    }
    case KeyEvent::KEY_a: {
      if (is_multiplayer_) {
        crosser_two_.Move(mylibrary::Direction::kLeft);
      }
      break;
    }
    case KeyEvent::KEY_d: {
      if (is_multiplayer_) {
        crosser_two_.Move(mylibrary::Direction::kRight);
      }
      break;
    }
    case KeyEvent::KEY_q: {
      isGameOver_ = false;
      isWinner_ = false;
      crosser_.SetScore(0);
      crosser_two_.SetScore(0);
      cinder::gl::clear();
      Reset();
    }
  }
}

void MyApp::drawCrosser(mylibrary::Crosser& crosser_obj,
    cinder::gl::Texture2dRef texture) {
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
        //if crosser hits a block, end game
        if (crosser_obj.DoesIntersect(loc.Row(), loc.Col(),
                                   loc.Row() + l.GetWidth(),
                                   loc.Col() + ktile_size)) {
          isWinner_ = false;
          isGameOver_ = true;
          crosser_.CalculateScore(speed_factor_);
          crosser_two_.CalculateScore(speed_factor_);
          Reset();
        }
      }
    }
    //check if crosser is in the winning position
    blockers_vect.clear();
    if (crosser_obj.IsInWinningPosition()) {
      isWinner_ = true;
      isGameOver_ = true;
      crosser_.CalculateScore(speed_factor_);
      crosser_two_.CalculateScore(speed_factor_);
      Reset();
    }
    count++;
  }

  //Implementation for the blockers that the crosser jumps onto
  safe_ = false;
  if (crosser_obj.GetLocation().Col() < (kboard_size -
  (ktile_size * num_obstacles_))) {
    for (mylibrary::Lane l : lanes_) {
      blockers_vect = l.GetBlockersVector();
      if (count >= num_obstacles_) {
        for (int i = 0; i < l.GetNumBlockers(); i++) {
          mylibrary::Location loc = (blockers_vect.at(i))->GetLocation();
          //if crosser hits the block, make it safe
          if (crosser_obj.DoesIntersect(loc.Row(), loc.Col(),
                                     loc.Row() + l.GetWidth(),
                                     loc.Col() + ktile_size)) {
            safe_ = true;
            crosser_obj.SetLocation(blockers_vect.at(i)->GetCenterLocation());
          }
        }
      }
      count++;
    }
    if (safe_ == false) {
      isWinner_ = false;
      isGameOver_ = true;
      safe_ = false;
      crosser_.CalculateScore(speed_factor_);
      crosser_two_.CalculateScore(speed_factor_);
      Reset();
    }
  }

  //set crossers location and draw
  mylibrary::Location loc = crosser_obj.GetLocation();
  cinder::gl::draw( texture, cinder::Rectf(loc.Row(), loc.Col(),
      loc.Row() + ktile_size,
      loc.Col() + ktile_size));
}

//draws all blocker objects
void MyApp::drawBlocker() {
  //loop through each lane and draw each blocker
  for (int i = 0; i < lanes_.size(); i++) {
    blockers_vector_ = lanes_[i].GetBlockersVector();
    for (int j = 0; j < lanes_[i].GetNumBlockers(); j++) {
      blockers_vector_.at(j)->MoveBlocker();
      mylibrary::Location loc_top = blockers_vector_.at(j)->GetLocation();
      if(i < num_obstacles_) {
        cinder::gl::draw( mTexBlocker, cinder::Rectf(loc_top.Row(),
            loc_top.Col(),loc_top.Row() + lanes_[i].GetWidth(),
            loc_top.Col() + ktile_size));
      } else {
        cinder::gl::draw( mTexLog, cinder::Rectf(loc_top.Row(),
            loc_top.Col(),loc_top.Row() + lanes_[i].GetWidth(),
            loc_top.Col() + ktile_size));
      }
    }
    blockers_vector_.clear();
  }
}

void MyApp::drawEndGameScreen(std::string screen_type) {
  cinder::gl::clear();
  //draw screen
  cinder::gl::draw( mTexEndGameBack, cinder::Rectf(0, 0,
                                            kboard_size,
                                            kboard_size));
  if (screen_type == "lose") {
    cinder::gl::draw( mTexlose, cinder::Rectf(kfifty*(ktwo+kthree),
                                              kfifty*ktwo,
                                              kboard_size/ktwo + kfifty*kthree,
        kboard_size/ktwo));
  }
  if (screen_type == "win") {
    cinder::gl::draw( mTexWin, cinder::Rectf(kfifty*kthree,
                                             kfifty*kthree,
                                             kboard_size - kfifty*kthree,
                                             kfifty*(kfive)));
  }

  //print score information
  const cinder::ivec2 size = {kboard_size/ktwo + khundred, kfifty};
  std::stringstream ss;
  std::stringstream ss_one;
  std::stringstream ss_two;
  std::stringstream ss_three;
  //print first player score
  const cinder::vec2 middle = {kboard_size/ktwo, kboard_size/ktwo};
  ss_one << "Score: " << crosser_.GetName() << " - " << crosser_.GetScore();
  PrintText(ss_one.str(), size, middle);
  //print multiplayer info
  if (is_multiplayer_) {
    ss_two << "Score: " << crosser_two_.GetName() << " - "
    << crosser_two_.GetScore();
    PrintText(ss_two.str(), size, {middle.x,
                               middle.y+kfifty});
    if(crosser_.GetScore() > crosser_two_.GetScore()) {
      ss_three << crosser_.GetName() << " WON!";
    } else if(crosser_.GetScore() < crosser_two_.GetScore()) {
      ss_three << crosser_two_.GetName() << " WON!";
    } else {
      ss_three << "TIE GAME";
    }
    PrintText(ss_three.str(), size, {middle.x,
                                     middle.y+khundred});
    ss.clear();
  }
  const cinder::vec2 center = {middle.x, middle.y+(khundred*ktwo)};
  PrintText("Press key Q to go back to the game", size,
      {middle.x, middle.y+(kthree*kfifty)});
  //print high scores
  PrintText("Overall Highest Scores", size, center);
  size_t row = 0;
  for (const myLibrary::Person& person : winners_) {
    std::stringstream ss;
    ss << person.name << " - " << person.score;
    PrintText(ss.str(), size, {center.x,
                               center.y + (++row) * kfifty});
  }
}

//prints text
void MyApp::PrintText(const std::string& text, const cinder::ivec2& size,
               const cinder::vec2& loc) {
  int font_size = 30;
  int two = 2;
  auto box = TextBox()
      .alignment(TextBox::CENTER)
      .font(cinder::Font("Arial", font_size))
      .size(size)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / two,
                             loc.y - box_size.y / two};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}

//resets game variables
void MyApp::Reset() {
  crosser_.SetLocation(mylibrary::Location(
      kboard_size/ktwo - ktile_size/ktwo,
      kboard_size - ktile_size));
  crosser_two_.SetLocation(mylibrary::Location(
      kboard_size/ktwo - ktile_size/ktwo,
      kboard_size - ktile_size));
  winners_.clear();
}

}  // namespace myapp
