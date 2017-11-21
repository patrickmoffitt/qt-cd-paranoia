#ifndef QTCDPARANOIA_HPP
#define QTCDPARANOIA_HPP

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include "cdio/cdio.h"

class QtCdParanoia : public QObject
{
    Q_OBJECT

    QString formatTrackNum(int track_num);
    QtCdParanoia *qtCdParanoia;
    bool cancelImportFlag;

public:
    explicit QtCdParanoia(QObject *parent = nullptr);
    ~QtCdParanoia();

    QObject *errorMap;

    int disc_id{0};

    bool setError(const char *key, const QJsonObject value);

    bool setError(const QString key, QByteArray value);

    bool setError(const char *key, const char *value);

    QJsonObject getError(const char *key);

    QString getErrorText(const char *key);
    QString getErrorText(const QString key);

    QString getErrorDetailedText(const char *key);
    QString getErrorDetailedText(const QString key);

    const CdIo_t *p_cdio;

    void updateWriteStatus(const QString text);


signals:
    void errorMessage(const QString text, const QString detailedText);
    void driveName(const QString text);
    void cddbSearchResult(const QJsonObject result);
    void cddbTrackInfo(const QJsonObject result);
    void busyIndicator();
    void updateStatus(const QString text);

public slots:
    bool setError(const QString key, const QJsonObject value);
    QJsonObject getError(const QString key);
    QString getMusicHome();
    void scanForDrive();
    void listTracks(const QString category);
    void writeTracks(const QJsonObject importData);
    void cancelImport();
};

#endif // QTCDPARANOIA_HPP
