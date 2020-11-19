#include "stops.h"
#include "input_parser.h"

#include<cmath>
#include<sstream>

using namespace std;


double StopManager::GetDistance(const Stop& lhs, const Stop& rhs) const {
    double l_lat = lhs.latitude / 180 * 3.1415926535;
    double r_lat = rhs.latitude / 180 * 3.1415926535; 
    double l_lon = lhs.longitude / 180 * 3.1415926535; 
    double r_lon = rhs.longitude / 180 * 3.1415926535; 

    return acos(sin(l_lat)*sin(r_lat) + cos(l_lat)*cos(r_lat)*cos(abs(l_lon - r_lon))) * 6371000;    
}

double StopManager::GetDistance(const string& lhs, const string& rhs) const {
    return GetDistance(*GetByName(lhs), *GetByName(rhs));
}



void StopManager::AddStop(AddQuery query) {
    Stop stop;
    string name = query.GetName();
    double lat = ConvertToDouble(query.GetContents()[0]);
    double lon = ConvertToDouble(query.GetContents()[1]);
    
    if (all_stops.count(name)) {
        all_stops[name].latitude = lat;
        all_stops[name].longitude = lon;
    } else {
        all_stops[name] = {name, lat, lon, {}};
    }
} 

void StopManager::AddBusStats(AddQuery query) {
    string number = query.GetName();
    
    for (auto& name : query.GetContents()) {
        if (!all_stops.count(name)) {
            all_stops[name] = {name, 0.0, 0.0, {}};
        }
        all_stops[name].buses_through.insert(number);
    }
    
}


optional<Stop> StopManager::GetByName(string name) const {
    if (all_stops.count(name)) {
        auto result = make_optional(all_stops.at(name));
        return result;
    } else {
        return nullopt;
    }
}

void StopManager::PrintStopStats(string name, ostream& output) {
    
    output << "Stop " << name << ":";

    auto stop_opt = GetByName(name);
       
    if (stop_opt) {
        if (stop_opt->buses_through.empty()) {
            output << " no buses\n";
        } else {
            output << " buses";
            for (const auto& b : stop_opt->buses_through) {
                output << " " << b;
            }
            output << "\n";
        }
    } else {
        output << " not found\n";
    }
}
