#include "../includes/Cluster.h"

int main(int argc, char* argv[])
{
    QApplication        app(argc, argv);


    QWidget *mainWindow = new QWidget;
    QWidget *mainPage = new QWidget;
    QWidget *secondPage = new QWidget;


    QStackedWidget *stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(mainPage);
    stackedWidget->addWidget(secondPage);


    TopBar*             topBar = new TopBar(mainPage);
    Blinkers*           leftB = new Blinkers(mainPage, "left", "off");
    Blinkers*           rightB = new Blinkers(mainPage, "right", "off");
    SpeedometerWidget*  speedometer = new SpeedometerWidget(mainPage);
    FanSpeedWidget*     fanspeed = new FanSpeedWidget(mainPage);
    CPUTempWidget*      cputemp = new CPUTempWidget(mainPage);
    //BatteryWidget*      battery2 = new BatteryWidget(mainPage);
    ArrowSymbolWidget*  arrowSymbol = new ArrowSymbolWidget(mainPage, "zero");
    CameraWidget        *cameraWidget = new CameraWidget(secondPage);


    EventManager        eventManager(arrowSymbol, speedometer1, leftB, rightB, stackedWidget);

    app.installEventFilter(&eventManager);


    QVBoxLayout*        firstLayout = new QVBoxLayout();
    QVBoxLayout*        secondLayout = new QVBoxLayout();
    QVBoxLayout*        mainPageLayout = new QVBoxLayout();
    QHBoxLayout*        mainContentLayout1 = new QHBoxLayout();
    QHBoxLayout*        mainContentLayout2 = new QHBoxLayout();
    QHBoxLayout*        smallerLayout = new QHBoxLayout();

    arrowSymbol->setFixedSize(400, 400);

    speedometer1->resize(900, 900);
    speedometer1->setFixedSize(400, 400);
    battery2->resize(900, 900);
    battery2->setFixedSize(400, 400);
    cameraWidget->resize(1920, 1080);
    cameraWidget->setFixedSize(700,500);

    mainPageLayout->addWidget(topBar, 0, Qt::AlignCenter);
    mainPageLayout->addWidget(fanspeed, 0);

    // Adding objects to the second page
    // secondPageLayout->addWidget(battery2, 1);
    // secondPage->setLayout(secondPageLayout);

    mainContentLayout1->addWidget(leftB, 1);
    mainContentLayout1->addWidget(speedometer1, 1);
    mainContentLayout1->addWidget(arrowSymbol, 1);
    mainContentLayout1->addWidget(battery2, 1);
    mainContentLayout1->addWidget(rightB, 1);
    mainPageLayout->addLayout(mainContentLayout1, 1);

    mainPage->setLayout(mainPageLayout);

    mainContentLayout2->addWidget(leftB, 1);
    mainContentLayout2->addWidget(cameraWidget, 1);
    mainContentLayout2->addWidget(rightB, 1);
    secondLayout->addLayout(mainContentLayout2, 1);
    secondPage->setLayout(secondLayout);
    QToolBar*   toolBar = new QToolBar;
    QAction*    page1Action = toolBar->addAction("Main");
    QAction*    page2Action = toolBar->addAction("Second");

    QObject::connect(page1Action, &QAction::triggered, [&]() {
        stackedWidget->setCurrentIndex(0);
    });
    QObject::connect(page2Action, &QAction::triggered, [&]() {
        stackedWidget->setCurrentIndex(1);
    });

    firstLayout->addWidget(toolBar);
    firstLayout->addWidget(stackedWidget);

    firstLayout->addWidget(topBar, 0, Qt::AlignCenter);

    fanspeed->setFixedSize(150,150);
    cputemp->setFixedSize(150,150);

    smallerLayout->addWidget(fanspeed, 0);
    smallerLayout->addWidget(cputemp, 0);
    smallerLayout->setSpacing(0);
    firstLayout->addLayout(smallerLayout, 0);
    firstLayout->setAlignment(smallerLayout, Qt::AlignTop | Qt::AlignLeft);

    // Set layout for the main window
    mainWindow->setLayout(firstLayout);
    mainWindow->resize(1468, 768);
    mainWindow->setStyleSheet("background-color: #0D2126");
    mainWindow->grabGesture(Qt::SwipeGesture);
    mainWindow->show();

    QTimer::singleShot(2000, [&]() { topBar->setImageState(0, true); });
    QTimer::singleShot(3000, [&]() { topBar->setImageState(1, true); });
    QTimer::singleShot(4000, [&]() { topBar->setImageState(2, true); });
    QTimer::singleShot(6000, [&]() { topBar->setImageState(4, true); });
    QTimer::singleShot(7000, [&]() { topBar->setImageState(3, true); });
    QTimer::singleShot(8000, [&]() { topBar->setImageState(5, true); });

    return app.exec();
}
