#ifndef SPORTTIMER_H
#define SPORTTIMER_H

#include <QWidget>
#include <QBasicTimer>
#include <QListWidget>
#include <QMouseEvent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

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
    void loadPresetTimers();
    void savePresetTimers();

private:
    Ui::SportTimer *ui;
    QList<int> durations;
    QList<QBasicTimer*> timers;
    int timerIndex;
    void loadPresetTimersFromDatabase();
    void updateTimerText(int index);
    void addTimer(int duration);
    void updateTimerListView();
    QRegularExpression fontSizeRegex();
};

#endif // SPORTTIMER_H
