#ifndef SPORTTIMER_H
#define SPORTTIMER_H

#include <QWidget>
#include <QBasicTimer>
#include <QListWidget>
#include <QMouseEvent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QOverload>
#include <QtMultimedia>

QT_BEGIN_NAMESPACE
namespace Ui { class SportTimer; }
QT_END_NAMESPACE

class SportTimer : public QWidget
{
    Q_OBJECT

public:
    SportTimer(QWidget *parent = nullptr);
    ~SportTimer();

protected:
    void timerEvent(QTimerEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void addTimerConnect();
    void startTimers();
    void pauseTimers();
    void savePresetTimers();
    void loadSelectedPreset();
    void onPresetNameChanged(const QString& text);
    void removeSelectedTimers();
    void removeTimers();
    void deletePreset();
    void adjustVolume(int value);

private:
    Ui::SportTimer *ui = {};
    QString stylesheet;
    QMap<int, QPair<int, QString>> durationWithExercise = {};
    QList<QBasicTimer*> timers = {};
    QRegularExpression fontSizeRegex();
    QSoundEffect soundEffect;
    bool firstPicture = false;
    bool secondPicture = false;
    bool thirdPicture = false;
    bool stopPainting = false;
    int timerIndex = {};

    void loadPresetTimersFromDatabase();
    void updateTimerText();
    void updateTimerListView();
    void savePresetToDatabase(const QString &presetName, const QMap<int, QPair<int, QString>> durationWithExercise);
    void loadPresetsFromDatabase();
    void initializeDatabase();
    void setup();
    void loadPresetTimersByName(QString trainingName);
    void animate();
};

#endif // SPORTTIMER_H
