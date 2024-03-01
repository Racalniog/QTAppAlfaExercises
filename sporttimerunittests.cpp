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

        // Initially, there should be no timers
        QCOMPARE(sportTimer.timers.size(), 0);

        sportTimer.addTimerConnect();

        QCOMPARE(sportTimer.timers.size(), 1);

        // Check if the timer is correctly added and initialized
        QVERIFY(!sportTimer.timers.first()->isActive());

        sportTimer.addTimerConnect();

        QCOMPARE(sportTimer.timers.size(), 2);

        // Check if the second timer is correctly added and initialized
        QVERIFY(!sportTimer.timers.last()->isActive());
    }

    void testStartTimers()
    {
        SportTimer sportTimer;

        sportTimer.addTimerConnect();
        sportTimer.addTimerConnect();
        sportTimer.addTimerConnect();

        sportTimer.startTimers();

        QVERIFY(sportTimer.timers.first()->isActive());

        for (int i = 1; i < sportTimer.timers.size(); ++i) {
            QVERIFY(!sportTimer.timers[i]->isActive());
        }
    }
};

QTEST_MAIN(SportTimerUnitTests)
#include "sporttimerunittests.moc"

// choose the SportTimerUnitTests class in the Qt Creator to run
