#include "json.h"

#include <iostream>

using namespace std;

namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    for (char c; input >> c && c != ']'; ) {
      if (c != ',') {
        input.putback(c);
      }
      result.push_back(LoadNode(input));
    }

    return Node(move(result));
  }

//   Node LoadNumber(istream& input) {
//     int result = 0;
//     while (isdigit(input.peek())) {
//       result *= 10;
//       result += input.get() - '0';
//     }
//     return Node(result);
//   }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

    for (char c; input >> c && c != '}'; ) {
      if (c == ',') {
        input >> c;
      }

      string key = LoadString(input).AsString();
      input >> c;
      result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadBool(istream& input) {
    bool result = false;
    while (isalpha(input.peek())) {
      if (input.get() == 't') {
          result = true;
      }
    }
    return Node(result);
  }
  
  Node LoadNumber(istream& input) {
    string s;
    while ((input.peek() == '.') || (isdigit(input.peek()))) {
      s.push_back(input.get());
    }
    return Node(stod(s));
  }
  
  Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
      return LoadArray(input);
    } else if (c == '{') {
      return LoadDict(input);
    } else if (c == '"') {
      return LoadString(input);
    } else {
      input.putback(c);
      if (isdigit(c)) {
        return LoadNumber(input);
      } else {
        return LoadBool(input);
      }
      
    }
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }

}
