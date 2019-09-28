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
#include <qwt_plot_histogram.h>
#include <QProgressDialog>
#include <QtCore/QThread>
#include "src/Scheduler/SchedulerNamespace.h"

using namespace SCHEDULER;
using namespace UI;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        TasksToScheduleModel(new QStandardItemModel()),
        ScheduleTimeModel(new QStandardItemModel()),
        ActiveDevicePropertie(nullptr),
        CanManager(new CAN::CanManager()) {
    ui.setupUi(this);
    ui.retranslateUi(this);
    ui.TasksListView->setModel(TasksToScheduleModel);
    ui.ScheduleTimeTableView->setModel(ScheduleTimeModel);
    fillStartUI();
    makeConnections();
    ui.ScheduleButton->setEnabled(Tasks.size() > 1);
}

MainWindow::~MainWindow() {
    delete TasksToScheduleModel;
    delete ScheduleTimeModel;
}

void MainWindow::multiThreaddingCheckstateChanged() {
    ui.CoreCountSpinBox->setEnabled(ui.MultiThreaddedRadioButton->isChecked());
    if (!ui.MultiThreaddedRadioButton->isChecked()) {
        for (int i = 0; i < ui.TasksWidget->count(); i++) {
            if (typeid(*ui.TasksWidget->widget(i)) == typeid(TaskTabWidget))
                (dynamic_cast<TaskTabWidget *>(ui.TasksWidget->widget(i)))->updateCoreCount(1);
        }
        ActiveDevicePropertie->setCoreCount(1);
    } else
        onCoreCountChanged();
}

void MainWindow::loadPreset() {
    int DefaultCanLoad = 10000;

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
    range->setLoad(DefaultCanLoad);
    range->addDependandTask(batteryCalc);
    range->addDependandTask(median);
    int *duration = new int[1];
    duration[0] = 10.2;
    range->addConstant(Type::INT, duration);
    float *minVoltage = new float[1];
    minVoltage[0] = 5.5;
    range->addConstant(Type::FLOAT, minVoltage);
    float *maxVoltage = new float[1];
    maxVoltage[0] = 12.5;
    range->addConstant(Type::FLOAT, maxVoltage);
    Tasks.emplace_back(range);

    SCHEDULER::Task *tractionControl = ScheduleManager->addTask("kernels/traction_kernel.cl", "tractionControl");
    tractionControl->setReturnDataType(SCHEDULER::Type::INT);
    tractionControl->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    tractionControl->addDependandTask(median);
    tractionControl->addDependandTask(speedCalcRL);
    tractionControl->addDependandTask(speedCalcRR);
    tractionControl->setLoad(DefaultCanLoad);
    float *tractionControl_threshhold = new float[1];
    tractionControl_threshhold[0] = 5.5;
    tractionControl->addConstant(Type::FLOAT, tractionControl_threshhold);
    Tasks.emplace_back(tractionControl);


    SCHEDULER::Task *turningRadius = ScheduleManager->addTask("kernels/turn_radius_kernel.cl", "radius");
    turningRadius->setReturnDataType(SCHEDULER::Type::FLOAT);
    turningRadius->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    turningRadius->addDependandTask(speedCalcFR);
    turningRadius->addDependandTask(speedCalcFL);
    turningRadius->setLoad(DefaultCanLoad);
    float *radius_axle = new float[1];
    radius_axle[0] = 10.5;
    turningRadius->addConstant(Type::FLOAT, radius_axle);
    Tasks.emplace_back(turningRadius);


    SCHEDULER::Task *distanceTracker = ScheduleManager->addTask("kernels/distanceTracker.cl", "distanceTracker");
    distanceTracker->setReturnDataType(SCHEDULER::Type::FLOAT);
    distanceTracker->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    distanceTracker->addDependandTask(median);
    distanceTracker->setLoad(DefaultCanLoad);
    float *distanceTracker_Time = new float[1];
    distanceTracker_Time[0] = 100;
    distanceTracker->addConstant(Type::FLOAT, distanceTracker_Time);
    distanceTracker->setLoad(DefaultCanLoad);
    Tasks.emplace_back(distanceTracker);


    SCHEDULER::Task *cruiseControl = ScheduleManager->addTask("kernels/cruiseControl.cl", "cruiseControl");
    cruiseControl->setReturnDataType(SCHEDULER::Type::FLOAT);
    cruiseControl->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    cruiseControl->addDependandTask(xAxis);
    cruiseControl->addDependandTask(median);
    cruiseControl->setLoad(DefaultCanLoad);
    int *cruiseControl_targetLimit = new int[1];
    cruiseControl_targetLimit[0] = 10;
    cruiseControl->addConstant(Type::INT, cruiseControl_targetLimit);
    Tasks.emplace_back(cruiseControl);

    SCHEDULER::Task *accidentControl = ScheduleManager->addTask("kernels/accident_kernel.cl", "accidentConst");
    accidentControl->setReturnDataType(SCHEDULER::Type::INT);
    accidentControl->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    accidentControl->addDependandTask(dualAxis);
    accidentControl->setLoad(DefaultCanLoad);
    float *accidentControl_min = new float[1];
    accidentControl_min[0] = -1;
    accidentControl->addConstant(Type::FLOAT, accidentControl_min);
    float *accidentControl_max = new float[1];
    accidentControl_max[0] = 1;
    accidentControl->addConstant(Type::FLOAT, accidentControl_max);
    Tasks.emplace_back(accidentControl);

    SCHEDULER::Task *temp_range_kernel = ScheduleManager->addTask("kernels/temp_Range_kernel.cl", "tempRange");
    temp_range_kernel->setReturnDataType(SCHEDULER::Type::INT);
    temp_range_kernel->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    temp_range_kernel->addDependandTask(temp);
    temp_range_kernel->setLoad(DefaultCanLoad);
    float *temp_range_kernel_min = new float[1];
    temp_range_kernel_min[0] = 5.5;
    temp_range_kernel->addConstant(Type::FLOAT, temp_range_kernel_min);
    float *temp_range_kernel_max = new float[1];
    temp_range_kernel_max[0] = 45.1;
    temp_range_kernel->addConstant(Type::FLOAT, temp_range_kernel_max);
    Tasks.emplace_back(temp_range_kernel);

    updateTasksModel();

}

