#ifndef TEST_H
#define TEST_H

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <map>
#include <GetOpt.h>
#include <numeric>

#include "structs.h"
#include "operators.h"

class Test {
    public:
        std::vector<std::vector<std::string> > make_test_table();
        void read_in(std::string filename);
        void defaults(Config &config);
        void defaults();
        void flag(char option_char);
        bool compare_rows(CSVRow r1, std::vector<std::string> r2);
        bool compare_rows(CSVRow r1, CSVRow r2);
        bool compare_tables(CSVTable t1, std::vector<std::vector<std::string> > t2);
        bool compare_tables(CSVTable t1, CSVTable t2);
        void test_aggregation();
        void test_derivation();
        void test_join();
        CSVTable table;
        CSVAggregator aggregator;
        CSVDerivator derivator;
        CSVJoiner joiner;
        Config config;
        Test();
};

#endif
