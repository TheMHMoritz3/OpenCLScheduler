#include "MainWindow.h"
#include "src/Scheduler/Device.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>
#include "OpenKernelFileWizard.h"
#include <sstream>
#include <QDebug>
#include <qwt_plot.h>
#include <qwt_plot_barchart.h>
#include <QProgressDialog>
#include <QtCore/QThread>

using namespace SCHEDULER;
using namespace UI;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        TasksToScheduleModel(new QStandardItemModel()),
        ActiveDevicePropertie(nullptr),
        ScheduleTimeModel(new QStandardItemModel()),
        CanManager(new CAN::CanManager()) {
    ui.setupUi(this);
    ui.retranslateUi(this);
    ui.TasksListView->setModel(TasksToScheduleModel);
    ui.ScheduleTimeTableView->setModel(ScheduleTimeModel);
    fillStartUI();
    makeConnections();
}

MainWindow::~MainWindow() {
    delete TasksToScheduleModel;
    delete ScheduleTimeModel;
}

void MainWindow::multiThreaddingCheckstateChanged() {
    ui.CoreCountSpinBox->setEnabled(ui.MultiThreaddedRadioButton->isChecked());
    if (!ui.MultiThreaddedRadioButton->isChecked()) {
        for (TaskTabWidget *widget : TaskWidgets) {
            widget->updateCoreCount(1);
        }
        ActiveDevicePropertie->setCoreCount(1);
    } else
        onCoreCountChanged();
}

