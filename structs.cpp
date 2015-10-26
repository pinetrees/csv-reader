#include "structs.h"

void CSVAggregate::sort() {
    sorted = vals;
    std::sort( sorted.begin(), sorted.end() );
}

bool CSVAggregate::is_sorted( std::vector<int> &sorted ) {
    return sorted.size() != 0 && std::is_sorted( sorted.begin(), sorted.end() );
}

void CSVAggregate::set_med(Config &config) {
    if( !is_sorted( sorted ) ) {
        sort();
    }
    int median_index = sorted.size() / 2;
    if ( config.upper_med == false ) {
        median_index--;
    }
    med = sorted[median_index];
}

void CSVAggregate::set_min() {
    if( !is_sorted( sorted ) ) {
        sort();
    }
    min = sorted.front();
}

void CSVAggregate::set_max() {
    if( !is_sorted( sorted ) ) {
        sort();
    }
    max = sorted.back();
}

void CSVAggregate::set_avg() {
    //int sum = std::accumulate(vals.front(), vals.back(), init);
    int sum = 0;
    for (int i = 0; i < vals.size(); i++) {
        sum += vals[i];
    }
    avg = sum / vals.size();
}

void CSVAggregate::recalculate_statistics(Config &config) {
    set_med(config);
    set_min();
    set_max();
    set_avg();
}

CSVAggregate::CSVAggregate(): is_clean(true) {
    sum = 0;
    avg = 0;
}

void CSVAggregate::push_back(int el) { 
    vals.push_back(el);
}

std::string const& CSVRow::operator[](std::size_t index) const
{
    return m_data[index];
}
std::size_t CSVRow::size() const
{
    return m_data.size();
}
void CSVRow::clear() {
    m_data.clear();
}

bool CSVRow::empty() {
    return m_data.empty();
}

void CSVRow::push_back(std::string el) { 
    m_data.push_back(el);
}

void CSVRow::print() {
    Printer::row(this->m_data, 20);
}

std::vector<std::string>::iterator CSVRow::begin() {
    return m_data.begin();
}

std::vector<std::string>::iterator CSVRow::end() {
    return m_data.end();
}

void CSVRow::append(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last) {
    m_data.insert(m_data.end(), first, last);
}

void CSVColumn::push_back(std::string el) { 
    m_data.push_back(el);
}

void CSVColumn::print() {
    Printer::row(this->m_data, 20);
}

std::size_t CSVColumn::size() const
{
    return m_data.size();
}

CSVColumn CSVColumn::operate(const CSVColumn& c2, char op) {
    CSVColumn c;
    if(this->aggregator.vals.size() != c2.aggregator.vals.size()) {
        throw std::invalid_argument( "The column sizes do not match" );
    }

    std::stringstream header;
    header << this->header << op << c2.header;
    c.header = header.str();

    for (int i = 0; i < this->aggregator.vals.size(); i++) {
        int val;
        switch (op)
        {
            case '+':
                val = this->aggregator.vals[i] + c2.aggregator.vals[i];
                break;
            case '-':
                val = this->aggregator.vals[i] - c2.aggregator.vals[i];
                break;
            case '*':
                val = this->aggregator.vals[i] * c2.aggregator.vals[i];
                break;
            case '/':
                if( c2.aggregator.vals[i] == 0 ) {
                    val = 0;
                } else {
                    val = this->aggregator.vals[i] / c2.aggregator.vals[i];
                }
                break;
        }
        c.aggregator.vals.push_back(val);

        std::stringstream ss;
        ss << val;
        c.push_back(ss.str());
    }
    return c;
}
CSVColumn CSVColumn::operator+(const CSVColumn& c2) {
    return this->operate(c2, '+');
}

CSVColumn CSVColumn::operator-(const CSVColumn& c2) {
    return this->operate(c2, '-');
}

CSVColumn CSVColumn::operator*(const CSVColumn& c2) {
    return this->operate(c2, '*');
}

CSVColumn CSVColumn::operator/(const CSVColumn& c2) {
    return this->operate(c2, '/');
}

std::string const& CSVColumn::operator[](std::size_t index) const
{
    return m_data[index];
}

void CSVColumn::recalculate_statistics(Config &config) {
    this->aggregator.recalculate_statistics(config);
}

void CSVReader::read_in(std::istream& str)
{
    std::string         line;
    std::getline(str,line);

    std::stringstream   lineStream(line);
    std::string         cell;

    m_row.clear();
    while(std::getline(lineStream,cell,','))
    {
        TextParser::strip_quotes(cell);
        m_row.push_back(cell);
    }
}

CSVReader& CSVReader::operator++() {
    if (m_str) { 
        (*m_str) >> *this;
        m_str = m_str->good() ? m_str : NULL;
    }
    
    return *this;
}

CSVReader CSVReader::operator++(int) {
    CSVReader tmp(*this);
    ++(*this);
    return tmp;
}

CSVRow const& CSVReader::operator*() const {
    return m_row;
}

CSVRow const* CSVReader::operator->() const {
    return &m_row;
}

