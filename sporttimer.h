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

private slots:
    void addTimerConnect();
    void startTimers();
    void pauseTimers();
    void savePresetTimers();
    void loadSelectedPreset();
    void onPresetNameChanged(const QString& text);
    void removeSelectedTimers();
    void removeTimers();

private:
    Ui::SportTimer *ui = {};
    QString stylesheet;
    QMap<int, QPair<int, QString>> durationWithExercise = {};
    QList<QBasicTimer*> timers = {};
    QRegularExpression fontSizeRegex();
    QSoundEffect soundEffect;
    int timerIndex = {};

    void loadPresetTimersFromDatabase();
    void updateTimerText();
    void updateTimerListView();
    void savePresetToDatabase(const QString &presetName, const QMap<int, QPair<int, QString>> durationWithExercise);
    void loadPresetsFromDatabase();
    void initializeDatabase();
    void setup();
    void loadPresetTimersByName(QString trainingName);
};

#endif // SPORTTIMER_H
