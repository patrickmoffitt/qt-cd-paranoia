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

#include "cdda_info.hpp"
#include "cdio_qt.hpp"
#include "cddb_qt.hpp"
#include "error_logging.hpp"
#include <sstream>
#include <iomanip>

#define STR_OR_NULL(s) ((s) ? s : "")

using std::string;
using std::list;

list<string> Cdda_info::categories = {
        "folk",
        "jazz",
        "misc",
        "rock",
        "country",
        "blues",
        "newage",
        "reggae",
        "classical",
        "soundtrack",
        "data",
};

Cdda_info::Cdda_info() {
    conn = cddb_qt::connect();
};

Cdda_info::~Cdda_info() {
    cddb_destroy(conn);
    cddb_disc_destroy(cddb_disc);
}

void Cdda_info::identify_disc_id(CdIo_t *p_cdio) {
    track_t i_track;
    track_t i_tracks;
    unsigned n = 0;

    i_tracks = cdio_qt::get_num_tracks(p_cdio);
    lba_t lba;
    for (i_track = 1; i_track <= i_tracks; ++i_track) {
        lba = cdio_qt::get_track_lba(p_cdio, i_track);
        n += cdio_qt::cddb_sum(lba / CDIO_CD_FRAMES_PER_SEC);
    }

    unsigned int start_sec;
    start_sec = static_cast<unsigned int>(cdio_qt::get_track_lba(p_cdio, 1)
                                          / CDIO_CD_FRAMES_PER_SEC);
    unsigned int leadout_sec;
    leadout_sec = static_cast<unsigned int>(cdio_qt::get_track_lba(p_cdio, CDIO_CDROM_LEADOUT_TRACK)
                                            / CDIO_CD_FRAMES_PER_SEC);
    unsigned total = (leadout_sec - start_sec);
    disc_id = ((n % 0xff) << 24 | total << 8 | i_tracks);
};

void Cdda_info::identify_disc() {
    Disc d;
    if (disc_id != 0 and conn != NULL) {
        // Create a new libcddb disc structure.
        cddb_disc = cddb_qt::disc_new();
        // Initialize the ID of the disc.
        cddb_disc_set_discid(cddb_disc, disc_id);
        for (auto category : categories) {
            cddb_disc_set_category_str(cddb_disc, category.c_str());
            if (cddb_qt::read(conn, cddb_disc) == 1) {
                d.id = disc_id;
                d.title = STR_OR_NULL(cddb_disc_get_title(cddb_disc));
                d.artist = STR_OR_NULL(cddb_disc_get_artist(cddb_disc));
                d.extra = STR_OR_NULL(cddb_disc_get_ext_data(cddb_disc));
                d.genre = STR_OR_NULL(cddb_disc_get_genre(cddb_disc));
                d.category = category;
                d.year = cddb_disc_get_year(cddb_disc);
                d.length = cddb_disc_get_length(cddb_disc);
                d.track_count = cddb_disc_get_track_count(cddb_disc);
                freedb_results.push_back(d);
            }
        }
    } else if (disc_id == 0) {
        set_error(__METHOD_NAME__,
                  "CDDB disc ID not set.",
                  "Querying the freedb.org database requires this id as its primary key.");
        ERROR_MSG("%s", "CDDB disc ID not set.");
    } else {
        set_error(__METHOD_NAME__,
                  "No connection to freedb.org.",
                  "Check internet connection and retry.");
        ERROR_MSG("%s", "No connection to freedb.org.");
    }
};

std::list<Track> Cdda_info::identify_tracks(std::string category) {
    cddb_disc = cddb_disc_new();        // Create a new libcddb disc structure.
    // Initialize the ID of the disc.
    cddb_disc_set_discid(cddb_disc, disc_id);
    cddb_disc_set_category_str(cddb_disc, category.c_str());
    std::list<Track> l;
    if (cddb_qt::read(conn, cddb_disc) == 1) {
        cddb_track_t *track = NULL; // libcddb track structure.
        Track t;
        for (track = cddb_disc_get_track_first(cddb_disc);
             track != NULL;
             track = cddb_disc_get_track_next(cddb_disc)) {
            t.track_num = cddb_track_get_number(track);
            t.song_title = cddb_track_get_title(track);
            t.length = cddb_track_get_length(track);
            t.ext = STR_OR_NULL(cddb_track_get_ext_data(track));
            l.push_back(t);
        }
    }
    return l;
};

string Cdda_info::format_seconds(int s) {
    time_t seconds(s);
    std::stringstream buff;
    tm *p = std::gmtime(&seconds);
    if (p != NULL) {
        buff << std::setw(2) << std::setfill('0') << p->tm_hour
             << ":"
             << std::setw(2) << std::setfill('0') << p->tm_min
             << ":"
             << std::setw(2) << std::setfill('0') << p->tm_sec;
        return buff.str();
    } else {
        return "00:00:00";
    }
};
