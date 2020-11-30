#pragma once

#include "buses.h"
#include "stops.h"
#include "query.h"
#include "json.h"
#include "graph.h"
#include "router.h"


#include <optional>
#include <memory>
#include <utility>

using std::shared_ptr;
using std::vector;

class TmCore {
public:
    template <typename T>
    vector<T> ParseQueries(std::istream& input) {
        int n_queries;
        string line;
        vector<T> result;
        
        input >> n_queries;
        input.ignore(1);
        for (int i = 0; i < n_queries; ++i) {
            getline(input, line);
            result.push_back(T(line));
        }
        return result;
    }
    
    std::pair<vector<AddQuery>, vector<ReadQuery>> ParseQueriesFromJson(const Json::Document doc);
    
    void ProcessAddQueries(vector<AddQuery> v);
    void ProcessReadQueries(vector<ReadQuery> v, std::ostream& output);
    void ProcessReadQueriesD(vector<ReadQuery> v, std::ostream& output);
    
    void BuildRouter();
    
    Json::Node ProcessReadQuery(const ReadQuery& r);
    
private:
    
    void AddStopSequenceToGraph(vector<string> stops, string bus);
    
    StopManager sm;
    BusManager bm;
    
    int wait_time;
    double bus_speed;
    
    

    
    std::shared_ptr<Graph::DirectedWeightedGraph<double>> graph;
    std::shared_ptr<Graph::Router<double>> router;
    
};
