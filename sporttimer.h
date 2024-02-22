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
    void loadSelectedPreset(int index);
    void onPresetNameChanged(const QString& text);
    void removeSelectedTimers();
    void removeTimers();

private:
    Ui::SportTimer *ui = {};
    QList<int> durations = {};
    QList<QBasicTimer*> timers = {};
    QRegularExpression fontSizeRegex();
    int timerIndex = {};

    void loadPresetTimersFromDatabase();
    void loadPresetTimersById(int presetId);
    void updateTimerText(int index);
    void updateTimerListView();
    void loadPresetsFromDatabase();
    void savePresetToDatabase(const QString &presetName);
    void initializeDatabase();
    void setup();
};

#endif // SPORTTIMER_H
