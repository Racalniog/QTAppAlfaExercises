#include "sporttimer.h"
#include "ui_sporttimer.h"

//cpp file for structure to reduce sporttimer.cpp length

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

    QMap<int, QString> loadedPresets;

    while (query.next()) {
        int presetId = query.value(0).toInt();
        QString presetName = query.value(1).toString();

        // Check if the preset (both ID and name) is already loaded in the combo box
        if (!loadedPresets.contains(presetId) && !loadedPresets.values().contains(presetName)) {
            ui->presetsComboBox->addItem(presetName, presetId);
            loadedPresets.insert(presetId, presetName);
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

    int count = {};
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
    }
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
    loadPresetsFromDatabase();
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
        QString presetName = text.trimmed();

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
