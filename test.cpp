#include <assert.h>

#include "test.h"

std::vector<std::vector<std::string> > Test::make_test_table() {
    std::vector<std::vector<std::string> > test_table;

    std::vector<std::string> row1;
    row1.push_back("0");
    row1.push_back("1");
    row1.push_back("2");

    std::vector<std::string> row2;
    row2.push_back("3");
    row2.push_back("4");
    row2.push_back("5");

    std::vector<std::string> row3;
    row3.push_back("6");
    row3.push_back("7");
    row3.push_back("8");

    test_table.push_back(row1);
    test_table.push_back(row2);
    test_table.push_back(row3);

    return test_table;
}

void Test::read_in(std::string filename) {
    table.read_in(filename, config);
}

void Test::defaults(Config &config) {
    assert(config.infile == "data/file.csv");
    assert(config.outfile == "data/outfile.csv");

    assert(config.aggregate_count == 0);
    assert(config.cell_width == 20);
    assert(config.row_count == 0);
    assert(config.column_count == 0);

    assert(config.separator == "");
    assert(config.tabulate == false);
    assert(config.upper_med == true);
}

void Test::defaults() {
    int argc = 1;
    char *argv[1] = {NULL};
    Config config;

    OptionParser::parse(argc, argv, config);

    defaults(config);
}

void Test::flag(char option_char) {
    std::stringstream flag;
    flag << "-";
    flag << option_char;
    Config config;
    char *argv[] = {(char*)"csv-reader-test", (char*)flag.str().c_str(), NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;

    OptionParser::parse(argc, argv, config);

    switch (option_char) {
        case 'p': 
            assert(config.print == true);
            assert(config.tabulate == true);
            break;
        case 'T':
            assert(config.tabulate == true);
            break;
        case 't':
            assert(config.tabulate == false);
            assert(config.separator == ",");
            break;
        case 'h':
            assert(config.headless == true);
            break;
        case 'j':
            assert(config.join == true);
            break;
        case 'l':
            assert(config.join_left == true);
            break;
        case 'r':
            assert(config.join_right == true);
            break;
        case 'm':
            break;
        case 'M':
            break;
        case 'E':
            break;
        case 'e':
            break;
        case 'A':
            break;
        case 'S':
            break;
        case 'a':
            break;
        case 'D':
            assert(config.derivations_only == true);
            break;
    }
}

bool Test::compare_rows(CSVRow r1, std::vector<std::string> r2) {
    if (r1.size() != r2.size() ) {
        return false;
    }

    for (int i = 0; i < r1.size(); i++ ) {
        if (r1[i] != r2[i]) {
            return false;
        }
    }

    return true;
}

bool Test::compare_rows(CSVRow r1, CSVRow r2) {
    if (r1.size() != r2.size() ) {
        return false;
    }

    for (int i = 0; i < r1.size(); i++ ) {
        if (r1[i] != r2[i]) {
            return false;
        }
    }

    return true;
}

bool Test::compare_tables(CSVTable t1, std::vector<std::vector<std::string> > t2) {
    if (t1.size() != t2.size() ) {
        return false;
    }

    for (int i = 0; i < t1.size(); i++ ) {
        if (compare_rows(t1.rows[i], t2[i]) == false) {
            return false;
        }
    }

    return true;
}

bool Test::compare_tables(CSVTable t1, CSVTable t2) {
    if (t1.size() != t2.size() ) {
        return false;
    }

    for (int i = 0; i < t1.size(); i++ ) {
        if (compare_rows(t1[i], t2[i]) == false) {
            return false;
        }
    }

    return true;
}

void Test::test_aggregation() {
    table.rows.clear();
    table.read_in(config.infile, config);
    assert(table.size() == 3);
    assert(table.columns.size() == 0);

    config.set_statistics();

    aggregator.aggregate(table, config);
    assert(table.columns.size() == 3);

    assert(table.columns[0].header == "0");
    assert(table.columns[1].header == "1");
    assert(table.columns[2].header == "2");

    assert(table.columns[0].aggregator.min == 3);
    assert(table.columns[1].aggregator.min == 4);
    assert(table.columns[2].aggregator.min == 5);

    assert(table.columns[0].aggregator.max == 6);
    assert(table.columns[1].aggregator.max == 7);
    assert(table.columns[2].aggregator.max == 8);

    assert(table.columns[0].aggregator.med == 6);
    assert(table.columns[1].aggregator.med == 7);
    assert(table.columns[2].aggregator.med == 8);

    assert(table.columns[0].aggregator.avg == 4);
    assert(table.columns[1].aggregator.avg == 5);
    assert(table.columns[2].aggregator.avg == 6);
}

void Test::test_derivation() {
    table.clear();
    table.read_in(config.infile, config);
    assert(table.size() == 3);
    assert(table.columns.size() == 0);

    aggregator.aggregate(table, config);
    assert(table.columns.size() == 3);

    config.derived_columns = TextParser::split("1+2, 1-2, 1*2, 1/2", ',');
    derivator.compute_derived_columns(table, config);
    assert(table.columns.size() == 7);

    aggregator.recalculate_statistics(table, config);

    assert(table.columns[3].aggregator.min == 9);
    assert(table.columns[3].aggregator.max == 15);
    assert(table.columns[3].aggregator.med == 15);
    assert(table.columns[3].aggregator.avg == 12);

    assert(table.columns[4].aggregator.min == -1);
    assert(table.columns[4].aggregator.max == -1);
    assert(table.columns[4].aggregator.med == -1);
    assert(table.columns[4].aggregator.avg == -1);

    assert(table.columns[5].aggregator.min == 20);
    assert(table.columns[5].aggregator.max == 56);
    assert(table.columns[5].aggregator.med == 56);
    assert(table.columns[5].aggregator.avg == 38);

    assert(table.columns[6].aggregator.min == 0);
    assert(table.columns[6].aggregator.max == 0);
    assert(table.columns[6].aggregator.med == 0);
    assert(table.columns[6].aggregator.avg == 0);
}

void Test::test_join() {
    table.clear();
    config.infile = "data/people.csv";
    table.read_in(config.infile, config);
    assert(table.size() == 4);

    config.joinfile = "data/animals.csv";
    config.join = true;
    config.join_columns = TextParser::split("id,ownerId", ',');

    CSVTable table2, table3;
    table2.read_in(config.joinfile, config);

    table3 = joiner.join(table, table2, config);
    assert(table3.size() == 4);

    // Left join
    config.join_left = true;
    table3 = joiner.join(table, table2, config);
    assert(table3.size() == 5);

    // Right join
    config.join_left = false;
    config.join_right = true;;
    table3 = joiner.join(table, table2, config);
    assert(table3.size() == 5);

    // Full join
    config.join_left = true;
    table3 = joiner.join(table, table2, config);
    assert(table3.size() == 6);
}

Test::Test() {
    config.infile = "data/test.csv";
}
