#ifndef UTIL_H
#define UTIL_H

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

// Helper class to facilitate printing rows and columns
class Printer {
    public:
        static void columnize(const std::string& s, const int& width, std::string separator);
        static void columnize(const std::string& s, const int& width);
        static void columnize(const std::string& s, const int& width, bool tabulate, std::string separator, std::stringstream &ss);
        static void columnize(const std::string& s, const int& width, std::stringstream &ss);
        static void columnize(const int& n, const int& width, std::stringstream &ss);
        static void columnize(const int& width, std::stringstream &ss);
        static void row(const std::vector<std::string>& v, const int& width);
        static void row(const std::vector<int>& v, const int& width);
};

// Helper class with several string utility methods
class TextParser {
    public:
        static inline std::string &ltrim(std::string &s);
        static inline std::string &rtrim(std::string &s);
        static inline std::string &trim(std::string &s);
        static inline std::string &strip_quotes(std::string &s);
        static std::string join_vector_to_string(std::vector<std::string> v);
        static std::string join_vector_to_string(std::vector<int> v);
        static std::vector<std::string> split(std::string str, char delimiter, bool trim_whitespace);
        static std::vector<std::string> split(std::string str, char delimiter);
        static std::vector<int> split_to_int(std::string str, char delimiter);
};

// trim from start
inline std::string &TextParser::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
inline std::string &TextParser::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
inline std::string &TextParser::trim(std::string &s) {
    return ltrim(rtrim(s));
}

// strip quotes
inline std::string &TextParser::strip_quotes(std::string &s) {
    if (s.front() == '"' && s.back() == '"') {
        s.erase(0, 1);
        s.erase(s.size() - 1);
    }
    return s;
}

#endif
