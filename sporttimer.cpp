#include "sporttimer.h"
#include "ui_sporttimer.h"
//TODO fix memory leak from qss reload in wheelEvent function
//TODO fix timer display bug
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

void SportTimer::pauseTimers()
{
    for (QBasicTimer *timer : timers) {
        timer->stop();
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
    ui->timerListWidget->addItem(QString::number(durationWithExercise.size()) +
                                 ". " + exerciseName + " "
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
    int remainingTime = durationWithExercise.value(index).first;
    int minutes = remainingTime / 60000;
    int seconds = (remainingTime % 60000) / 1000;
    QString timerText = QString::number(minutes).rightJustified(2, '0')
                        + ":" + QString::number(seconds).rightJustified(2, '0');

    QListWidgetItem *item = ui->timerListWidget->item(index);
    item->setText(QString::number(durationWithExercise.keys().at(index)) +
                  ". " + durationWithExercise.value(index).second + " "
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

SportTimer::~SportTimer()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.close();
    delete ui;
}
