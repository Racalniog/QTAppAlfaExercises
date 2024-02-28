#include "sporttimer.h"
#include "ui_sporttimer.h"
//TODO add volume controls, add animations, multiplatform support
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
    connect(ui->deletePresetComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SportTimer::deletePreset);

    ui->minutesSpinBox->setMinimum(0);
    ui->minutesSpinBox->setMaximum(59);
    ui->secondsSpinBox->setMinimum(0);
    ui->secondsSpinBox->setMaximum(59);

    QFile file2(":/Adaptic.qss");
    if (file2.open(QIODevice::ReadOnly)) {
        stylesheet = QTextStream(&file2).readAll();
        setStyleSheet(stylesheet + QString("\n* { font-size: %1pt; }").arg(16));
        file2.close();
    } else {
        qDebug() << "Error: Could not open stylesheet file";
        qDebug() << "File path: " << file2.fileName();
        qDebug() << "Error string: " << file2.errorString();
    }

    soundEffect.setSource(QUrl::fromLocalFile(":/mixkit-arcade-bonus-alert-767.wav"));
    soundEffect.setVolume(0.25);
    this->setMinimumHeight(600);
}

void SportTimer::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    stopPainting = true;
}

void SportTimer::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    int middleX = ui->timerListWidget->width();
    int middleY = painter.device()->height() - ui->timerListWidget->height();
    if(!timers.empty() && !stopPainting){
        if(timers.first()->isActive()){
            if(!firstPicture && !secondPicture){
                painter.drawPixmap(middleX, middleY,
                                   QPixmap(":/stick-man-dancing-svgrepo-com.svg").
                                   scaled(ui->timerListWidget->width(), ui->timerListWidget->height()));
                firstPicture = true;
                secondPicture = false;
                thirdPicture = false;
            }else if(firstPicture && !secondPicture){
                painter.drawPixmap(middleX, middleY,
                                   QPixmap(":/stick-man-excercising-svgrepo-com.svg").
                                   scaled(ui->timerListWidget->width(), ui->timerListWidget->height()));
                firstPicture = false;
                secondPicture = true;
                thirdPicture = false;
            }else{
                painter.drawPixmap(middleX, middleY,
                                   QPixmap(":/stick-man-running-svgrepo-com.svg").
                                   scaled(ui->timerListWidget->width(), ui->timerListWidget->height()));
                firstPicture = false;
                secondPicture = false;
                thirdPicture = true;
            }
        }
    }
    stopPainting = false;
}

void SportTimer::animate(){
    update();
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
            durationWithExercise.remove(durationWithExercise.keys().at(index));
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
        if (i < timers.size() && !timers[i]->isActive()) {
            timers[i]->start(1000, this);
            timerIndex = i;
            updateTimerText();
            break;
        }
    }
}

/**
 * @brief Updates the text of the timer list item with the remaining duration.
 *
 * @param index The index of the timer in the durations list.
 */
void SportTimer::updateTimerText()
{
    animate();
    int remainingTime = durationWithExercise.first().first;
    int minutes = remainingTime / 60000;
    int seconds = (remainingTime % 60000) / 1000;
    QString timerText = QString::number(minutes).rightJustified(2, '0')
                        + ":" + QString::number(seconds).rightJustified(2, '0');

    QListWidgetItem *item = ui->timerListWidget->item(0);
    item->setText(QString::number(durationWithExercise.keys().first()+1) +
                  ". " + durationWithExercise.first().second + " "
                  + ": " + timerText);
    if (remainingTime <= 5000) {
        if (remainingTime % 1000 == 0) {
            if (item->foreground() == Qt::red)
                item->setForeground(Qt::black);
            else{
                soundEffect.play();
                item->setForeground(Qt::red);
            }
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
    if( timers.size()>0){
        if (event->timerId() == timers[timerIndex]->timerId()) {
            // Check if timerIndex is valid
            if (timerIndex >= 0 && 0 < timers.size()) {
                int currentDuration = durationWithExercise.first().first;

                currentDuration -= 1000;

                if (currentDuration <= 0) {
                    // Stop and remove the timer
                    timers[timerIndex]->stop();
                    delete timers[timerIndex];
                    timers.removeAt(timerIndex);
                    delete ui->timerListWidget->takeItem(timerIndex);

                    durationWithExercise.remove(durationWithExercise.keys().first());

                    // Start the next timer if available
                    if (timerIndex < timers.size()) {
                        timers[timerIndex]->start(1000, this);
                    }
                } else {
                    durationWithExercise.first().first = currentDuration;
                    updateTimerText();
                }
            } else {
                qDebug() << "Invalid timerIndex:" << timerIndex;
            }
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
        stopPainting = true;
        int delta = event->angleDelta().y();
        int fontSizeIncrement = 1;
        QFont font = this->font();
        int newPointSize = font.pointSize() + (delta > 0 ? fontSizeIncrement : -fontSizeIncrement);
        if (newPointSize > 0) {
            font.setPointSize(newPointSize);
            this->setFont(font);
            // Combine stylesheet with new font-size rule
            QString newStyleSheet = stylesheet + QString("\n* { font-size: %1pt; }").arg(newPointSize);
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
