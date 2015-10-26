#include "structs.h"
#include "operators.h"

int main(int argc, char *argv[])
{
    Config config;
    CSVTable table;
    CSVAggregator aggregator;

    OptionParser::parse(argc, argv, config);
    table.read_in(config.infile, config);

    if (config.join == true) {
        CSVTable table2;
        CSVJoiner joiner;
        table2.read_in(config.joinfile, config);
        table = joiner.join(table, table2, config);
    }

    if (config.aggregate == true) {
        aggregator.aggregate(table, config);
    }

    if (config.derived_columns.size() != 0) {
        CSVDerivator::compute_derived_columns(table, config);
        aggregator.recalculate_statistics(table, config);
    }

    if (config.statistics == true && config.write == false ) {
        aggregator.print(table, config, !config.headless);
    }

    if (config.print == true && config.statistics != true) {
        table.print(config);
    }

    if (config.write == true) {
        if (config.statistics == true) {
            aggregator.write_out(table, config);
        } else {
            table.write_out(config);
        }
    }

}
