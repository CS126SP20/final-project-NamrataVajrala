// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>

#include <cinder/Font.h>
#include <cinder/Text.h>
#include <cinder/Vector.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
//#include <gflags/gflags.h>
#include "cinder/gl/Texture.h"

#include "cinder/audio/audio.h"
#include "cinder/audio/Source.h"
#include "cinder/audio/SamplePlayerNode.h"

#include <mylibrary/location.h>
#include <mylibrary/direction.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
namespace myapp {
cinder::gl::Texture2dRef mTex;
cinder::gl::Texture2dRef mTexBack;


using cinder::app::KeyEvent;

MyApp::MyApp() {
}

void MyApp::setup() {

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
}

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
//    case KeyEvent::KEY_p: {
//      paused_ = !paused_;
//
//      if (paused_) {
//        last_pause_time_ = system_clock::now();
//      } else {
//        last_intact_time_ += system_clock::now() - last_pause_time_;
//      }
//      break;
//    }
//    case KeyEvent::KEY_r: {
//      ResetGame();
//      break;
//    }
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

}  // namespace myapp
