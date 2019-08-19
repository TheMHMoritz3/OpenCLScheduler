#pragma once


#include <QWidget>
#include "src/Scheduler/Task.h"
#include "src/CAN/CanManager.h"
#include "src/CAN/CanNamespace.h"
#include "ui_TaskTabWidget.h"
#include <QStandardItemModel>


class TaskTabWidget : public QWidget
{
	Q_OBJECT
public:
	TaskTabWidget(SCHEDULER::Task* task, QWidget* parent = NULL);
	~TaskTabWidget();
	void refresh();

	void setCanManager(CAN::CanManager* canManager);

	void updateCoreCount(int coreCount);
private:
	void makeConnections();
	void decorateForTask();
	void readDataFromTask();
	void readValuesFromTask();

	void loadCanData(CAN::CanID canId, int load);

	void readUIntDataFromTask(std::vector<void*> data);
	void readIntDataFromTask(std::vector<void*> data);
	void readFloatDataFromTask(std::vector<void*> data);
	void readCharDataFromTask(std::vector<void*> data);
	void readDoubleDataFromTask(std::vector<void*> data);

	void generateExecutionTimeDiagramm();

	void clear();
	Ui::TaskTabWidget Ui;
	SCHEDULER::Task* Task;
	QStandardItemModel* Model;
	QStandardItemModel* ExecutionTimeModel;
	QStringList HeaderList;
	CAN::CanManager* CanManager;
private slots:
	void generateDataTriggered();
	void diffrentTasksChecked();
	void randomNumberChecked();
	void canBusActivated();
	void readDataFromBusClicked();
};

