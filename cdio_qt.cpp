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

#include <iostream>
#include "cdio_qt.hpp"
#include "error_logging.hpp"

int cdio_qt::cddb_sum(int n)
{
    int result{0};
    while (n > 0) {
        result += (n % 10);
        n /= 10;
    }
    return result;
}

CdIo_t *cdio_qt::open(const char *psz_source, driver_id_t driver_id) {
    CdIo_t *p_cdio = cdio_open(psz_source, driver_id);
    if (p_cdio == nullptr) {
        set_error(__METHOD_NAME__,
                  "Failed to open CD-ROM drive.",
                  "Check that the drive is plugged in, powered up, and has an audio CD in it.");
        ERROR_MSG("%s", "Failed to open CD-ROM drive.");
        p_cdio = nullptr;
    }
    return p_cdio;
};

track_t cdio_qt::get_num_tracks(const CdIo_t *p_cdio) {
    track_t track = cdio_get_num_tracks(p_cdio);
    if (track == CDIO_INVALID_TRACK) {
        set_error(__METHOD_NAME__,
                  "CD-ROM TOC read failed.",
                  "Failed to read the disc's Table of Contents (TOC). Clean the disc and retry.");
        ERROR_MSG("%s", "CD-ROM TOC read failed.");
    }
    return track;
};

lba_t cdio_qt::get_track_lba(const CdIo_t *p_cdio, track_t i_track) {
    lba_t lba = cdio_get_track_lba(p_cdio, i_track);
    if (lba == CDIO_INVALID_LBA) {
        set_error(__METHOD_NAME__,
                  "Can't find track " + std::to_string(int(i_track)) + ".",
                  "Con't read Logical Block Address (LBA). Clean the disc and retry.");
        ERROR_MSG("%s %d %s", "Can't get LBA for track ", int(i_track), ".");
    }
    return lba;
};

cdio_hwinfo_t cdio_qt::get_hardware_info(const CdIo_t *p_cdio) {
    cdio_hwinfo_t info;
    bool successful = cdio_get_hwinfo(p_cdio, &info);
    if (not successful) {
        set_error(__METHOD_NAME__,
                  "CD-ROM hardware information read error.",
                  "Failed to read CD-ROM drive vendor, model, and revision.");
        ERROR_MSG("%s", "Failed to read CD-ROM hardware information.");
    }
    return info;
};
