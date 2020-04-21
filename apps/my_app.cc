// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>

#include <cinder/Font.h>
#include <cinder/Text.h>
#include <cinder/Vector.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
//#include <gflags/gflags.h>

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
cinder::gl::Texture2dRef mTex;
cinder::gl::Texture2dRef mTexBack;
cinder::gl::Texture2dRef mTexBlocker;


using cinder::app::KeyEvent;

MyApp::MyApp() {
}

void MyApp::setup() {
  int num_of_obstacles[knumber_lanes] = {0, 2, 3, 4, 3, 1, 5, 2, 3, 2, 4, 3, 2, 5, 1, 0 };
  int width[knumber_lanes] = {0, 200, 100, 50, 100, 300, 50, 200, 100, 200, 50, 100, 200, 50, 300, 0 };
  int speed[knumber_lanes] = {0, 2, 3, 20, 3, 15, 5, 2, 3, 10, 4, 3, 9, 5, 10, 0 };

  for (int i = 0; i < knumber_lanes; i++) {
    mylibrary::Lane lane(num_of_obstacles[i], width[i], i + 1, speed[i]);
    lanes_.push_back(lane);
  }
}

void MyApp::update() {
  cinder::gl::clear();
  draw();
  
}

void MyApp::draw() {
  //draw background
  auto img_crosser_back = loadImage(cinder::app::loadAsset( "2ec01f5f9ef54422276d913e6cb4e8f9.jpg" ) );
  mTexBack = cinder::gl::Texture2d::create( img_crosser_back );
  cinder::gl::draw( mTexBack, cinder::Rectf(0, 0, 800, 800));

  //draw crosser
  drawCrosser();

  //draw all blockers
  drawBlocker();

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
  }
}

void MyApp::drawCrosser() {
  auto img_crosser = loadImage(cinder::app::loadAsset( "f3a453e988c557182b5494a3ac794d92.png" ) );
  mTex = cinder::gl::Texture2d::create( img_crosser );

  mylibrary::Location loc = crosser_.GetLocation();
  cinder::gl::draw( mTex, cinder::Rectf(loc.Row(), loc.Col(),
      loc.Row() + ktile_size,
      loc.Col() + ktile_size));
}

void MyApp::drawBlocker() {
  //intialize vector
  std::vector<std::string> blocker_images;

  for (int i = 0; i < lanes_.size(); i++) {
    //set images for lane
    auto img_blocker = loadImage(cinder::app::loadAsset("hand-drawn-rounded-rectangle-rubber-stamp-hand-drawn-border-11563533358gjigxkjksg.png") );
    mTexBlocker = cinder::gl::Texture2d::create( img_blocker );

    //set initial x locations
    int x_new = 0;
    std::vector<mylibrary::Blocker> blockers = lanes_[i].GetBlockersVector();

    for (mylibrary::Blocker blocker: blockers) {
      mylibrary::Location loc_top = blocker.GetLocation();

      cinder::gl::draw( mTexBlocker, cinder::Rectf(loc_top.Row(), loc_top.Col(),
                                                   loc_top.Row() + lanes_[i].GetWidth(),
                                                   loc_top.Col() + ktile_size));
//      x_new = x_new + lanes[i].GetSpeed();
//      mylibrary::Location loc_new = mylibrary::Location(x_new, loc_top.Col());
//      blocker.SetLocation(loc_new);
//      cinder::gl::draw( mTexBlocker, cinder::Rectf(loc_new.Row(), loc_new.Col(),
//                                            loc_new.Row() + lanes[i].GetWidth(),
//                                            loc_top.Col() + ktile_size));
    }
  }
}

}  // namespace myapp
