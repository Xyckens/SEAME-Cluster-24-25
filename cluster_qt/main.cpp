#include "includes/Cluster.h"

int main(int argc, char* argv[])
{
    QApplication        app(argc, argv);

    // Set a main window where the pages will be included
    QWidget *mainWindow = new QWidget;

    // Set the pages
    QWidget *mainPage = new QWidget;
    QWidget *secondPage = new QWidget;

    // Set a stack of Widgets to store the pages
    QStackedWidget *stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(mainPage);
    stackedWidget->addWidget(secondPage);

    // Creating the objects for the window
    TopBar*             topBar = new TopBar(mainPage);
    Blinkers*           leftB = new Blinkers(mainPage, "left", "off");
    Blinkers*           rightB = new Blinkers(mainPage, "right", "off");
    SpeedometerWidget*  speedometer = new SpeedometerWidget(mainPage);
    FanSpeedWidget*     fanspeed = new FanSpeedWidget(mainPage);
    CPUTempWidget*      cputemp = new CPUTempWidget(mainPage);
    //BatteryWidget*      battery2 = new BatteryWidget(mainPage);
    ArrowSymbolWidget*  arrowSymbol = new ArrowSymbolWidget(mainPage, "zero");
    // CameraWidget        *cameraWidget = new CameraWidget(secondPage);
    BatteryAndSpeedWidget*      bas = new BatteryAndSpeedWidget(mainPage);

    //setCentralWidget(cameraWidget);

    // Set a manager to handle keyboard events
    EventManager        eventManager(arrowSymbol, speedometer, leftB, rightB, stackedWidget);
    app.installEventFilter(&eventManager);

    // Create layouts
    QVBoxLayout*        mainLayout = new QVBoxLayout();
    QVBoxLayout*        mainPageLayout = new QVBoxLayout(); // Main vertical layout
    QHBoxLayout*        mainContentLayout = new QHBoxLayout(); // Horizontal layout for speedometers
    QHBoxLayout*        smallerLayout = new QHBoxLayout();
    QVBoxLayout*        secondPageLayout = new QVBoxLayout(); // Main vertical layout

    arrowSymbol->setFixedSize(400, 400);

    bas->resize(900, 900);
    bas->setFixedSize(400, 400);
    //battery2->resize(900, 900);
    //battery2->setFixedSize(400, 400);


    mainPageLayout->addWidget(topBar, 0, Qt::AlignCenter); // Add the TopBar widget at the top
    mainPageLayout->addWidget(fanspeed, 0); // Add the TopBar widget at the top

    // Adding objects to the second page
    // secondPageLayout->addWidget(battery2, 1);
    // secondPage->setLayout(secondPageLayout);

    // Add the horizontal layout to the main vertical layout
    mainContentLayout->addWidget(leftB, 1);
    mainContentLayout->addWidget(bas, 1);
    mainContentLayout->addWidget(arrowSymbol, 1);
    //mainContentLayout->addWidget(battery2, 1);
    mainContentLayout->addWidget(rightB, 1);
    // Add Layout to the main Page
    mainPageLayout->addLayout(mainContentLayout, 1);
    // Set the layout for the main page
    mainPage->setLayout(mainPageLayout);

    //Add a navigation tool for pages
    QToolBar*   toolBar = new QToolBar;
    QAction*    page1Action = toolBar->addAction("Main");
    QAction*    page2Action = toolBar->addAction("Second");

    // Connect the labels to trigger the pages
    QObject::connect(page1Action, &QAction::triggered, [&]() {
        stackedWidget->setCurrentIndex(0);
    });
    QObject::connect(page2Action, &QAction::triggered, [&]() {
        stackedWidget->setCurrentIndex(1);
    });

    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(stackedWidget);

    mainLayout->addWidget(topBar, 0, Qt::AlignCenter);

    fanspeed->setFixedSize(150,150);
    cputemp->setFixedSize(150,150);

    smallerLayout->addWidget(fanspeed, 0);
    smallerLayout->addWidget(cputemp, 0);
    smallerLayout->setSpacing(0);
    mainLayout->addLayout(smallerLayout, 0);
    mainLayout->setAlignment(smallerLayout, Qt::AlignTop | Qt::AlignLeft);

    // Set layout for the main window
    mainWindow->setLayout(mainLayout);
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
