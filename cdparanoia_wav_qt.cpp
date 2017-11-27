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

#include <QDir>
#include <iostream>
#include <cdio/cdio.h>
#include <cdio/paranoia/paranoia.h>
#include "cdparanoia_wav_qt.hpp"
#include <fcntl.h>
#include <unistd.h>
#include "cdio_qt.hpp"
#include "error_logging.hpp"
#include "boost/filesystem.hpp"

namespace std {
    namespace filesystem = boost::filesystem;
}

namespace fs = std::filesystem;

Cdparanoia_wav_qt::Cdparanoia_wav_qt(QString dest_root_dir,
                                     QString dest_artist_dir,
                                     QString dest_album_dir) {

    this->dest_root_dir = dest_root_dir;
    this->dest_artist_dir = dest_artist_dir;
    this->dest_album_dir = dest_album_dir;

    const char *nixUser = std::getenv("USER");
    const char *winUser = std::getenv("USERNAME");
    sizeof(nixUser) > 0 ? userName = nixUser : userName = winUser;

    QDir dest(this->dest_root_dir);
    bool destRootDirExists = dest.exists();
    QFileInfo rootDir(this->dest_root_dir);
    bool destRootDirWritable = rootDir.isWritable();

    if (not destRootDirExists) {
        Cd_error_qt::set_error(__METHOD_NAME__,
                  "Music import home directory does not exist.",
                  "Please create this directory and retry.");
        ERROR_MSG("%s", "Music import home directory does not exist.");
    } else {
        if (not destRootDirWritable) {
            Cd_error_qt::set_error(__METHOD_NAME__,
                      "Permission denied for " + QByteArray(userName).toStdString(),
                      "Unable to write in music import home directory: " + this->dest_root_dir.toStdString());
            ERROR_MSG("%s", "Unable to write in music import home directory.");
        }
    }
    fs::path path{this->dest_root_dir.toStdString()};
    path /= this->dest_artist_dir.toStdString();
    path /= this->dest_album_dir.toStdString();
    path += fs::path::preferred_separator;
    targetPath = QString(QByteArray(path.c_str()));

    if (destRootDirExists and destRootDirWritable) {
        if (not dest.mkpath(targetPath)) {
            Cd_error_qt::set_error(__METHOD_NAME__,
                      "Failed to create album directory.",
                      "Attempt to create " + targetPath.toStdString() + " failed.");
            ERROR_MSG("%s", "Failed to create album directory.");
            targetPath = "";
        }
    }
};

Cdparanoia_wav_qt::~Cdparanoia_wav_qt() {}

void Cdparanoia_wav_qt::put_num(long int num, int f, int bytes) {
    unsigned int i;
    unsigned char c;
    for (i=0; bytes--; i++) {
        c = static_cast<unsigned char>((num >> (i<<3)) & 0xff);
        if (write(f, &c, 1)==-1) {
            ERROR_MSG("%s", "Could not write to output.");
            exit(1);
        }
    }
};

#define writestr(fd, s) write(fd, s, (sizeof(s) - 1))    // Minus 1 for trailing '\0'.

// Write a the header for a WAV file.
void Cdparanoia_wav_qt::write_WAV_header(int fd, int32_t i_bytecount) {
    ssize_t bytes_ret __attribute__((unused));
    // quick and dirty @ToDo How dirty Rocky?
    bytes_ret = writestr(fd, "RIFF");     /*  0-3 */
    assert(bytes_ret > 0);
    put_num(i_bytecount+44-8, fd, 4);     /*  4-7 */
    bytes_ret = writestr(fd, "WAVEfmt "); /*  8-15 */
    assert(bytes_ret > 0);
    put_num(16, fd, 4);                   /* 16-19 */
    put_num(1, fd, 2);                    /* 20-21 */
    put_num(2, fd, 2);                    /* 22-23 */
    put_num(44100, fd, 4);                /* 24-27 */
    put_num(44100*2*2, fd, 4);            /* 28-31 */
    put_num(4, fd, 2);                    /* 32-33 */
    put_num(16, fd, 2);                   /* 34-35 */
    bytes_ret = writestr(fd, "data");     /* 36-39 */
    assert(bytes_ret > 0);
    put_num(i_bytecount, fd, 4);          /* 40-43 */
};

const void Cdparanoia_wav_qt::write_track(track_t track_num, QString title) {

    if (track_num < 1 or track_num > 99) {
        ERROR_MSG("%s %d %s %s %s",
                  "Track number (",
                  int(track_num),
                  ") is out of bounds for \"",
                  title.toStdString(),
                  "\".");
    }
    cdrom_drive_t *drive = nullptr;
    CdIo_t *p_cdio = cdio_qt::open(nullptr, DRIVER_UNKNOWN);
    drive = cdio_cddap_identify_cdio(p_cdio, CDDA_MESSAGE_FORGETIT, nullptr);

    if (!drive) {
        ERROR_MSG("%s", "Unable to identify audio CD disc.");
    }

    if (cdda_open(drive) != 0) {
        ERROR_MSG("%s", "Unable to open disc.");
        exit(77);
    }

    cdrom_paranoia_t *p = paranoia_init(drive);

    // Return the lsn for the start of track_num.
    lsn_t i_first_lsn;
    i_first_lsn = cdio_cddap_track_firstsector(drive, track_num);
    if (-1 == i_first_lsn) {
        ERROR_MSG("%s %d %s",
                  "Failed to get starting LSN for track ",
                  int(track_num),
                  ".");
    } else {
        lsn_t i_cursor;
        ssize_t bytes_ret;
        track_t i_track = cdda_sector_gettrack(drive, i_first_lsn);
        lsn_t i_last_lsn = cdda_track_lastsector(drive, i_track);
        title = targetPath + title + ".wav";

        ERROR_MSG("%s %s",
                  "Creating ",
                  title.toStdString());
        int fd = creat(title.toUtf8(), 0644);
        if (-1 == fd) {
            set_error(__METHOD_NAME__,
                      "Track write error",
                      "Unable to create " + title.toStdString());
        }
        // Set reading mode for full paranoia, but allow skipping sectors.
        paranoia_modeset(p, PARANOIA_MODE_FULL^PARANOIA_MODE_NEVERSKIP);
        paranoia_seek(p, i_first_lsn, SEEK_SET);
        write_WAV_header(fd, (i_last_lsn - i_first_lsn + 1) * CDIO_CD_FRAMESIZE_RAW);

        for (i_cursor = i_first_lsn; i_cursor <= i_last_lsn; i_cursor++) {
            // read a sector
            int16_t *p_readbuf = paranoia_read(p, NULL);
            char *psz_err = cdda_errors(drive);
            char *psz_mes = cdda_messages(drive);

            if (psz_mes || psz_err) {
                ERROR_MSG("%s %d", psz_mes, psz_err);
            }
            free(psz_err);
            free(psz_mes);
            if (!p_readbuf) {
                ERROR_MSG("%s", "cd-paranoia read error. Stopping.");
                break;
            }
            bytes_ret = write(fd, p_readbuf, CDIO_CD_FRAMESIZE_RAW);
            assert(bytes_ret > 0);
        }
        close(fd);
    }
    paranoia_free(p);
    cdda_close(drive);
};

