#include "test_runner.h"

#include "query.h"
#include "stops.h"
#include "buses.h"
#include "core.h"
#include "json.h"

#include<fstream>
#include<vector>

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
        double dist = sm.GetPhysicalDistance(a, b);
        ASSERT_EQUAL((dist/1000 > 634), (dist/1000 < 635));
    }
}


void TestManagers() {
    {
        BusManager bm;
        StopManager sm;
        
        bm.AddBus(AddQuery("Bus 1: A > B > A"s));
        
        sm.AddStop(AddQuery("Stop A: 0.0, 0.0"s));
        sm.AddStop(AddQuery("Stop B: 0.0, 0.05"s));
        
        stringstream ss;
        string expected = "Bus 1: 3 stops on route, 2 unique stops, 11119.5 route length\n";
        bm.PrintBusStatsA("1", ss, sm);
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

void TestPartB() {
        stringstream input("13\n"
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
                            "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
                            "Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n"
                            "Stop Prazhskaya: 55.611678, 37.603831\n"
                            "6\n"
                            "Bus 256\n"
                            "Bus 750\n"
                            "Bus 751\n"
                            "Stop Samara\n"
                            "Stop Prazhskaya\n"
                            "Stop Biryulyovo Zapadnoye\n");
    
        
        string expected = "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length\n"
                        "Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length\n"
                        "Bus 751: not found\n"
                        "Stop Samara: not found\n"
                        "Stop Prazhskaya: no buses\n"
                        "Stop Biryulyovo Zapadnoye: buses 256 828\n";
    
    TmCore tmc;
    
    stringstream ss;
    tmc.ProcessAddQueries(tmc.ParseQueries<AddQuery>(input));
    tmc.ProcessReadQueries(tmc.ParseQueries<ReadQuery>(input), ss);
    
    ASSERT_EQUAL(expected, ss.str());
}

void TestAddStopC() {
    StopManager sm;
    
    sm.AddBusStats(AddQuery("Bus 1: A - B"s));
    sm.AddStop(AddQuery("Stop A: 1.0, 2.0, 10m to C, 20m to D"s));
    
    auto stop = *(sm.GetByName("A"));
    ASSERT_EQUAL(stop.name, "A");
    ASSERT_EQUAL(stop.latitude, 1.0);
    ASSERT_EQUAL(stop.longitude, 2.0);
    ASSERT_EQUAL(stop.distance_to["C"], 10);
    ASSERT_EQUAL(stop.distance_to["D"], 20);
    
}

void TestPartC() {
    stringstream input("13\n"
                        "Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino\n"
                        "Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka\n"
                        "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
                        "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
                        "Stop Rasskazovka: 55.632761, 37.333324\n"
                        "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam\n"
                        "Stop Biryusinka: 55.581065, 37.64839, 750m to Universam\n"
                        "Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya\n"
                        "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya\n"
                        "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye\n"
                        "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
                        "Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n"
                        "Stop Prazhskaya: 55.611678, 37.603831\n"
                        "6\n"
                        "Bus 256\n"
                        "Bus 750\n"
                        "Bus 751\n"
                        "Stop Samara\n"
                        "Stop Prazhskaya\n"
                        "Stop Biryulyovo Zapadnoye\n");
    
        
    string expected = "Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.361239 curvature\n"
                        "Bus 750: 5 stops on route, 3 unique stops, 27600 route length, 1.318084 curvature\n"
                        "Bus 751: not found\n"
                        "Stop Samara: not found\n"
                        "Stop Prazhskaya: no buses\n"
                        "Stop Biryulyovo Zapadnoye: buses 256 828\n";
    
    TmCore tmc;
    
    stringstream ss;
    tmc.ProcessAddQueries(tmc.ParseQueries<AddQuery>(input));
    tmc.ProcessReadQueries(tmc.ParseQueries<ReadQuery>(input), ss);
    
    ASSERT_EQUAL(expected, ss.str());
}

void TestLoadDefault() {
    ifstream input("./testtexts/partD_input.json");
    auto doc = Json::Load(input);
    auto mp = doc.GetRoot().AsMap();
    ASSERT(mp.count("base_requests"));
    ASSERT(mp.count("stat_requests"));
}

void TestJsonUpdate() {
    ifstream input("./testtexts/boolfloat.json");
    
    auto doc = Json::Load(input);

    auto m = doc.GetRoot().AsMap();
    ASSERT_EQUAL(10, m["int10"].AsInt());
    ASSERT_EQUAL(-0.33, m["float03"].AsDouble());
    ASSERT_EQUAL(-0.66, m["float06"].AsDouble());
    auto a = m["boolarray"].AsArray();
    ASSERT(a[0].AsBool());
    ASSERT(!a[1].AsBool());
}


void TestPartDInput() {
    TmCore tmc;
    ifstream input("./testtexts/partD_input.json");
    auto doc = Json::Load(input);
    
    auto queries = tmc.ParseQueriesFromJson(doc);
    
    ASSERT_EQUAL(13, queries.first.size());
    ASSERT_EQUAL(6, queries.second.size());
    
    tmc.ProcessAddQueries(queries.first);
    
    
    tmc.ProcessReadQueriesD(queries.second, cout);
}

void TestStopIds() {
    StopManager sm;
    
    
    sm.AddStop(AddQuery("Stop C: 0.0, 0.0"s));
    sm.AddBusStats(AddQuery("Bus 1: A - B"s));
    sm.AddStop(AddQuery("Stop A: 1.0, 2.0, 10m to C, 20m to D"s));
    
    
    ASSERT_EQUAL(sm.GetByName("C")->id, 0);
    ASSERT_EQUAL(sm.GetByName("A")->id, 1);
    ASSERT_EQUAL(sm.GetByName("B")->id, 2);
    
    vector<string> expected_ids = {"C", "A", "B"};
    
    ASSERT_EQUAL(sm.GetIdList(), expected_ids);
    
}

void TestReadRouteInput() {
    
}

void TestE1() {
    TmCore tmc;
    ifstream input("./testtexts/E1.json");
    auto doc = Json::Load(input);
    
    auto queries = tmc.ParseQueriesFromJson(doc);
    
    tmc.ProcessAddQueries(queries.first);
    tmc.BuildRouter();
    
    tmc.ProcessReadQueriesD(queries.second, cout);
}




void TestAll() {
    TestRunner tr;
    
    RUN_TEST(tr, TestAddQueryConstructor);
    RUN_TEST(tr, TestDistance);
    RUN_TEST(tr, TestManagers);
    RUN_TEST(tr, TestParseQueries);
//     RUN_TEST(tr, TestCore);              //irrelevant since output format changed
//     RUN_TEST(tr, TestPartA);
//     RUN_TEST(tr, TestPartB);
    RUN_TEST(tr, TestAddStopC);
//    RUN_TEST(tr, TestPartC);
  //  RUN_TEST(tr, TestLoadDefault);
   RUN_TEST(tr, TestJsonUpdate);
//    RUN_TEST(tr, TestPartDInput);
   RUN_TEST(tr, TestStopIds);
   RUN_TEST(tr, TestE1);
}
