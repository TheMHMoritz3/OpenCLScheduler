#pragma once

#include "ui_MainWindow.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include "src/Scheduler/ScheduleManager.h"
#include "src/Scheduler/DevicePropertys.h"
#include "src/Scheduler/Task.h"
#include "TaskTabWidget.h"
#include "src/CAN/CanManager.h"

namespace SCHEDULER {
	class Device;
}
namespace UI {
	class MainWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		MainWindow(QWidget* parent = NULL);
		~MainWindow();

	private slots:
		void multiThreaddingCheckstateChanged();
		void loadPreset();
		void addKernel();

		void deviceComboboxChanged();
		void onCoreCountChanged();
		void onSchedulingTypeChanged();
		void onTasksToScheduleItemClicked(QStandardItem* item);
		void startSchedule();

	private:
		Ui::MainWindow ui;

		void fillStartUI();
		void makeConnections();
		void updateTasksModel();
		void readDeviceData(std::string deviceName);
		void addTaskToScheduledTasks(std::string taskName);

		void decorateAllDevices();

		QStandardItemModel* const TasksToScheduleModel;
		QStandardItemModel* const ScheduleTimeModel;
		SCHEDULER::ScheduleManager* ScheduleManager;
		std::vector<SCHEDULER::DeviceProperties*> Devices;
		SCHEDULER::DeviceProperties* ActiveDevicePropertie;
		std::vector<SCHEDULER::Task*> Tasks;
		std::vector<TaskTabWidget*> TaskWidgets;
		CAN::CanManager* CanManager;
	};
}
