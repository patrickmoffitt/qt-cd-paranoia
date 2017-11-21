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

#ifndef CDPARANOIA_CDIO_QT_HPP
#define CDPARANOIA_CDIO_QT_HPP
#include "cd_error_qt.hpp"
#include <cdio/cdio.h>
#include <cdio/device.h>

class cdio_qt : public Cd_error_qt {
public:
    static int cddb_sum(int n);

    static CdIo_t *open(const char *psz_source, driver_id_t driver_id);

    static track_t get_num_tracks(const CdIo_t *p_cdio);

    static lba_t get_track_lba(const CdIo_t *p_cdio, track_t i_track);

    static cdio_hwinfo_t get_hardware_info(const CdIo_t *p_cdio);

};
#endif //CDPARANOIA_CDIO_QT_HPP
