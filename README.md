## JSON ++

JSON++ is a header only JSON Parser for C++. You can read from a file and it will automagically parse it. You can also write an object to a file or parse a JSON string directly. This allows for saving data as JSON which can then be used by Node.js or other javascript runtimes.

### How To Use

To use it extract the files from the include folder into your own location / include folder.

To use in your program include the `JSON.hpp` File

**EXAMPLE:**
```cpp
#include <iostream>
#include "include/Json.hpp"

int main(void) {
  JSONObject obj = JSON::parse("JSON STRING HERE");

  return 0;
}
```

#### Methods on JSON Class

There are also some more methods and things you can do with it.
Here are some methods with the JSON class.
```cpp
void writeToFile(std::string filepath, std::string data, int spacing = 0); // Validates JSON String and writes it to a file
void writeToFile(std::string filepath, JSONObject obj, int spacing = 0); // Write a JSONObject to a file

JSONObject JSON::readFromFile(std::string str); // Read JSON from a filepath (ex: data.json) returns JSONObject
JSONObject JSON::parse(std::string str); // Parses a JSON string
std::string JSON::stringify(JSONObject obj, int spacing = 0); // Stringify a JSON Object
```


#### Access JSONObject Data

You can use subscripts on the object to access different values in the object.
Example:
```cpp
JSONObject x = JSON::readFromFile(std::string);
x["hello"] = "world";

// To get a value use .getTYPE();
std::cout << x["hello"].getString();
```
There are many methods to get the value. These should be used on the subscript.
```cpp
std::string getString();
int getInt();
double getDouble();
bool getBoolean();
```

#### Object to String

If you want to convert a JSONObject into a string then you can use the `std::string JSON::stringify(JSONObject)` function.

**EXAMPLE:**
```cpp
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
```

There are some limitations right now, like you cannot create JSON Arrays or set a key to an array. Only basic values can be set.

That's All