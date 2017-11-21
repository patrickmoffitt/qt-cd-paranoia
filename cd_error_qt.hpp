#ifndef CD_ERROR_QT_HPP
#define CD_ERROR_QT_HPP
#include <string>
#include "qtcdparanoia.hpp"

class Cd_error_qt
{
public:
    Cd_error_qt();
    static void set_error(std::string method_name,
                          std::string text,
                          std::string detailedText);
};

#endif // CD_ERROR_QT_HPP
