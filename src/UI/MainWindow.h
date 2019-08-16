#pragma once

#include "ui_MainWindow.h"
#include <QMainWindow>
#include "src/Scheduler/ScheduleManager.h"
#include "src/Scheduler/DevicePropertys.h"
#include "src/Scheduler/Task.h"
#include "TaskTabWidget.h"

namespace SCHEDULER {
	class Device;
}
namespace UI {
	class MainWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		MainWindow(QWidget* parent = NULL);

	private slots:
		void multiThreaddingCheckstateChanged();
		void loadPreset();
		void addKernel();

		void schedulePressed();
		void deviceComboboxChanged();

	private:
		Ui::MainWindow ui;

		void fillStartUI();
		void makeConnections();
		void updateTasksModel();
		void readDeviceData(std::string deviceName);


		SCHEDULER::ScheduleManager* ScheduleManager;
		std::vector<SCHEDULER::DeviceProperties*> Devices;
		std::vector<SCHEDULER::Task*> Tasks;
		std::vector<TaskTabWidget*> TaskWidgets;
	};
}
