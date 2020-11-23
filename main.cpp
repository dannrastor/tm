#include "core.h"
#include "query.h"
#include "stops.h"
#include "buses.h"

#include "tests.h"


void PerformRoutine() {
    TmCore tmc;
    auto doc = Json::Load(cin);
    
    auto queries = tmc.ParseQueriesFromJson(doc);
    tmc.ProcessAddQueries(queries.first);
    
    
    tmc.ProcessReadQueriesD(queries.second, cout);
}

int main() {
    //TestAll();
    PerformRoutine();
    return 0;
} 
