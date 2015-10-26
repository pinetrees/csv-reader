#include "util.h"
#include "config.h"
#include "structs.h"
#include "operators.h"

// This method does as much as it can in one pass through of the table, converting all rows to columns, and calculating the minimum, maximum, and average along the way. The median as saved for later if requested, since it requires a sort.
void CSVAggregator::aggregate(CSVTable &t, Config &config) {
    for (int i = 0; i < t.column_count; i++) {
        CSVColumn column;
        t.columns.push_back(column);
    }
    for (int j = 0; j < t.rows[0].size(); j++) {
        t.columns[j].header = t.rows[0][j];
    }
    for (int i = 1; i < t.rows.size(); i++) {
        for (int j = 0; j<t.rows[i].size(); j++) {
            std::string el = t.rows[i][j];
            t.columns[j].push_back(el);

            TextParser::strip_quotes(el);
            int val = atoi(el.c_str());
            t.columns[j].aggregator.sum += val;
            t.columns[j].aggregator.push_back(val);
            
            // This is a new aggregator (or it has been reset) - we assume the first value passed is both the min and the max
            if(t.columns[j].aggregator.is_clean == true) {
                t.columns[j].aggregator.min = val;
                t.columns[j].aggregator.max = val;
                t.columns[j].aggregator.is_clean = false;
            } else {
                if(val < t.columns[j].aggregator.min) {
                    t.columns[j].aggregator.min = val;
                }
                if(val > t.columns[j].aggregator.max) {
                    t.columns[j].aggregator.max = val;
                }
            }
        }
    }

    // After all the values have been loaded in, we can compute the average and median
    for (int i = 0; i < t.columns.size(); i++) {
        t.columns[i].aggregator.avg = t.columns[i].aggregator.sum / t.columns[i].aggregator.vals.size();

        // Only do this if requested
        if (config.med == true) {
            med(t.columns[i], config);
        }
    }
}

// Aggregate a set of tables
void CSVAggregator::aggregate(CSVDatabase &d, Config &config) {
    for (int i = 0; i < d.tables.size(); i++) {
        aggregate(d.tables[i], config);
    }
}

// Performs a recalculation for each column in the table. Almost always an overcalculation - the code should be refactored to only recalculate columns which have changed.
void CSVAggregator::recalculate_statistics(CSVTable &t, Config &config) {
    for (int i = 0; i < t.columns.size(); i++) {
        t.columns[i].recalculate_statistics(config);
    }
}

// Printer the statistics header (the orientation is changed), based on the provided statistics. If only one is requests, we omit the header.
void CSVAggregator::print_header(Config &config, std::stringstream &ss) {
    if( config.column_count != 1 ) {
        Printer::columnize(config.cell_width, ss);
    }
    if( config.min == true ) {
        Printer::columnize("MIN", config.cell_width, ss);
    }
    if( config.max == true ) {
        Printer::columnize("MAX", config.cell_width, ss);
    }
    if( config.med == true ) {
        Printer::columnize("MED", config.cell_width, ss);
    }
    if( config.avg == true ) {
        Printer::columnize("AVG", config.cell_width, ss);
    }
    if( config.sum == true ) {
        Printer::columnize("SUM", config.cell_width, ss);
    }
    ss << std::endl;
}

// Print statistics for the provided column, based on the configuration
void CSVAggregator::print(CSVColumn c, Config &config, std::stringstream &ss) {
    if( config.column_count != 1 ) {
        Printer::columnize(c.header, config.cell_width, ss);
    }

    if( config.min == true ) {
        Printer::columnize(c.aggregator.min, config.cell_width, ss);
    }
    if( config.max == true ) {
        Printer::columnize(c.aggregator.max, config.cell_width, ss);
    }
    if( config.med == true ) {
        Printer::columnize(c.aggregator.med, config.cell_width, ss);
    }
    if( config.avg == true ) {
        Printer::columnize(c.aggregator.avg, config.cell_width, ss);
    }
    if( config.sum == true ) {
        Printer::columnize(c.aggregator.sum, config.cell_width, ss);
    }

    ss << std::endl;
}

// Print the statistics for the table
void CSVAggregator::print(CSVTable t, Config &config, std::stringstream &ss) {
    for (int i = 0; i < t.columns.size(); i++) {
        print(t.columns[i], config, ss);
    }
}

// Prepare the statistic output as a stream, so that we can write it to a file
void CSVAggregator::prepare_stream(CSVTable t, Config &config, bool with_header) {
    std::stringstream ss;

    if( with_header == true && config.aggregate_count > 1 ) {
        print_header(config, ss);
    }
    print(t, config, ss);

    output = ss.str();
}

