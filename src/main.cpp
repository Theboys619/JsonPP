#include <iostream>
#include "include/Json.hpp"

int main(void) {
  // Read JSONObject from a json file
  JSONObject x = JSON::readFromFile("./tests/test.json");

  // Stringify a JSONObject into a string
  std::string str = JSON::stringify(x, 2); // 2 = Spacing of JSON String
  std::cout << str << std::endl; // Log the Stringed JSON

  // Stringify a JSONObject into a file
  JSON::writeToFile("./tests/test.json", x, 2); // 2 = Spacing of JSON String

  return 0;
}