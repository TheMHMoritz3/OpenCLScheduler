#pragma once


#include <QWidget>
#include "src/Scheduler/Task.h"
#include "ui_TaskTabWidget.h"

class TaskTabWidget : public QWidget
{
	Q_OBJECT
public:
	TaskTabWidget(SCHEDULER::Task* task, QWidget* parent = NULL);

private:
	void makeConnections();
	void decorateForTask();
	Ui::TaskTabWidget ui;
	SCHEDULER::Task* Task;
};

