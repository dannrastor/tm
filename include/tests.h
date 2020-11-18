#include "test_runner.h"

#include "query.h"
#include "stops.h"
#include "buses.h"



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
        string input = "Stop Ass: 69.420 13.37";
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
        
        sm.AddStop(AddQuery("Stop A: 0.0 0.0"));
        sm.AddStop(AddQuery("Stop B: 0.0 0.05"));
        
        stringstream ss;
        string expected = "Bus 1: 3 stops on route, 2 unique stops, 11119.5 route length\n";
        bm.PrintBusStats(1, ss, sm);
        ASSERT_EQUAL(expected, ss.str());
    }
    
}

void TestAll() {
    TestRunner tr;
    
    RUN_TEST(tr, TestAddQueryConstructor);
    RUN_TEST(tr, TestDistance);
    RUN_TEST(tr, TestManagers);
}