void MainWindow::loadPreset() {
    int DefaultCanLoad = 10;

    QMessageBox msg;
    msg.setIcon(QMessageBox::Question);
    msg.setText(tr("Do you want to load the preset from the CAN-Bus?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    bool CANAccepted = (msg.exec() == QMessageBox::StandardButton::Yes);


    SCHEDULER::Task *xAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "xAxis");
    xAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
    xAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    Tasks.emplace_back(xAxis);
    if (CANAccepted) {
        xAxis->setDataDependancy(SCHEDULER::DependancyType::UserInput);
        loadCanData(CAN::CanID::AccelerationLongitudinal, DefaultCanLoad, xAxis);
    }

    SCHEDULER::Task *yAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "yAxis");
    yAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
    yAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    Tasks.emplace_back(yAxis);
    if (CANAccepted) {
        yAxis->setDataDependancy(SCHEDULER::DependancyType::UserInput);
        loadCanData(CAN::CanID::AccelerationLateral, DefaultCanLoad, yAxis);
    }

    SCHEDULER::Task *dualAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "dualAxis");
    dualAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
    dualAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    Tasks.emplace_back(dualAxis);
    if (CANAccepted) {
        dualAxis->setDataDependancy(SCHEDULER::DependancyType::UserInput);
        loadCanData(CAN::CanID::AccelerationLongitudinal, DefaultCanLoad, dualAxis);
        loadCanData(CAN::CanID::AccelerationLateral, DefaultCanLoad, dualAxis);
    }

    SCHEDULER::Task *batteryCalc = ScheduleManager->addTask("kernels/battery_kernel.cl", "batteryCalc");
    batteryCalc->setReturnDataType(SCHEDULER::Type::FLOAT);
    batteryCalc->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    Tasks.emplace_back(batteryCalc);
    if (CANAccepted) {
        batteryCalc->setDataDependancy(SCHEDULER::DependancyType::UserInput);
        loadCanData(CAN::CanID::BatteryVoltage, DefaultCanLoad, batteryCalc);
    }

    SCHEDULER::Task *temp = ScheduleManager->addTask("kernels/temp_kernel.cl", "temp");
    temp->setReturnDataType(SCHEDULER::Type::FLOAT);
    temp->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    Tasks.emplace_back(temp);
    if (CANAccepted) {
        temp->setDataDependancy(SCHEDULER::DependancyType::UserInput);
        loadCanData(CAN::CanID::Temperature, DefaultCanLoad, temp);
    }

    SCHEDULER::Task *speedCalcFR = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
    speedCalcFR->setReturnDataType(SCHEDULER::Type::FLOAT);
    speedCalcFR->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    Tasks.emplace_back(speedCalcFR);
    if (CANAccepted) {
        speedCalcFR->setDataDependancy(SCHEDULER::DependancyType::UserInput);
        loadCanData(CAN::CanID::WheelFrontRight, DefaultCanLoad, speedCalcFR);
    }

    SCHEDULER::Task *speedCalcFL = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
    speedCalcFL->setReturnDataType(SCHEDULER::Type::FLOAT);
    speedCalcFL->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    Tasks.emplace_back(speedCalcFL);
    if (CANAccepted) {
        speedCalcFL->setDataDependancy(SCHEDULER::DependancyType::UserInput);
        loadCanData(CAN::CanID::WheelFrontLeft, DefaultCanLoad, speedCalcFL);
    }

    SCHEDULER::Task *speedCalcRR = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
    speedCalcRR->setReturnDataType(SCHEDULER::Type::FLOAT);
    speedCalcRR->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    Tasks.emplace_back(speedCalcRR);
    if (CANAccepted) {
        speedCalcRR->setDataDependancy(SCHEDULER::DependancyType::UserInput);
        loadCanData(CAN::CanID::WheelRearRight, DefaultCanLoad, speedCalcRR);
    }

    SCHEDULER::Task *speedCalcRL = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
    speedCalcRL->setReturnDataType(SCHEDULER::Type::FLOAT);
    speedCalcRL->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    Tasks.emplace_back(speedCalcRL);
    if (CANAccepted) {
        speedCalcRL->setDataDependancy(SCHEDULER::DependancyType::UserInput);
        loadCanData(CAN::CanID::WheelRearLeft, DefaultCanLoad, speedCalcRL);
    }

    SCHEDULER::Task *median = ScheduleManager->addTask("kernels/speed_kernel.cl", "median");
    median->setReturnDataType(SCHEDULER::Type::FLOAT);
    median->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    median->addDependandTask(speedCalcFL);
    median->addDependandTask(speedCalcFR);
    Tasks.emplace_back(median);

    SCHEDULER::Task *range = ScheduleManager->addTask("kernels/range_kernel.cl", "range");
    range->setReturnDataType(SCHEDULER::Type::FLOAT);
    range->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    range->addDependandTask(batteryCalc);
    range->addDependandTask(median);
    int *duration = new int[1];
    duration[0] = 10.2;
    range->addConstant(Type::INT, duration);
    float *minVoltage = new float[1];
    minVoltage[0] = 5.5;
    range->addConstant(Type::FLOAT, minVoltage);
    float *maxVoltage = new float[1];
    maxVoltage[0] = 7.5;
    range->addConstant(Type::FLOAT, maxVoltage);
    Tasks.emplace_back(range);

    SCHEDULER::Task *temInformation = ScheduleManager->addTask("kernels/temp_kernel.cl", "temInformation");
    temInformation->setReturnDataType(SCHEDULER::Type::FLOAT);
    temInformation->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    temInformation->addDependandTask(temp);
    float *temInformation_min = new float[1];
    temInformation_min[0] = 10.2;
    temInformation->addConstant(Type::FLOAT, temInformation_min);
    float *temInformation_max = new float[1];
    temInformation_max[0] = 25.5;
    temInformation->addConstant(Type::FLOAT, temInformation_max);
    Tasks.emplace_back(temInformation);


    SCHEDULER::Task *tractionControl = ScheduleManager->addTask("kernels/traction_kernel.cl", "tractionControl");
    tractionControl->setReturnDataType(SCHEDULER::Type::INT);
    tractionControl->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    tractionControl->addDependandTask(median);
    tractionControl->addDependandTask(speedCalcRL);
    tractionControl->addDependandTask(speedCalcRR);
    float *tractionControl_threshhold = new float[1];
    tractionControl_threshhold[0] = 5.5;
    tractionControl->addConstant(Type::FLOAT, tractionControl_threshhold);
    Tasks.emplace_back(tractionControl);


    SCHEDULER::Task *turningRadius = ScheduleManager->addTask("kernels/turn_radius_kernel.cl", "radius");
    turningRadius->setReturnDataType(SCHEDULER::Type::FLOAT);
    turningRadius->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    turningRadius->addDependandTask(speedCalcFR);
    turningRadius->addDependandTask(speedCalcFL);
    float *radius_axle = new float[1];
    radius_axle[0] = 10.5;
    turningRadius->addConstant(Type::FLOAT, radius_axle);
    Tasks.emplace_back(turningRadius);


    SCHEDULER::Task *distanceTracker = ScheduleManager->addTask("kernels/distanceTracker.cl", "distanceTracker");
    distanceTracker->setReturnDataType(SCHEDULER::Type::FLOAT);
    distanceTracker->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    distanceTracker->addDependandTask(median);
    float *distanceTracker_Time = new float[1];
    distanceTracker_Time[0] = 100;
    distanceTracker->addConstant(Type::FLOAT, distanceTracker_Time);
    Tasks.emplace_back(distanceTracker);


    SCHEDULER::Task *cruiseControl = ScheduleManager->addTask("kernels/cruiseControl.cl", "cruiseControl");
    cruiseControl->setReturnDataType(SCHEDULER::Type::FLOAT);
    cruiseControl->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    cruiseControl->addDependandTask(xAxis);
    cruiseControl->addDependandTask(median);
    int *cruiseControl_targetLimit = new int[1];
    cruiseControl_targetLimit[0] = 10;
    cruiseControl->addConstant(Type::INT, cruiseControl_targetLimit);
    Tasks.emplace_back(cruiseControl);

    SCHEDULER::Task *accidentControl = ScheduleManager->addTask("kernels/accident_kernel.cl", "accidentConst");
    accidentControl->setReturnDataType(SCHEDULER::Type::FLOAT);
    accidentControl->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    accidentControl->addDependandTask(dualAxis);
    int *accidentControl_min = new int[1];
    accidentControl_min[0] = 1;
    accidentControl->addConstant(Type::INT, accidentControl_min);
    int *accidentControl_max = new int[1];
    accidentControl_max[0] = 2;
    accidentControl->addConstant(Type::INT, accidentControl_max);
    Tasks.emplace_back(accidentControl);

    SCHEDULER::Task *temp_range_kernel = ScheduleManager->addTask("kernels/temp_Range_kernel.cl", "tempRange");
    temp_range_kernel->setReturnDataType(SCHEDULER::Type::FLOAT);
    temp_range_kernel->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    temp_range_kernel->addDependandTask(temp);
    float *temp_range_kernel_min = new float[1];
    temp_range_kernel_min[0] = 5.5;
    temp_range_kernel->addConstant(Type::FLOAT, temp_range_kernel_min);
    float *temp_range_kernel_max = new float[1];
    temp_range_kernel_max[0] = 10.5;
    temp_range_kernel->addConstant(Type::FLOAT, temp_range_kernel_max);
    Tasks.emplace_back(temp_range_kernel);

    updateTasksModel();

}

