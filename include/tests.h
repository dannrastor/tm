#include "test_runner.h"

#include "query.h"
#include "stops.h"
#include "buses.h"
#include "core.h"


void TestAddQueryConstructor() {
    {
        string input = "Bus 1: Bochka bass - kolbasit - solo";
        vector<string> expected_contents = {"Bochka bass", "kolbasit", "solo"};
        AddQuery aq(input);
        ASSERT_EQUAL(aq.GetName(), "1");
        ASSERT((aq.GetType() == AddQuery::Type::BUS_TWO_WAY));
        ASSERT_EQUAL(aq.GetContents(), expected_contents);
    }
    {
        string input = "Bus 2: Kolbaser > po poyas > golui";
        vector<string> expected_contents = {"Kolbaser", "po poyas", "golui"};
        AddQuery aq(input);
        ASSERT_EQUAL(aq.GetName(), "2");
        ASSERT((aq.GetType() == AddQuery::Type::BUS_ROUND_TRIP));
        ASSERT_EQUAL(aq.GetContents(), expected_contents);
    }
    {
        string input = "Stop Ass: 69.420, 13.37";
        vector<string> expected_contents = {"69.420", "13.37"};
        AddQuery aq(input);
        ASSERT_EQUAL(aq.GetName(), "Ass");
        ASSERT((aq.GetType() == AddQuery::Type::STOP));
        ASSERT_EQUAL(aq.GetContents(), expected_contents);
    }
} 

void TestDistance() {
    {
        StopManager sm;
        Stop a = {"Red square", 55.7539, 37.6208};
        Stop b = {"Hermitage", 59.9398, 30.3146};
        double dist = sm.GetDistance(a, b);
        ASSERT_EQUAL((dist/1000 > 634), (dist/1000 < 635));
    }
}


void TestManagers() {
    {
        BusManager bm;
        StopManager sm;
        
        bm.AddBus(AddQuery("Bus 1: A > B > A"));
        
        sm.AddStop(AddQuery("Stop A: 0.0, 0.0"));
        sm.AddStop(AddQuery("Stop B: 0.0, 0.05"));
        
        stringstream ss;
        string expected = "Bus 1: 3 stops on route, 2 unique stops, 11119.5 route length\n";
        bm.PrintBusStats(1, ss, sm);
        ASSERT_EQUAL(expected, ss.str());
    }
    
}

void TestParseQueries() {
    TmCore tmc;
    stringstream add_input("3\n"
                            "Bus 1: A > B > A\n"
                            "Stop A: 1.0, 2.0\n"
                            "Stop B: 0.0, 0.05");
    auto aqs = tmc.ParseQueries<AddQuery>(add_input);
    
    ASSERT_EQUAL(aqs.size(), 3);
    ASSERT((aqs[0].GetType() == AddQuery::Type::BUS_ROUND_TRIP));
    ASSERT_EQUAL(aqs[1].GetName(), "A");
    ASSERT_EQUAL(aqs[2].GetContents()[1], "0.05");
}


void TestCore() {
    stringstream add_input("3\n"
                            "Bus 1: A > B > A\n"
                            "Stop A: 0.0, 0.0\n"
                            "Stop B: 0.0, 0.05");
    stringstream read_input("1\n"
                             "Bus 1");
    stringstream ss;
    string expected = "Bus 1: 3 stops on route, 2 unique stops, 11119.5 route length\n";
    
    TmCore tmc;
    auto add_queries = tmc.ParseQueries<AddQuery>(add_input);
    auto read_queries = tmc.ParseQueries<ReadQuery>(read_input);
    tmc.ProcessAddQueries(add_queries);
    tmc.ProcessReadQueries(read_queries, ss);
    ASSERT_EQUAL(expected, ss.str());
    
}

void TestPartA() {
    stringstream input(
        "10\n"
        "Stop Tolstopaltsevo: 55.611087, 37.20829\n"
        "Stop Marushkino: 55.595884, 37.209755\n"
        "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
        "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
        "Stop Rasskazovka: 55.632761, 37.333324\n"
        "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517\n"
        "Stop Biryusinka: 55.581065, 37.64839\n"
        "Stop Universam: 55.587655, 37.645687\n"
        "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n"
        "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n"
        "3\n"
        "Bus 256\n"
        "Bus 750\n"
        "Bus 751\n");
    string expected = "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length\n"
                      "Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length\n"
                      "Bus 751: not found\n";
    
    TmCore tmc;
    
    stringstream ss;
    tmc.ProcessAddQueries(tmc.ParseQueries<AddQuery>(input));
    tmc.ProcessReadQueries(tmc.ParseQueries<ReadQuery>(input), ss);
    
    ASSERT_EQUAL(expected, ss.str());
    
    
    
}

void TestAll() {
    TestRunner tr;
    
    RUN_TEST(tr, TestAddQueryConstructor);
    RUN_TEST(tr, TestDistance);
    RUN_TEST(tr, TestManagers);
    RUN_TEST(tr, TestParseQueries);
    RUN_TEST(tr, TestCore);
    RUN_TEST(tr, TestPartA);
}
