#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <initializer_list>
#include "JsonParser.hpp"

std::string numberToString(int value) {
  std::string integer = std::to_string(value);

  return integer;
}
std::string numberToString(double value) {
  std::string decimal = std::to_string(value);

  if (decimal[decimal.find('0') - 1] == '.')
    return decimal.substr(0, decimal.find_first_of('0') + 1);

  decimal.erase(decimal.find_last_not_of('0') + 1, std::string::npos);
  return decimal;
}

class JSONObject {
  public:
  std::vector<Expression*> entries;
  std::map<std::string, JSONObject> mapentries;

  Expression* jsonexp;
  JSONToken token;

  enum class Types {
    Object,
    Array,
    Value
  };

  Types type;
  void* value;
  bool isArray;

  class JSONValue {
    public:
    JSONObject& obj;
    std::string key;
    Types type;

    JSONValue(JSONObject& obj)
      : obj(obj)
    {};

    JSONValue(JSONObject& obj, std::string key)
      : obj(obj),
        key(key)
    {};

    JSONValue(JSONObject& obj, std::string key, Types type)
      : obj(obj),
        key(key),
        type(type)
    {};

    JSONObject operator= (std::string newvalue) {
      obj.setValue(key, newvalue);
      return obj.mapentries[key];
    }
    JSONObject operator= (const char* newvalue) {
      obj.setValue(key, std::string(newvalue));
      return obj.mapentries[key];
    }
    JSONObject operator= (bool newvalue) {
      obj.setValue(key, newvalue);
      return obj.mapentries[key];
    }
    JSONObject operator= (int newvalue) {
      obj.setValue(key, newvalue);
      return obj.mapentries[key];
    }
    JSONObject operator= (double newvalue) {
      obj.setValue(key, newvalue);
      return obj.mapentries[key];
    }

    JSONValue operator[](std::string newkey) {
      JSONObject* o = &obj.mapentries[key];
      return JSONValue(*o, newkey, obj.mapentries[key].mapentries[newkey].type);
    }
    JSONValue operator[](int index) {
      std::string newkey = numberToString(index);
      JSONObject* o = &obj.mapentries[key];
      return JSONValue(*o, newkey, obj.mapentries[key].mapentries[newkey].type);
    }

    std::string getString() {
      return obj.mapentries[key].toString();
    }
    int getInt() {
      return obj.mapentries[key].toInt();
    }
    double getDouble() {
      return obj.mapentries[key].toDouble();
    }
    bool getBoolean() {
      return obj.mapentries[key].toBoolean();
    }

    JSONObject& read() {
      JSONObject* o = &obj.mapentries[key];
      return *o;
    }
  };

  JSONObject() {};
  JSONObject(std::string value) {
    token = JSONToken(value);
    jsonexp = new Expression(ExprTypes::String, token);
    type = Types::Value;
    initValue(token);
  }
  JSONObject(bool value) {
    token = JSONToken("Boolean", std::string(value ? "true" : "false"));
    jsonexp = new Expression(ExprTypes::Boolean, token);
    type = Types::Value;
    initValue(token);
  }
  JSONObject(int value) {
    token = JSONToken("Integer", std::to_string(value));
    jsonexp = new Expression(ExprTypes::Integer, token);
    type = Types::Value;
    initValue(token);
  }
  JSONObject(double value) {
    token = JSONToken("Double", numberToString(value));
    jsonexp = new Expression(ExprTypes::Double, token);
    type = Types::Value;
    initValue(token);
  }
  JSONObject(Expression* jsonvalue, bool isArray = false): isArray(isArray) {
    jsonexp = jsonvalue;
    token = jsonvalue->value;

    if (jsonvalue->type == ExprTypes::Object) {
      type = Types::Object;
      entries = jsonvalue->entries;
      for (Expression* entry : entries) {
        mapentries[entry->key] = JSONObject(entry->jsonvalue);
      }
    } else if (jsonvalue->type == ExprTypes::Array) {
      type = Types::Array;
      entries = jsonvalue->entries;
      isArray = true;
      int i = 0;
      for (auto entry : entries) {
        mapentries[numberToString(i)] = JSONObject(entry, true);
        i++;
      }
    } else {
      type = Types::Value;
      initValue(token);
    }
  };

  JSONValue operator[](std::string key) {
    return JSONValue(*this, key, mapentries[key].type);
  }
  JSONValue operator[](int index) {
    std::string key = numberToString(index);
    return JSONValue(*this, key, mapentries[key].type);
  }

