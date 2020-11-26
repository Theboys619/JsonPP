#include <iostream>
#include "include/Json.hpp"

int main(int argc, char** argv) {
  
  JSONObject x = JSON::readFromFile("./tests/test.json");

  // std::cout << x["why"][0]["are"].getString() << std::endl;
  // std::cout << x["why"][1].getString() << std::endl; // Should be 2
  // std::cout << x["why"][2].getString() << std::endl; // Should be 3
  // std::cout << x["why"][3].getInt() << std::endl; // Should be 4
  std::cout << x["coupon_url"].getString() << std::endl;

  JSON::writeToFile("./tests/test.json", "{ \"test\": \"lol\" }", 2);

  return 0;
}