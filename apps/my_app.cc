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

#include <algorithm>
#include <chrono>
#include <cmath>
#include <string>
namespace myapp {
cinder::gl::Texture2dRef mTex;

using cinder::app::KeyEvent;

MyApp::MyApp() {
  tile_size_ = 50;
}

void MyApp::setup() {

}

void MyApp::update() {
  draw();
}

void MyApp::draw() {
  drawCrosser();
}

void MyApp::keyDown(KeyEvent event) {
//  switch (event.getCode()) {
//    case KeyEvent::KEY_UP:
//    case KeyEvent::KEY_k:
//    case KeyEvent::KEY_w: {
//      engine_.SetDirection(Direction::kLeft);
//      break;
//    }
//    case KeyEvent::KEY_DOWN:
//    case KeyEvent::KEY_j:
//    case KeyEvent::KEY_s: {
//      engine_.SetDirection(Direction::kRight);
//      break;
//    }
//    case KeyEvent::KEY_LEFT:
//    case KeyEvent::KEY_h:
//    case KeyEvent::KEY_a: {
//      engine_.SetDirection(Direction::kUp);
//      break;
//    }
//    case KeyEvent::KEY_RIGHT:
//    case KeyEvent::KEY_l:
//    case KeyEvent::KEY_d: {
//      engine_.SetDirection(Direction::kDown);
//      break;
//    }
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
//  }
}

void MyApp::drawCrosser() {
  auto img_crosser = loadImage(cinder::app::loadAsset( "f3a453e988c557182b5494a3ac794d92.png" ) );
  mTex = cinder::gl::Texture2d::create( img_crosser );
  cinder::gl::draw( mTex, cinder::Rectf(375, 750, 425, 800));
}

}  // namespace myapp
