#pragma once

#include "query.h"

#include<string> 
#include<unordered_map>
#include<iostream>


using std::string;
using std::istream;


struct Stop {
    string name;
    double latitude;
    double longitude;    
};

class StopManager {
public:
    double GetDistance(const Stop& lhs, const Stop& rhs) const;
    double GetDistance(const string& lhs, const string& rhs) const;
    const Stop& GetByName(const string& name) const;
    void AddStop(const AddQuery& query);
private:
    std::unordered_map<string, Stop> all_stops;
};
