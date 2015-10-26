#include "structs.h"
#include "operators.h"
#include "test.h"

int main(int argc, char *argv[])
{
    Config config;
    CSVTable table;
    CSVAggregator aggregator;
    OptionParser op;
    Test t;

    std::vector<std::vector<std::string> > test_table = t.make_test_table();

    table.read_in("data/test.csv", config);
    config.print = true;
    config.cell_width = 15;
    
    bool te = t.compare_tables( table, test_table );

    /* Nonfunctional pseudo-code to test each of the flags by asserting that the appropriate configuration options are set, and then that the rest of the configuration options are set to the default (latter portion not implemented)
    //t.defaults();
    t.flag('p');
    t.flag('T');
    t.flag('t');
    t.flag('h');
    t.flag('j');
    t.flag('l');
    t.flag('r');
    t.flag('m');
    t.flag('M');
    t.flag('E');
    t.flag('e');
    t.flag('A');
    t.flag('S');
    t.flag('a');
    t.flag('D');
    */
    
    //t.test_aggregation();
    //t.test_derivation();
    t.test_join();
}
