#include "MainWindow.h"
#include "src/Scheduler/Device.h"

using namespace SCHEDULER;

MainWindow::MainWindow(QWidget* parent):
QMainWindow(parent)
{
	ui.setupUi(this);
	ui.retranslateUi(this);
	fillStartUI();
}

void MainWindow::fillStartUI()
{
	ScheduleManager = new SCHEDULER::ScheduleManager();
	ScheduleManager->searchForDevices();
	for (int i = 0; i<ScheduleManager->getDeviceCount(); i++)
	{
		ui.DeviceCombobox->addItem(ScheduleManager->getDeviceName(i).c_str());
	}
}
