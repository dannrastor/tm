#include "core.h"
#include "query.h"
#include "stops.h"
#include "buses.h"

#include "tests.h"


void PerformRoutine() {
    TmCore tmc;
    auto doc = Json::Load(cin);
    
    auto queries = tmc.ParseQueriesFromJson(doc);
    tmc.ProcessAddQueries(move(queries.first));
    tmc.BuildRouter();
    
    tmc.ProcessReadQueriesD(move(queries.second), cout);
}

int main() {
    cin.tie (nullptr);
    ios_base::sync_with_stdio(false);
    //TestAll();
    PerformRoutine();
    return 0;
} 
