#include "core.h"
#include "input_parser.h"
#include "graph.h"

#include <iomanip>
#include <iterator>
#include <utility>

using namespace std;

std::pair<vector<AddQuery>, vector<ReadQuery>> TmCore::ParseQueriesFromJson(const Json::Document doc) {
    vector<AddQuery> aqs;
    vector<ReadQuery> rqs;
    
    bus_speed = doc.GetRoot().AsMap().at("routing_settings").AsMap().at("bus_velocity").AsDouble();
    wait_time = doc.GetRoot().AsMap().at("routing_settings").AsMap().at("bus_wait_time").AsInt();
    
    
    auto aqs_nodearray = doc.GetRoot().AsMap().at("base_requests").AsArray();
    auto rqs_nodearray = doc.GetRoot().AsMap().at("stat_requests").AsArray();
    
    for (auto& node : aqs_nodearray) {
        aqs.push_back(AddQuery(node));
    }
    
    for (auto& node : rqs_nodearray) {
        rqs.push_back(ReadQuery(node));
    }
    
    pair<vector<AddQuery>, vector<ReadQuery>> result = {move(aqs), move(rqs)};
    
    return move(result);
}



void TmCore::ProcessAddQueries(const vector<AddQuery>& v) {
    for (auto& q : v) {
        switch (q.GetType()) {
            case AddQuery::Type::STOP:
                sm.AddStop(q);
                break;
            case AddQuery::Type::BUS_TWO_WAY:
                bm.AddBus(q);
                sm.AddBusStats(q);
                break;
            case AddQuery::Type::BUS_ROUND_TRIP:
                bm.AddBus(q);
                sm.AddBusStats(q);
                break;
        }
    }
}

void TmCore::ProcessReadQueries(const vector<ReadQuery>& v, std::ostream& output) {
    
    for (auto& q : v) {
        
        switch (q.GetType()) {
            case ReadQuery::Type::BUS:
            {
                string number = q.GetName();
                bm.PrintBusStatsC(number, output, sm);
                break;
            }

            case ReadQuery::Type::STOP:
                string name = q.GetName();
                sm.PrintStopStats(name, output);
                break;
        }
    }
}

void TmCore::ProcessReadQueriesD(const vector<ReadQuery>& v, std::ostream& output) {
    
    output << "[" << "\n"; 
    
    if (!v.empty()) {
        cout << "{" << "\n";
    }
    
    
    bool firstq = true;
    for (auto& q : v) {
        
        
        if(!firstq) {
            output << "}," << "\n" << "{" << "\n";
        }
        firstq = false;
 
        
        
        
        switch (q.GetType()) {
            case ReadQuery::Type::BUS:
            {
                auto stats = bm.GetBusStats(q.GetName(), sm);
                if (stats) {
                    output << "\"stop_count\": " << stats->stop_number  << "," << "\n";
                    output << "\"unique_stop_count\": " << stats->unique_stops  << "," << "\n";
                    output << "\"route_length\": " << stats->route_length << ","<< "\n";
                    output << "\"curvature\": " << setprecision(6) << stats->curvature  << "," << "\n";
                } else {
                    output << "\"error_message\": \"not found\""  << "," << "\n";
                }
                break;
            }

            case ReadQuery::Type::STOP:
            {
                auto stats = sm.GetStopStats(q.GetName());
                if (stats) {
                   output << "\"buses\": [";
                   
                    bool first = true;
                    for (auto b : stats->buses_through) {
                        if (!first) {
                            output << ", ";
                        } 
                        first = false;
                        
                        output << "\"" << b << "\"";
                    }
                    output << "]," << "\n";
                    
                } else {
                    output << "\"error_message\": \"not found\"," << "\n";
                }
                break;
            }
                
            case ReadQuery::Type::ROUTE:
            {
                auto stops = q.GetContents();
                size_t from_id = sm.GetByName(stops.first)->id;
                size_t to_id = sm.GetByName(stops.second)->id;
                auto route = router->BuildRoute(from_id, to_id);
                if (route) {
                    //cout << "BUILD!!! edge_count=" << route->edge_count << " time=" << route->weight << "\n";
                    output << "\"total_time\": " << route->weight << "," << "\n"; 
                    output << "\"items\": [" << "\n";
                    for (int i = 0; i < route->edge_count; ++i) {
                        auto edge = graph->GetEdge(router->GetRouteEdge(route->id, i));
                        
                        output << "{" << "\n";
                        output << "\"type\": \"Wait\"," << "\n";
                        output << "\"time\": " << wait_time << "," << "\n";
                        output << "\"stop_name\": " << "\"" << sm.GetIdList()[edge.from] << "\"" << "\n";
                        output << "}," << "\n";
                        
                        output << "{" << "\n";
                        output << "\"type\": \"Bus\"," << "\n";
                        output << "\"time\": " << edge.weight - wait_time << "," << "\n";
                        output << "\"span_count\": " << edge.span_count << "," << "\n";
                        output << "\"bus\": " << "\"" << edge.bus << "\"" << "\n";
                        output << "}";
                        if (i != route->edge_count - 1) {
                            output << ",";
                        }
                        output << "\n";
                        
                        
                    }
                    output << "]," << "\n";
                    router->ReleaseRoute(route->id);
                } else {
                    output << "\"error_message\": \"not found\"," << "\n";
                    //cout << "ROUTE NOT FOUND" << "\n";
                }
                
                break;
            }
                
        }
        
        output << "\"request_id\": " << q.GetId() << "\n";

    }
    
    if (!v.empty()) {
        cout << "}" << "\n";
    }
    
    output << "]" << "\n";
}


void TmCore::AddStopSequenceToGraph(vector<string> stops, string bus) {
    
    for (auto it_begin = begin(stops); it_begin != prev(end(stops)); ++it_begin) {
        for (auto it_end = next(it_begin); it_end != end(stops); ++it_end) {
            Graph::Edge<double> edge;
            edge.from = sm.GetByName(*it_begin)->id;
            edge.to = sm.GetByName(*(it_end))->id;
            edge.span_count = it_end - it_begin;
            edge.weight = wait_time;
            edge.bus = bus;
            for (auto it_stop = it_begin; it_stop != it_end; ++it_stop) {
                double distance = sm.GetRoadDistance(*it_stop, *next(it_stop));
                edge.weight += distance / (bus_speed * 1000 / 60);
            }
            graph->AddEdge(edge);
            
            //cout << "adding bus" << bus << " " << *it_begin << "(" << edge.from << ") - " << *(it_end) << "(" << edge.to << ")" << "\n";
        }
    }
}


void TmCore::BuildRouter() {
    graph = make_shared<Graph::DirectedWeightedGraph<double>>(sm.GetIdList().size());
    
    for (const auto& [n, bus] : bm.GetBuses()) {
        AddStopSequenceToGraph(bus.stops, bus.number);
        if (bus.route_type == RouteType::TWO_WAY) {
            AddStopSequenceToGraph({rbegin(bus.stops), rend(bus.stops)}, bus.number);
        }
    }
    
    router = make_shared<Graph::Router<double>>(*graph);
}
