#pragma once


#include <QWidget>
#include "src/Scheduler/Task.h"
#include "ui_TaskTabWidget.h"
#include <QStandardItemModel>

class TaskTabWidget : public QWidget
{
	Q_OBJECT
public:
	TaskTabWidget(SCHEDULER::Task* task, QWidget* parent = NULL);
	~TaskTabWidget();
	void refresh();

private:
	void makeConnections();
	void decorateForTask();
	void readDataFromTask();
	void readValuesFromTask();

	void readUIntDataFromTask(std::vector<void*> data);
	void readIntDataFromTask(std::vector<void*> data);
	void readFloatDataFromTask(std::vector<void*> data);
	void readCharDataFromTask(std::vector<void*> data);
	void readDoubleDataFromTask(std::vector<void*> data);

	void clear();
	Ui::TaskTabWidget Ui;
	SCHEDULER::Task* Task;
	QStandardItemModel* Model;
	QStringList HeaderList;
private slots:
	void generateDataTriggered();
	void diffrentTasksChecked();
	void randomNumberChecked();
	void canBusActivated();
};

