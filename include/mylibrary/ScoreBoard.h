//
// Created by Namrata Vajrala on 4/25/20.
//

#ifndef FINALPROJECT_SCOREBOARD_H
#define FINALPROJECT_SCOREBOARD_H

#include "../../cmake-build-debug/_deps/sqlite-modern-cpp-src/hdr/sqlite_modern_cpp.h"
#include "person.h"

class ScoreBoard {
 private:
  sqlite::database db_;

 public:
  /**
   * Creates a new leaderboard table if it doesn't already exist.
   *
   */
  explicit ScoreBoard(const std::string& db_path);

  /**
   * adds a player to the scoreboard
   */
  void AddScore(const myLibrary::Person&);

  /**
   * gets the highest scoring players
   * @param limit max amount of players
   * @return vector of top players
   */
  std::vector<myLibrary::Person> RetrieveHighScores(const size_t limit);
};

#endif  // FINALPROJECT_SCOREBOARD_H
