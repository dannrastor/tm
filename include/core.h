#pragma once

#include "buses.h"
#include "stops.h"
#include "query.h"

#include <memory>

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
    void ProcessAddQueries(vector<AddQuery> v);
    void ProcessReadQueries(vector<ReadQuery> v, std::ostream& output);
private:
    StopManager sm;
    BusManager bm;
};
