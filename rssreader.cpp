#include "rssreader.h"
#include "ui_rssreader.h"

RSSReader::RSSReader(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RSSReader)
{
    ui->setupUi(this);
    connect(ui->addFeedButton, &QPushButton::clicked, this, &RSSReader::addFeed);
    connect(ui->feedListWidget, &QListWidget::currentTextChanged, this, &RSSReader::showFeedContent);
    ui->feedListWidget->addItem("https://www.spiegel.de/schlagzeilen/rss/0,5291,,00.xml");
}

RSSReader::~RSSReader()
{
    delete ui;
}

void RSSReader::addFeed()
{
    QString feedUrl = ui->feedUrlLineEdit->text();
    if (!feedUrl.isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem(feedUrl);
        ui->feedListWidget->addItem(item);
        ui->feedUrlLineEdit->clear();
    }
}

void RSSReader::showFeedContent(const QString &text)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();

            // Parse XML data
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
