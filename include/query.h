#pragma once

#include "query.h"
#include "json.h"

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
    explicit AddQuery(const Json::Node& nd);
    Type GetType() const;
    std::vector<string> GetContents() const;

private:
    Type type;
    std::vector<string> contents;
};

class ReadQuery : public Query{
public:
    enum class Type {
        BUS,
        STOP
    };
    
    ReadQuery(const string& input);
    explicit ReadQuery(const Json::Node& node);
    Type GetType() const;

private:
    size_t request_id;
    Type type;
};
