#include "core.h"
#include "query.h"
#include "stops.h"
#include "buses.h"

#include "tests.h"


void PerformRoutine() {
    TmCore tmc;
    tmc.ProcessAddQueries(tmc.ParseQueries<AddQuery>(cin));
    tmc.ProcessReadQueries(tmc.ParseQueries<ReadQuery>(cin), cout);
}

int main() {
    TestAll();
    PerformRoutine();
    return 0;
} 
