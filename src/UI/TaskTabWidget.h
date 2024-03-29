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
	void setTaskModel(std::vector<SCHEDULER::Task*> tasks);

	void updateCoreCount(int coreCount);
private:
	void makeConnections();
	void decorateForTask();
	void readDataFromTask();
	void readConstantsFromTask();
	void readValuesFromTask();

	void loadCanData(CAN::CanID canId, int load);

	void readUIntDataFromTask(std::vector<void*> data);
	void readIntDataFromTask(std::vector<void*> data);
	void readFloatDataFromTask(std::vector<void*> data);
	void readCharDataFromTask(std::vector<void*> data);
	void readDoubleDataFromTask(std::vector<void*> data);

	void readUINTConstantFromTask(void* data);
    void readINTConstantFromTask(void* data);
    void readFLOATConstantFromTask(void* data);
    void readCHARConstantFromTask(void* data);
    void readDOUBLEConstantFromTask(void* data);

	void generateExecutionTimeDiagramm();

	void clear();

	void generateGraph();
	Ui::TaskTabWidget Ui;
	SCHEDULER::Task* Task;
	QStandardItemModel* Model;
	QStandardItemModel* ExecutionTimeModel;
	QStandardItemModel* ConstantModel;
	QStandardItemModel* TasksModel;
	QStringList HeaderList;
	QStringList ConstantModelHeaderList;
	CAN::CanManager* CanManager;
    std::vector<SCHEDULER::Task*> Tasks;
private slots:
	void generateDataTriggered();
	void diffrentTasksChecked();
	void randomNumberChecked();
	void canBusActivated();
	void readDataFromBusClicked();
	void addConstantClicked();
	void onItemChanged(QStandardItem* item);
	void onDataExportClicked();
	void onTimeExportClicked();
};

