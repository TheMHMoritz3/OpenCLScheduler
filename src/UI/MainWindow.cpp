#include "MainWindow.h"
#include "src/Scheduler/Device.h"

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
}

void MainWindow::updateTasksModel()
{

}
