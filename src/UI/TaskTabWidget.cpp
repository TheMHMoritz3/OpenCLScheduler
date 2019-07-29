#include "TaskTabWidget.h"


using namespace SCHEDULER;


TaskTabWidget::TaskTabWidget(SCHEDULER::Task* task, QWidget* parent) :
	QWidget(parent)
{
	Task = task;
	ui.setupUi(this);
	ui.retranslateUi(this);
	makeConnections();
	decorateForTask();
}

void TaskTabWidget::makeConnections()
{
	
}

void TaskTabWidget::decorateForTask()
{

}

