#ifndef STRUCTS_H
#define STRUCTS_H

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
#include "config.h"

// Used to represent and operate on integer versions of a column's data
class CSVAggregate {
    public:
        int min;
        int max;
        int med;
        int avg;
        int sum;
        std::vector<int> vals;
        std::vector<int> sorted;
        bool is_clean;

        void sort();
        bool is_sorted( std::vector<int> &sorted );
        void set_med(Config &config);
        void set_min();
        void set_max();
        void set_avg();
        void recalculate_statistics(Config &config);
        void push_back(int el); 
        CSVAggregate();
};

// Represents a row
class CSVRow {
    public:
        std::string const& operator[](std::size_t index) const;
        std::size_t size() const;
        void clear();
        bool empty();
        void push_back(std::string el); 
        void print();
        std::vector<std::string>::iterator begin();
        std::vector<std::string>::iterator end();
        void append(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last);
    private:
        std::vector<std::string>    m_data;

};

// Represents a column
class CSVColumn {
    public:
        std::string                 header;
        CSVAggregate                aggregator;

        void push_back(std::string el); 
        void print();
        std::size_t size() const;
        CSVColumn operate(const CSVColumn& c2, char op);
        CSVColumn operator+(const CSVColumn& c2);
        CSVColumn operator-(const CSVColumn& c2);
        CSVColumn operator*(const CSVColumn& c2);
        CSVColumn operator/(const CSVColumn& c2);
        std::string const& operator[](std::size_t index) const;
        void recalculate_statistics(Config &config);
    private:
        std::vector<std::string>    m_data;
};

// Helper class used to iterate over rows
class CSVReader {
    public:
        void read_in(std::istream& str);
        CSVReader(std::istream& str);
        CSVReader();

        CSVReader& operator++();       
        CSVReader operator++(int);       
        CSVRow const& operator*()   const;  
        CSVRow const* operator->()  const;      

        bool operator==(CSVReader const& rhs);
        bool operator!=(CSVReader const& rhs);
    private:
        std::istream* m_str;
        CSVRow m_row;
};

std::istream& operator>>(std::istream& str,CSVReader& data);

// Represents a table
class CSVTable {
    public:
        CSVRow header;
        std::vector<CSVRow> rows;
        std::vector<CSVColumn> columns;
        int row_count;
        int column_count;
        int cell_width;
        bool show_header;
        void read_in(std::string filename, Config &config);
        void write_out(Config config);
        CSVRow operator[](std::size_t index) const;
        int size();
        void clear();
        void print_cell(int i, int j, Config &config);
        void print_cell(int i, int j, Config &config, std::stringstream &ss);
        void print_row(int i, Config &config);
        void print_row(int i, Config &config, std::stringstream &ss);
        void print_column(int j, Config &config);
        void prepare_stream(Config &config);
        void print(Config &config);
        void print(int start, int end, Config &config);
        void print_header(Config &config);
        CSVRow get_header();
        int get_column_index(std::string title);
        void extend_rows(CSVColumn c);
        void clear_rows();
        void push_back(CSVRow row); 
        CSVTable();
        CSVTable(Config &config);
        CSVTable(bool show_header);
        std::string output;
};

// Represents a collection of tables
class CSVDatabase {
    public:
        std::vector<CSVTable> tables;
        void make_tables(std::vector<std::string> files, Config &config);
        void print(Config &config);
};

#endif
