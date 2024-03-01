#include <QObject>
#include <QtTest>

#include "sporttimer.h"

class SportTimer;

class SportTimerUnitTests : public QObject
{
    Q_OBJECT
private:
    friend class SportTimer;

private slots:
    void testAddTimer()
    {
        SportTimer sportTimer;

        int initialTimerCount = sportTimer.durationWithExercise.size();

        QString exerciseName = "Test Exercise";
        int minutes = 5;
        int seconds = 0;
        int duration = (minutes * 60 + seconds) * 1000;
        sportTimer.durationWithExercise.insert(duration, {duration, exerciseName});

        int timerCountAfterAdd = sportTimer.durationWithExercise.size();
        qDebug() << "initialTimerCount:"<< initialTimerCount << "\ntimerCountAfterAdd:" << timerCountAfterAdd;
        QCOMPARE(timerCountAfterAdd, initialTimerCount + 1);
    }
    void testStartTimers()
    {
        SportTimer sportTimer;

        //QVERIFY(sportTimer.areTimersRunning);
    }

};

QTEST_MAIN(SportTimerUnitTests)
#include "sporttimerunittests.moc"

//testname [options] [testfunctions[:testdata]]...
//sporttimertest.exe testAddTimer
