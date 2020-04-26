//
// Created by Namrata Vajrala on 4/25/20.
//

#ifndef FINALPROJECT_SCOREBOARD_H
#define FINALPROJECT_SCOREBOARD_H

#include <sqlite_modern_cpp.h>
#include "mylibrary/person.h"


class ScoreBoard {
 private:
  sqlite::database db_;

 public:
  // Creates a new leaderboard table if it doesn't already exist.
  explicit ScoreBoard(const std::string& db_path);

  // Adds a player to the leaderboard.
  void AddScore(const myLibrary::Person&);

  // Returns a list of the players with the highest scores, in decreasing order.
  // The size of the list should be no greater than `limit`.
  std::vector<myLibrary::Person> RetrieveHighScores(const size_t limit);
};

#endif  // FINALPROJECT_SCOREBOARD_H
