#include "TaskTabWidget.h"
#include "RandomNumberGenerator.h"
#include "../Scheduler/KernelFileParser.h"

using namespace SCHEDULER;
using namespace UI;

TaskTabWidget::TaskTabWidget(SCHEDULER::Task* task, QWidget* parent) :
	QWidget(parent)
{
	Task = task;
	Ui.setupUi(this);
	Ui.retranslateUi(this);
	Model = new QStandardItemModel();
	makeConnections();
	decorateForTask();
	Ui.DataOfTaskTableView->setModel(Model);
}

TaskTabWidget::~TaskTabWidget()
{
	delete Model;
}

void TaskTabWidget::readValuesFromTask()
{
	std::pair<Type, std::vector<std::vector<void*>>> data = Task->getReturnData();
	int i = 1;
	for (std::vector<void*> internalData : data.second)
	{
		switch (data.first) {
		case Type::INT:
			readIntDataFromTask(internalData);
			break;
		case Type::UINT:
			readUIntDataFromTask(internalData);
			break;
		case Type::STRING:
		case Type::CHAR:
			readCharDataFromTask(internalData);
			break;
		case Type::FLOAT:
			readFloatDataFromTask(internalData);
			break;
		case Type::DOUBLE:
			readDoubleDataFromTask(internalData);
			break;
		default:;
		}
		HeaderList.append(tr("Result: %1").arg(i));
		i++;
	}
}

void TaskTabWidget::makeConnections()
{
	connect(Ui.DiffrentTaksRadioButton, SIGNAL(clicked()), this, SLOT(diffrentTasksChecked()));
	connect(Ui.RandomNumberRadioButton, SIGNAL(clicked()), this, SLOT(randomNumberChecked()));
	connect(Ui.CanRadioButton, SIGNAL(clicked()), this, SLOT(canBusActivated()));
	connect(Ui.GenerateDataButton, SIGNAL(clicked()), this, SLOT(generateDataTriggered()));
}

void TaskTabWidget::decorateForTask()
{
	HeaderList.clear();

	if (!Task->hasDependencies())
		readDataFromTask();
	else if (Task->dependenciesAreCalculated())
		readValuesFromTask();


	for (std::string value : Task->kernelArguments())
		HeaderList.append(QString::fromStdString(value).split(" ").last());

	readValuesFromTask();

	Model->setHorizontalHeaderLabels(HeaderList);

	switch (Task->dependancyType()) {
		case OtherTask: 
			Ui.DiffrentTaksRadioButton->setChecked(true);
			diffrentTasksChecked();
			break;
		case OutsideDependancy:
			Ui.CanRadioButton->setChecked(true);
			canBusActivated();
		break;
		case UserInput:
		default:;
	}
}

void TaskTabWidget::readDataFromTask()
{
	auto TaskAllData = Task->getAllData();
	for (auto TaskDataSet : TaskAllData)
	{
		switch (TaskDataSet.first) {
		case Type::INT:
			readIntDataFromTask(TaskDataSet.second);
			break;
		case Type::UINT:
			readUIntDataFromTask(TaskDataSet.second);
			break;
		case Type::STRING:
		case Type::CHAR:
			readCharDataFromTask(TaskDataSet.second);
			break;
		case Type::FLOAT:
			readFloatDataFromTask(TaskDataSet.second);
			break;
		case Type::DOUBLE:
			readDoubleDataFromTask(TaskDataSet.second);
			break;
		default:;
		}
	}
}

void TaskTabWidget::readUIntDataFromTask(std::vector<void*> data)
{
	QList<QStandardItem*> items;
	for (int i = 0; i < data.size(); i++)
	{
		int iDataPoint = *((unsigned int*)data.at(i));
		QStandardItem* item = new QStandardItem(tr("%1").arg(iDataPoint));
		items.append(item);

	}
	Model->appendColumn(items);
}

void TaskTabWidget::readIntDataFromTask(std::vector<void*> data)
{
	QList<QStandardItem*> items;
	for (int i = 0; i < data.size(); i++)
	{
		int iDataPoint = *((int*)data.at(i));
		QStandardItem* item = new QStandardItem(tr("%1").arg(iDataPoint));
		items.append(item);

	}
	Model->appendColumn(items);
}

void TaskTabWidget::readFloatDataFromTask(std::vector<void*> data)
{
	QList<QStandardItem*> items;
	for (int i = 0; i < data.size(); i++)
	{
		int iDataPoint = *((float*)data.at(i));
		QStandardItem* item = new QStandardItem(tr("%1").arg(iDataPoint));
		items.append(item);

	}
	Model->appendColumn(items);
}

void TaskTabWidget::readCharDataFromTask(std::vector<void*> data)
{
	QList<QStandardItem*> items;
	for (int i = 0; i < data.size(); i++)
	{
		int iDataPoint = *((char*)data.at(i));
		QStandardItem* item = new QStandardItem(tr("%1").arg(iDataPoint));
		items.append(item);

	}
	Model->appendColumn(items);
}

void TaskTabWidget::readDoubleDataFromTask(std::vector<void*> data)
{
	QList<QStandardItem*> items;
	for (int i = 0; i < data.size(); i++)
	{
		int iDataPoint = *((double*)data.at(i));
		QStandardItem* item = new QStandardItem(tr("%1").arg(iDataPoint));
		items.append(item);

	}
	Model->appendColumn(items);
}

void TaskTabWidget::clear()
{
	Model->clear();
}

void TaskTabWidget::generateDataTriggered()
{
	clear();
	Task->setLoad(Ui.LoadSpinBox->value());
	for (int j = 0; j < Task->kernelArguments().size(); j++) {
		std::vector<void*> data = RandomNumberGenerator::generateRandomNumbers(Ui.LoadSpinBox->value(), Type::UINT);
		Task->addData(data, Type::INT);
	}
	decorateForTask();
}

void TaskTabWidget::diffrentTasksChecked()
{
	if (Ui.DiffrentTaksRadioButton->isChecked())
	{
		Ui.LoadSpinBox->setEnabled(false);
		Ui.stackedWidget->setCurrentIndex(0);
		clear();
	}
}

void TaskTabWidget::randomNumberChecked()
{
	if (Ui.RandomNumberRadioButton->isChecked())
	{
		Ui.LoadSpinBox->setEnabled(true);
		Ui.stackedWidget->setCurrentIndex(2);
		clear();
	}
}

void TaskTabWidget::canBusActivated()
{
	if (Ui.CanRadioButton->isChecked())
	{
		Ui.LoadSpinBox->setEnabled(true);
		Ui.stackedWidget->setCurrentIndex(1);
		clear();
	}
}