void MainWindow::addKernel() {
    OpenKernelFileWizard *wizard = new OpenKernelFileWizard(this);
    wizard->exec();
}

void MainWindow::deviceComboboxChanged() {
    if (ui.DeviceCombobox->currentIndex() >= Devices.size())
        decorateAllDevices();
    else {
        ActiveDevicePropertie = Devices.at(ui.DeviceCombobox->currentIndex());
        ScheduleManager->setActiveDevice(ui.DeviceCombobox->currentIndex());
        if ((ActiveDevicePropertie == nullptr) ||
            (ActiveDevicePropertie->getName() != ui.DeviceCombobox->currentText().toStdString())) {
            QString deviceName = ui.DeviceCombobox->currentText();
            readDeviceData(deviceName.toStdString());
        }
    }
}

void MainWindow::onCoreCountChanged() {
    ActiveDevicePropertie->setCoreCount(ui.CoreCountSpinBox->value());
    for (TaskTabWidget *widget : TaskWidgets) {
        widget->updateCoreCount(ui.CoreCountSpinBox->value());
    }
}

void MainWindow::onSchedulingTypeChanged() {
    qDebug() << (ScheduleType) ui.SchedulingTypeSpinBox->currentIndex();
    ActiveDevicePropertie->setSchedule((ScheduleType) ui.SchedulingTypeSpinBox->currentIndex());
}

