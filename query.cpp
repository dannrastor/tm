#include "query.h"
#include "input_parser.h"

#include <iostream>
#include <string>

using namespace std;

AddQuery::AddQuery(const string& input) {
    string_view s(input);
    
    string_view prompt = ReadToken(s, ": ");
    string_view query_type = ReadToken(prompt, " ");
    name = string(prompt);
    
    string delim = " ";
    if (query_type == "Bus") {
        size_t pos = s.find(" > ");
        if (pos == s.npos) {
            delim = " - ";
            type = Type::BUS_TWO_WAY;
        } else {
            delim = " > ";
            type = Type::BUS_ROUND_TRIP;
        }
    } else if (query_type == "Stop") {
        type = Type::STOP;

    }
    
    while(!s.empty()) {
        contents.push_back(string(ReadToken(s, delim)));
    }
    
}

AddQuery::Type AddQuery::GetType() const {
    return type;
}

string AddQuery::GetName() const {
    return name;
}

vector<string> AddQuery::GetContents() const {
    return contents;
}
