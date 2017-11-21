/*
  Copyright (C) 2017
  Patrick Moffitt

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "cd_error_qt.hpp"

Cd_error_qt::Cd_error_qt() {};

void Cd_error_qt::set_error(std::string method_name,
                            std::string text,
                            std::string detailedText) {
    extern QtCdParanoia qtCdParanoia;
    QJsonObject value{
        {"text", QString(QByteArray(text.c_str()))},
        {"detailedText", QString(QByteArray(detailedText.c_str()))}
    };
    qtCdParanoia.setError(method_name.c_str(), value);
};