void MainWindow::onTasksToScheduleItemClicked(QStandardItem *item) {
    if (item->checkState() == Qt::Checked) {
        addTaskToScheduledTasks(item->text().toStdString());
    }
}

void MainWindow::startSchedule() {
    auto start = std::chrono::steady_clock::now();
    if (ui.DeviceCombobox->currentIndex() >= Devices.size()) {
        ScheduleManager->startMultiDeviceScheduling();
    } else {
        ScheduleManager->startSingleDeviceScheduling();
    }
    for (TaskTabWidget *TaskWidget : TaskWidgets) {
        TaskWidget->refresh();
    }

    auto end = std::chrono::steady_clock::now();
    QStandardItem *item = new QStandardItem(
            tr("%1").arg(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()));
    ScheduleTimeModel->appendRow(item);
}

void MainWindow::fillStartUI() {
    ScheduleManager = new SCHEDULER::ScheduleManager();
    ScheduleManager->searchForDevices();
    for (int i = 0; i < ScheduleManager->getDeviceCount(); i++) {
        DeviceProperties *props = ScheduleManager->getDeviceProperties(i);
        Devices.push_back(props);
        ui.DeviceCombobox->addItem(props->getName().c_str());
    }
    deviceComboboxChanged();

    ui.DeviceCombobox->addItem(tr("All Devices"));

    QStringList headerItems;
    headerItems.append(tr("Execution Times"));

    ScheduleTimeModel->setHorizontalHeaderLabels(headerItems);
}

void MainWindow::makeConnections() {
    connect(ui.MultiThreaddedRadioButton, SIGNAL(clicked()), this, SLOT(multiThreaddingCheckstateChanged()));
    connect(ui.SingleThreaddedRadioButton, SIGNAL(clicked()), this, SLOT(multiThreaddingCheckstateChanged()));
    connect(ui.actionLoad_Preset, SIGNAL(triggered(bool)), this, SLOT(loadPreset()));
    connect(ui.actionOpen_Kernel, SIGNAL(triggered(bool)), this, SLOT(addKernel()));
    connect(ui.DeviceCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(deviceComboboxChanged()));
    connect(TasksToScheduleModel, SIGNAL(itemChanged(QStandardItem * )), this,
            SLOT(onTasksToScheduleItemClicked(QStandardItem * )));
    connect(ui.ScheduleButton, SIGNAL(clicked()), this, SLOT(startSchedule()));
    connect(ui.CoreCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onCoreCountChanged()));
    connect(ui.SchedulingTypeSpinBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSchedulingTypeChanged()));
    connect(ui.ShowScheduleGraphicButton, SIGNAL(clicked()), this, SLOT(onShowScheduleGraphClicked()));
    connect(ui.TasksWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseClicked(int)));
    connect(ui.TasksListView, SIGNAL(doubleClicked(
                                             const QModelIndex&)), this, SLOT(onTaskWidgetDoubleClicked(
                                                                                      const QModelIndex&)));
    connect(ui.actionSchow_Execution_Graphic, SIGNAL(triggered(bool)), this, SLOT(onShowScheduleGraphClicked()));
    connect(ui.OutOfOrderScheduling, SIGNAL(stateChanged(int)), this, SLOT(onActivateOutOfOrderSchedulingClicked()));
}

void MainWindow::updateTasksModel() {
    TasksToScheduleModel->clear();

    for (Task *task : Tasks) {
        QStandardItem *item = new QStandardItem(tr((task->getKernelName() + "- %1").c_str()).arg(task->getId()));
        item->setCheckable(true);
        item->setEditable(false);
        TasksToScheduleModel->invisibleRootItem()->appendRow(item);
        TaskTabWidget *widget = new TaskTabWidget(task, this);
        TaskWidgets.push_back(widget);
        widget->setTaskModel(Tasks);
        widget->refresh();
        widget->setCanManager(CanManager);
    }
}

