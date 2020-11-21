#include <iostream>
#include "includes/Json.hpp"

int main(int argc, char** argv) {
  
  JSONObject x = JSON::readFromFile("./tests/test.json");
  x["allow"] = false;

  while (true) {
    std::string input;
    std::cout
      << "Type C to increase Cookies by 1\n"
      << "Or Type Exit / exit to leave\nInput: ";

    std::cin >> input;

    if (input == "Exit" || input == "exit") break;
    else if (input == "c" || input == "C") {
      int cookies = x["Cookies"].getInt() + 1;
      x["Cookies"] = cookies;

      JSON::writeToFile("./tests/test.json", x);
    } else {
      std::cout
        << "Incorrect Input. Enter 'c' / 'C' or 'Exit'/ 'exit'."
        << std::endl;
    }
  }

  return 0;
}