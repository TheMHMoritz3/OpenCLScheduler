#include "MainWindow.h"
#include "src/Scheduler/Device.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>
#include "OpenKernelFileWizard.h"
#include <sstream>

using namespace SCHEDULER;
using namespace UI;

MainWindow::MainWindow(QWidget* parent):
QMainWindow(parent),
TasksToScheduleModel(new QStandardItemModel()),
ActiveDevicePropertie(nullptr),
CanManager(new CAN::CanManager())
{
	ui.setupUi(this);
	ui.retranslateUi(this);
	ui.TasksListView->setModel(TasksToScheduleModel);
	fillStartUI();
	makeConnections();
}

MainWindow::~MainWindow()
{
	delete TasksToScheduleModel;
}

void MainWindow::multiThreaddingCheckstateChanged()
{
	ui.CoreCountSpinBox->setEnabled(ui.MultiThreaddedRadioButton->isChecked());
	if(!ui.MultiThreaddedRadioButton->isChecked())
	{
		ActiveDevicePropertie->setCoreCount(1);
	}
	else
		deviceComboboxChanged();
}

void MainWindow::loadPreset()
{
	SCHEDULER::Task* xAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "xAxis");
	xAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
	xAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(xAxis, DefaultStaticModeLoad, 5);
	Tasks.emplace_back(xAxis);

	SCHEDULER::Task* yAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "yAxis");
	yAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
	yAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(yAxis, DefaultStaticModeLoad, 6);
	Tasks.emplace_back(yAxis);

	SCHEDULER::Task* dualAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "dualAxis");
	dualAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
	dualAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(dualAxis, DefaultStaticModeLoad, 5);
	//addCanMethod(dualAxis, DefaultStaticModeLoad, 6);
	Tasks.emplace_back(dualAxis);

	SCHEDULER::Task* batteryCalc = ScheduleManager->addTask("kernels/battery_kernel.cl", "batteryCalc");
	batteryCalc->setReturnDataType(SCHEDULER::Type::FLOAT);
	batteryCalc->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(batteryCalc, DefaultStaticModeLoad, 4);
	Tasks.emplace_back(batteryCalc);

	SCHEDULER::Task* temp = ScheduleManager->addTask("kernels/temp_kernel.cl", "temp");
	temp->setReturnDataType(SCHEDULER::Type::FLOAT);
	temp->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(temp, DefaultStaticModeLoad, 7);
	Tasks.emplace_back(temp);

	SCHEDULER::Task* speedCalcFR = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
	speedCalcFR->setReturnDataType(SCHEDULER::Type::FLOAT);
	speedCalcFR->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(speedCalcFR, DefaultStaticModeLoad, 0);
	Tasks.emplace_back(speedCalcFR);

	SCHEDULER::Task* speedCalcFL = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
	speedCalcFL->setReturnDataType(SCHEDULER::Type::FLOAT);
	speedCalcFL->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(speedCalcFL, DefaultStaticModeLoad, 1);
	Tasks.emplace_back(speedCalcFL);

	SCHEDULER::Task* speedCalcRR = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
	speedCalcRR->setReturnDataType(SCHEDULER::Type::FLOAT);
	speedCalcRR->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(speedCalcRR, DefaultStaticModeLoad, 3);
	Tasks.emplace_back(speedCalcRR);

	SCHEDULER::Task* speedCalcRL = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
	speedCalcRL->setReturnDataType(SCHEDULER::Type::FLOAT);
	speedCalcRL->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	//addCanMethod(speedCalcRL, DefaultStaticModeLoad, 2);
	Tasks.emplace_back(speedCalcRL);
	
	SCHEDULER::Task* median = ScheduleManager->addTask("kernels/speed_kernel.cl", "median");
	median->setReturnDataType(SCHEDULER::Type::FLOAT);
	median->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	median->addDependandTask(speedCalcFL);
	median->addDependandTask(speedCalcFR);
	Tasks.emplace_back(median);
	
	SCHEDULER::Task* range = ScheduleManager->addTask("kernels/range_kernel.cl", "dist");
	range->setReturnDataType(SCHEDULER::Type::FLOAT);
	range->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	range->addDependandTask(batteryCalc);
	range->addDependandTask(median);
	Tasks.emplace_back(range);
	
	SCHEDULER::Task* tractionControl = ScheduleManager->addTask("kernels/traction_kernel.cl", "tractionControl");
	tractionControl->setReturnDataType(SCHEDULER::Type::INT);
	tractionControl->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	tractionControl->addDependandTask(median);
	tractionControl->addDependandTask(speedCalcRL);
	tractionControl->addDependandTask(speedCalcRR);
	Tasks.emplace_back(tractionControl);
	
	SCHEDULER::Task* turningRadius = ScheduleManager->addTask("kernels/turn_radius_kernel.cl", "radius");
	turningRadius->setReturnDataType(SCHEDULER::Type::FLOAT);
	turningRadius->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	turningRadius->addDependandTask(speedCalcFR);
	turningRadius->addDependandTask(speedCalcFL);
	Tasks.emplace_back(turningRadius);
	
	SCHEDULER::Task* distanceTracker = ScheduleManager->addTask("kernels/distanceTracker.cl", "distanceTracker");
	distanceTracker->setReturnDataType(SCHEDULER::Type::FLOAT);
	distanceTracker->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	distanceTracker->addDependandTask(median);
	Tasks.emplace_back(distanceTracker);
	
	SCHEDULER::Task* cruiseControl = ScheduleManager->addTask("kernels/cruiseControl.cl", "cruiseControl");
	cruiseControl->setReturnDataType(SCHEDULER::Type::FLOAT);
	cruiseControl->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	cruiseControl->addDependandTask(xAxis);
	cruiseControl->addDependandTask(median);
	Tasks.emplace_back(cruiseControl);

	updateTasksModel();
}

