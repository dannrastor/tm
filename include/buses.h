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
    
    struct BusStats {
        int stop_number;
        int unique_stops;
        int route_length;
        double curvature;
    };
    
    
    void PrintBusStatsA(string number, std::ostream& output, StopManager& sm) const;
    void PrintBusStatsC(string number, std::ostream& output, StopManager& sm) const;     
    void AddBus(AddQuery query);
    
    std::optional<BusStats> GetBusStats(string number, StopManager& sm) const;
    
    const std::unordered_map<string, Bus>& GetBuses() const { return buses; }; 
    
private:
    std::optional<Bus> GetByNumber(string number) const;
    

    int GetStopNumber(Bus b) const;
    int GetUniqueStopNumber(Bus b) const;

    
    std::unordered_map<string, Bus> buses;
};

