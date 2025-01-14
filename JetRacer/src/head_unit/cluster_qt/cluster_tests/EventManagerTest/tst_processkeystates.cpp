#include <QCoreApplication>
#include <QtTest>
#include "../../include/EventManager.h"

class MockArrowSymbolWidget : public ArrowSymbolWidget {};
class MockSpeedometerWidget : public SpeedometerWidget {};
class MockBlinkers : public Blinkers {};
class MockStackedWidget : public QStackedWidget {};

class EventManagerTestProcessKeys : public QObject
{
    Q_OBJECT

public:
    EventManagerTestProcessKeys();
    ~EventManagerTestProcessKeys();

private slots:
    void    testProcessCallsLeftArrow();
    void    testProcessCallsRightArrow();
    void    testProcessCallsFowardArrow();
    void    testProcessCallsAccelerate();
    void    testProcessCallsReduceSpeed();
    void    testProcessCallsLeftBlinker();
    void    testProcessCallsRightBlinker();
};

EventManagerTestProcessKeys::EventManagerTestProcessKeys() {}

EventManagerTestProcessKeys::~EventManagerTestProcessKeys() {}

void    EventManagerTestProcessKeys::testProcessCallsLeftArrow()
{
    auto arrows = new MockArrowSymbolWidget();

    EventManager eventManager(arrows, nullptr, nullptr, nullptr, nullptr, nullptr);

    eventManager.installEventFilter(&eventManager);

    QTest::keyPress(&eventManager, Qt::Key_Left);

    eventManager.processKeyStates();

    QVERIFY(arrows->getM_greenL());
    QVERIFY(!arrows->getM_greenR());
    QVERIFY(!arrows->getM_greenV());

    QTest::keyRelease(&eventManager, Qt::Key_Left);
}

void    EventManagerTestProcessKeys::testProcessCallsRightArrow()
{
    auto arrows = new MockArrowSymbolWidget();

    EventManager eventManager(arrows, nullptr, nullptr, nullptr, nullptr, nullptr);

    eventManager.installEventFilter(&eventManager);

    QTest::keyPress(&eventManager, Qt::Key_Right);

    eventManager.processKeyStates();

    QVERIFY(!arrows->getM_greenL());
    QVERIFY(arrows->getM_greenR());
    QVERIFY(!arrows->getM_greenV());

    QTest::keyRelease(&eventManager, Qt::Key_Right);
}

void    EventManagerTestProcessKeys::testProcessCallsFowardArrow()
{
    auto arrows = new MockArrowSymbolWidget();

    EventManager eventManager(arrows, nullptr, nullptr, nullptr, nullptr, nullptr);

    eventManager.installEventFilter(&eventManager);

    QTest::keyPress(&eventManager, Qt::Key_Up);

    eventManager.processKeyStates();

    QVERIFY(!arrows->getM_greenL());
    QVERIFY(!arrows->getM_greenR());
    QVERIFY(arrows->getM_greenV());

    QTest::keyRelease(&eventManager, Qt::Key_Up);
}

void    EventManagerTestProcessKeys::testProcessCallsAccelerate()
{
    auto speedometer = new MockSpeedometerWidget();

    EventManager eventManager(nullptr, speedometer, nullptr,  nullptr, nullptr, nullptr);

    eventManager.installEventFilter(&eventManager);

    QTest::keyPress(&eventManager, Qt::Key_Space);

    eventManager.processKeyStates();

    QCOMPARE(speedometer->getCurrentSpeed(), 2);

    QTest::keyRelease(&eventManager, Qt::Key_Space);
}

void    EventManagerTestProcessKeys::testProcessCallsReduceSpeed()
{
    auto speedometer = new MockSpeedometerWidget();

    EventManager eventManager(nullptr, speedometer, nullptr, nullptr, nullptr, nullptr);

    eventManager.installEventFilter(&eventManager);

    QTest::keyPress(&eventManager, Qt::Key_Space);
    eventManager.processKeyStates();
    QTest::keyPress(&eventManager, Qt::Key_Space);
    eventManager.processKeyStates();
    QTest::keyRelease(&eventManager, Qt::Key_Space);
    QTest::keyPress(&eventManager, Qt::Key_Down);
    eventManager.processKeyStates();

    QCOMPARE(speedometer->getCurrentSpeed(), 2);
}

void    EventManagerTestProcessKeys::testProcessCallsLeftBlinker()
{
    auto left_blinker = new MockBlinkers();
    auto right_blinker = new MockBlinkers();

    EventManager eventManager(nullptr, nullptr, nullptr, left_blinker, right_blinker, nullptr);

    eventManager.installEventFilter(&eventManager);

    QTest::keyPress(&eventManager, Qt::Key_L);

    eventManager.processKeyStates();

    QVERIFY(left_blinker->get_blinking());
    QVERIFY(!right_blinker->get_blinking());

    QTest::keyPress(&eventManager, Qt::Key_L);
    eventManager.processKeyStates();

    QVERIFY(!left_blinker->get_blinking());
    QVERIFY(!right_blinker->get_blinking());
}

void    EventManagerTestProcessKeys::testProcessCallsRightBlinker()
{
    auto left_blinker = new MockBlinkers();
    auto right_blinker = new MockBlinkers();

    EventManager eventManager(nullptr, nullptr, nullptr, left_blinker, right_blinker, nullptr);

    eventManager.installEventFilter(&eventManager);

    QTest::keyPress(&eventManager, Qt::Key_R);

    eventManager.processKeyStates();

    QVERIFY(!left_blinker->get_blinking());
    QVERIFY(right_blinker->get_blinking());

    QTest::keyPress(&eventManager, Qt::Key_R);
    eventManager.processKeyStates();

    QVERIFY(!left_blinker->get_blinking());
    QVERIFY(!right_blinker->get_blinking());
}

QTEST_MAIN(EventManagerTestProcessKeys)

#include "tst_processkeystates.moc"


