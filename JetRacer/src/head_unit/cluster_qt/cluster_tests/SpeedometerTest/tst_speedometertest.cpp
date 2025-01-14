#include <QCoreApplication>
#include <QtTest>
#include "../../include/SpeedometerWidget.h"


class MockSpeedometerWidget : public SpeedometerWidget{};

class SpeedometerWidgetTest : public QObject
{
    Q_OBJECT

public:
    SpeedometerWidgetTest();
    ~SpeedometerWidgetTest();

private slots:
    void test_accelerateIncreasesSpeedByTwo();
    void test_accelerateDecreasesSpeedByTwo();
    void test_accelerateSpeedMaxLimit();
    void test_accelerateSpeedMinLimit();
    void test_speedStartsAtZero();
};

SpeedometerWidgetTest::SpeedometerWidgetTest() {}

SpeedometerWidgetTest::~SpeedometerWidgetTest() {}

void SpeedometerWidgetTest::test_accelerateIncreasesSpeedByTwo()
{
    MockSpeedometerWidget   speedometer;

    speedometer.accelerate(Qt::Key_Space);
    QCOMPARE(speedometer.getCurrentSpeed(), 2);
}

void SpeedometerWidgetTest::test_accelerateDecreasesSpeedByTwo()
{
    MockSpeedometerWidget   speedometer;

    speedometer.accelerate(Qt::Key_Space);
    speedometer.accelerate(Qt::Key_Space);
    speedometer.accelerate(Qt::Key_Space);

    speedometer.accelerate(Qt::Key_Down);

    QCOMPARE(speedometer.getCurrentSpeed(), 4);
}

void SpeedometerWidgetTest::test_accelerateSpeedMaxLimit()
{
    MockSpeedometerWidget   speedometer;

    for (int speed = 0; speed < 90; speed++)
        speedometer.accelerate(Qt::Key_Space);

    QCOMPARE(speedometer.getCurrentSpeed(), 160);
}

void SpeedometerWidgetTest::test_accelerateSpeedMinLimit()
{
    MockSpeedometerWidget   speedometer;

    for (int speed = 0; speed < 10; speed++)
        speedometer.accelerate(Qt::Key_Down);

    QCOMPARE(speedometer.getCurrentSpeed(), 0);
}

void SpeedometerWidgetTest::test_speedStartsAtZero()
{
    MockSpeedometerWidget   speedometer;

    QCOMPARE(speedometer.getCurrentSpeed(), 0);
}

QTEST_MAIN(SpeedometerWidgetTest)

#include "tst_speedometertest.moc"
