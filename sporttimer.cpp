#include "sporttimer.h"
#include "ui_sporttimer.h"
//TODO fix memory leak from qss reload in wheelEvent function
SportTimer::SportTimer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SportTimer)
{
    ui->setupUi(this);
    setup();
    initializeDatabase();
    loadPresetsFromDatabase();
}

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
    connect(ui->removeSelectedTimerButton, &QPushButton::clicked, this, &SportTimer::removeSelectedTimers);

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
    durationWithExercise.clear();

    for (QBasicTimer *timer : timers) {
        timer->stop();
        delete timer;
    }
    timers.clear();

    ui->timerListWidget->clear();
}

/**
 * @brief Removes selected timer from the list widget and durations list.
 */
void SportTimer::removeSelectedTimers() {
    QList<QListWidgetItem*> selectedItems = ui->timerListWidget->selectedItems();

    for (QListWidgetItem *item : selectedItems) {
        int index = ui->timerListWidget->row(item);
        delete item;

        if (index >= 0 && index < durationWithExercise.size()) {
            durationWithExercise.remove(index);
            timers.remove(index);
        }
    }
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

        savePresetToDatabase(presetName, durationWithExercise);

        ui->presetSaveLineEdit->clear();
    }
}

/**
 * @brief Saves the preset with the given name of the presetSaveLineEdit ui
 * element to the database.
 */
