/*
 * Contributed to Stack Overflow by Even Teran.
 * https://stackoverflow.com/users/13430/evan-teran
 *
 * Edited there by jotik
 * https://stackoverflow.com/users/3919155/jotik
 *
 */

#include <algorithm>
#include <cctype>
#include <locale>
#include <regex>
#include <QString>

namespace str {
    // trim from start (in place)
    static void ltrim(std::string &s) {
        using std::locale;
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !isspace(ch);
        }));
    }

    // trim from end (in place)
    static void rtrim(std::string &s) {
        using std::locale;
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    // trim from start (copying)
    static std::string ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    static std::string rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    static std::string trim_copy(std::string s) {
        trim(s);
        return s;
    }

    // trim from the middle
    static std::string condense(std::string &s) {
        // Multiple spaces are condensed to one.
        std::regex rgx_spaces("(\\s{2,})");
        s = regex_replace(s, rgx_spaces, " ");
        return s;
    }

    // condense and trim
    static void condense_trim(std::string &s) {
        condense(s);
        trim(s);
    }

    static void apply_char_ptr(std::function<void(std::string &s)> apply, char *c) {
        std::string s{c};
        apply(s);
        strcpy(c, s.c_str());
    }

    // Copy char* and return QString.
    static QString char_ptr_qstring_copy(char *c) {
        return QString(QByteArray(c));
    }

    // Copy std::string and return QString
    static QString string_qstring_copy(std::string s) {
        return QString(QByteArray(s.c_str()));
    }
}

