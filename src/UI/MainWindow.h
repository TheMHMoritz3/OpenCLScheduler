#pragma once

#include "ui_MainWindow.h"
#include <QMainWindow>
#include "src/Scheduler/ScheduleManager.h"
#include "src/Scheduler/DevicePropertys.h"
#include "src/Scheduler/Task.h"

namespace SCHEDULER {
	class Device;
}

class MainWindow : public QMainWindow 
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = NULL);

private slots:
	void multiThreaddingCheckstateChanged();
	void loadPreset();

private:
	Ui::MainWindow ui;

	void fillStartUI();
	void makeConnections();
	void updateTasksModel();

	SCHEDULER::ScheduleManager* ScheduleManager;
	std::vector<SCHEDULER::DeviceProperties*> Devices;
	std::vector<SCHEDULER::Task*> Tasks;
};

