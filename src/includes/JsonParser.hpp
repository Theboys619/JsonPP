#include <iostream>
#include "JsonLexer.hpp"

class SyntaxError : public std::exception {
  std::string message;
  public:

  const char* what() const noexcept {
    return message.c_str();
  };

  SyntaxError(std::string message): message(message) {};
};

enum class ExprTypes {
  None,
  Integer,
  Double,
  String,
  Boolean,
  Key,
  Entry,
  Object,
};

class Expression {
  public:
  ExprTypes type;
  JSONToken value;

  std::string key;
  Expression* jsonvalue;

  std::vector<Expression*> entries;

  Expression(JSONToken value): type(ExprTypes::None), value(value) {};
  Expression(ExprTypes type, JSONToken value): type(type), value(value) {};
};

class JSONParser {
  std::vector<JSONToken> tokens;
  int pos = 0;

  JSONToken curTok;

  Expression* ast;

  public:

  JSONParser(std::vector<JSONToken> tokens): tokens(tokens) {};

  JSONToken advance(int amt = 1) {
    pos += amt;
    return curTok = tokens[pos];
  }

  JSONToken peek(int amt = 1) {
    return tokens[pos + amt];
  }

  bool isType(std::string type, std::string value, JSONToken peeked) {
    return peeked.type == type && peeked.getString() == value;
  }
  bool isType(std::string type, JSONToken peeked) {
    return peeked.type == type;
  }
  bool isType(std::string type, std::string value) {
    return curTok.type == type && curTok.getString() == value;
  }
  bool isType(std::string type) {
    return curTok.type == type;
  }

  bool isIgnore(JSONToken tok) {
    return tok.type == "Delimiter" && tok.getString() == "\n";
  }

  bool isEOF() {
    if (curTok.isNull()) return true;

    return curTok.type == "EOF";
  }

  void skipOverVal(std::string val, JSONToken tok) {
    if (tok.getString() != val) throw SyntaxError("Unexpected JSONToken with value '" + val + "'");

    advance();
  }

  void skipOver(std::string type, JSONToken tok) {
    if (tok.type != type) 
      throw SyntaxError("Unexpected JSONToken '" + type + "'");

    advance();
  }

  void skipOver(std::string type, std::string val, JSONToken tok) {
    if (tok.type != type) 
      throw SyntaxError("Unexpected JSONToken '" + type + "'");
    if (tok.getString() != val) 
      throw SyntaxError("Unexpected JSONToken with value '" + val + "'");

    advance();
  }

  std::vector<Expression*> pDelimiters(std::string start, std::string end, std::string separator) {
    std::vector<Expression*> values;
    bool isFirst = true;

    skipOverVal(start, curTok);
    if (isIgnore(curTok)) advance();

    while (!isEOF()) {
      if (isType("Delimiter", end)) {
        break;
      } else if (isFirst) {
        isFirst = false;
      } else {
        skipOverVal(separator, curTok);
      }

      Expression* val = pExpression();
      values.push_back(val);
    }
    skipOverVal(end, curTok);

    return values;
  }

  Expression* pEntry() {
    JSONToken keyTok = curTok;

    advance(); // Skip over "key"
    skipOverVal(":", curTok); // Skip over ":"

    Expression* value = pExpression();

    Expression* entry = new Expression(ExprTypes::Entry, keyTok);
    entry->key = keyTok.getString();
    entry->jsonvalue = value;

    return entry;
  }

  Expression* pObject() {
    Expression* object = new Expression(ExprTypes::Object, curTok);
    std::vector<Expression*> entries = pDelimiters("{", "}", ",");

    object->entries = entries;

    return object;
  }

  Expression* pAll() {
    // Other Stuff Goes Here

    Expression* oldTok = new Expression(curTok);

    if (isType("Delimiter", "{", curTok))
      return pObject();

    if (isType("String", curTok)) {
      if (isType("Operator", ":", peek()))
        return pEntry();
      else
        advance();

      oldTok->type = ExprTypes::String;

      return oldTok;
    }

    if (isType("Integer", curTok)) {
      oldTok->type = ExprTypes::Integer;
      advance();


      return oldTok;
    }

    if (isType("Double", curTok)) {
      oldTok->type = ExprTypes::Double;
      advance();

      return oldTok;
    }

    if (isType("Boolean", curTok)) {
      oldTok->type = ExprTypes::Boolean;
      advance();

      return oldTok;
    }

    if (isIgnore(curTok))
      return oldTok;    
  }

  Expression* pExpression() {
    return pAll();
  }

  Expression* parse() {
    curTok = tokens[0];

    if (curTok.type != "Delimiter" && curTok.getString() != "{") {
      std::cout << "SyntaxError: Invalid Token" << std::endl;
      return ast;
    }

    ast = pExpression();

    return ast;
  }
};