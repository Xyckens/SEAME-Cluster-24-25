#include "../includes/Cluster.h"

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    QApplication app(argc, argv);

    QWidget* main_window = new QWidget;
    QWidget* page1 = new QWidget;
    QWidget* page2 = new QWidget;

    QStackedWidget* stacked_widget = new QStackedWidget;
    stacked_widget->addWidget(page1); // Mode 1
    stacked_widget->addWidget(page2); // Mode 2

    // Mode 1
    SpeedometerWidget*      speedometer = new SpeedometerWidget(page1, 90, 145, 300, 300);
    BatteryWidget*          battery = new BatteryWidget(page1, 615, 145, 300, 300);
    ArrowSymbolWidget*      arrowSymbol = new ArrowSymbolWidget(page1, 390, 0, 200, 500);
    Blinkers*               leftB = new Blinkers(page1, "left", "off", -5, 250, 100, 100);
    Blinkers*               rightB = new Blinkers(page1, "right", "off", 900, 250, 100, 100);
    FanSpeedWidget*         fanspeed = new FanSpeedWidget(page1, 0, 530, 150, 140);
    CPUTempWidget*          cputemp = new CPUTempWidget(page1, 100, 530, 150, 140);
    TopBar*                 topBar = new TopBar(page1, 250, 600, 250, 50);
    ListeningWidget*        listening = new ListeningWidget(page1, 0, 620, 950, 50);

    // Mode 2
    BatteryAndSpeedWidget*  bas = new BatteryAndSpeedWidget(page2, 110, 80, 200, 400);
    StatsWidget*            stats = new StatsWidget(page2, 350, 150, 550, 400);
    Blinkers*               leftB2 = new Blinkers(page2, "left", "off", -5, 250, 100, 100);
    Blinkers*               rightB2 = new Blinkers(page2, "right", "off", 900, 250, 100, 100);
    FanSpeedWidget*         fanspeed2 = new FanSpeedWidget(page2, 0, 530, 150, 140);
    CPUTempWidget*          cputemp2 = new CPUTempWidget(page2, 100, 530, 150, 140);
    TopBar*                 topBar2 = new TopBar(page2, 250, 600, 250, 50);


    auto node = std::make_shared<RosNode>();

    EventManager eventManager(arrowSymbol, speedometer, battery, bas, leftB, rightB,
                              leftB2, rightB2, stats, fanspeed, fanspeed2, cputemp,
                              cputemp2, topBar, topBar2, stacked_widget, main_window, listening, node);
    app.installEventFilter(&eventManager);
    //settings->setGeometry(1120, -10, 250, 250);
    // Toolbar for switching modes
    QToolBar* toolBar = new QToolBar;
    QAction* mode1Action = toolBar->addAction("Mode 1");
    QAction* mode2Action = toolBar->addAction("Mode 2");

    QObject::connect(mode1Action, &QAction::triggered,
                     [&]()
                     {
                         stacked_widget->setCurrentIndex(0);
                     });
    QObject::connect(mode2Action, &QAction::triggered,
                     [&]()
                     {
                         stacked_widget->setCurrentIndex(1);
                     });

    // Main Layout
    Color color = Color();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(stacked_widget);
    main_window->setLayout(mainLayout);
    //main_window->resize(850, 700);
    main_window->setStyleSheet(QString("%1; margin: 0px; padding: 0px;").arg(color.background));
    main_window->showFullScreen();

    int ret = app.exec();
    rclcpp::shutdown();
    return ret;
}
