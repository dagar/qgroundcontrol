#ifndef QGCTILEFETCHER_H
#define QGCTILEFETCHER_H

#include <QtNetwork/QNetworkReply>
#include <QtLocation/QGeoCodeReply>
#include <QtLocation/private/qgeotilespec_p.h>

class QGCTileFetcher : public QObject
{
    Q_OBJECT
public:
    QGCTileFetcher(QNetworkAccessManager*  networkManager, const QNetworkRequest& request, const QGeoTileSpec &spec, QObject *parent = 0);
    ~QGCTileFetcher();
    void abort();

signals:
    void tileCached(const QGeoTileSpec tileSpec);

private slots:
    void replyDestroyed         ();
    void networkReplyFinished   ();
    void networkReplyError      (QNetworkReply::NetworkError error);

private:
    QNetworkReply*          _reply;
    QNetworkRequest         _request;
    QNetworkAccessManager*  _networkManager;

    const QGeoTileSpec      _spec;
};

#endif // QGCTILEFETCHER_H
