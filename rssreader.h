#ifndef RSSREADER_H
#define RSSREADER_H

#include <QWidget>
#include <QListWidgetItem>
#include <qnetwork.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class RSSReader;
}

class RSSReader : public QWidget
{
    Q_OBJECT

public:
    explicit RSSReader(QWidget *parent = nullptr);
    ~RSSReader();

private slots:
    void addFeed();
    void showFeedContent(const QString &text);
    void removeFeed();

private:
    void loadFeedsFromDatabase();
    Ui::RSSReader *ui;
};

#endif // RSSREADER_H
