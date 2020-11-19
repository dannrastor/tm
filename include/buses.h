#pragma once

#include "query.h"
#include "stops.h"

#include<string>
#include<vector>
#include<unordered_set>
#include<iostream>
#include<optional>

using std::string;

enum class RouteType {
    TWO_WAY,
    ROUND_TRIP
};

struct Bus {
    string number;
    std::vector<string> stops;
    RouteType route_type;    
};

class BusManager {
public:

    

    void PrintBusStats(string number, std::ostream& output, StopManager& sm) const;
    
    void AddBus(AddQuery query);
    
    
private:
    std::optional<Bus> GetByNumber(string number) const;
    
    //number should be valid
    int GetStopNumber(Bus b) const;
    int GetUniqueStopNumber(Bus b) const;

    
    std::unordered_map<string, Bus> buses;
};

