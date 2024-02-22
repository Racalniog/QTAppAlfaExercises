#include "sporttimer.h"
#include "ui_sporttimer.h"

SportTimer::SportTimer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SportTimer)
{
    ui->setupUi(this);
    setup();
    initializeDatabase();
    loadPresetsFromDatabase();
}
/**
 * @brief Setup function to initialize UI elements and connect signals to slots.
 */
void SportTimer::setup(){
    setMouseTracking(true);
    connect(ui->addTimerButton, &QPushButton::clicked, this, &SportTimer::addTimerConnect);
    connect(ui->startButton, &QPushButton::clicked, this, &SportTimer::startTimers);
    connect(ui->pauseButton, &QPushButton::clicked, this, &SportTimer::pauseTimers);
    connect(ui->savePresetButton, &QPushButton::clicked, this, &SportTimer::savePresetTimers);
    connect(ui->presetsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SportTimer::loadSelectedPreset);
    connect(ui->presetSaveLineEdit, &QLineEdit::textChanged, this, &SportTimer::onPresetNameChanged);
    connect(ui->removeTimersButton, &QPushButton::clicked, this, &SportTimer::removeTimers);

    ui->minutesSpinBox->setMinimum(0);
    ui->minutesSpinBox->setMaximum(59);
    ui->secondsSpinBox->setMinimum(0);
    ui->secondsSpinBox->setMaximum(59);

    QFile file2(":/Adaptic.qss");

    if (file2.open(QIODevice::ReadOnly)) {
        QString stylesheet = QTextStream(&file2).readAll();
        setStyleSheet(stylesheet);
        file2.close();
    } else {
        qDebug() << "Error: Could not open stylesheet file";
        qDebug() << "File path: " << file2.fileName();
        qDebug() << "Error string: " << file2.errorString();
    }
}
/**
 * @brief Removes all timers and clears the timer list widget
 * with the removeTimersButton
 */
void SportTimer::removeTimers()
{
    durations.clear();

    for (QBasicTimer *timer : timers) {
        timer->stop();
        delete timer;
    }
    timers.clear();

    ui->timerListWidget->clear();
}
//TODO add button for this selectiv removable
/**
 * @brief Removes selected timer from the list widget and durations list.
 */
void SportTimer::removeSelectedTimers() {
    QList<QListWidgetItem*> selectedItems = ui->timerListWidget->selectedItems();

    for (QListWidgetItem *item : selectedItems) {
        int index = ui->timerListWidget->row(item);
        delete item;

        if (index >= 0 && index < durations.size()) {
            durations.removeAt(index);

            // Adjust the timer index if necessary
            if (index == timerIndex) {
                if (timers[index]->isActive()) {
                    timers[index]->stop();
                }

                if (timerIndex >= durations.size()) {
                    timerIndex = durations.size() - 1;
                }
            } else if (index < timerIndex) {
                --timerIndex;
            }
        }
    }

    updateTimerListView();
}
/**
 * @brief Slot invoked when the preset name is changed in
 * the text field and saved to the db with enter.
 *
 * @param text The new text in the preset name line edit.
 */
void SportTimer::onPresetNameChanged(const QString& text) {
    if (text.isEmpty()) {
        return;
    }

    // Check if the user has pressed Enter or Return
    if (text.endsWith("\n")) {
        // Remove the newline character from the text
        QString presetName = text.trimmed();

        // Check if the preset already exists in the database
        QSqlQuery query;
        query.prepare("SELECT id FROM presets WHERE name = :name");
        query.bindValue(":name", presetName);
        if (query.exec() && query.next()) {
            qDebug() << "Preset" << presetName << "already exists.";
            return;
        }

        savePresetToDatabase(presetName);

        ui->presetSaveLineEdit->clear();
    }
}
/**
 * @brief Saves the preset with the given name of the presetSaveLineEdit ui
 * element to the database.
 */
void SportTimer::savePresetToDatabase(const QString& presetName) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        qDebug() << "Database connection is invalid.";
        return;
    }

    if (!db.transaction()) {
        qDebug() << "Transaction failed to start:" << db.lastError().text();
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO presets (name, duration) VALUES (:name, :duration)");

    for (int duration : durations) {
        query.bindValue(":name", presetName);
        query.bindValue(":duration", duration);

        if (!query.exec()) {
            qDebug() << "Error executing query:" << query.lastError().text();
            db.rollback();
            return;
        }
    }

    if (!db.commit()) {
        qDebug() << "Transaction failed to commit:" << db.lastError().text();
        return;
    }

    // optionall message
    qDebug() << "Preset" << presetName << "saved to database.";

    loadPresetsFromDatabase();
}

/**
 * @brief Loads presets from the database and populates the presets combo box.
 */
