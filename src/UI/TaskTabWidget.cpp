#include "TaskTabWidget.h"
#include "RandomNumberGenerator.h"
#include "../Scheduler/KernelFileParser.h"
#include <QDebug>


//using namespace SCHEDULER;
using namespace UI;

TaskTabWidget::TaskTabWidget(SCHEDULER::Task* task, QWidget* parent) :
	QWidget(parent)
{
	Task = task;
	Ui.setupUi(this);
	Ui.retranslateUi(this);
	Model = new QStandardItemModel();
	ExecutionTimeModel = new QStandardItemModel();
	makeConnections();
	decorateForTask();
	Ui.DataOfTaskTableView->setModel(Model);
	Ui.ExecutionTimeTableView->setModel(ExecutionTimeModel);
}

TaskTabWidget::~TaskTabWidget()
{
	delete Model;
}

void TaskTabWidget::refresh()
{
	switch (Task->dependancyType()) {
	case SCHEDULER::OtherTask:
		Ui.DiffrentTaksRadioButton->setChecked(true);
		diffrentTasksChecked();
		break;
	case SCHEDULER::OutsideDependancy:
		Ui.CanRadioButton->setChecked(true);
		canBusActivated();
		break;
	case SCHEDULER::UserInput:
	default:;
	}

	decorateForTask();
}

void TaskTabWidget::readValuesFromTask()
{
	std::pair<SCHEDULER::Type, std::vector<std::vector<void*>>> data = Task->getReturnData();
	int i = 1;
	for (std::vector<void*> internalData : data.second)
	{
		switch (data.first) {
		case SCHEDULER::Type::INT:
			readIntDataFromTask(internalData);
			break;
		case SCHEDULER::Type::UINT:
			readUIntDataFromTask(internalData);
			break;
		case SCHEDULER::Type::STRING:
		case SCHEDULER::Type::CHAR:
			readCharDataFromTask(internalData);
			break;
		case SCHEDULER::Type::FLOAT:
			readFloatDataFromTask(internalData);
			break;
		case SCHEDULER::Type::DOUBLE:
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
	QStringList headerData;
	headerData.append(tr("ExecutionTimes"));
	ExecutionTimeModel->setHorizontalHeaderLabels(headerData);
}

void TaskTabWidget::decorateForTask()
{
	HeaderList.clear();
	Model->clear();

	qDebug() << Task->getKernelName().c_str() << ": " << Task->hasDependencies() << " - " << Task->dependenciesAreCalculated();

	if (!Task->hasDependencies())
		readDataFromTask();
	else if (Task->dependenciesAreCalculated())
		readDataFromTask();


	for (std::string value : Task->kernelArguments())
		HeaderList.append(QString::fromStdString(value).split(" ").last());

	readValuesFromTask();

	Model->setHorizontalHeaderLabels(HeaderList);

	generateExecutionTimeDiagramm();
}

void TaskTabWidget::readDataFromTask()
{
	auto TaskAllData = Task->getAllData();
	for (auto TaskDataSet : TaskAllData)
	{
		switch (TaskDataSet.first) {
		case SCHEDULER::Type::INT:
			readIntDataFromTask(TaskDataSet.second);
			break;
		case SCHEDULER::Type::UINT:
			readUIntDataFromTask(TaskDataSet.second);
			break;
		case SCHEDULER::Type::STRING:
		case SCHEDULER::Type::CHAR:
			readCharDataFromTask(TaskDataSet.second);
			break;
		case SCHEDULER::Type::FLOAT:
			readFloatDataFromTask(TaskDataSet.second);
			break;
		case SCHEDULER::Type::DOUBLE:
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
		unsigned int iDataPoint = *((unsigned int*)data.at(i));
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
		float iDataPoint = *((float*)data.at(i));
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
		char iDataPoint = *((char*)data.at(i));
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
		double iDataPoint = *((double*)data.at(i));
		QStandardItem* item = new QStandardItem(tr("%1").arg(iDataPoint));
		items.append(item);

	}
	Model->appendColumn(items);
}

void TaskTabWidget::generateExecutionTimeDiagramm()
{
	if(Task->elapsedTime()>0.0)
	{
		QStandardItem* item = new QStandardItem();
		item->setText(tr("%1").arg(Task->elapsedTime()));
		ExecutionTimeModel->appendRow(item);
	}
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
		std::vector<void*> data = RandomNumberGenerator::generateRandomNumbers(Ui.LoadSpinBox->value(), SCHEDULER::Type::UINT);
		Task->addData(data, SCHEDULER::Type::INT);
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
		Task->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
	}
}

void TaskTabWidget::randomNumberChecked()
{
	if (Ui.RandomNumberRadioButton->isChecked())
	{
		Ui.LoadSpinBox->setEnabled(true);
		Ui.stackedWidget->setCurrentIndex(2);
		clear();
		Task->setDataDependancy(SCHEDULER::DependancyType::UserInput);
	}
}

void TaskTabWidget::canBusActivated()
{
	if (Ui.CanRadioButton->isChecked())
	{
		Ui.LoadSpinBox->setEnabled(true);
		Ui.stackedWidget->setCurrentIndex(1);
		clear();
		Task->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
	}
}