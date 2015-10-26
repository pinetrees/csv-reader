#ifndef OPERATORS_H
#define OPERATORS_H

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

#include "util.h"
#include "structs.h"

// Helper class used to transform a table's rows into columns, and collect aggregate data during the process
class CSVAggregator {
    public:
        void aggregate(CSVTable &t, Config &config);
        void aggregate(CSVDatabase &d, Config &config);
        void recalculate_statistics(CSVTable &t, Config &config);
        void print_header(Config &config, std::stringstream &ss);
        void print(CSVColumn c, Config &config, std::stringstream &ss);
        void print(CSVTable t, Config &config, std::stringstream &ss);
        void prepare_stream(CSVTable t, Config &config, bool with_header);
        void print(CSVTable t, Config &config, bool with_header);
        void write_out(CSVTable t, Config &config);
        std::string output;
    private:
        void med(CSVColumn &c, Config &config);

};

// Helper class used to perform joins on two or more tables
class CSVJoiner {
    public:
        CSVTable join(CSVTable t1, CSVTable t2, std::string c1, std::string c2, Config &config);
        CSVTable join(CSVTable t1, CSVTable t2, Config &config);

};

// Helper class used to parse and calculate requests for derived columns
class CSVDerivator {
    public:
        static char is_derived_column_index(std::string index);
        static CSVColumn compute_derived_column(std::string combined_column_index, char op, CSVTable t);
        static void compute_derived_columns(CSVTable &t, Config &config);
};

#endif
