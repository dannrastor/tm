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
    std::unordered_map<string, int> distance_to;
};

class StopManager {
public:
    
    struct StopStats {
        std::set
        <string> buses_through;
    };
    
    double GetPhysicalDistance(const Stop& lhs, const Stop& rhs) const;
    double GetPhysicalDistance(const string& lhs, const string& rhs) const;
    
    int GetRoadDistance(const Stop& from, const Stop& to) const;
    int GetRoadDistance(const string& from, const string& to) const;

    void AddStop(AddQuery query);
    void AddBusStats(AddQuery query);
    void PrintStopStats(string name, std::ostream& output);
    
    double CalculatePhysicalLength(const Bus& b) const;
    int CalculateRoadLength(const Bus& b) const;
    
    std::optional<Stop> GetByName(string name) const;
    
    std::optional<StopStats> GetStopStats(string name) const;
    
private:
    
    std::unordered_map<string, Stop> all_stops;
};
