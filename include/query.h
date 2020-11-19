#pragma once

#include "query.h"

#include<string>
#include<string_view>
#include<vector>

using std::string;

class Query {
public:    
    string GetName() const;

protected:
    string name;
};


class AddQuery : public Query {
public:    
    enum class Type {
        BUS_ROUND_TRIP,
        BUS_TWO_WAY,
        STOP
    };
    
    AddQuery(const string& input);
    Type GetType() const;
    std::vector<string> GetContents() const;

private:
    Type type;
    std::vector<string> contents;
};

class ReadQuery : public Query{
public:
    enum class Type {
        BUS
    };
    
    ReadQuery(const string& input);
    Type GetType() const;

private:
    Type type;
};
