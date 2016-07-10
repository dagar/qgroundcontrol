#include "QGCTileFetcher.h"

#include "QGCMapEngine.h"

#include <QtLocation/private/qgeotilespec_p.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QFile>
#include <QtLocation/private/qgeotilespec_p.h>

//-----------------------------------------------------------------------------
QGCTileFetcher::QGCTileFetcher(QNetworkAccessManager *networkManager, const QNetworkRequest &request, const QGeoTileSpec &spec, QObject *parent)
    : QObject(parent)
    , _reply(NULL)
    , _request(request)
    , _networkManager(networkManager)
    , _spec(spec)
{

}

//-----------------------------------------------------------------------------
QGCTileFetcher::~QGCTileFetcher()
{
    if (_reply) {
        _reply->deleteLater();
        _reply = 0;
    }
}

//-----------------------------------------------------------------------------
void
QGCTileFetcher::replyDestroyed()
{
    _reply = 0;
}

//-----------------------------------------------------------------------------
void
QGCTileFetcher::networkReplyFinished()
{
    if (!_reply) {
        return;
    }
    if (_reply->error() != QNetworkReply::NoError) {
        return;
    }
    QByteArray a = _reply->readAll();
    QString format = getQGCMapEngine()->urlFactory()->getImageFormat((UrlFactory::MapType)_spec.mapId(), a);
    if(!format.isEmpty()) {
        getQGCMapEngine()->cacheTile((UrlFactory::MapType)_spec.mapId(), _spec.x(), _spec.y(), _spec.zoom(), a, format);
    }
    _reply->deleteLater();
    _reply = 0;
}

//-----------------------------------------------------------------------------
void
QGCTileFetcher::networkReplyError(QNetworkReply::NetworkError error)
{
    if (!_reply) {
        return;
    }
    if (error != QNetworkReply::OperationCanceledError) {
        qWarning() << "Fetch tile error:" << _reply->errorString();
    }
    _reply->deleteLater();
    _reply = 0;
}