void SportTimer::savePresetToDatabase(const QString& presetName, const QMap<int, QPair<int, QString>> durationWithExercise) {
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
    query.prepare("INSERT INTO presets (id, training_name, duration, exercise_name) VALUES (:id, :training_name, :duration, :exercise_name)");
    QMap<int, QPair<int, QString>>::const_iterator it;
    for (it = durationWithExercise.constBegin(); it != durationWithExercise.constEnd(); ++it) {
        query.bindValue(":id", it.key());
        query.bindValue(":training_name", presetName);
        query.bindValue(":duration", it.value().first);
        query.bindValue(":exercise_name", it.value().second);

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

    qDebug() << "Preset" << presetName << "saved to database.";

    loadPresetsFromDatabase();
}


/**
 * @brief Loads presets from the database and populates the presets combo box.
 */
void SportTimer::loadPresetsFromDatabase()
{
    QSqlQuery query;
    if (!query.exec("SELECT id, training_name FROM presets")) {
        qDebug() << "Error fetching presets:" << query.lastError().text();
        return;
    }

    ui->presetsComboBox->clear();

    while (query.next()) {
        int presetId = query.value(0).toInt();
        QString presetName = query.value(1).toString();

        // Check if the preset is already loaded in the combo box
        bool isLoaded = false;
        for (int i = 0; i < ui->presetsComboBox->count(); ++i) {
            if (ui->presetsComboBox->itemData(i).toInt() == presetId) {
                isLoaded = true;
                break;
            }
        }

        // If the preset is not already loaded, add it to the combo box
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
void SportTimer::loadSelectedPreset()
{
    QString trainingName = ui->presetsComboBox->currentText();
    int presetId = ui->presetsComboBox->currentData().toInt();
    loadPresetTimersByName(trainingName);

    // Clear the selection and show the placeholder text again
    ui->presetsComboBox->setCurrentIndex(-1);
    ui->presetsComboBox->setPlaceholderText("Load Selected Preset");
}

/**
 * @brief Loads the timers associated with a selected preset from the database.
 *
 * @param presetId The ID of the selected preset.
 */
void SportTimer::loadPresetTimersByName(QString trainingName)
{
    QSqlQuery query;
    query.prepare("SELECT id, duration, exercise_name FROM presets WHERE training_name = :training_name");
    query.bindValue(":training_name", trainingName);
    if (!query.exec()) {
        qDebug() << "Error fetching preset timers:" << query.lastError().text();
        return;
    }

    //durationWithExercise.clear(); // Clear the map before loading new data

    int count = 0; // Initialize count to zero
    while (query.next()) {
        count += 1;
        int duration = query.value(1).toInt();
        QString exerciseName = query.value(2).toString();
        durationWithExercise.insert(count, qMakePair(duration, exerciseName));

        int minutes = duration / 60000;
        int seconds = (duration % 60000) / 1000;
        QString timerText = QString::number(minutes).rightJustified(2, '0') + ":" +
                            QString::number(seconds).rightJustified(2, '0');

        ui->timerListWidget->addItem("Timer " + QString::number(count) + " " + exerciseName + ": " + timerText);
        timers.append(new QBasicTimer());
        qDebug() << "Counter " << count;
    }

    qDebug() << "Durations from database:" << durationWithExercise; // Debug message to check durations list
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
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS presets ("
                               "id INTEGER, "
                               "training_name TEXT, "
                               "duration INTEGER, "
                               "exercise_name TEXT, "
                               "PRIMARY KEY (id, training_name)"
                               ");";
    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creating table:" << query.lastError().text();
        return;
    }
}


void SportTimer::updateTimerListView()
{
    //ui->timerListWidget->clear();
    QMap<int, QPair<int, QString>>::const_iterator it;
    for (it = durationWithExercise.constBegin(); it != durationWithExercise.constEnd(); ++it) {
        int minutes = it.value().first / 60000;
        int seconds = (it.value().first % 60000) / 1000;
        QString timerText = QString::number(minutes).rightJustified(2, '0') + ":"
                            + QString::number(seconds).rightJustified(2, '0');
        ui->timerListWidget->addItem("Timer " + QString::number(it.key()) + ": " + timerText);
    }
    qDebug() << "Timer list view updated."; // Debug message to verify list view update

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

    savePresetToDatabase(presetName, durationWithExercise);

    ui->presetSaveLineEdit->clear();

    loadPresetsFromDatabase();
}

/**
 * @brief Handles the wheel event for the widget to
 * resize the font sizes.
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

            QString existingStyleSheet = this->styleSheet();

            // Combine existing stylesheet with new font-size rule
            QString newStyleSheet = existingStyleSheet + QString("\n* { font-size: %1pt; }").arg(newPointSize);
            this->setStyleSheet(newStyleSheet);
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
        // Check if timerIndex is valid
        if (timerIndex >= 0 && 0 < timers.size()) {
            // Retrieve the duration for the first timer
            int currentDuration = durationWithExercise.first().first;

            // Update the current duration
            currentDuration -= 1000;

            if (currentDuration <= 0) {
                // Stop and remove the timer
                timers[timerIndex]->stop();
                delete timers[timerIndex];
                timers.removeAt(timerIndex);
                delete ui->timerListWidget->takeItem(timerIndex);

                // Remove the duration and exercise entry
                durationWithExercise.remove(timerIndex);

                // Start the next timer if available
                if (timerIndex < timers.size()) {
                    timers[timerIndex]->start(1000, this);
                }
            } else {
                // Update the duration in the map
                durationWithExercise[timerIndex].first = currentDuration;
                updateTimerText(timerIndex);
            }
        } else {
            qDebug() << "Invalid timerIndex:" << timerIndex;
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
    QString exerciseName = ui->exerciseNamelineEdit->text().trimmed();
    //TODO change durations to keep time and exercise name
    if (exerciseName.isEmpty()) {
        qDebug() << "No exercise name given. Setting to default";
        exerciseName = "exercise";
    }

    int minutes = ui->minutesSpinBox->value();
    int seconds = ui->secondsSpinBox->value();
    int duration = (minutes * 60 + seconds) * 1000;
    durationWithExercise.insert(durationWithExercise.size(), {duration, exerciseName});

    QString timerText = QString::number(minutes).rightJustified(2, '0')
                        + ":" + QString::number(seconds).rightJustified(2, '0');
    ui->timerListWidget->addItem("Timer " + QString::number(durationWithExercise.size()) +
                                 " " + exerciseName + " "
                                 + ": " + timerText);

    timers.append(new QBasicTimer());
}

void SportTimer::startTimers()
{
    for (int i = 0; i < durationWithExercise.size(); ++i) {
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
    //int index = durationWithExercise.value()
    int remainingTime = durationWithExercise.value(index).first;
    int minutes = remainingTime / 60000;
    int seconds = (remainingTime % 60000) / 1000;
    QString timerText = QString::number(minutes).rightJustified(2, '0') + ":" + QString::number(seconds).rightJustified(2, '0');

    QListWidgetItem *item = ui->timerListWidget->item(index);
    //item->setText("Timer " + QString::number(durationWithExercise.value(index).first) + ": " + timerText);
    item->setText("Timer " + QString::number(durationWithExercise.firstKey()) +
                  " " + durationWithExercise.first().second + " "
                  + ": " + timerText);
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


