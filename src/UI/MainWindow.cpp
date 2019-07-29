#include "MainWindow.h"
#include "src/Scheduler/Device.h"
#include <QStandardItemModel>
#include "TaskTabWidget.h"

using namespace SCHEDULER;

MainWindow::MainWindow(QWidget* parent):
QMainWindow(parent)
{
	ui.setupUi(this);
	ui.retranslateUi(this);
	fillStartUI();
	makeConnections();
}

void MainWindow::multiThreaddingCheckstateChanged()
{
	ui.spinBox->setEnabled(ui.MultiThreaddedRadioButton->isChecked());
}

void MainWindow::loadPreset()
{
	int DefaultStaticModeLoad = 10;
	//Task xAxis

	SCHEDULER::Task* xAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "xAxis");
	xAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
	xAxis->setLoad(DefaultStaticModeLoad);
	xAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(xAxis, DefaultStaticModeLoad, 5);
	Tasks.emplace_back(xAxis);
	//Task yAxis

	SCHEDULER::Task* yAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "yAxis");
	yAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
	yAxis->setLoad(DefaultStaticModeLoad);
	yAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(yAxis, DefaultStaticModeLoad, 6);
	Tasks.emplace_back(yAxis);
	//Task dualAxis
	//decorateNormalMessage("dualAxis");
	SCHEDULER::Task* dualAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "dualAxis");
	dualAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
	dualAxis->setLoad(DefaultStaticModeLoad);
	dualAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(dualAxis, DefaultStaticModeLoad, 5);
	//addCanMethod(dualAxis, DefaultStaticModeLoad, 6);
	Tasks.emplace_back(dualAxis);
	//Task batteryCalc
	//decorateNormalMessage("batteryCalc");
	SCHEDULER::Task* batteryCalc = ScheduleManager->addTask("kernels/battery_kernel.cl", "batteryCalc");
	batteryCalc->setReturnDataType(SCHEDULER::Type::FLOAT);
	batteryCalc->setLoad(DefaultStaticModeLoad);
	batteryCalc->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(batteryCalc, DefaultStaticModeLoad, 4);
	Tasks.emplace_back(batteryCalc);
	//Task temp
	//decorateNormalMessage("temp");
	SCHEDULER::Task* temp = ScheduleManager->addTask("kernels/temp_kernel.cl", "temp");
	temp->setReturnDataType(SCHEDULER::Type::FLOAT);
	temp->setLoad(DefaultStaticModeLoad);
	temp->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(temp, DefaultStaticModeLoad, 7);
	Tasks.emplace_back(temp);
	//Task speedCalcFR
	//decorateNormalMessage("speedCalcFR");
	SCHEDULER::Task* speedCalcFR = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
	speedCalcFR->setReturnDataType(SCHEDULER::Type::FLOAT);
	speedCalcFR->setLoad(DefaultStaticModeLoad);
	speedCalcFR->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(speedCalcFR, DefaultStaticModeLoad, 0);
	Tasks.emplace_back(speedCalcFR);
	//Task speedCalcFL
	//decorateNormalMessage("speedCalcFL");
	SCHEDULER::Task* speedCalcFL = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
	speedCalcFL->setReturnDataType(SCHEDULER::Type::FLOAT);
	speedCalcFL->setLoad(DefaultStaticModeLoad);
	speedCalcFL->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(speedCalcFL, DefaultStaticModeLoad, 1);
	Tasks.emplace_back(speedCalcFL);
	//Task speedCalcRR
	//decorateNormalMessage("speedCalcRR");
	SCHEDULER::Task* speedCalcRR = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
	speedCalcRR->setReturnDataType(SCHEDULER::Type::FLOAT);
	speedCalcRR->setLoad(DefaultStaticModeLoad);
	speedCalcRR->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(speedCalcRR, DefaultStaticModeLoad, 3);
	Tasks.emplace_back(speedCalcRR);
	//Task speedCalcRL
	//decorateNormalMessage("speedCalcRL");
	SCHEDULER::Task* speedCalcRL = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
	speedCalcRL->setReturnDataType(SCHEDULER::Type::FLOAT);
	speedCalcRL->setLoad(DefaultStaticModeLoad);
	speedCalcRL->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(speedCalcRL, DefaultStaticModeLoad, 2);
	Tasks.emplace_back(speedCalcRL);
	//Task median
	SCHEDULER::Task* median = ScheduleManager->addTask("kernels/speed_kernel.cl", "median");
	median->setReturnDataType(SCHEDULER::Type::FLOAT);
	median->setLoad(DefaultStaticModeLoad);
	median->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	median->addDependandTask(speedCalcFL);
	median->addDependandTask(speedCalcFR);
	Tasks.emplace_back(median);
	//Task range
	SCHEDULER::Task* range = ScheduleManager->addTask("kernels/range_kernel.cl", "dist");
	range->setReturnDataType(SCHEDULER::Type::FLOAT);
	range->setLoad(DefaultStaticModeLoad);
	range->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	range->addDependandTask(batteryCalc);
	range->addDependandTask(median);
	Tasks.emplace_back(range);
	//Task tractionControl
	SCHEDULER::Task* tractionControl = ScheduleManager->addTask("kernels/traction_kernel.cl", "tractionControl");
	tractionControl->setReturnDataType(SCHEDULER::Type::INT);
	tractionControl->setLoad(DefaultStaticModeLoad);
	tractionControl->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	tractionControl->addDependandTask(median);
	tractionControl->addDependandTask(speedCalcRL);
	tractionControl->addDependandTask(speedCalcRR);
	Tasks.emplace_back(tractionControl);
	//Task turningRadius
	SCHEDULER::Task* turningRadius = ScheduleManager->addTask("kernels/turn_radius_kernel.cl", "radius");
	turningRadius->setReturnDataType(SCHEDULER::Type::FLOAT);
	turningRadius->setLoad(DefaultStaticModeLoad);
	turningRadius->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	turningRadius->addDependandTask(speedCalcFR);
	turningRadius->addDependandTask(speedCalcFL);
	Tasks.emplace_back(turningRadius);
	//Task distanceTracker
	SCHEDULER::Task* distanceTracker = ScheduleManager->addTask("kernels/distanceTracker.cl", "distanceTracker");
	distanceTracker->setReturnDataType(SCHEDULER::Type::FLOAT);
	distanceTracker->setLoad(DefaultStaticModeLoad);
	distanceTracker->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	distanceTracker->addDependandTask(median);
	Tasks.emplace_back(distanceTracker);
	//Task cruiseControl
	SCHEDULER::Task* cruiseControl = ScheduleManager->addTask("kernels/cruiseControl.cl", "cruiseControl");
	cruiseControl->setReturnDataType(SCHEDULER::Type::FLOAT);
	cruiseControl->setLoad(DefaultStaticModeLoad);
	cruiseControl->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	cruiseControl->addDependandTask(xAxis);
	cruiseControl->addDependandTask(median);
	Tasks.emplace_back(cruiseControl);

	updateTasksModel();
}

