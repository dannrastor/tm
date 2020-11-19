#include "buses.h"
#include "input_parser.h"

#include <set>
#include <iomanip>

using namespace std;

optional<Bus> BusManager::GetByNumber(int number) const {
    if (buses.count(number)) {
        auto result = make_optional(buses.at(number));
        return result;
    } else {
        return nullopt;
    }
}

void BusManager::AddBus(AddQuery query) {
    Bus bus;
    bus.number = ConvertToInt(query.GetName());
    if (query.GetType() == AddQuery::Type::BUS_TWO_WAY) {
        bus.route_type = RouteType::TWO_WAY;
    } else if (query.GetType() == AddQuery::Type::BUS_ROUND_TRIP) {
        bus.route_type = RouteType::ROUND_TRIP;
    }
    bus.stops = query.GetContents();
    
    buses[bus.number] = bus;
}



int BusManager::GetStopNumber(Bus b) const {
    int result = b.stops.size();
    if (b.route_type == RouteType::TWO_WAY) {
        return result * 2 - 1;
    }
    return result;
}

int BusManager::GetUniqueStopNumber(Bus b) const {
    set<string> s(b.stops.begin(), b.stops.end());
    return s.size();
}

double BusManager::GetRouteLength(Bus b, StopManager& sm) const {
    auto it_b = b.stops.begin();
    auto it_e = b.stops.end();
    
    double result = 0;
    
    for (auto& it = it_b; it != prev(it_e); ++it) {
        result += sm.GetDistance(*it, *next(it));
    }
    
    if (b.route_type == RouteType::TWO_WAY) {
        result *= 2;
    }
    
    return result;
}

void BusManager::PrintBusStats(int number, std::ostream& output, StopManager& sm) const {
    output << "Bus " << number << ": ";

    auto bus_opt = GetByNumber(number);
       
    if (bus_opt) {
        output << GetStopNumber(*bus_opt) << " stops on route, ";
        output << GetUniqueStopNumber(*bus_opt) << " unique stops, ";
        output << setprecision(6) << GetRouteLength(*bus_opt, sm) << " route length\n";

    } else {
        output << "not found\n";
    }
    
}
