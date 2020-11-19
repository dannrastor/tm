#include "core.h"
#include "input_parser.h"

using namespace std;

void TmCore::ProcessAddQueries(vector<AddQuery> v) {
    for (auto& q : v) {
        switch (q.GetType()) {
            case AddQuery::Type::STOP:
                sm.AddStop(q);
                break;
            case AddQuery::Type::BUS_TWO_WAY:
                bm.AddBus(q);
                break;
            case AddQuery::Type::BUS_ROUND_TRIP:
                bm.AddBus(q);
                break;
        }
    }
}

void TmCore::ProcessReadQueries(vector<ReadQuery> v, std::ostream& output) {
    for (auto& q : v) {
        switch (q.GetType()) {
            case ReadQuery::Type::BUS:
                string number = q.GetName();
                bm.PrintBusStats(number, output, sm);
                break;
        }
    }
}
