#include "util.h"

// Prints a tabulated cell with the specified separator
void Printer::columnize(const std::string& s, const int& width, std::string separator) {
    std::cout << s;
    for (int i = s.size(); i < width; i++) {
        std::cout << " ";
    }
    std::cout << separator;
}

// Prints a tabulated cell
void Printer::columnize(const std::string& s, const int& width) {
    columnize(s, width, "");
}

// Pipes a column into a provided stream
void Printer::columnize(const std::string& s, const int& width, bool tabulate, std::string separator, std::stringstream &ss) {
    ss << s;
    if (tabulate == true) {
        for (int i = s.size(); i < width; i++) {
            ss << " ";
        }
    }
    ss << separator;
}

// Pipes a column into a proovided stream, formatted for aggregation
void Printer::columnize(const std::string& s, const int& width, std::stringstream &ss) {
    columnize(s, width, true, "", ss);
}

// Prints a tabulated cell by first converting the provided integer to a string
void Printer::columnize(const int& n, const int& width, std::stringstream &ss) {
    std::stringstream _ss;
    _ss << n;
    columnize(_ss.str(), width, ss);
}

// Prints a blank tabulated cell
void Printer::columnize(const int& width, std::stringstream &ss) {
    std::string s = "";
    columnize(s, width, ss);
}

// Prints a string vector out as a row of columnized cells
void Printer::row(const std::vector<std::string>& v, const int& width) {
    std::stringstream ss;

    for (int i = 0; i < v.size(); i++) {
        columnize(v[i], width, ss);
    }

    std::cout << ss.str() << std::endl;
}

// Prints an integer vector out as a row of columnized cells
void Printer::row(const std::vector<int>& v, const int& width) {
    std::stringstream ss;

    for (int i = 0; i < v.size(); i++) {
        columnize(v[i], width, ss);
    }
    std::cout << ss.str() << std::endl;
}

// Joins a string vector into a comma separated string
std::string TextParser::join_vector_to_string(std::vector<std::string> v) {
    std::stringstream ss;
    for (size_t i = 0; i < v.size(); ++i) {
        if(i != 0) {
            ss << ",";
        }
        ss << v[i];
    }
    return ss.str();
}

// Joins an integer vector into a comma separated string
std::string TextParser::join_vector_to_string(std::vector<int> v) {
    std::stringstream ss;
    for (size_t i = 0; i < v.size(); ++i) {
        if(i != 0) {
            ss << ",";
        }
        ss << v[i];
    }
    return ss.str();
}

// Splits a string into a string vector, based on the provided delimiter. Optionally trim the whitespace off each element
std::vector<std::string> TextParser::split(std::string str, char delimiter, bool trim_whitespace) {
    std::vector<std::string> internal;
    std::stringstream ss(str); // Turn the string into a stream.
    std::string tok;

    while(getline(ss, tok, delimiter)) {
        if( trim_whitespace == true ) {
            trim(tok);
        }
        internal.push_back(tok);
    }

    return internal;
}

// Splits a string with whitespace trimmed
std::vector<std::string> TextParser::split(std::string str, char delimiter) {
    return split(str, delimiter, true);
}

// Splits a string into a vector of integers
std::vector<int> TextParser::split_to_int(std::string str, char delimiter) {
    std::vector<int> internal;
    std::stringstream ss(str);
    std::string tok;

    while(getline(ss, tok, delimiter)) {
        trim(tok);
        int i = atoi(tok.c_str());
        internal.push_back(i);
    }

    return internal;
}
