//
// Created by Namrata Vajrala on 4/25/20.
//

#ifndef FINALPROJECT_PERSON_H
#define FINALPROJECT_PERSON_H

#include <string>

namespace myLibrary {

struct Person {
  /**
   * person constructor with name and score
   * @param name name of person
   * @param score score of person
   */
  Person(const std::string& name, size_t score) : name(name), score(score) {}
  std::string name;
  size_t score;
};

}  // namespace snake

#endif  // FINALPROJECT_PERSON_H
