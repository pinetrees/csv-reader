NAME
    csv-reader -- read the contents of a CSV

SYNOPSIS
    csv-reader [-pw:f:Fc:C:hHjgolramMEASNW:d:D]

BUILD INSTRUCTIONS
    Simply run the included make-csv-reader script

DESCRIPTION
    Reads the contents of a CSV file and performs operations on it. Supported operations are outlined by the options, and include 
        - printing the contents
        - writing the output (which may differ based on passed options)
        - printing a subset of the contents limited by column indices or column names
        - collecting aggregated statistics on specified columns (minimum, maximum, median, and average
        - performing inner and outer joins on two tables
    This program should perform in approximately five seconds or less for files under 5MB

    The following options are available (options listed with a ':' require arguments):

    -p                  Print the output to the terminal

    -w:OUTFILE          Write the output to OUTFILE

    -f:INFILE           Use the contents of INFILE for the source

    -T                  Specify to tabulate the output written to a file

    -t                  Specify to skip tabulation on streamed output

    -c:COLUMN_INDICES   Run the program against a subset of INFILE's contents restricted to columns which match the indices provided. Indices should be a comma separated list of non-negative integers. Whitespace is stripped.

    -C:COLUMN_NAMES     Run the program against a subset of INFILE's contents restricted to columns which match the names provided. Names should be comma separated. Whitespace is stripped from the outside of the strings.

    -h                  Removes the header from the output

    -H

    -j:JOINFILE         Join against the contents of JOINFILE

    -o:JOINCONDS        Specify the join conditions

    -l                  Do a left join

    -r                  Do a right join

    -m                  Outputs the minimum value for each column specified. If no columns are specified, outputs the minimum for all columns in the dataset.

    -M                  Outputs the maximum value for each column specified. If no columns are specified, outputs the maximum for all columns in the dataset.

    -E                  Outputs the median value for each column specified. If no columns are specified, outputs the median for all columns in the dataset. Requires sorting.

    -e                  Outputs the lower median for even sized lists

    -A                  Outputs the average value for each column specified. If no columns are specified, outputs the average for all columns in the dataset.

    -S                  Outputs the sum of the values for each column specified. If no columns are specified, outputs the sum for all columns in the dataset.

    -a                  Aggregates the data and outputs all statistics options (minimum, maximum, median, and average)

    -N

    -W:CELL_WIDTH       Specifies the total character width of each output cell.

    -d:DERIVED_COLUMNS  Add the specified derived columns onto the output. This will create a new column for each DERIVED_COLUMN in DERIVED_COLUMNS. DERIVED_COLUMNS should consist of column indicies separated by a single type of arithmetic operator in the set of +, -, *, and /, and should be comma separated (e.g. "1+2+3,1-2,2*3*4").

    -D                  Remove the original column contents and output only the derived columns.

EXAMPLES
    >> read -f file.csv -p
    Prints the contents of file.csv

    >> read -f file.csv -c 1 -p
    Prints the second column of file.csv

    >> read -f file.csv -c 1,2 -p
    Prints the second and third columns of file.csv

    >> read -f file.csv -C "First Name" -p
    Prints the column with the header of "First Name" from file.csv

    >> read -f file.csv -c 1 -w outfile.csv
    Writes the second column of file.csv to outfile.csv

    >> read -f file.csv -w outfile.csv -T
    Writes the contents of file.csv to outfile.csv, and tabulates the output

    >> read -f file.csv -w outfile.csv -T -s "|"
    Writes the contents of file.csv to outfile.csv, tabulates the output, and uses a pipe to separate the data

    >> read -f file.csv -a
    Prints the minimum, maximum, median, and average values for each column in the contents of file.csv

    >> read -f animals.csv -j people.csv -o 'ownerId, id' -lr -p
    Reads in the file animals.csv, and performs a full join against people.csv on (animal.)ownerId = (person.)id, and prints the output

FUTURE DEVELOPMENT
    - Add a flag to parse decimals, which would require a new of structures which support floats
    - Add a "where" filter to compliment the join functionality
    - Join across multiple tables with one command
    - Specify a range of rows