void MainWindow::addKernel() {
    OpenKernelFileWizard *wizard = new OpenKernelFileWizard(this);
    wizard->exec();
}

void MainWindow::deviceComboboxChanged() {
    if (ui.DeviceCombobox->currentIndex() >= (int) Devices.size())
        decorateAllDevices();
    else {
        ActiveDevicePropertie = Devices.at(ui.DeviceCombobox->currentIndex());
        ScheduleManager->setActiveDevice(ui.DeviceCombobox->currentIndex());
        QString deviceName = ui.DeviceCombobox->currentText();
        readDeviceData(deviceName.toStdString());
    }
}

void MainWindow::onCoreCountChanged() {
    ActiveDevicePropertie->setCoreCount(ui.CoreCountSpinBox->value());
    for (int i = 0; i < ui.TasksWidget->count(); i++) {
        if (typeid(*ui.TasksWidget->widget(i)) == typeid(TaskTabWidget))
            (dynamic_cast<TaskTabWidget *>(ui.TasksWidget->widget(i)))->updateCoreCount(ui.CoreCountSpinBox->value());
    }
}

void MainWindow::onSchedulingTypeChanged() {
    if (ui.DeviceCombobox->currentText().contains("pthread")) {
        if(((ScheduleType) ui.SchedulingTypeSpinBox->currentIndex())==SCHEDULER::ASAPHC) {
            ActiveDevicePropertie->setSchedule(SCHEDULER::ASAP_POCL);
            return;
        }
        if(((ScheduleType) ui.SchedulingTypeSpinBox->currentIndex())==SCHEDULER::STATIC) {
            ActiveDevicePropertie->setSchedule(SCHEDULER::STATIC_POCL);
            return;
        }
    }
    ActiveDevicePropertie->setSchedule((ScheduleType) ui.SchedulingTypeSpinBox->currentIndex());
}

void MainWindow::onTasksToScheduleItemClicked(QStandardItem *item) {
    ActiveDevicePropertie->clearTasksToSchedule();
    for (int i = 0; i < TasksToScheduleModel->rowCount(); i++) {
        if (TasksToScheduleModel->item(i)->checkState() == Qt::Checked) {
            ActiveDevicePropertie->addTaskToSchedule(Tasks.at(i));
        }
    }
}

