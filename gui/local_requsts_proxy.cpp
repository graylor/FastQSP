#include "local_requsts_proxy.h"
#include <QNetworkRequest>
#include <QDebug>


void LocalRequestsProxy::setGameDirectory(const QString &_gameDir)
{
    gameDir = _gameDir;
}

QNetworkReply *LocalRequestsProxy::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QString host = request.url().host();
    //qDebug() << "Request: " << request.url().toString();
    if (op == GetOperation && host == "qspgame.local")
    {
        QUrl url = request.url();
        //qDebug() << "Request catched!" << url.toString();
        url.setScheme("file");
        url.setHost("");
        url.setPath(gameDir + url.path());
        QNetworkRequest getRequest = request;
        getRequest.setUrl(url);
        //qDebug() << "get url:" << url.toString();
        return get(getRequest);
    }
    else
    {
        return QNetworkAccessManager::createRequest(op, request, outgoingData);
    }
}