// Written after the streamer - this method uses the stream to output the contents
void CSVAggregator::print(CSVTable t, Config &config, bool with_header) {
    prepare_stream(t, config, with_header);
    std::cout << output;
}

void CSVAggregator::med(CSVColumn &c, Config &config) {
    c.aggregator.set_med(config);
}

// Write the statistics out to the provided file
void CSVAggregator::write_out(CSVTable t, Config &config) {
    std::ofstream file;
    prepare_stream(t, config, true);
    file.open(config.outfile);
    file << output;
    file.close();
}

// Perform a join on two tables, using two provided strings representing the headers of the respective columns to join on
CSVTable CSVJoiner::join(CSVTable t1, CSVTable t2, std::string c1, std::string c2, Config &config) {
    CSVTable t3;
    CSVRow row;
    row.append( t1.rows[0].begin(), t1.rows[0].end() );
    row.append( t2.rows[0].begin(), t2.rows[0].end() );
    t3.push_back(row);

    int comp1 = t1.get_column_index(c1);
    int comp2 = t2.get_column_index(c2);

    // For left and right joins. Would be great if this was simply a vector of pointers.
    std::vector<CSVRow> remaining_rows1 = t1.rows;
    std::vector<CSVRow> remaining_rows2 = t2.rows;

    for (int i = 1; i < t1.rows.size(); i++) {
        for (int j = 1; j < t2.rows.size(); j++) {
            if( t1.rows[i][comp1] == t2.rows[j][comp2] ) {
                CSVRow row;
                row.append( t1.rows[i].begin(), t1.rows[i].end() );
                row.append( t2.rows[j].begin(), t2.rows[j].end() );
                t3.push_back(row);

                remaining_rows1[i].clear();
                remaining_rows2[j].clear();
            }
        }
    }

    if( config.join_left == true ) {
        for (int i = 1; i < remaining_rows1.size(); i++) {
            if( !remaining_rows1[i].empty() ) {
                CSVRow row = remaining_rows1[i];
                for (int j = 0; j < t2.column_count; j++) {
                    row.push_back("");
                }
                t3.push_back(row);
            }
        }
    }

    if( config.join_right == true ) {
        for (int i = 1; i < remaining_rows2.size(); i++) {
            if( !remaining_rows2[i].empty() ) {
                CSVRow row;
                for (int j = 0; j<t1.column_count; j++) {
                    row.push_back("");
                }
                row.append( remaining_rows2[i].begin(), remaining_rows2[i].end() );
                t3.push_back(row);
            }
        }
    }

    return t3;
}

// Use the configuration's join columns to perform the join
CSVTable CSVJoiner::join(CSVTable t1, CSVTable t2, Config &config) {
    std::vector<std::string> join_columns = config.join_columns;
    return join(t1, t2, join_columns[0], join_columns[1], config);
}

char CSVDerivator::is_derived_column_index(std::string index) {
    char ops[] = {'+','-','*','/'};

    for (int i = 0; i < 4; i++) {
        if(index.find(ops[i]) != std::string::npos) {
            return ops[i];
        }
    }

    return ' ';
}

CSVColumn CSVDerivator::compute_derived_column(std::string combined_column_index, char op, CSVTable t) {
    CSVColumn c;

    std::vector<std::string> indices = TextParser::split(combined_column_index, op);
    for (int i = 0; i < indices.size(); i++) {
        char _op = is_derived_column_index(indices[i]);
        if( _op == ' ' ) {
            int column_index = atoi(indices[i].c_str());
            if(c.size() == 0) {
                c = t.columns[column_index];
            } else {
                switch (op)
                {
                    case '+':
                        c = c + t.columns[column_index];
                        break;
                    case '-':
                        c = c - t.columns[column_index];
                        break;
                    case '*':
                        c = c * t.columns[column_index];
                        break;
                    case '/':
                        c = c / t.columns[column_index];
                        break;
                }
            }
        } else {
            c = compute_derived_column(indices[i], op, t);
        }
    }

    return c;
}

// Parse derived columns represented as strings into derived columns, calculating the values along the way
void CSVDerivator::compute_derived_columns(CSVTable &t, Config &config) {
    if( config.derivations_only == true ) {
        t.clear_rows();
    }
    for (int i = 0; i < config.derived_columns.size(); i++) {
        char op = is_derived_column_index(config.derived_columns[i]);
        CSVColumn c = compute_derived_column(config.derived_columns[i], op, t);
        t.columns.push_back(c);
        t.extend_rows(c);
    }
}

