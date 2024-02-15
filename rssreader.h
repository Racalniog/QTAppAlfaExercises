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

private:
    Ui::RSSReader *ui;
};

#endif // RSSREADER_H
