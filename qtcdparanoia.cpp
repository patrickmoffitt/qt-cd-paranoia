#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrent>
#include "cdio_qt.hpp"
#include "cdio_hwinfo_wrap.hpp"
#include "cdda_info.hpp"
#include "trim.cpp"
#include "error_logging.hpp"
#include "cdparanoia_wav_qt.hpp"
#include "error_logging.hpp"


QString QtCdParanoia::formatTrackNum(int track_num) {
    std::string numeric_string;
    if (track_num > 0 and track_num < 10) {
        numeric_string = "0" + std::to_string(track_num);
    } else {
        numeric_string = std::to_string(track_num);
    }
    return QString(QByteArray(numeric_string.c_str()));
}

QtCdParanoia::QtCdParanoia(QObject *parent) : QObject(parent)
{
    errorMap = new QObject(this);
    extern QtCdParanoia qtCdParanoia;
    this->qtCdParanoia = &qtCdParanoia;
}

QtCdParanoia::~QtCdParanoia() {
    errorMap->deleteLater();
    qtCdParanoia = nullptr;
}

QString QtCdParanoia::getMusicHome() {
    return QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first();
}

bool QtCdParanoia::setError(const char *key, QJsonObject value) {
    return errorMap->setProperty(key, QVariant(value));
}

// Slot specialization.
bool QtCdParanoia::setError(const QString key, const QJsonObject value) {
    ERROR_MSG("%s %s", "value.text: ", value.value("text").toString().toStdString().c_str());
    return errorMap->setProperty(key.toLatin1().data(), QVariant(value));
}

bool QtCdParanoia::setError(const QString key, QByteArray value) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(value);
    QJsonObject jsonObject = jsonDoc.object();
    return errorMap->setProperty(key.toLocal8Bit().constData(), QVariant(jsonObject));
}

bool QtCdParanoia::setError(const char *key, const char *value) {
    QByteArray ba(value);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(ba);
    QJsonObject jsonObject = jsonDoc.object();
    return errorMap->setProperty(key, QVariant(jsonObject));
}

QJsonObject QtCdParanoia::getError(const char *key) {
    QVariant ob;
    // Always read the errorMap in the global instance;
    ob = qtCdParanoia->errorMap->property(key);
    return ob.toJsonObject();
}

// Slot specialization.
QJsonObject QtCdParanoia::getError(const QString key) {
    QVariant ob;
    // Always read the errorMap in the global instance;
    ob = qtCdParanoia->errorMap->property(key.toLatin1().data());
    return ob.toJsonObject();
}

QString QtCdParanoia::getErrorText(const char *key) {
    QJsonObject err = getError(key);
    return err.value("text").toString();
}

QString QtCdParanoia::getErrorText(const QString key) {
    QJsonObject err = getError(key);
    return err.value("text").toString();
}

QString QtCdParanoia::getErrorDetailedText(const char *key) {
    QJsonObject err = getError(key);
    return err.value("detailedText").toString();
}

QString QtCdParanoia::getErrorDetailedText(const QString key) {
    QJsonObject err = getError(key);
    return err.value("detailedText").toString();
}

void QtCdParanoia::updateWriteStatus(const QString text) {
    updateStatus(text);
}

