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

#ifndef CDPARANOIA_CDDA_INFO_H
#define CDPARANOIA_CDDA_INFO_H

#include "cd_error_qt.hpp"
#include <cddb/cddb.h>
#include <cdio/cdio.h>
#include <string>
#include <list>

using std::string;
using std::list;

// Models the CDDB/freedb.org data provided for audio tracks.
struct Track {
    int track_num;
    string song_title;
    int length;
    string ext;
};

// Models the CDDB/freedb.org data provided for CDDA discs.
struct Disc {
    int id{0};
    string title;
    string artist;
    string extra;
    int year;
    int length;
    string category;
    string genre;
    int track_count;
    list<Track> tracks;
};

class Cdda_info : public Cd_error_qt {
public:
    Cdda_info();
    ~Cdda_info();
    list<Disc> freedb_results;
    Disc disc;
    int disc_id{0};
    string format_seconds(int s);
    list<Track> identify_tracks(string category);
    void identify_disc_id(CdIo_t *p_cdio);
    void identify_disc();
private:
    cddb_conn_t *conn = NULL;        // libcddb connection struct.
    cddb_disc_t *cddb_disc = NULL;   // libcddb disc struct.
    static list<string> categories;
};
#endif //CDPARANOIA_CDDA_INFO_H
