#include "config.h"

void Config::print_opts( std::map<std::string,std::string> opts ) {
    std::map<std::string,std::string>::iterator optit;
    for ( optit = opts.begin(); optit != opts.end(); optit++) {
        std::cout << optit->first << ":" << optit->second << std::endl;
    }
}
void Config::print_opts() {
    std::map<std::string,std::string>::iterator optit;
    for ( optit = opts.begin(); optit != opts.end(); optit++) {
        std::cout << optit->first << ":" << optit->second << std::endl;
    }
}
void Config::print_default_opts() {
    print_opts( default_opts );
}

void Config::set_min() {
    aggregate = true;
    statistics = true;
    min = true;
    aggregate_count++;
}

void Config::set_max() {
    aggregate = true;
    statistics = true;
    max = true;
    aggregate_count++;
}

void Config::set_med(bool upper) {
    aggregate = true;
    statistics = true;
    med = true;
    upper_med = upper;
    aggregate_count++;
}

void Config::set_avg() {
    aggregate = true;
    statistics = true;
    avg = true;
    aggregate_count++;
}

void Config::set_sum() {
    aggregate = true;
    statistics = true;
    sum = true;
    aggregate_count++;
}

void Config::set_statistics() {
    aggregate = true;
    statistics = true;
    min = true;
    max = true;
    med = true;
    avg = true;
    aggregate_count = 4;
}

Config::Config() {
    infile = "data/file.csv";
    outfile = "data/outfile.csv";

    aggregate_count = 0;
    cell_width = 20;
    row_count = 0;
    column_count = 0;

    separator = "";
    tabulate = false;
    upper_med = true;
}

// Parses options of the format --key="value" into a map of key/value pairs. Takes a map of default options to compose agains. NOTE: this method is legacy after switching to GetOpt
std::map<std::string,std::string> OptionParser::_parse(int argc, char *argv[], std::map<std::string,std::string> &default_opts) {
    for (int i = 0; i < argc; i++) {
        std::vector<std::string> s = TextParser::split(argv[i], '=', false);
        if( s[0].size() >= 2 ) {
            // If it starts like an option, with --
            if( s[0][0] == '-' && s[0][1] == '-' ) {
                // Sets the key/value pair for the option
                default_opts[s[0].substr(2, s[0].size() - 2)] = s[1];
            }
        }
    }

    return default_opts;
}

void OptionParser::parse(int argc, char *argv[], Config &config) {
    int option_char;
    while ((option_char = getopt(argc, argv, "pw:f:Ttc:C:hHj:o:lrmMEeASaW:d:Ds:")) != EOF)
        switch (option_char)
        {
            case 'p': 
                config.print = true;
                config.tabulate = true;
                break;
            case 'w':
                config.write = true;
                config.separator = ",";
                config.outfile = optarg;
                break;
            case 'f':
                config.infile = optarg;
                break;
            case 'T':
                config.tabulate = true;
                break;
            case 't':
                config.tabulate = false;
                config.separator = ",";
                break;
            case 'c':
                config.columns = TextParser::split_to_int(optarg, ',');
                break;
            case 'C':
                config.column_names = TextParser::split(optarg, ',');
                break;
            case 'h':
                config.headless = true;
                break;
            case 'j':
                config.join = true;
                config.joinfile = optarg;
                break;
            case 'o':
                config.join_columns = TextParser::split(optarg, ',');
                break;
            case 'l':
                config.join_left = true;
                break;
            case 'r':
                config.join_right = true;
                break;
            case 'm':
                config.set_min();
                break;
            case 'M':
                config.set_max();
                break;
            case 'E':
                config.set_med(true);
                break;
            case 'e':
                config.set_med(false);
                break;
            case 'A':
                config.set_avg();
                break;
            case 'S':
                config.set_sum();
                break;
            case 'a':
                config.set_statistics();
                break;
            case 'W':
                config.cell_width = atoi(optarg);
                break;
            case 'd':
                config.derived_columns = TextParser::split(optarg, ',');
                config.aggregate = true;
                break;
            case 'D':
                config.derivations_only = true;
                break;
            case 's':
                config.separator = optarg;
                break;
        }

        handle_errors(config);
}

void OptionParser::handle_errors(Config &config) {
    if (!file_exists(config.infile)) {
        throw std::invalid_argument("The file provided does not exist");
    }

    if (config.join) {
        if (!file_exists(config.joinfile)) {
            throw std::invalid_argument("The join file provided does not exist");
        }

        if (config.join_columns.size() != 2 ) {
            throw std::invalid_argument("The join operator format is not correct - try 'col1,col2'");
        }
    }
}
