#include "stops.h"
#include "input_parser.h"
#include "buses.h"

#include<cmath>
#include<sstream>
#include<iterator>

using namespace std;


double StopManager::GetPhysicalDistance(const Stop& lhs, const Stop& rhs) const {
    double l_lat = lhs.latitude / 180 * 3.1415926535;
    double r_lat = rhs.latitude / 180 * 3.1415926535; 
    double l_lon = lhs.longitude / 180 * 3.1415926535; 
    double r_lon = rhs.longitude / 180 * 3.1415926535; 

    return acos(sin(l_lat)*sin(r_lat) + cos(l_lat)*cos(r_lat)*cos(abs(l_lon - r_lon))) * 6371000;    
}

double StopManager::GetPhysicalDistance(const string& lhs, const string& rhs) const {
    return GetPhysicalDistance(*GetByName(lhs), *GetByName(rhs));
}

int StopManager::GetRoadDistance(const Stop& from, const Stop& to) const {
    if (from.distance_to.count(to.name)) {
        return from.distance_to.at(to.name);
    } else {
        return to.distance_to.at(from.name);
    }
}

int StopManager::GetRoadDistance(const string& from, const string& to) const {
    return GetRoadDistance(*GetByName(from), *GetByName(to));
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
        all_stops[name] = {name, lat, lon, {}, {}};
        //auto& contents = query.GetContents();
    }
    
//     for (auto it : query.GetContents()) {
//         cout << "[" << it << "]" << endl;
//     }
    
    
    int n_contents = query.GetContents().size();
    
  
    
    if (n_contents > 2) {
        for (int i = 2; i < n_contents; ++i) {
            string s = query.GetContents()[i];
            string_view sv(s);
            string distance(ReadToken(sv, "m to "));
            string destination(sv);
            all_stops[name].distance_to[destination] = ConvertToInt(distance);
        }
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


optional<StopManager::StopStats> StopManager::GetStopStats(string name) const {
    auto stop_opt = GetByName(name);
    
    if (stop_opt) {
        StopManager::StopStats stats;
        stats.buses_through = stop_opt -> buses_through;
        return stats;
    } else {
        return nullopt;
    }
}


double StopManager::CalculatePhysicalLength(const Bus& b) const {
    auto it_b = b.stops.begin();
    auto it_e = b.stops.end();
    
    double result = 0;
    
    for (auto& it = it_b; it != prev(it_e); ++it) {
        result += GetPhysicalDistance(*it, *next(it));
    }
    
    if (b.route_type == RouteType::TWO_WAY) {
        result *= 2;
    }
    
    return result;
}

int StopManager::CalculateRoadLength(const Bus& b) const {
    
    auto it_b = b.stops.begin();
    auto it_e = b.stops.end();
    
    int result = 0;
    
    for (auto& it = it_b; it != prev(it_e); ++it) {
        result += GetRoadDistance(*it, *next(it));
    }
    
    if (b.route_type == RouteType::TWO_WAY) {
        auto rit_b = rbegin(b.stops);
        auto rit_e = rend(b.stops);
        for (auto& it = rit_b; it != prev(rit_e); ++it) {
            result += GetRoadDistance(*it, *next(it));
        }
    }
    
    return result;
}
