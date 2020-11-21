#include <iostream>
#include "includes/Json.hpp"

int main(int argc, char** argv) {
  
  JSONObject x = JSON::readFromFile("./tests/test.json");

  std::cout << x["test"][0]["another"].getString() << std::endl;
  std::cout << x["test"][1].getInt() << std::endl; // Should be 2
  std::cout << x["test"][2].getInt() << std::endl; // Should be 3
  std::cout << x["test"][3].getInt() << std::endl; // Should be 4

  std::cout << JSON::stringify(x) << std::endl;

  return 0;
}