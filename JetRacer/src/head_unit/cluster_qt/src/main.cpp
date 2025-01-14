#include "../include/Cluster.h"

void cluster(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWidget* mainWindow = new QWidget;
    QWidget* mode1Page = new QWidget;
    QWidget* mode2Page = new QWidget;

    QStackedWidget* stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(mode1Page); // Mode 1
    stackedWidget->addWidget(mode2Page); // Mode 2

    // Mode 1
    TopBar*                 topBar = new TopBar(mainWindow);
    Blinkers*               leftB = new Blinkers(mainWindow, "left", "off");
    Blinkers*               rightB = new Blinkers(mainWindow, "right", "off");
    SpeedometerWidget*      speedometer = new SpeedometerWidget(mainWindow);
    BatteryWidget*          battery = new BatteryWidget(mainWindow);
    FanSpeedWidget*         fanspeed = new FanSpeedWidget(mainWindow);
    CPUTempWidget*          cputemp = new CPUTempWidget(mainWindow);
    ArrowSymbolWidget*      arrowSymbol = new ArrowSymbolWidget(mainWindow, "zero");

    // Mode 2
    TopBar*                 topBar2 = new TopBar(mainWindow);
    Blinkers*               rightB2 = new Blinkers(mainWindow, "right", "off");
    Blinkers*               leftB2 = new Blinkers(mainWindow, "left", "off");
    BatteryAndSpeedWidget*  bas = new BatteryAndSpeedWidget(mainWindow);
    FanSpeedWidget*         fanspeed2 = new FanSpeedWidget(mainWindow);
    CPUTempWidget*          cputemp2 = new CPUTempWidget(mainWindow);

    // Layout for Mode 1
    QVBoxLayout* mode1Layout = new QVBoxLayout();
    QHBoxLayout* mode1ContentLayout = new QHBoxLayout();

    EventManager eventManager(arrowSymbol, speedometer, bas, leftB, rightB, stackedWidget);
    app.installEventFilter(&eventManager);

    arrowSymbol->setFixedSize(400, 400);

    speedometer->resize(900, 900);
    speedometer->setFixedSize(400, 400);
    battery->resize(900, 900);
    battery->setFixedSize(400, 400);
    //cameraWidget->resize(1920, 1080);
    //cameraWidget->setFixedSize(700, 500);

    bas->resize(900, 900);
    bas->setFixedSize(400, 400);

    mode1ContentLayout->addWidget(leftB, 1);
    mode1ContentLayout->addWidget(speedometer, 1);
    mode1ContentLayout->addWidget(arrowSymbol, 1);
    mode1ContentLayout->addWidget(battery, 1);
    mode1ContentLayout->addWidget(rightB, 1);

    mode1Layout->addLayout(mode1ContentLayout, 1);
    mode1Layout->addWidget(fanspeed, 0, Qt::AlignBottom);
    mode1Layout->addWidget(cputemp, 0, Qt::AlignBottom);
    mode1Layout->addWidget(topBar, 0, Qt::AlignBottom);
    mode1Page->setLayout(mode1Layout);

    // Layout for Mode 2
    QVBoxLayout* mode2Layout = new QVBoxLayout();
    QHBoxLayout* mode2ContentLayout = new QHBoxLayout();

    mode2ContentLayout->addWidget(leftB2, 1);
    mode2ContentLayout->addWidget(bas, 2);
    mode2ContentLayout->addWidget(rightB2, 1);


    mode2Layout->addLayout(mode2ContentLayout, 1);
    mode2Layout->addWidget(fanspeed2, 0, Qt::AlignBottom);
    mode2Layout->addWidget(cputemp2, 0, Qt::AlignBottom);
    mode2Layout->addWidget(topBar2, 0, Qt::AlignBottom);
    mode2Page->setLayout(mode2Layout);

    // Toolbar for switching modes
    QToolBar* toolBar = new QToolBar;
    QAction* mode1Action = toolBar->addAction("Mode 1");
    QAction* mode2Action = toolBar->addAction("Mode 2");

    QObject::connect(mode1Action, &QAction::triggered, [&]() {
        stackedWidget->setCurrentIndex(0); // Switch to Mode 1
    });
    QObject::connect(mode2Action, &QAction::triggered, [&]() {
        stackedWidget->setCurrentIndex(1); // Switch to Mode 2
    });

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(stackedWidget);

    mainWindow->setLayout(mainLayout);
    mainWindow->resize(1200, 800);
    mainWindow->setStyleSheet("background-color: #0D2126");
    mainWindow->show();

    /*
    QTimer::singleShot(2000, [&]() { topBar->setImageState(0, true); });
    QTimer::singleShot(3000, [&]() { topBar->setImageState(1, true); });
    QTimer::singleShot(4000, [&]() { topBar->setImageState(2, true); });
    QTimer::singleShot(6000, [&]() { topBar->setImageState(4, true); });
    QTimer::singleShot(7000, [&]() { topBar->setImageState(3, true); });
    QTimer::singleShot(8000, [&]() { topBar->setImageState(5, true); });
    */
    app.exec();
}

