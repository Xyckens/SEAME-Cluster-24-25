#include <QCoreApplication>
#include <QtTest>
#include "../../include/Blinkers.h"
#include <QTimer>


class MockBlinkers : public Blinkers {};

class BlinkerTest : public QObject
{
    Q_OBJECT

public:
    BlinkerTest();
    ~BlinkerTest();

private slots:
    void testTurnOnLeftBlinkerStartsBlinking();
    void testTurnOnRightBlinkerStartsBlinking();
    void testTurnOffLeftBlinkerStopsBlinking();
    void testTurnOffRightBlinkerStopsBlinking();
    void testTimerStartsWhenBlinkerIsTurnedOn();
    void testTimerStopsWhenBlinkerIsTurnedOff();
    void testImageUpdatesWhenBlinkerIsTurnedOff();
};

BlinkerTest::BlinkerTest() {}

BlinkerTest::~BlinkerTest() {}

void BlinkerTest::testTurnOnLeftBlinkerStartsBlinking()
{
    auto left_blinker = new MockBlinkers();
    auto right_blinker = new MockBlinkers();

    left_blinker->turnOnBlinkers(1);

    QVERIFY(left_blinker->get_blinking());
}

void BlinkerTest::testTurnOnRightBlinkerStartsBlinking()
{
    auto left_blinker = new MockBlinkers();
    auto right_blinker = new MockBlinkers();

    right_blinker->turnOnBlinkers(1);

    QVERIFY(right_blinker->get_blinking());
}

void BlinkerTest::testTurnOffLeftBlinkerStopsBlinking()
{
    auto left_blinker = new MockBlinkers();
    auto right_blinker = new MockBlinkers();

    left_blinker->turnOnBlinkers(1);
    left_blinker->turnOnBlinkers(0);

    QVERIFY(!left_blinker->get_blinking());
}

void BlinkerTest::testTurnOffRightBlinkerStopsBlinking()
{
    auto left_blinker = new MockBlinkers();
    auto right_blinker = new MockBlinkers();

    right_blinker->turnOnBlinkers(1);
    right_blinker->turnOnBlinkers(0);

    QVERIFY(!right_blinker->get_blinking());
}

void BlinkerTest::testTimerStartsWhenBlinkerIsTurnedOn()
{
    Blinkers leftBlinker;

    leftBlinker.turnOnBlinkers(1);

    QVERIFY(leftBlinker.getToggleTimer() != nullptr);
    QVERIFY(leftBlinker.getToggleTimer()->isActive());
}

void BlinkerTest::testTimerStopsWhenBlinkerIsTurnedOff()
{
    Blinkers leftBlinker;

    leftBlinker.turnOnBlinkers(1);
    leftBlinker.turnOnBlinkers(0);

    QVERIFY(leftBlinker.getToggleTimer() != nullptr);
    QVERIFY(!leftBlinker.getToggleTimer()->isActive());
}


void BlinkerTest::testImageUpdatesWhenBlinkerIsTurnedOff()
{
    Blinkers leftBlinker;

    leftBlinker.turnOnBlinkers(1);
    leftBlinker.turnOnBlinkers(0);

    QVERIFY(!leftBlinker.getIsImage1Visible());
}

QTEST_MAIN(BlinkerTest)

#include "tst_blinkertest.moc"
