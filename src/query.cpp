#include "query.h"
#include "input_parser.h"

#include <iostream>
#include <string>

using namespace std;

string Query::GetName() const {
    return name;
}


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
        delim = ", ";
    }
    
    while(!s.empty()) {
        contents.push_back(string(ReadToken(s, delim)));
    }
    
}


AddQuery::Type AddQuery::GetType() const {
    return type;
}

vector<string> AddQuery::GetContents() const {
    return contents;
}

ReadQuery::ReadQuery(const string& input) {
    string_view s(input);    
    string_view query_type = ReadToken(s, " ");
    name = string(s);
    
    if (query_type == "Bus") {
        type = Type::BUS;
    } 
    
}

ReadQuery::Type ReadQuery::GetType() const {
    return type;
}
