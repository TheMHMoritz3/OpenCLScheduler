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

void TaskTabWidget::makeConnections()
{
	connect(ui.GenerateDataButton, SIGNAL(clicked()), this, SLOT(generateDataTriggered()));
}

void TaskTabWidget::decorateForTask()
{

}

void TaskTabWidget::generateDataTriggered()
{
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

