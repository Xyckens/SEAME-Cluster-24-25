#include <QCoreApplication>
#include <QtTest>
#include "../../include/TopBar.h"


class MockTopBar : public TopBar{};

class TopBarTest : public QObject
{
    Q_OBJECT

public:
    TopBarTest();
    ~TopBarTest();

private slots:
    void test_setImageStateWithInvalidIndex();
    void test_setImageStateWithValidIndex();
    void test_setImageStateSetsTrueState();
    void test_setImageStateSetsFalseState();
};

TopBarTest::TopBarTest() {}

TopBarTest::~TopBarTest() {}

void TopBarTest::test_setImageStateWithInvalidIndex()
{
    MockTopBar  TopBar;

    QTest::ignoreMessage(QtWarningMsg, "Invalid index: -1");
    QVERIFY(!TopBar.setImageState(-1, true));

}

void TopBarTest::test_setImageStateWithValidIndex()
{
    MockTopBar  TopBar;

    QVERIFY(!TopBar.setImageState(1, false));
}

void TopBarTest::test_setImageStateSetsTrueState()
{
    MockTopBar  TopBar;

    QVERIFY(TopBar.setImageState(0, true));
}

void TopBarTest::test_setImageStateSetsFalseState()
{
    MockTopBar  TopBar;

    QVERIFY(!TopBar.setImageState(0, false));
}

QTEST_MAIN(TopBarTest)

#include "tst_topbartest.moc"