// Scan for a CD-ROM drive with a CD-DA disc. If found, fetch the CDDB info for it.
void QtCdParanoia::scanForDrive() {
    busyIndicator();
    QFuture<void> future = QtConcurrent::run([=]() {
        Cdda_info info;
        CdIo_t* p_cdio = cdio_qt::open(nullptr, DRIVER_UNKNOWN);
        if (p_cdio != nullptr) {
            cdio_hwinfo_wrap wrap;
            wrap.info = cdio_qt::get_hardware_info(p_cdio);
            QJsonObject cdioHwInfoErr = getError("cdio_qt::get_hardware_info");
            if (cdioHwInfoErr.isEmpty()) {
                wrap.apply(str::apply_char_ptr, str::condense_trim);
                QString vendor   = str::char_ptr_qstring_copy(wrap.info_ptr->psz_vendor);
                QString model    = str::char_ptr_qstring_copy(wrap.info_ptr->psz_model);
                QString revision = str::char_ptr_qstring_copy(wrap.info_ptr->psz_revision);
                QString label    = vendor + ": " + model + ", rev " + revision;
                driveName(label);
                info.identify_disc_id(p_cdio);
                this->disc_id = info.disc_id;
                cdio_destroy(p_cdio);
                info.identify_disc();
                for (auto d : info.freedb_results) {
                    cddbSearchResult(
                                QJsonObject({
                                                { "category", QJsonValue(d.category.c_str()) },
                                                { "artist", QJsonValue(d.artist.c_str()) },
                                                { "title",  QJsonValue(d.title.c_str()) }
                                            }));
                }
            } else {
                errorMessage(getErrorText("cdio_qt::get_hardware_info"),
                             getErrorDetailedText("cdio_qt::get_hardware_info"));
            }
        } else {
            errorMessage(getErrorText("cdio_qt::open"),
                         getErrorDetailedText("cdio_qt::open"));
        }
        busyIndicator();
    }); // End of thread.
}

void QtCdParanoia::listTracks(const QString category) {
    busyIndicator();
    QFuture<void> future = QtConcurrent::run([=]() {
        Cdda_info info;
        info.disc_id = this->disc_id;
        std::list<Track> tracks = info.identify_tracks(category.toStdString());
        if(not tracks.empty()) {
            for (auto track : tracks) {
                cddbTrackInfo({
                                  {"track", QJsonValue(track.track_num)},
                                  {"length", QJsonValue(info.format_seconds(track.length).c_str())},
                                  {"title", QJsonValue(track.song_title.c_str())}
                              });
            }
        } else {
            Cd_error_qt::set_error(__METHOD_NAME__,
                      "No track list found.",
                      "Check internet connection to freedb.org and retry.");
            ERROR_MSG("%s", "No track list found.");
        }
        busyIndicator();
    }); // End of thread.
}

void QtCdParanoia::writeTracks(const QJsonObject importData) {
    busyIndicator();
    cancelImportFlag = false;
    QFuture<void> future = QtConcurrent::run([=]() {
        Cdparanoia_wav_qt wavWriter(importData.value("dest_root_dir").toString(),
                                    importData.value("dest_artist_dir").toString(),
                                    importData.value("dest_album_dir").toString()
                                    );
        updateWriteStatus("Writing album to: " + wavWriter.targetPath);
        if (not importData.empty()) {
            QJsonObject writeErr = getError("Cdparanoia_wav_qt::Cdparanoia_wav_qt");
            if (writeErr.isEmpty()) {
                QJsonObject trackObject = importData.value("tracks").toObject();
                foreach (auto item, trackObject) {
                    if (cancelImportFlag) {
                        updateWriteStatus("Import canceled by user.");
                        break;
                    }
                    QJsonObject track = item.toObject();
                    track_t track_num = track.value("track_num").toInt();
                    QString title = formatTrackNum(track_num) + " " + track.value("title").toString();
                    updateWriteStatus("Writing: " + title);
                    wavWriter.write_track(track_num, title);
                    QJsonObject trackErr = getError("Cdparanoia_wav_qt::write_track");
                    if(not trackErr.isEmpty()){
                        errorMessage(getErrorText("Cdparanoia_wav_qt::write_track"),
                                     getErrorDetailedText("Cdparanoia_wav_qt::write_track"));
                    }
                }

            } else {
                errorMessage(getErrorText("Cdparanoia_wav_qt::Cdparanoia_wav_qt"),
                             getErrorDetailedText("Cdparanoia_wav_qt::Cdparanoia_wav_qt"));
            }

        } else {
            Cd_error_qt::set_error(__METHOD_NAME__,
                                   "No track list found.",
                                   "Check internet connection to freedb.org and retry.");
            ERROR_MSG("%s", "No track list found.");
        }
        busyIndicator();
    }); // End of thread.
}

void QtCdParanoia::cancelImport() {
    cancelImportFlag = true;
    errorMessage("Import Canceled", "The import will stop after the current track is written.");
}