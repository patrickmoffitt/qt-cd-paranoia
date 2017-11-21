/*
  Copyright (C) 2017
  Patrick Moffitt

  Copyright (C) 2011
  Rocky Bernstein <rocky@gnu.org>

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

#ifndef CDPARANOIA_CDDB_QT_H
#define CDPARANOIA_CDDB_QT_H

#include <cddb/cddb.h>
#include "cd_error_qt.hpp"

class cddb_qt : public Cd_error_qt {

public:
    static cddb_conn_t *connect(void);

    static cddb_disc_t *disc_new(void);

    static int read(cddb_conn_t *conn, cddb_disc_t *disc);

    static std::string format_conn_err(cddb_conn_t *conn);
};

#endif //CDPARANOIA_CDDB_QT_H
