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
    return GetDistance(GetByName(lhs), GetByName(rhs));
}

const Stop& StopManager::GetByName(const string& name) const {
    return all_stops.at(name);
}

void StopManager::AddStop(AddQuery query) {
    Stop stop;
    stop.name = query.GetName();
    stop.latitude = ConvertToDouble(query.GetContents()[0]);
    stop.longitude = ConvertToDouble(query.GetContents()[1]);
    all_stops[stop.name] = stop;
} 