void MainWindow::addKernel()
{
	OpenKernelFileWizard* wizard = new OpenKernelFileWizard(this);
	wizard->exec();
}

void MainWindow::deviceComboboxChanged()
{
	if (ui.DeviceCombobox->currentIndex() >= Devices.size())
		decorateAllDevices();
	else {
		if ((ActiveDevicePropertie==nullptr)||(ActiveDevicePropertie->getName() != ui.DeviceCombobox->currentText().toStdString())) {
			QString deviceName = ui.DeviceCombobox->currentText();
			readDeviceData(deviceName.toStdString());
		}
	}
}

void MainWindow::onCoreCountChanged()
{
	ActiveDevicePropertie->setCoreCount(ui.CoreCountSpinBox->value());
}

void MainWindow::onSchedulingTypeChanged()
{
	ActiveDevicePropertie->setSchedule((ScheduleType)ui.SchedulingTypeSpinBox->currentIndex());
}

void MainWindow::onTasksToScheduleItemClicked(QStandardItem* item)
{
	if(item->checkState()==Qt::Checked)
	{
		addTaskToScheduledTasks(item->text().toStdString());
	}
}

void MainWindow::startSchedule()
{
	ScheduleManager->startMultiDeviceScheduling();
	for(TaskTabWidget* TaskWidget : TaskWidgets)
	{
		TaskWidget->refresh();
	}
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
	deviceComboboxChanged();

	ui.DeviceCombobox->addItem(tr("All Devices"));
}

void MainWindow::makeConnections()
{
	connect(ui.MultiThreaddedRadioButton, SIGNAL(clicked()), this, SLOT(multiThreaddingCheckstateChanged()));
	connect(ui.SingleThreaddedRadioButton, SIGNAL(clicked()), this, SLOT(multiThreaddingCheckstateChanged()));
	connect(ui.actionLoad_Preset, SIGNAL(triggered(bool)), this, SLOT(loadPreset()));
	connect(ui.actionOpen_Kernel, SIGNAL(triggered(bool)), this, SLOT(addKernel()));
	connect(ui.DeviceCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(deviceComboboxChanged()));
	connect(TasksToScheduleModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onTasksToScheduleItemClicked(QStandardItem*)));
	connect(ui.ScheduleButton, SIGNAL(clicked()), this, SLOT(startSchedule()));
	connect(ui.CoreCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onCoreCountChanged()));
}

void MainWindow::updateTasksModel()
{
	TasksToScheduleModel->clear();
	
	for(Task* task : Tasks)
	{
		QStandardItem* item = new QStandardItem(tr((task->getKernelName() + "- %1").c_str()).arg(task->getId()));
		item->setCheckable(true);
		TasksToScheduleModel->invisibleRootItem()->appendRow(item);
		TaskTabWidget *widget = new TaskTabWidget(task, this);
		ui.TasksWidget->addTab(widget, tr((task->getKernelName() + "- %1").c_str()).arg(task->getId()));
		TaskWidgets.push_back(widget);
		widget->refresh();
		widget->setCanManager(CanManager);
	}
}

void MainWindow::readDeviceData(std::string deviceName)
{
	for(DeviceProperties* devProp : Devices)
	{
		if(deviceName==devProp->getName())
		{
			ActiveDevicePropertie = devProp;
			if(devProp->getCoureCount()>1)
			{
				ui.MultiThreaddedRadioButton->setChecked(true);
				ui.CoreCountSpinBox->setValue(devProp->getCoureCount());
			}else
			{
				ui.SingleThreaddedRadioButton->setChecked(true);
			}

			ui.SchedulingTypeSpinBox->setCurrentIndex(devProp->getSchedule());

			for(Task* task : devProp->getTasksToSchedule())
			{
				for(int i = 0; i<TasksToScheduleModel->invisibleRootItem()->rowCount(); i++)
				{
					QStandardItem* item = TasksToScheduleModel->invisibleRootItem()->child(i);
					if(item->text().toStdString()==task->getKernelName())
					{
						TasksToScheduleModel->invisibleRootItem()->child(i)->setCheckState(Qt::Checked);
					}
				}
			}
		}
	}
}

void MainWindow::addTaskToScheduledTasks(std::string taskName)
{
	for(Task* task:Tasks)
	{
		std::stringstream ss;
		ss << task->getId();

		if ((task->getKernelName() + "- " + ss.str()) == taskName)
			ActiveDevicePropertie->addTaskToSchedule(task);
	}
}

void MainWindow::decorateAllDevices()
{
	QMessageBox msg;
	msg.setIcon(QMessageBox::Critical);
	msg.setText(tr("This feature is currently not Supported"));
	msg.exec();

	ui.DeviceCombobox->setCurrentIndex(0);
}
