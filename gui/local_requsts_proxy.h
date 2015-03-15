#ifndef LOCAL_REQUSTS_PROXY_H
#define LOCAL_REQUSTS_PROXY_H
#include <QNetworkAccessManager>

class LocalRequestsProxy : public QNetworkAccessManager {
public:
  void setGameDirectory(const QString &_gameDir);

  // QNetworkAccessManager interface
protected:
  virtual QNetworkReply *createRequest(Operation op,
                                       const QNetworkRequest &request,
                                       QIODevice *outgoingData);

private:
  QString gameDir;
};

#endif // LOCAL_REQUSTS_PROXY_H
