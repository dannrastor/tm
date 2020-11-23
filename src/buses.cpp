#include "buses.h"
#include "input_parser.h"

#include <set>
#include <iomanip>

using namespace std;

optional<Bus> BusManager::GetByNumber(string number) const {
    if (buses.count(number)) {
        auto result = make_optional(buses.at(number));
        return result;
    } else {
        return nullopt;
    }
}

void BusManager::AddBus(AddQuery query) {
    Bus bus;
    bus.number = query.GetName();
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

void BusManager::PrintBusStatsA(string number, std::ostream& output, StopManager& sm) const {
    output << "Bus " << number << ": ";

    auto bus_opt = GetByNumber(number);
       
    if (bus_opt) {
        output << GetStopNumber(*bus_opt) << " stops on route, ";
        output << GetUniqueStopNumber(*bus_opt) << " unique stops, ";
        output << setprecision(6) << sm.CalculatePhysicalLength(*bus_opt) << " route length\n";

    } else {
        output << "not found\n";
    }
    
}

void BusManager::PrintBusStatsC(string number, std::ostream& output, StopManager& sm) const {
    output << "Bus " << number << ": ";

    auto bus_opt = GetByNumber(number);
    

    
    if (bus_opt) {
        output << GetStopNumber(*bus_opt) << " stops on route, ";
        output << GetUniqueStopNumber(*bus_opt) << " unique stops, ";
    
        
        int road_length = sm.CalculateRoadLength(*bus_opt);
        double physical_length = sm.CalculatePhysicalLength(*bus_opt);
        double curvature = 1.0 * road_length / physical_length;
        
        output << road_length << " route length, ";
        output << setprecision(7) << curvature << " curvature\n";

    } else {
        output << "not found\n";
    }
    
}
