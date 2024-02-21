#include "rssreader.h"
#include "ui_rssreader.h"
// TODO: add webcrawler for finding RSS feeds
RSSReader::RSSReader(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RSSReader)
{
    ui->setupUi(this);
    connect(ui->addFeedButton, &QPushButton::clicked, this, &RSSReader::addFeed);
    connect(ui->feedListWidget, &QListWidget::currentTextChanged, this, &RSSReader::showFeedContent);
    connect(ui->removeFeedButton, &QPushButton::clicked, this, &RSSReader::removeFeed);
    ui->feedListWidget->addItem("https://www.spiegel.de/schlagzeilen/rss/0,5291,,00.xml");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("feeds.db");
    if (!db.open()) {
        qDebug() << "Error: Failed to open database:" << db.lastError().text();
    } else {
        qDebug() << "Database opened successfully";
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS feeds (url TEXT PRIMARY KEY)");

        loadFeedsFromDatabase();
    }
}

void RSSReader::removeFeed()
{
    QListWidgetItem *selectedItem = ui->feedListWidget->currentItem();
    if (selectedItem) {
        QString feedUrl = selectedItem->text();
        int row = ui->feedListWidget->currentRow();
        delete ui->feedListWidget->takeItem(row);

        QSqlQuery query;
        query.prepare("DELETE FROM feeds WHERE url = (:url)");
        query.bindValue(":url", feedUrl);
        if (!query.exec()) {
            qDebug() << "Error removing feed from database:" << query.lastError().text();
        }
    }
}

void RSSReader::loadFeedsFromDatabase()
{
    QSqlQuery query("SELECT url FROM feeds");
    while (query.next()) {
        QString feedUrl = query.value(0).toString();
        QListWidgetItem *item = new QListWidgetItem(feedUrl);
        ui->feedListWidget->addItem(item);
    }
}

RSSReader::~RSSReader()
{
    delete ui;
}

void RSSReader::addFeed()
{
    QString feedUrl = ui->feedUrlLineEdit->text();
    if (!feedUrl.isEmpty()) {
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT url FROM feeds WHERE url = (:url)");
        checkQuery.bindValue(":url", feedUrl);
        if (checkQuery.exec() && checkQuery.next()) {
            qDebug() << "Feed URL already exists in the database";
        } else {
            QSqlQuery query;
            query.prepare("INSERT INTO feeds (url) VALUES (:url)");
            query.bindValue(":url", feedUrl);
            if (!query.exec()) {
                qDebug() << "Error inserting feed into database:" << query.lastError().text();
            } else {
                QListWidgetItem *item = new QListWidgetItem(feedUrl);
                ui->feedListWidget->addItem(item);
                ui->feedUrlLineEdit->clear();
            }
        }
    }
}

void RSSReader::showFeedContent(const QString &text)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();

            QDomDocument doc;
            if (doc.setContent(data)) {
                QString content;
                QDomNodeList items = doc.elementsByTagName("item");
                for (int i = 0; i < items.size(); ++i) {
                    QDomNode itemNode = items.item(i);
                    if (itemNode.isElement()) {
                        QDomElement itemElement = itemNode.toElement();
                        QString title = itemElement.elementsByTagName("title").item(0).toElement().text();
                        //QString link = itemElement.elementsByTagName("link").item(0).toElement().text();
                        QString description = itemElement.elementsByTagName("description").item(0).toElement().text();
                        content += "Title: " + title + "\n";
                        //content += "Link: " + link + "\n";
                        if(description.isEmpty())
                            content += "Description: No Description available.\n\n";
                        else
                            content += "Description: " + description + "\n\n";
                    }
                }
                ui->feedContentTextEdit->setText(content);
            } else {
                ui->feedContentTextEdit->setText("Error parsing feed content: Invalid XML format");
            }
        } else {
            ui->feedContentTextEdit->setText("Error fetching feed content: " + reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    });

    QUrl url(text);
    manager->get(QNetworkRequest(url));
}