void MainWindow::fillStartUI()
{
	ScheduleManager = new SCHEDULER::ScheduleManager();
	ScheduleManager->searchForDevices();
	for (int i = 0; i<ScheduleManager->getDeviceCount(); i++)
	{
		DeviceProperties* props = ScheduleManager->getDeviceProperties(i);
		Devices.push_back(props);
		ui.DeviceCombobox->addItem(props->getName().c_str());
	}
}

void MainWindow::makeConnections()
{
	connect(ui.MultiThreaddedRadioButton, SIGNAL(clicked()), this, SLOT(multiThreaddingCheckstateChanged()));
	connect(ui.SingleThreaddedRadioButton, SIGNAL(clicked()), this, SLOT(multiThreaddingCheckstateChanged()));
	connect(ui.actionLoad_Preset, SIGNAL(triggered(bool)), this, SLOT(loadPreset()));
}

void MainWindow::updateTasksModel()
{
	QStandardItemModel* itemModel = new QStandardItemModel();
	
	for(Task* task : Tasks)
	{
		QStandardItem* item = new QStandardItem(task->getKernelName().c_str());
		item->setCheckable(true);
		itemModel->invisibleRootItem()->appendRow(item);
		TaskTabWidget *widget = new TaskTabWidget(task, this);
		ui.TasksWidget->addTab(widget, task->getKernelName().c_str());
	}

	ui.TasksListView->setModel(itemModel);
}