  std::string toString() {
    if (token.type == "Boolean") return token.getString();
    if (token.type == "Integer") return token.getString();
    if (token.type == "Double" ) return token.getString();
    if (token.type == "Null"   ) return token.getString();
    return *(std::string*)value;
  }
  int toInt() {
    return *(int*)value;
  }
  double toDouble() {
    return *(double*)value;
  }
  bool toBoolean() {
    return *(bool*)value;
  }

  void setValue(std::string key, std::string val) {
    mapentries[key] = JSONObject(val);
  }
  void setValue(std::string key, int val) {
    mapentries[key] = JSONObject(val);
  }
  void setValue(std::string key, double val) {
    mapentries[key] = JSONObject(val);
  }
  void setValue(std::string key, bool val) {
    mapentries[key] = JSONObject(val);
  }

  void initValue(JSONToken tok) {
    if (tok.type == "String") {
      value = new std::string(tok.getString());
    } else if (tok.type == "Integer") {
      value = new int(std::stoi(tok.getString()));
    } else if (tok.type == "Double") {
      value = new double(std::stod(tok.getString()));
    } else if (tok.type == "Boolean") {
      value = new bool(tok.getString() == "true" ? true : false);
    } else if (tok.type == "Null") {
      value = new std::string(tok.getString());
    }

    return;
  };
};

class JSON {
  class Spacing {
    public:
    std::string spacing;
    int spaces;

    Spacing(int spaces = 0): spaces(spaces) {
      std::string space = " ";
      spacing = "";

      for (int i = 0; i < spaces; i++)
        spacing += space;
    }

    std::string getString() { return spacing; };
  };

  public:

  static std::string readFile(std::string filename) {
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

  static JSONObject readFromFile(std::string filepath) {
    std::string data = JSON::readFile(filepath);
    
    return parse(data);
  }

  static void writeToFile(std::string filepath, JSONObject obj, int spacing = 0) {
    std::ofstream File(filepath);

    File << JSON::stringify(obj, spacing);
    File.close();
  }

  static JSONObject parse(std::string data) {
    JSONLexer lexer = JSONLexer(data);
    std::vector<JSONToken> tokens = lexer.tokenize();

    // for (auto token : tokens) {
    //   token.debugPrint();
    // }

    JSONParser parser = JSONParser(tokens);
    Expression* ast = parser.parse();

    JSONObject obj = JSONObject(ast);
    return obj;
  }

  // static std::string stringit()

  static std::string stringify(JSONObject obj, int spacing = 0, int originalSpacing = 0) {
    if (originalSpacing == 0)
      originalSpacing = spacing;

    Spacing space = Spacing(originalSpacing != spacing ? spacing : 0);

    bool isArray = obj.type == JSONObject::Types::Array;
    bool isFirst = true;
    std::string main = (!isArray ? "{" : "[");

    if (obj.mapentries.begin() != obj.mapentries.end()) {
      if (originalSpacing != spacing && spacing > 0) main += "\n";
        main += space.getString();
    }

    for(auto it = obj.mapentries.begin(); it != obj.mapentries.end(); it++) {
      Spacing newspacing = Spacing(spacing);

      if (!it->second.isArray && !isArray)
        main += ((originalSpacing == spacing && spacing > 0) || spacing > 0 && !isFirst ? "\n" + newspacing.getString() : "") + "\"" + it->first + "\":";
      // else if (it->second.type == JSONObject::Types::Object || it->second.type == JSONObject::Types::Array)
      //   main += "\n" + newspacing.getString();

      if (it->second.type == JSONObject::Types::Object)
        main += (!isArray && spacing > 0 ? " " : "") + JSON::stringify(it->second, spacing + originalSpacing, originalSpacing);
      else if (it->second.type == JSONObject::Types::Array) {
        main += (isArray ? newspacing.getString() : (spacing > 0 ? " " : "")) + JSON::stringify(it->second, spacing + originalSpacing, originalSpacing);
      } else {
        if (it->second.token.type == "String")
          main += (spacing > 0 && !isArray ? " " : "") + std::string("\"") + it->second.token.getString() + "\"";
        else
          main += (spacing > 0 && !isArray ? " " : "") + it->second.token.getString();
      }
      if ((++it) != obj.mapentries.end())
        main += "," + (spacing > 0 ? "\n" + newspacing.getString() : "");
      // else {
      //   main += (spacing > 0 ? "\n" + newspacing.getString() : "");
      // }

      it--;
      isFirst = false;
    }

    if (obj.mapentries.begin() != obj.mapentries.end())
      main += (spacing > 0 ? "\n" : "");

    if (obj.mapentries.begin() != obj.mapentries.end())
      if (spacing != originalSpacing) main += Spacing(spacing - originalSpacing).getString();

    main += (!isArray ? "}" : "]");;

    return main;
  }
};