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

#ifndef CDPARANOIA_WAV_QT_HPP
#define CDPARANOIA_WAV_QT_HPP

#include "cd_error_qt.hpp"
#include <string>
#include <cstdlib>
#include <cdio/paranoia/cdda.h>
#include <QObject>
#include <QString>

class Cdparanoia_wav_qt :  public Cd_error_qt
{
    const char *userName;
    QString dest_root_dir;
    QString dest_artist_dir;
    QString dest_album_dir;

    void put_num(long int num, int f, int bytes);
    void write_WAV_header(int fd, int32_t i_bytecount);

public:
    QString targetPath;
    explicit Cdparanoia_wav_qt(QString dest_root_dir,
                               QString dest_artist_dir,
                               QString dest_album_dir);
    ~Cdparanoia_wav_qt();

    const void write_track(track_t track_num, QString title);

signals:
    void updateStatus(const QString text);

public slots:

private:


};


#endif //CDPARANOIA_WAV_QT_HPP
