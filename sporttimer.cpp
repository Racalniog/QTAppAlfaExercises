#include "sporttimer.h"
#include "ui_sporttimer.h"

SportTimer::SportTimer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SportTimer)
{
    ui->setupUi(this);
    setMouseTracking(true);
    connect(ui->addTimerButton, &QPushButton::clicked, this, &SportTimer::addTimerConnect);
    connect(ui->startButton, &QPushButton::clicked, this, &SportTimer::startTimers);
    connect(ui->pauseButton, &QPushButton::clicked, this, &SportTimer::pauseTimers);
    connect(ui->loadPresetButton, &QPushButton::clicked, this, &SportTimer::loadPresetTimers);
    connect(ui->savePresetButton, &QPushButton::clicked, this, &SportTimer::savePresetTimers);

    ui->minutesSpinBox->setMinimum(0);
    ui->minutesSpinBox->setMaximum(59);
    ui->secondsSpinBox->setMinimum(0);
    ui->secondsSpinBox->setMaximum(59);
}

void SportTimer::pauseTimers()
{
    for (QBasicTimer *timer : timers) {
        timer->stop();
    }
}

void SportTimer::addTimer(int duration)
{
    durations.append(duration);
}

void initializeDatabase()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("presets.db"); // Set the database name

    if (!db.open()) {
        qDebug() << "Error: Unable to open database:" << db.lastError().text();
        return;
    }

    // Create table for presets if it doesn't exist
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS presets (id INTEGER PRIMARY KEY, duration INTEGER)")) {
        qDebug() << "Error creating table:" << query.lastError().text();
        return;
    }
}
void SportTimer::updateTimerListView()
{
    ui->timerListWidget->clear(); // Clear the list view first

    // Add the loaded timers to the list view
    for (int i = 0; i < durations.size(); ++i) {
        int minutes = durations[i] / 60000;
        int seconds = (durations[i] % 60000) / 1000;
        QString timerText = QString::number(minutes).rightJustified(2, '0') + ":" + QString::number(seconds).rightJustified(2, '0');
        ui->timerListWidget->addItem("Timer " + QString::number(i + 1) + ": " + timerText);
    }
}
// Function to load preset timers from the database
void SportTimer::loadPresetTimersFromDatabase()
{
    QSqlQuery query;
    if (!query.exec("SELECT duration FROM presets")) {
        qDebug() << "Error fetching presets:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int duration = query.value(0).toInt();

        this->addTimer(duration);
    }updateTimerListView();
}
// Function to load preset timers from the database
void SportTimer::loadPresetTimers()
{
    initializeDatabase(); // Initialize the database
    loadPresetTimersFromDatabase();

}
// Function to save the current timers as a preset in the database
void savePresetTimersToDatabase(const QList<int> &durations)
{
    QSqlQuery query;
    for (int duration : durations) {
        if (!query.prepare("INSERT INTO presets (duration) VALUES (:duration)")) {
            qDebug() << "Error preparing query:" << query.lastError().text();
            return;
        }
        query.bindValue(":duration", duration);
        if (!query.exec()) {
            qDebug() << "Error executing query:" << query.lastError().text();
            return;
        }
    }
}

SportTimer::~SportTimer()
{
    delete ui;
}



// Function to save the current timers as a preset in the database
void SportTimer::savePresetTimers()
{
    initializeDatabase(); // Initialize the database

    QSqlQuery query;
    for (int duration : durations) {
        if (!query.prepare("INSERT INTO presets (duration) VALUES (:duration)")) {
            qDebug() << "Error preparing query:" << query.lastError().text();
            return;
        }
        query.bindValue(":duration", duration);
        if (!query.exec()) {
            qDebug() << "Error executing query:" << query.lastError().text();
            return;
        }
    }
}


void SportTimer::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) { // Check if Ctrl key is pressed
        int delta = event->angleDelta().y();
        int fontSizeIncrement = 1; // Set the font size increment
        if (delta > 0) {
            // Increase font size
            QFont font = QApplication::font();
            font.setPointSize(font.pointSize() + fontSizeIncrement);
            QApplication::setFont(font);
        } else if (delta < 0) {
            // Decrease font size
            QFont font = QApplication::font();
            font.setPointSize(font.pointSize() - fontSizeIncrement);
            QApplication::setFont(font);
        }
    } else {
        // Pass the event to the base class to handle regular scrolling
        QWidget::wheelEvent(event);
    }
}
void SportTimer::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timers[timerIndex]->timerId()) {
        durations[timerIndex] -= 1000;
        if (durations[timerIndex] <= 0) {
            durations[timerIndex] = 0;
            timers[timerIndex]->stop();
            delete timers[timerIndex];
            timers.removeAt(timerIndex);
            durations.removeAt(timerIndex);
            delete ui->timerListWidget->takeItem(timerIndex);
            if (timerIndex < timers.size()) {
                timers[timerIndex]->start(1000, this);
            }
        } else {
            updateTimerText(timerIndex);
        }
    }
}

void SportTimer::addTimerConnect()
{
    int minutes = ui->minutesSpinBox->value();
    int seconds = ui->secondsSpinBox->value();
    int duration = (minutes * 60 + seconds) * 1000;
    durations.append(duration);

    QString timerText = QString::number(minutes).rightJustified(2, '0') + ":" + QString::number(seconds).rightJustified(2, '0');
    ui->timerListWidget->addItem("Timer " + QString::number(durations.size()) + ": " + timerText);

    timers.append(new QBasicTimer());
}

void SportTimer::startTimers()
{
    for (int i = 0; i < durations.size(); ++i) {
        if (!timers[i]->isActive()) {
            timers[i]->start(1000, this);
            timerIndex = i;
            updateTimerText(i);
            break;
        }
    }
}

void SportTimer::updateTimerText(int index)
{
    int remainingTime = durations[index];
    int minutes = remainingTime / 60000;
    int seconds = (remainingTime % 60000) / 1000;
    QString timerText = QString::number(minutes).rightJustified(2, '0') + ":" + QString::number(seconds).rightJustified(2, '0');

    QListWidgetItem *item = ui->timerListWidget->item(index);
    item->setText("Timer " + QString::number(index + 1) + ": " + timerText);

    if (remainingTime <= 5000) {
        if (remainingTime % 1000 == 0) {
            if (item->foreground() == Qt::red)
                item->setForeground(Qt::black);
            else
                item->setForeground(Qt::red);
        }
    } else {
        item->setForeground(Qt::black);
    }
}