void MainWindow::readDeviceData(std::string deviceName) {
    for (DeviceProperties *devProp : Devices) {
        if (deviceName == devProp->getName()) {
            ActiveDevicePropertie = devProp;
            if (devProp->getCoureCount() > 1) {
                ui.MultiThreaddedRadioButton->setChecked(true);
                ui.CoreCountSpinBox->setValue(devProp->getCoureCount());
            } else {
                ui.SingleThreaddedRadioButton->setChecked(true);
            }

            ui.SchedulingTypeSpinBox->setCurrentIndex(devProp->getSchedule());

            for (Task *task : devProp->getTasksToSchedule()) {
                for (int i = 0; i < TasksToScheduleModel->invisibleRootItem()->rowCount(); i++) {
                    QStandardItem *item = TasksToScheduleModel->invisibleRootItem()->child(i);
                    if (item->text().toStdString() == task->getKernelName()) {
                        TasksToScheduleModel->invisibleRootItem()->child(i)->setCheckState(Qt::Checked);
                    }
                }
            }
        }
    }
}

void MainWindow::addTaskToScheduledTasks(std::string taskName) {
    for (Task *task:Tasks) {
        std::stringstream ss;
        ss << task->getId();

        if ((task->getKernelName() + "- " + ss.str()) == taskName)
            ActiveDevicePropertie->addTaskToSchedule(task);
    }
}

void MainWindow::decorateAllDevices() {
    QMessageBox msg;
    msg.setIcon(QMessageBox::Critical);
    msg.setText(tr("This feature is currently not Supported"));
    msg.exec();

    ui.DeviceCombobox->setCurrentIndex(0);
}

void MainWindow::loadCanData(CAN::CanID canID, int canLoad, SCHEDULER::Task *task) {
    int* dataSet = CanManager->getValuesFromSimulation(canID, canLoad);
    int* DataSet = new int[canLoad];
    std::vector<void *> taskData;
    int i = 0;
    for (int i = 0; i<canLoad; i++) {
        DataSet[i] = dataSet[i];
        taskData.push_back(&DataSet[i]);
        i++;
    }
    task->setLoad(canLoad);
    task->addData(taskData, SCHEDULER::INT);
}

void MainWindow::onShowScheduleGraphClicked() {
    if (ScheduleTimeModel->rowCount() == 0) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setText(tr("Please run a Schedule at least once."));
        msg.exec();
        return;
    }

    QwtPlot *plot = new QwtPlot(this);


    plot->setTitle("Execution Times");
    plot->setCanvasBackground(Qt::white);

    QVector<QPointF> points;

    for (int i = 0; i < ScheduleTimeModel->rowCount(); i++)
        points << QPointF(i, ScheduleTimeModel->item(i)->text().toDouble());

    plot->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8("Run Number"));
    plot->setAxisAutoScale(QwtPlot::xBottom);
    plot->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("Elapsed Times in ms"));
    plot->setAxisAutoScale(QwtPlot::yLeft);

    QwtPlotBarChart *curve = new QwtPlotBarChart();
    curve->setSamples(points);
    curve->attach(plot);

    plot->replot();

    plot->show();

    ui.TasksWidget->addTab(plot, "Plot");
    ui.TasksWidget->setCurrentIndex(ui.TasksWidget->count() - 1);
}

void MainWindow::onTabCloseClicked(int id) {
    ui.TasksWidget->removeTab(id);
}

void MainWindow::onTaskWidgetDoubleClicked(const QModelIndex &index) {

    ui.TasksWidget->addTab(TaskWidgets.at(index.row()), TasksToScheduleModel->item(index.row())->text());
    TaskWidgets.at(index.row())->refresh();
}

void MainWindow::onActivateOutOfOrderSchedulingClicked() {
    ActiveDevicePropertie->setOutOfOrderExecution(ui.OutOfOrderScheduling->checkState() == Qt::Checked);
}
