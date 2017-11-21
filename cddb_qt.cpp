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

#include <sstream>
#include "error_logging.hpp"
#include "cddb_qt.hpp"


cddb_conn_t *cddb_qt::connect(void) {
    cddb_conn_t *conn;
    conn = cddb_new();
    if (conn == NULL) {
        set_error(__METHOD_NAME__,
                  "freedb.org connection error.",
                  format_conn_err(conn));
        auto message = format_conn_err(conn);
        ERROR_MSG("%s", message.c_str());
        conn = nullptr;
    }
    return conn;
};

cddb_disc_s *cddb_qt::disc_new(void) {
    cddb_disc_t *cddb_disc;
    cddb_disc = cddb_disc_new();
    if (cddb_disc == NULL) {
        set_error(__METHOD_NAME__,
                  "Memory allocation error.",
                  "Failed to allocate memory for the CDDB disc structure.");
        ERROR_MSG("%s", "Failed to allocate memory for the CDDB disc structure.");
        cddb_disc = nullptr;
    }
    return cddb_disc;
};

int cddb_qt::read(cddb_conn_t *conn, cddb_disc_t *disc) {
    int success{0};
    success = cddb_read(conn, disc);
    if (success != 1) {
        set_error(__METHOD_NAME__,
                  "freedb.org connection error.",
                  format_conn_err(conn));
        auto message = format_conn_err(conn);
        ERROR_MSG("%s", message.c_str());
    }
    return success;
};

std::string cddb_qt::format_conn_err(cddb_conn_t *conn) {
    std::stringstream buff;
    cddb_error_t errnum;
    errnum = cddb_errno(conn);
    const char *errmsg;
    errmsg = cddb_error_str(errnum);
    buff << "CDDB read error " << errnum << "; " << errmsg << ".";
    return buff.str();
};


