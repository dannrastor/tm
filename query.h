#pragma once

#include "query.h"

#include<string>
#include<string_view>
#include<vector>

using std::string;

class AddQuery {
public:    
    enum class Type {
        BUS_ROUND_TRIP,
        BUS_TWO_WAY,
        STOP
    };
    
    AddQuery(const string& input);
    Type GetType() const;
    string GetName() const;
    std::vector<string> GetContents() const;

private:
    Type type;
    string name;
    std::vector<string> contents;
};


