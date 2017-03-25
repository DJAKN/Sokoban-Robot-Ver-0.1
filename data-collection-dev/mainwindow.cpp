#include "mainwindow.h"
#include "ui_mainwindow.h"

#define MONITOR 1
#define STORAGE 0

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString config = "config.xml";
    SensorTimer *xtion = new SensorTimer("Robot", "Sensor_xtion", "xtion", config, 100);
    xtion->setOutputNodesName(QStringList() << "xtionVis;xtionSto;action");

    VisualizationMono *xtionVis = new VisualizationMono("Robot", "Sensor_xtion", "xtionVis", config);
    xtionVis->setInputNodesName(QStringList() << "xtion");
    xtionVis->connectExternalTrigger(0, DRAINSLOT);

    StorageMono *xtionSto = new StorageMono("Robot", "Sensor_xtion", "xtionSto", config);
    xtionSto->setInputNodesName(QStringList() << "xtion");
    xtionSto->connectExternalTrigger(0, DRAINSLOT);

    SensorTimer *urg = new SensorTimer("Robot", "Sensor_URG", "urg", config, 100);
    urg->setOutputNodesName(QStringList() << "urgVis;urgSto;obDetector");

    VisualizationMono *urgVis = new VisualizationMono("Robot", "Sensor_URG", "urgVis", config);
    urgVis->setInputNodesName(QStringList() << "urg");
    urgVis->connectExternalTrigger(0, DRAINSLOT);

    StorageMono *urgSto = new StorageMono("Robot", "Sensor_URG", "urgSto", config);
    urgSto->setInputNodesName(QStringList() << "urg");
    urgSto->connectExternalTrigger(0, DRAINSLOT);

    SourceDrainMono *EncoderIMU = new SourceDrainMono("Robot", "Sensor_EncoderIMU", "odom", config);
    EncoderIMU->setInputNodesName(QStringList() << "action");
    EncoderIMU->setOutputNodesName(QStringList() << "EncoderIMUVis;EncoderIMUSto;action");
    EncoderIMU->connectInternalTrigger(SOURCESLOT);
    EncoderIMU->connectExternalTrigger(0, DRAINSLOT);

    VisualizationMono *EncoderIMUVis = new VisualizationMono("Robot", "Sensor_EncoderIMU", "EncoderIMUVis", config);
    EncoderIMUVis->setInputNodesName(QStringList() << "odom");
    EncoderIMUVis->connectExternalTrigger(0, DRAINSLOT);

    StorageMono *EncoderIMUSto = new StorageMono("Robot", "Sensor_EncoderIMU", "EncoderIMUSto", config);
    EncoderIMUSto->setInputNodesName(QStringList() << "odom");
    EncoderIMUSto->connectExternalTrigger(0, DRAINSLOT);




    ProcessorMulti *action = new ProcessorMulti("Robot","Processor_Action","action",config);
    action->setInputNodesName(QStringList()<<"odom"<<"xtion");
    action->setOutputNodesName(QStringList()<<"odom;colorvis");
    action->connectExternalTrigger(1, PROCESSORSLOT);

    VisualizationMono *colorvis=new VisualizationMono("Robot","color_visualization","colorvis",config);
    colorvis->setInputNodesName(QStringList()<<"action");
    colorvis->connectExternalTrigger(0,DRAINSLOT);

    edge.addNode(xtion, 1, 1);
    edge.addNode(xtionVis, 0, 0);
    edge.addNode(urg, 1, 1);
    edge.addNode(urgVis, 0, 0);
    edge.addNode(EncoderIMU, 1, 1);
    edge.addNode(EncoderIMUVis, 0, 0);

    edge.addNode(action, 1, 1);
    edge.addNode(colorvis, 0, 0);

    if (STORAGE) {
        edge.addNode(xtionSto, 1, 1);
        edge.addNode(urgSto, 1, 1);
        edge.addNode(EncoderIMUSto, 1, 1);
    }
    edge.connectAll();

    connect(ui->open, &QPushButton::clicked, &edge, &Edge::openAllNodesSlot);
    connect(ui->close, &QPushButton::clicked, &edge, &Edge::closeAllNodesSlot);
    connect(ui->start, &QPushButton::clicked, xtion, &SensorTimer::startTimerSlot);
    connect(ui->stop, &QPushButton::clicked, xtion, &SensorTimer::stopTimerSlot);
    connect(ui->start, &QPushButton::clicked, urg, &SensorTimer::startTimerSlot);
    connect(ui->stop, &QPushButton::clicked, urg, &SensorTimer::stopTimerSlot);

    QWidget *color = xtionVis->getVisualizationWidgets()[0];
    QWidget *depth = xtionVis->getVisualizationWidgets()[1];
    ui->area1->setWidget(color);
    ui->area2->setWidget(depth);
    QWidget *laser = urgVis->getVisualizationWidgets()[0];
    ui->area3->setWidget(laser);
    QWidget *odom = EncoderIMUVis->getVisualizationWidgets()[0];
    ui->area4->setWidget(odom);

    QWidget *colorvisu=colorvis->getVisualizationWidgets()[0];
    ui->tabWidget->addTab(colorvisu,"color");
    if (MONITOR)
        ui->tabWidget->addTab(&edge, "Monitor");
}

MainWindow::~MainWindow()
{
    delete ui;
}
