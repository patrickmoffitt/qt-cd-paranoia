/*
 * Copyright (C) 2017
 * Patrick Moffitt
 *
 * Provides the ability to log errors to stderr using a conditional marco.
 *
 * To use this macro add the following two lines to the top of main.cpp in the
 * order shown:
 *
 *     #define STD_ERROR_LOG
 *     #include "error_logging.hpp"
 *
 * Comment out the first line to disable the macro.
 * Error message output will show the class name::method name (line #) if there
 * is a class or simply method name.
 *
 * When STD_ERROR_LOG is defined the macro function ERROR_MSG(FormatLiteral, ...)
 * exists. FormatLiteral is a printf format string literal see
 * https://en.wikipedia.org/wiki/Printf_format_string for details. This string is a
 * template for the ordered, count and type of arguments provided variadically.
 *
 * NOTE: This implementation requires clang/llvm. Change ##__VA_ARGS__ below to
 * __VA_ARGS__ if you're using another compiler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ERROR_LOGGING_HPP
    #define ERROR_LOGGING_HPP
    #include <string>
    #include <cstring>

    inline char *methodName(const std::string& prettyFunction) {
        std::string name;
        size_t colons = prettyFunction.find("::");
        if(colons == std::string::npos) {
            size_t begin = prettyFunction.rfind(" ") + 1;
            size_t count = prettyFunction.rfind("(") - begin;
            name = prettyFunction.substr(begin, count);
        } else {
            size_t begin = prettyFunction.substr(0, colons).rfind(" ") + 1;
            size_t count = prettyFunction.find("(") - begin;
            std::string with_colons{prettyFunction.substr(begin, count)};
            size_t star = with_colons.find("*");
            if(star != std::string::npos) {
                name = with_colons.replace(with_colons.find("*"), 1, "");
            } else {
                name = with_colons;
            }
        }
        char * name_array = new char [name.length()+1];
        std::strcpy (name_array, name.c_str());
        return name_array;
    }

    #define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__)

    #ifdef STD_ERROR_LOG
        #include <cstdio>
        #include <iostream>
        # define ERROR_MSG(FormatLiteral, ...)  \
        fprintf (stderr, "Error %s (%d): " FormatLiteral "\n", __METHOD_NAME__, __LINE__,\
                 ##__VA_ARGS__)
    #else
        #define ERROR_MSG(...) {}
    #endif

#endif //ERROR_LOGGING_HPP
