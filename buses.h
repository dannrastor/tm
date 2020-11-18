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
    int number;
    std::vector<string> stops;
    RouteType route_type;    
};

class BusManager {
public:

    

    void PrintBusStats(int number, std::ostream& output, StopManager& sm) const;
    
    void AddBus(const AddQuery& query);
    
    
private:
    std::optional<const Bus&> GetByNumber(int number) const;
    
    //number should be valid
    int GetStopNumber(int number) const;
    int GetUniqueStopNumber(int number) const;
    double GetRouteLength(int number, int StopManager& sm) const;
    
    std::unordered_map<int, Bus> buses;
};

