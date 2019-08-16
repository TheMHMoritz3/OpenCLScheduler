#include "TaskTabWidget.h"
#include "RandomNumberGenerator.h"


using namespace SCHEDULER;
using namespace UI;

TaskTabWidget::TaskTabWidget(SCHEDULER::Task* task, QWidget* parent) :
	QWidget(parent)
{
	Task = task;
	ui.setupUi(this);
	ui.retranslateUi(this);
	model = new QStandardItemModel();
	makeConnections();
	decorateForTask();
	ui.DataOfTaskTableView->setModel(model);
}

void TaskTabWidget::readValuesFromTask()
{
	std::pair<Type, std::vector<std::vector<void*>>> data = Task->getReturnData();
	
	for(std::vector<void*> internalData : data.second)
	{
		QList<QStandardItem*> column;
		for (void* dataPoint : internalData) {
			float dataPointf = *((float*)dataPoint);
			QStandardItem* item = new QStandardItem(tr("%1").arg(dataPointf));
			column.append(item);
		}
		model->appendColumn(column);
	}
}

void TaskTabWidget::makeConnections()
{
	connect(ui.GenerateDataButton, SIGNAL(clicked()), this, SLOT(generateDataTriggered()));
}

void TaskTabWidget::decorateForTask()
{

}

void TaskTabWidget::generateDataTriggered()
{
	Task->setLoad(ui.LoadSpinBox->value());
	std::vector<void*> data = RandomNumberGenerator::generateRandomNumbers(ui.LoadSpinBox->value(),Type::UINT);
	Task->addData(data, Type::INT);
	QList<QStandardItem*> items;
	for(void* dataPoint : data)
	{
		int iDataPoint = *((int*)dataPoint);
		QStandardItem* item = new QStandardItem(tr("%1").arg(iDataPoint));
		items.append(item);
	}
	model->appendColumn(items);
}


