//
// Created by Namrata Vajrala on 4/25/20.
//

#include "mylibrary/ScoreBoard.h"


ScoreBoard::ScoreBoard(const std::string& db_path) : db_{db_path} {
  db_ << "CREATE TABLE if not exists scoreboard (\n"
         "  name  TEXT NOT NULL,\n"
         "  score INTEGER NOT NULL\n"
         ");";
}

void ScoreBoard::AddScore(const myLibrary::Person& person) {
  db_ << "INSERT INTO scoreboard (name,score) values (?,?);"
      << person.name
      << person.score;
}

std::vector<myLibrary::Person> ScoreBoard::RetrieveHighScores(const size_t limit) {
  auto rows = db_ << "SELECT name, score "
                     "FROM scoreboard "
                     "ORDER BY score DESC "
                     "LIMIT (?);"
                  << limit;
  std::vector<myLibrary::Person> people;
  for (auto&& row : rows) {
    std::string name;
    size_t score;
    row >> name >> score;
    myLibrary::Person person = {name, score};
    people.push_back(person);
  }

  return people;
}