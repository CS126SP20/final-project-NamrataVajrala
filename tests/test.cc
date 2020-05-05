// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <mylibrary/crosser.h>
#include <mylibrary/lane.h>
#include <mylibrary/blocker.h>
#include <mylibrary/location.h>
#include <mylibrary/direction.h>
#include <catch2/catch.hpp>

TEST_CASE("Set location for crosser test", "[void]") {
  mylibrary::Crosser crosser_("meap");
  mylibrary::Location loc = {200, 400};
  crosser_.SetLocation(loc);

  //getter and setter tests
  SECTION("Set and get location for crosser test") {
    REQUIRE(crosser_.GetLocation().Row() == 200);
    REQUIRE(crosser_.GetLocation().Col() == 400);
  }

  //tests for does intersect crosser helper method
  SECTION("crosser intersects when blocker interects bottom right") {
    REQUIRE(crosser_.DoesIntersect(225, 425, 275, 475));
  }

  SECTION("crosser intersects when blocker interects top right") {
    REQUIRE(crosser_.DoesIntersect(225, 375, 275, 425));
  }

  SECTION("crosser intersects when blocker interects top left") {
    REQUIRE(crosser_.DoesIntersect(225, 375, 425, 425));
  }

  SECTION("crosser intersects when blocker interects bottom left") {
    REQUIRE(crosser_.DoesIntersect(225, 425, 425, 475));
  }

  SECTION("crosser doesn't interest when blocker doesn't interect") {
    REQUIRE(!crosser_.DoesIntersect(500, 100, 600, 150));
  }

  //score calc test
  SECTION("Calculate score for crosser test") {
    crosser_.CalculateScore(2);
    REQUIRE((int)crosser_.GetScore() == 900);
  }

  SECTION("Calculate score for crosser test if crosser didn't move") {
    crosser_.SetLocation({375, 750});
    crosser_.CalculateScore(2);
    REQUIRE((int)crosser_.GetScore() == 0);
  }

  //win position tests
  SECTION("crosser is in winning position gives true") {
    crosser_.SetLocation({200, 0});
    REQUIRE(crosser_.IsInWinningPosition());
  }

  SECTION("crosser is in winning position gives false") {
    REQUIRE(!crosser_.IsInWinningPosition());
  }

  //crosser move tests
  SECTION("Crosser moves up") {
    crosser_.Move(mylibrary::Direction::kUp);
    REQUIRE(crosser_.GetLocation().Row() == 200);
    REQUIRE(crosser_.GetLocation().Col() == 350);
  }
  SECTION("Crosser moves down") {
    crosser_.Move(mylibrary::Direction::kDown);
    REQUIRE(crosser_.GetLocation().Row() == 200);
    REQUIRE(crosser_.GetLocation().Col() == 450);
  }
  SECTION("Crosser moves left") {
    crosser_.Move(mylibrary::Direction::kLeft);
    REQUIRE(crosser_.GetLocation().Row() == 150);
    REQUIRE(crosser_.GetLocation().Col() == 400);
  }
  SECTION("Crosser moves Right") {
    crosser_.Move(mylibrary::Direction::kRight);
    REQUIRE(crosser_.GetLocation().Row() == 250);
    REQUIRE(crosser_.GetLocation().Col() == 400);
  }

  //set and get score tests
  SECTION("Getter for score") {
    REQUIRE(crosser_.GetScore() == 0);
  }

  SECTION("Setter for score") {
    crosser_.SetScore(5);
    REQUIRE(crosser_.GetScore() == 5);
  }

  //get name test
  SECTION("Getter for name") {
    REQUIRE(crosser_.GetName() == "meap");
  }
}

TEST_CASE("Blocker tests", "[void]") {
  mylibrary::Blocker blocker(200, 400, 2, 100);

  SECTION("Get blocker location") {
    REQUIRE(blocker.GetLocation().Row() == 200);
    REQUIRE(blocker.GetLocation().Col() == 400);
  }

  SECTION("Set blocker location") {
    mylibrary::Location loc = {300, 500};
    blocker.SetLocation(loc);
    REQUIRE(blocker.GetLocation().Row() == 300);
    REQUIRE(blocker.GetLocation().Col() == 500);
  }

  SECTION("Get Center location for blocker test") {
    REQUIRE(blocker.GetCenterLocation().Row() == 225);
    REQUIRE(blocker.GetCenterLocation().Col() == 400);
  }

  SECTION("Move blocker when speed is positive test") {
    mylibrary::Blocker blocker_other(901, 400, 2, 100);
    blocker_other.MoveBlocker();
    REQUIRE(blocker_other.GetLocation().Row() == -100);
    REQUIRE(blocker_other.GetCenterLocation().Col() == 400);
  }

  SECTION("Move blocker when speed is negative test") {
    mylibrary::Blocker blocker_other(-200, 400, -2, 100);
    blocker_other.MoveBlocker();
    REQUIRE(blocker_other.GetLocation().Row() == 900);
    REQUIRE(blocker_other.GetCenterLocation().Col() == 400);
  }

  SECTION("Move blocker when speed is 0 test") {
    mylibrary::Blocker blocker_other(200, 400, 0, 100);
    blocker_other.MoveBlocker();
    REQUIRE(blocker_other.GetLocation().Row() == 200);
    REQUIRE(blocker_other.GetCenterLocation().Col() == 400);
  }
}

TEST_CASE("Lane tests", "[void]") {
  std::vector<mylibrary::Blocker *> blockers_vector_;
  mylibrary::Lane lane(3, 100, 1,
      2, blockers_vector_);

  SECTION("Get blockers vector test") {
    blockers_vector_ = lane.GetBlockersVector();
    REQUIRE(blockers_vector_.size() == 3);
  }

  SECTION("Get width lane test") {
    REQUIRE(lane.GetWidth() == 100);
  }

  SECTION("Get height lane test") {
    REQUIRE(lane.GetHeight() == 750);
  }

  SECTION("Get speed lane test") {
    REQUIRE(lane.GetSpeed() == 2);
  }

  SECTION("Get numblockers lane test") {
    REQUIRE(lane.GetNumBlockers() == 3);
  }
}

TEST_CASE("Location tests", "[void]") {
  mylibrary::Location loc = {200, 400};
  mylibrary::Location loc_other = {100, 300};

  SECTION("+ operator test") {
    mylibrary::Location loc_sum = {300, 700};
    REQUIRE(loc + loc_other == loc_sum);
  }

  SECTION("- operator test") {
    mylibrary::Location loc_diff = {100, 100};
    REQUIRE(loc - loc_other == loc_diff);
  }

  SECTION("-operator test") {
    mylibrary::Location loc_neg = {-200, -400};
    REQUIRE(-loc == loc_neg);
  }

  SECTION("+= operator test") {
    mylibrary::Location loc_sum = {300, 700};
    loc += loc_other;
    REQUIRE(loc == loc_sum);
  }

  SECTION("get row test") {
    REQUIRE(loc.Row() == 200);
  }

  SECTION("get col test") {
    REQUIRE(loc.Col() == 400);
  }

  SECTION("== operator test equal") {
    mylibrary::Location loc_equal = {200, 400};
    REQUIRE(loc == loc_equal);
  }

  SECTION("== operator test not equal") {
    REQUIRE(!(loc == loc_other));
  }
}