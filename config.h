#ifndef CONFIG_H
#define CONFIG_H

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

// Configuration class containing pseudo-global options to be passed through the program
class Config {
    public:
        // Set this to true if we want to print the output to the console
        bool print;

        // Set this to true if we want to write the output to a file
        bool write;

        // Set this to true if we want to trim the header from the output
        bool headless;
        bool join;
        bool aggregate;
        bool statistics;
        bool derivations_only;
        bool join_left;
        bool join_right;
        bool tabulate;

        bool min;
        bool max;
        bool med;
        bool avg;
        bool sum;
        bool upper_med;

        int aggregate_count;
        int cell_width;
        int row_count;
        int column_count;

        std::map<std::string,std::string> default_opts;
        std::map<std::string,std::string> opts;

        std::vector<std::string> files;
        std::vector<std::string> column_names;
        std::vector<std::string> derived_columns;
        std::vector<std::string> join_columns;
        std::vector<int> columns;
        std::vector<int> joins;

        std::string infile;
        std::string outfile;
        std::string joinfile;
        std::string separator;

        void print_opts( std::map<std::string,std::string> opts );
        void print_opts();
        void print_default_opts();
        void set_min();
        void set_max();
        void set_med(bool upper);
        void set_avg();
        void set_sum();
        void set_statistics();
        Config();
};

class OptionParser {
    public:
        std::map<std::string,std::string> _parse(int argc, char *argv[], std::map<std::string,std::string> &default_opts);
        static void parse(int argc, char *argv[], Config &config);
        static void handle_errors(Config &config);
};

inline bool file_exists (const std::string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}

#endif