int main(int argc, char* argv[])
{
    //rclcpp::init(argc, argv);
        QApplication app(argc, argv);

    QWidget* mainWindow = new QWidget;
    QWidget* mode1Page = new QWidget;
    QWidget* mode2Page = new QWidget;

    QStackedWidget* stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(mode1Page); // Mode 1
    stackedWidget->addWidget(mode2Page); // Mode 2

    // Mode 1
    TopBar*                 topBar = new TopBar(mainWindow);
    Blinkers*               leftB = new Blinkers(mainWindow, "left", "off");
    Blinkers*               rightB = new Blinkers(mainWindow, "right", "off");
    SpeedometerWidget*      speedometer = new SpeedometerWidget(mainWindow);
    BatteryWidget*          battery = new BatteryWidget(mainWindow);
    FanSpeedWidget*         fanspeed = new FanSpeedWidget(mainWindow);
    CPUTempWidget*          cputemp = new CPUTempWidget(mainWindow);
    ArrowSymbolWidget*      arrowSymbol = new ArrowSymbolWidget(mainWindow, "zero");

    // Mode 2
    TopBar*                 topBar2 = new TopBar(mainWindow);
    Blinkers*               rightB2 = new Blinkers(mainWindow, "right", "off");
    Blinkers*               leftB2 = new Blinkers(mainWindow, "left", "off");
    BatteryAndSpeedWidget*  bas = new BatteryAndSpeedWidget(mainWindow);
    FanSpeedWidget*         fanspeed2 = new FanSpeedWidget(mainWindow);
    CPUTempWidget*          cputemp2 = new CPUTempWidget(mainWindow);

    // Layout for Mode 1
    QVBoxLayout* mode1Layout = new QVBoxLayout();
    QHBoxLayout* mode1ContentLayout = new QHBoxLayout();

    EventManager eventManager(arrowSymbol, speedometer, bas, leftB, rightB, stackedWidget);
    app.installEventFilter(&eventManager);

    arrowSymbol->setFixedSize(400, 400);

    speedometer->resize(900, 900);
    speedometer->setFixedSize(400, 400);
    battery->resize(900, 900);
    battery->setFixedSize(400, 400);
    //cameraWidget->resize(1920, 1080);
    //cameraWidget->setFixedSize(700, 500);

    bas->resize(900, 900);
    bas->setFixedSize(400, 400);

    mode1ContentLayout->addWidget(leftB, 1);
    mode1ContentLayout->addWidget(speedometer, 1);
    mode1ContentLayout->addWidget(arrowSymbol, 1);
    mode1ContentLayout->addWidget(battery, 1);
    mode1ContentLayout->addWidget(rightB, 1);

    mode1Layout->addLayout(mode1ContentLayout, 1);
    mode1Layout->addWidget(fanspeed, 0, Qt::AlignBottom);
    mode1Layout->addWidget(cputemp, 0, Qt::AlignBottom);
    mode1Layout->addWidget(topBar, 0, Qt::AlignBottom);
    mode1Page->setLayout(mode1Layout);

    // Layout for Mode 2
    QVBoxLayout* mode2Layout = new QVBoxLayout();
    QHBoxLayout* mode2ContentLayout = new QHBoxLayout();

    mode2ContentLayout->addWidget(leftB2, 1);
    mode2ContentLayout->addWidget(bas, 2);
    mode2ContentLayout->addWidget(rightB2, 1);


    mode2Layout->addLayout(mode2ContentLayout, 1);
    mode2Layout->addWidget(fanspeed2, 0, Qt::AlignBottom);
    mode2Layout->addWidget(cputemp2, 0, Qt::AlignBottom);
    mode2Layout->addWidget(topBar2, 0, Qt::AlignBottom);
    mode2Page->setLayout(mode2Layout);

    // Toolbar for switching modes
    QToolBar* toolBar = new QToolBar;
    QAction* mode1Action = toolBar->addAction("Mode 1");
    QAction* mode2Action = toolBar->addAction("Mode 2");

    QObject::connect(mode1Action, &QAction::triggered, [&]() {
        stackedWidget->setCurrentIndex(0); // Switch to Mode 1
    });
    QObject::connect(mode2Action, &QAction::triggered, [&]() {
        stackedWidget->setCurrentIndex(1); // Switch to Mode 2
    });

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(stackedWidget);

    mainWindow->setLayout(mainLayout);
    mainWindow->resize(1200, 800);
    mainWindow->setStyleSheet("background-color: #0D2126");
    mainWindow->show();

    /*
    QTimer::singleShot(2000, [&]() { topBar->setImageState(0, true); });
    QTimer::singleShot(3000, [&]() { topBar->setImageState(1, true); });
    QTimer::singleShot(4000, [&]() { topBar->setImageState(2, true); });
    QTimer::singleShot(6000, [&]() { topBar->setImageState(4, true); });
    QTimer::singleShot(7000, [&]() { topBar->setImageState(3, true); });
    QTimer::singleShot(8000, [&]() { topBar->setImageState(5, true); });
    */
    int ret = app.exec();
    //rclcpp::shutdown();

    return ret;
}