void MainWindow::startSchedule() {
    bool acceptedOnce = false;
    for (Task *task : Tasks) {
        if ((task->dependenciesAreCalculated()) && (task->getLoad() % ui.CoreCountSpinBox->value()) &&
            (!acceptedOnce)) {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Question);
            msg.setText(
                    tr("The Load is not dividable by the Workitem Count. This can cause Problems.\nDo you want to start anyway?"));
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            if (msg.exec() != QMessageBox::StandardButton::Yes) {
                return;
            }
            acceptedOnce = true;
        }
    }

    for (int i = 0; i < ui.RepititionsSpinBox->value(); i++) {

        if (ui.DeviceCombobox->currentIndex() >= (int) Devices.size()) {
            ScheduleManager->startMultiDeviceScheduling();
        } else {
            try {
                ScheduleManager->startSingleDeviceScheduling();
            }
            catch (std::exception *ex) {
                std::cout << "Exception: " << ex->what() << std::endl;
            }
        }

        QList<QStandardItem *> items;
        QStandardItem *infoItem = new QStandardItem(ActiveDevicePropertie->toString().c_str());
        items.append(infoItem);
        QStandardItem *dataitem = new QStandardItem(
                tr("%1").arg(ScheduleManager->getLastScheduleTime()));
        items.append(dataitem);

        ScheduleTimeModel->appendRow(items);
    }
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
    headerItems.append(tr("Informations"));
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
    connect(ui.CSVExportButton, SIGNAL(clicked()), this, SLOT(onCSVExportClicked()));
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
    }

    ui.ScheduleButton->setEnabled(Tasks.size() > 1);
}

void MainWindow::readDeviceData(std::string deviceName) {

    if (ActiveDevicePropertie->getCoureCount() > 1) {
        ui.MultiThreaddedRadioButton->setChecked(true);
        ui.CoreCountSpinBox->setValue(ActiveDevicePropertie->getCoureCount());
    } else {
        ui.SingleThreaddedRadioButton->setChecked(true);
    }

    ui.CoreCountSpinBox->setMaximum(ActiveDevicePropertie->getMaxCoreCount());
    ui.SchedulingTypeSpinBox->setCurrentIndex(ActiveDevicePropertie->getSchedule());

    for (Task *task : ActiveDevicePropertie->getTasksToSchedule()) {
        for (int i = 0; i < TasksToScheduleModel->invisibleRootItem()->rowCount(); i++) {
            QStandardItem *item = TasksToScheduleModel->invisibleRootItem()->child(i);
            if (item->text().toStdString() == task->getKernelName()) {
                TasksToScheduleModel->invisibleRootItem()->child(i)->setCheckState(Qt::Checked);
            }
        }
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
    task->setLoad(canLoad);
    task->addData(CanManager->getValuesFromSimulation(canID, canLoad), SCHEDULER::UINT);
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

    QVector<QwtIntervalSample> points;

    for (int i = 0; i < ScheduleTimeModel->rowCount(); i++) {
        QwtIntervalSample sample(ScheduleTimeModel->item(i, 1)->text().toDouble(), i + 0.8, i + 1.2);
        points << sample;
    }

    plot->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8("Run Number"));
    plot->setAxisAutoScale(QwtPlot::xBottom);
    plot->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("Elapsed Times in ms"));
    plot->setAxisAutoScale(QwtPlot::yLeft);

    QwtPlotHistogram *curve = new QwtPlotHistogram();
    curve->setSamples(points);
    curve->attach(plot);
    curve->setPen(QPen(QColor(Qt::GlobalColor::blue)));
    curve->setBrush(QBrush(QColor(Qt::GlobalColor::darkBlue), Qt::BrushStyle::SolidPattern));
    curve->setStyle(QwtPlotHistogram::Columns);

    plot->replot();

    plot->show();

    ui.TasksWidget->addTab(plot, "Plot");
    ui.TasksWidget->setCurrentIndex(ui.TasksWidget->count() - 1);
}

void MainWindow::onTabCloseClicked(int id) {
    QWidget *deleteAbleWidget = ui.TasksWidget->widget(id);
    ui.TasksWidget->removeTab(id);
    delete deleteAbleWidget;
}

void MainWindow::onTaskWidgetDoubleClicked(const QModelIndex &index) {
    TaskTabWidget *widget = new TaskTabWidget(Tasks.at(index.row()), this);
    widget->setTaskModel(Tasks);
    widget->setCanManager(CanManager);

    ui.TasksWidget->addTab(widget, TasksToScheduleModel->item(index.row())->text());
    widget->refresh();
    ui.TasksWidget->setCurrentIndex(ui.TasksWidget->count() - 1);
}

void MainWindow::onActivateOutOfOrderSchedulingClicked() {
    ActiveDevicePropertie->setOutOfOrderExecution(ui.OutOfOrderScheduling->checkState() == Qt::Checked);
}

void MainWindow::onCSVExportClicked() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export CSV"), "", tr("File (*.csv);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }
        QTextStream out(&file);
        for (int i = 0; i < ScheduleTimeModel->rowCount(); i++) {
            for (int j = 0; j < ScheduleTimeModel->columnCount(); j++) {
                if (j == ScheduleTimeModel->columnCount() - 1) {
                    out << ScheduleTimeModel->item(i, j)->text() << "\n";
                } else {
                    out << ScheduleTimeModel->item(i, j)->text() << "; ";
                }
            }
        }
    }
}
