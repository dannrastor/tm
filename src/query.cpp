#include "query.h"
#include "input_parser.h"
#include "json.h"

#include <iostream>
#include <sstream>
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

AddQuery::AddQuery(const Json::Node& node) {
    auto query_map = node.AsMap();
    
    string query_type = query_map.at("type").AsString();
    if (query_type == "Bus") {
        if (query_map.at("is_roundtrip").AsBool()) {
            type = Type::BUS_ROUND_TRIP;
        } else {
            type = Type::BUS_TWO_WAY;
        }
    } else if (query_type == "Stop") {
        type = Type::STOP;
    }
    
    name = query_map.at("name").AsString();
    
    if (type == Type::STOP) {
        contents.push_back(to_string(query_map.at("latitude").AsDouble()));
        contents.push_back(to_string(query_map.at("longitude").AsDouble()));
        
        for (auto [destination, distance] : query_map.at("road_distances").AsMap()) {
            stringstream ss;
            ss << distance.AsInt();
            ss << "m to ";
            ss << destination;
            contents.push_back(ss.str());
        }
    } else {
        for (auto& stop : query_map.at("stops").AsArray()) {
            contents.push_back(stop.AsString());
        }
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
    } else if (query_type == "Stop") {
        type = Type::STOP;
    }
    
}


ReadQuery::ReadQuery(const Json::Node& node) {
    auto query_map = node.AsMap();
    
    string query_type = query_map.at("type").AsString();
    if (query_type == "Bus") {
        type = Type::BUS;
        name = query_map.at("name").AsString();
    } else if (query_type == "Stop") {
        type = Type::STOP;
        name = query_map.at("name").AsString();
    } else if (query_type == "Route") {
        type = Type::ROUTE;
        contents.first = query_map.at("from").AsString();
        contents.second = query_map.at("to").AsString();
    }
    
    
    request_id = query_map.at("id").AsInt();
}

ReadQuery::Type ReadQuery::GetType() const {
    return type;
}
