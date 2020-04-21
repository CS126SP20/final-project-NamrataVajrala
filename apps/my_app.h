// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <mylibrary/crosser.h>
#include "mylibrary/lane.h"


namespace myapp {

const int ktile_size = 50;
const int knumber_lanes = 16;

class MyApp : public cinder::app::App {
 private:
  mylibrary::Crosser crosser_;
  std::vector<mylibrary::Lane> lanes_;

 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
  void drawCrosser();
  void drawBlocker();
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
