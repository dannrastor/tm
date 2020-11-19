#pragma once

#include "query.h"
//#include "buses.h"

#include<string> 
#include<unordered_map>
#include<iostream>
#include<set>
#include<optional>

using std::string;
using std::istream;

struct Bus;

struct Stop {
    string name;
    double latitude;
    double longitude;
    std::set<string> buses_through;
};

class StopManager {
public:
    double GetPhysicalDistance(const Stop& lhs, const Stop& rhs) const;
    double GetPhysicalDistance(const string& lhs, const string& rhs) const;

    void AddStop(AddQuery query);
    void AddBusStats(AddQuery query);
    void PrintStopStats(string name, std::ostream& output);
    
    double CalculatePhysicalLength(const Bus& b) const;
    
private:
    std::optional<Stop> GetByName(string name) const;
    std::unordered_map<string, Stop> all_stops;
};
