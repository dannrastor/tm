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

void TestAll() {
    TestRunner tr;
    RUN_TEST(tr, TestAddQueryConstructor);
}
