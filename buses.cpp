#include"buses.h"
#include "input_parser.h"

#include <set>

using namespace std;

optional<const Bus&> BusManager::GetByNumber(int number) const {
    if (buses.count(number)) {
        return make_optional(buses.at(number));
    } else {
        return nullopt;
    }
}

void BusManager::AddBus(const AddQuery& query) {
    Bus bus;
    bus.number = ConvertToInt(query.name);
    if (query.GetType() == AddQuery::Type::BUS_TWO_WAY) {
        route_type = RouteType::TWO_WAY;
    } else if (query.GetType() == AddQuery::Type::BUS_ROUND_TRIP) {
        route_type = RouteType::ROUND_TRIP;
    }
    bus.stops = query.GetContents();
}


int BusManager::GetStopNumber(Bus b) const {
    return b.stops.size();
}

int BusManager::GetUniqueStopNumber(Bus b) const {
    set<string> s(b.stops.begin(), b.stops.end());
    return s.size();
}

double BusManager::GetRouteLength(Bus b, int StopManager& sm) const {
    auto it_b = b.stops.begin();
    auto it_e = b.stops.end();
    
    double result = 0;
    
    for (const auto& it_b; it != it_e; ++it) {
        result += sm.GetDistance(*it, *next(it));
    }
    
    return result;
}

void PrintBusStats(int number, std::ostream& output, StopManager& sm) const {
    output << "Bus " << number << ": ";
    auto bus_opt = GetByNumber(number);
    if (bus_opt) {
        output << GetStopNumber(*b) << " stops on route, ";
        output << GetUniqueStopNumber(*b) << "unique stops, ";
        output << setprecision(6) << GetUniqueStopNumber(*b) << "route length, ";
    }
    
}
