#include "core.h"
#include "query.h"
#include "stops.h"
#include "buses.h"

#include "tests.h"


int main() {
    //TestAll();
    TmCore tmc;
    tmc.ProcessAddQueries(tmc.ParseQueries<AddQuery>(cin));
    tmc.ProcessReadQueries(tmc.ParseQueries<ReadQuery>(cin), cout);
    return 0;
} 
