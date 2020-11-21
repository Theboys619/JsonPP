#include <iostream>
#include <fstream>
#include <string>

std::string readFile(std::string filename) {
  std::ifstream ReadFile(filename);
  std::string text = "";
  std::string line;

  while (std::getline(ReadFile, line)) {
    text += line + "\n";
  }

  if (text[text.length()-1] == EOF) text = text.substr(0, text.length() - 1);

  ReadFile.close();

  return text;
}