CSVReader::CSVReader(std::istream& str) :m_str(str.good() ? &str : NULL) { 
    ++(*this); 
}

CSVReader::CSVReader()                   :m_str(NULL) {}

std::istream& operator>>(std::istream& str, CSVReader& reader)
{
    reader.read_in(str);
    return str;
}   

bool CSVReader::operator==(CSVReader const& rhs) {
    return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));
}

bool CSVReader::operator!=(CSVReader const& rhs) {
    return !((*this) == rhs);
}

void CSVTable::read_in(std::string filename, Config &config) {
    std::ifstream file(filename);
    for (CSVReader loop(file); loop != CSVReader(); ++loop) {
        if(header.size() == 0) {
            header = (*loop);

            // Prepare the columns. Be sure to do this only once.
            if(config.column_names.size() != 0) {
                for (int i = 0; i < config.column_names.size(); i++) {
                    int index = -1;
                    for (int j = 0; j < header.size(); j++) {
                        //if(index == -1 && header[j] == config.column_names[i]) {
                        if(header[j] == config.column_names[i]) {
                            index = j;
                            config.columns.push_back(index);
                        }
                    }
                }
            }
        }

        if(config.columns.size() == 0) {
            rows.push_back((*loop));
        } else {
            CSVRow row;

            // Note that it is possible to do this earlier on, while reading in the row, which would improve performance significantly when we are only interested in a small subset of columns.
            for (int i = 0; i < config.columns.size(); i++) {
                int index = config.columns[i];
                row.push_back((*loop)[index]);
            }
            rows.push_back(row);
        }
        row_count += 1;
        config.row_count += 1;
    }

    column_count = rows[0].size();
    config.column_count += column_count;
}

CSVRow CSVTable::operator[](std::size_t index) const
{
    return rows[index];
}
int CSVTable::size() {
    return rows.size();
}

void CSVTable::clear() {
    rows.clear();
    columns.clear();
    row_count = 0;
}

void CSVTable::print_cell(int i, int j, Config &config) {
    Printer::columnize(rows[i][j], config.cell_width, config.separator);
}
void CSVTable::print_cell(int i, int j, Config &config, std::stringstream &ss) {
    std::string separator;
    if (j == rows[i].size() - 1) {
        separator = "";
    } else {
        separator = config.separator;
    }
    Printer::columnize(rows[i][j], config.cell_width, config.tabulate, separator, ss);
}
void CSVTable::print_row(int i, Config &config) {
    for (int j = 0; j < rows[i].size(); j++) {
        print_cell(i, j, config);
    }
    std::cout << "\n";
}
void CSVTable::print_row(int i, Config &config, std::stringstream &ss) {
    for (int j = 0; j < rows[i].size(); j++) {
        print_cell(i, j, config, ss);
    }
    ss << "\n";
}
void CSVTable::print_column(int j, Config &config) {
    for (int i = 0; i < rows.size(); i++) {
        print_cell(i, j, config);
        std::cout << "\n";
    }
};
void CSVTable::prepare_stream(Config &config) {
    std::stringstream ss;

    int start = 0;
    if( config.headless == true ) {
        start++;
    }
    for (int i = start; i < rows.size(); i++) {
        print_row(i, config, ss);
    }
    output = ss.str();
}
void CSVTable::print(Config &config) {
    prepare_stream(config);

    std::cout << output;
}

void CSVTable::print(int start, int end, Config &config) {
    for (int i = start; i <= end; i++) {
        print_row(i, config);
    }
}
void CSVTable::print_header(Config &config) {
    print_row(0, config);
}

CSVRow CSVTable::get_header() {
    return rows[0];
}

void CSVTable::write_out(Config config) {
    std::ofstream file;
    prepare_stream(config);
    file.open(config.outfile);
    file << output;
    file.close();
}

int CSVTable::get_column_index(std::string title) {
    CSVRow header = get_header();
    for (int i = 0; i < header.size(); i++) {
        if(header[i] == title) {
            return i;
        }
    }
    return -1;
}

void CSVTable::extend_rows(CSVColumn c) {
    rows[0].push_back(c.header);
    for (int i = 0; i < c.size(); i++) {
        rows[i+1].push_back(c[i]);
    }
}

void CSVTable::clear_rows() {
    for (int i = 0; i < rows.size(); i++) {
        rows[i].clear();
    }
}

void CSVTable::push_back(CSVRow row) { 
    rows.push_back(row);
}

CSVTable::CSVTable(): row_count(0), column_count(0), cell_width(20), show_header(false) {}
CSVTable::CSVTable(Config &config): row_count(0), column_count(0), cell_width(config.cell_width), show_header(false) {}
CSVTable::CSVTable(bool show_header): show_header(show_header) {}

void CSVDatabase::make_tables(std::vector<std::string> files, Config &config) {
    for (int i = 0; i < files.size(); i++) {
        CSVTable table;
        table.read_in(files[i], config);
        tables.push_back(table);
    }
}

void CSVDatabase::print(Config &config) {
    for (int i = 0; i < tables.size(); i++) {
        tables[i].print(config);
    }
}