void SportTimer::loadPresetsFromDatabase()
{
    QSqlQuery query;
    if (!query.exec("SELECT id, name FROM presets")) {
        qDebug() << "Error fetching presets:" << query.lastError().text();
        return;
    }

    ui->presetsComboBox->clear();

    while (query.next()) {
        int presetId = query.value(0).toInt();
        QString presetName = query.value(1).toString();

        bool isLoaded = false;
        for (int i = 0; i < ui->presetsComboBox->count(); ++i) {
            if (ui->presetsComboBox->itemText(i) == presetName) {
                isLoaded = true;
                break;
            }
        }
        if (!isLoaded) {
            ui->presetsComboBox->addItem(presetName, presetId);
        }
    }
}

/**
 * @brief Loads the timers associated with a selected preset from the combo
 * box drop down menu.
 *
 * @param index The index of the selected preset in the combo box.
 */
void SportTimer::loadSelectedPreset(int index)
{
    int presetId = ui->presetsComboBox->currentData().toInt();
    loadPresetTimersById(presetId);

    // Clear the selection and show the placeholder text again
    ui->presetsComboBox->setCurrentIndex(-1);
    ui->presetsComboBox->setPlaceholderText("Load Selected Preset");
}
/**
 * @brief Loads the timers associated with a selected preset from the database.
 *
 * @param presetId The ID of the selected preset.
 */
void SportTimer::loadPresetTimersById(int presetId)
{
    QSqlQuery query;
    query.prepare("SELECT duration FROM presets WHERE id = :id");
    query.bindValue(":id", presetId);
    if (!query.exec()) {
        qDebug() << "Error fetching preset timers:" << query.lastError().text();
        return;
    }

    durations.clear();
    while (query.next()) {
        int duration = query.value(0).toInt();
        durations.append(duration);
    }

    updateTimerListView();
}

void SportTimer::pauseTimers()
{
    for (QBasicTimer *timer : timers) {
        timer->stop();
    }
}

void SportTimer::initializeDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("presets.db");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS presets (id INTEGER PRIMARY KEY, name TEXT, duration INTEGER)")) {
        qDebug() << "Error creating table:" << query.lastError().text();
        return;
    }
}

void SportTimer::updateTimerListView()
{
    ui->timerListWidget->clear();

    for (int i = 0; i < durations.size(); ++i) {
        int minutes = durations[i] / 60000;
        int seconds = (durations[i] % 60000) / 1000;
        QString timerText = QString::number(minutes).rightJustified(2, '0') + ":" + QString::number(seconds).rightJustified(2, '0');
        ui->timerListWidget->addItem("Timer " + QString::number(i + 1) + ": " + timerText);
    }
}

SportTimer::~SportTimer()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.close();
    delete ui;
}

/**
 * @brief Saves the preset timers to the database with
 * savePresetButton.
 *
 * @note This function assumes that the preset name
 * is provided via the presetSaveLineEdit widget.
 */
void SportTimer::savePresetTimers()
{
    QString presetName = ui->presetSaveLineEdit->text().trimmed();

    if (presetName.isEmpty()) {
        qDebug() << "Preset name is empty.";
        return;
    }

    savePresetToDatabase(presetName);

    ui->presetSaveLineEdit->clear();

    loadPresetsFromDatabase();
}
/**
 * @brief Handles the wheel event for the widget to
 * resize the font sizes.
 * TODO fix with qss
 * @param event The wheel event object.
 */
void SportTimer::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) { // Check if Ctrl key is pressed
        int delta = event->angleDelta().y();
        int fontSizeIncrement = 1;
        QFont font = this->font();
        int newPointSize = font.pointSize() + (delta > 0 ? fontSizeIncrement : -fontSizeIncrement);
        if (newPointSize > 0) {
            font.setPointSize(newPointSize);
            this->setFont(font);
        }
    } else {
        QWidget::wheelEvent(event);
    }
}

/**
 * @brief Handles timer events for the widget.
 *
 * This function is responsible for updating the remaining durations of active timers.
 *
 * @param event The timer event object.
 */
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
/**
 * @brief Connects the signals and slots for various UI elements.
 *
 * This function sets up the connections between buttons and their respective slots.
 */
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
        qDebug() << "Starting timer" << i;
        qDebug() << "timers.size()" << timers.size();
        if (i < timers.size() && !timers[i]->isActive()) {
            qDebug() << "timers[i]->isActive()" << timers[i]->isActive();
            qDebug() << "Starting timer" << i;
            timers[i]->start(1000, this);
            timerIndex = i;
            updateTimerText(i);
            break;
        }
    }
}

/**
 * @brief Updates the text of the timer list item with the remaining duration.
 *
 * @param index The index of the timer in the durations list.
 */
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


