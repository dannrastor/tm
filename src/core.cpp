#include "core.h"
#include "input_parser.h"

#include <iomanip>

using namespace std;

std::pair<vector<AddQuery>, vector<ReadQuery>> TmCore::ParseQueriesFromJson(const Json::Document doc) {
    vector<AddQuery> aqs;
    vector<ReadQuery> rqs;
    
    auto aqs_nodearray = doc.GetRoot().AsMap().at("base_requests").AsArray();
    auto rqs_nodearray = doc.GetRoot().AsMap().at("stat_requests").AsArray();
    
    for (auto& node : aqs_nodearray) {
        aqs.push_back(AddQuery(node));
    }
    
    for (auto& node : rqs_nodearray) {
        rqs.push_back(ReadQuery(node));
    }
    
    return {aqs, rqs};
}



void TmCore::ProcessAddQueries(vector<AddQuery> v) {
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

void TmCore::ProcessReadQueries(vector<ReadQuery> v, std::ostream& output) {
    
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

void TmCore::ProcessReadQueriesD(vector<ReadQuery> v, std::ostream& output) {
    
    output << "[" << endl; 
    
    if (!v.empty()) {
        cout << "{" << endl;
    }
    
    
    bool firstq = true;
    for (auto& q : v) {
        
        
        if(!firstq) {
            output << "}," << endl << "{" << endl;
        }
        firstq = false;
 
        
        
        
        switch (q.GetType()) {
            case ReadQuery::Type::BUS:
            {
                auto stats = bm.GetBusStats(q.GetName(), sm);
                if (stats) {
                    output << "\"stop_count\": " << stats->stop_number  << "," << endl;
                    output << "\"unique_stop_count\": " << stats->unique_stops  << "," << endl;
                    output << "\"route_length\": " << stats->route_length << ","<< endl;
                    output << "\"curvature\": " << setprecision(6) << stats->curvature  << "," << endl;
                } else {
                    output << "\"error_message\": \"not found\""  << "," << endl;
                }
                break;
            }

            case ReadQuery::Type::STOP:
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
                    output << "]," << endl;
                    
                } else {
                    output << "\"error_message\": \"not found\"," << endl;
                }
                break;
        }
        
        output << "\"request_id\": " << q.GetId() << endl;

    }
    
    if (!v.empty()) {
        cout << "}" << endl;
    }
    
    output << "]" << endl;
}
