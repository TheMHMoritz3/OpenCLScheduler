#pragma once

#include "ui_MainWindow.h"
#include <QMainWindow>
#include "src/Scheduler/ScheduleManager.h"

namespace SCHEDULER {
	class Device;
}

class MainWindow : public QMainWindow 
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = NULL);

private:
	Ui::MainWindow ui;

	void fillStartUI();

	SCHEDULER::ScheduleManager* ScheduleManager;
};

