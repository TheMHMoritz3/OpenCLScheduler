#include "TaskTabWidget.h"
#include "../Scheduler/KernelFileParser.h"
#include "ConstantDialog.h"
#include <QDebug>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_barchart.h>



TaskTabWidget::TaskTabWidget(SCHEDULER::Task* task, QWidget* parent) :
	QWidget(parent)
{
	Task = task;
	Ui.setupUi(this);
	Ui.retranslateUi(this);
	Model = new QStandardItemModel();
	ExecutionTimeModel = new QStandardItemModel();
	ConstantModel = new QStandardItemModel();
	TasksModel = new QStandardItemModel();
	Ui.DependandTasksList->setModel(TasksModel);
	ConstantModelHeaderList << tr("Value");
	ConstantModel->setHorizontalHeaderLabels(ConstantModelHeaderList);
	makeConnections();
	decorateForTask();
	Ui.DataOfTaskTableView->setModel(Model);
	Ui.ExecutionTimeTableView->setModel(ExecutionTimeModel);
	Ui.ConstantTableView->setModel(ConstantModel);
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

void TaskTabWidget::updateCoreCount(int coreCount)
{
	int modulo = Ui.LoadSpinBox->value() % coreCount;
	Ui.WarningLabel->setVisible(modulo != 0);
	Ui.DataGrupBox->setEnabled(modulo == 0);
}

void TaskTabWidget::makeConnections()
{
	connect(Ui.DiffrentTaksRadioButton, SIGNAL(clicked()), this, SLOT(diffrentTasksChecked()));
	connect(Ui.RandomNumberRadioButton, SIGNAL(clicked()), this, SLOT(randomNumberChecked()));
	connect(Ui.CanRadioButton, SIGNAL(clicked()), this, SLOT(canBusActivated()));
	connect(Ui.GenerateDataButton, SIGNAL(clicked()), this, SLOT(generateDataTriggered()));
	connect(Ui.ReadCanBusButton, SIGNAL(clicked()), this, SLOT(readDataFromBusClicked()));
	connect(Ui.AddConstantButton, SIGNAL(clicked()), this, SLOT(addConstantClicked()));
	connect(TasksModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onItemChanged(QStandardItem*)));

}

void TaskTabWidget::decorateForTask()
{
	Ui.AddConstantButton->setEnabled(Task->getTaskConstants().size() > 0);

	HeaderList.clear();
	Model->clear();

	if (!Task->hasDependencies())
		readDataFromTask();
	else if (Task->dependenciesAreCalculated())
		readDataFromTask();


	for (std::string value : Task->kernelArguments())
		HeaderList.append(QString::fromStdString(value).split(" ").last());

	readConstantsFromTask();


	readValuesFromTask();

	Model->setHorizontalHeaderLabels(HeaderList);

	generateExecutionTimeDiagramm();

	if (Task->isCalculationDone())
		generateGraph();
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
    ExecutionTimeModel->clear();
    QStringList headerData;
    headerData.append(tr("Execution Times"));
    ExecutionTimeModel->setHorizontalHeaderLabels(headerData);
	Ui.ExecutionTimes->setTitle("Execution Times");
	Ui.ExecutionTimes->setCanvasBackground(Qt::white);
	QVector<QPointF> points;
	for (int i = 0; i < Task->elapsedTime().size(); i++) {
	    qDebug()<<"Test Times: "<<Task->elapsedTime().at(i);
		if (Task->elapsedTime().at(i) > 0.0)
		{
			QStandardItem* item = new QStandardItem();
			item->setText(tr("%1").arg(Task->elapsedTime().at(i)));
			ExecutionTimeModel->appendRow(item);
			points << QPointF(i, Task->elapsedTime().at(i));
		}
	}
	Ui.ExecutionTimes->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8("Run Number"));
	Ui.ExecutionTimes->setAxisAutoScale(QwtPlot::xBottom);
	Ui.ExecutionTimes->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("Elapsed Times in ns"));
	Ui.ExecutionTimes->setAxisAutoScale(QwtPlot::yLeft);

	QwtPlotBarChart* curve = new QwtPlotBarChart();
	curve->setSamples(points);
	curve->attach(Ui.ExecutionTimes);

	Ui.ExecutionTimes->replot();
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
		std::vector<void*> data = CanManager->getValuesFromSimulation(CAN::WheelFrontLeft, Ui.LoadSpinBox->value());
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

void TaskTabWidget::readDataFromBusClicked() {
	//qDebug() << "Read Can Data";
	Task->setDataDependancy(SCHEDULER::DependancyType::UserInput);
	switch (Ui.CanAdressComboBox->currentIndex()) {
	case 0:
		loadCanData(CAN::CanID::WheelFrontRight, Ui.LoadSpinBox->value());
		break;
	case 1:
		loadCanData(CAN::CanID::WheelFrontLeft, Ui.LoadSpinBox->value());
		break;
	case 2:
		loadCanData(CAN::CanID::WheelRearRight, Ui.LoadSpinBox->value());
		break;
	case 3:
		loadCanData(CAN::CanID::WheelRearLeft, Ui.LoadSpinBox->value());
		break;
	case 4:
		loadCanData(CAN::CanID::BatteryVoltage, Ui.LoadSpinBox->value());
		break;
	case 5:
		loadCanData(CAN::CanID::AccelerationLongitudinal, Ui.LoadSpinBox->value());
		break;
	case 6:
		loadCanData(CAN::CanID::AccelerationLateral, Ui.LoadSpinBox->value());
		break;
	case 7:
		loadCanData(CAN::CanID::AccelerationLongitudinal, Ui.LoadSpinBox->value());
		break;
	case 8:
		loadCanData(CAN::CanID::Temperature, Ui.LoadSpinBox->value());
		break;
	default:
		break;
	}
	decorateForTask();
}

void TaskTabWidget::addConstantClicked()
{
	ConstantDialog* dialog = new ConstantDialog(this);
	if (dialog->exec() == QDialog::Accepted) {
		Task->addConstant(dialog->getConstantType(), dialog->constantValue());
		decorateForTask();
	}
}


void TaskTabWidget::loadCanData(CAN::CanID canId, int load) {
	CanManager->create(canId, load);
	std::vector<uint32_t*> dataSet = CanManager->getData(canId);
	uint32_t* DataSet = new uint32_t[dataSet.size()];
	std::vector<void*> taskData;
	int i = 0;
	for (uint32_t* data : dataSet)
	{
		DataSet[i] = *data;
		taskData.push_back(&DataSet[i]);
		i++;
	}
	Task->setLoad(load);
	Task->addData(taskData, SCHEDULER::UINT);
}

void TaskTabWidget::setCanManager(CAN::CanManager* canManager) {
	CanManager = canManager;
}

void TaskTabWidget::readConstantsFromTask() {
	ConstantModel->clear();
	int i = 0;
	for (std::pair<SCHEDULER::Type, void*> constant : Task->getAllConstantData()) {
		switch (constant.first) {
		case SCHEDULER::Type::INT:
			readINTConstantFromTask(constant.second);
			break;
		case SCHEDULER::Type::UINT:
			readUINTConstantFromTask(constant.second);
			break;
		case SCHEDULER::Type::CHAR:
			readCHARConstantFromTask(constant.second);
			break;
		case SCHEDULER::Type::FLOAT:
			readFLOATConstantFromTask(constant.second);
			break;
		case SCHEDULER::Type::DOUBLE:
			readDOUBLEConstantFromTask(constant.second);
			break;
		case SCHEDULER::Type::STRING:
		default:
			break;
		}
		i++;
	}
}

void TaskTabWidget::readUINTConstantFromTask(void* data) {
	QStandardItem* item = new QStandardItem();
	unsigned int ui_data = *((unsigned int*)data);
	item->setText(tr("%1").arg(ui_data));
	ConstantModel->appendRow(item);
}

void TaskTabWidget::readINTConstantFromTask(void* data) {
	QStandardItem* item = new QStandardItem();
	int i_data = *((int*)data);
	item->setText(tr("%1").arg(i_data));
	ConstantModel->appendRow(item);
}

void TaskTabWidget::readFLOATConstantFromTask(void* data) {
	QStandardItem* item = new QStandardItem();
	float _data = *((float*)data);
	item->setText(tr("%1").arg(_data));
	ConstantModel->appendRow(item);
}

void TaskTabWidget::readCHARConstantFromTask(void* data) {
	QStandardItem* item = new QStandardItem();
	char _data = *((char*)data);
	item->setText(tr("%1").arg(_data));
	ConstantModel->appendRow(item);
}

void TaskTabWidget::readDOUBLEConstantFromTask(void* data) {
	QStandardItem* item = new QStandardItem();
	double _data = *((double*)data);
	item->setText(tr("%1").arg(_data));
	ConstantModel->appendRow(item);
}

void TaskTabWidget::setTaskModel(std::vector<SCHEDULER::Task*> tasks) {
	Tasks = tasks;
	for (SCHEDULER::Task* task : tasks) {
		QStandardItem* item = new QStandardItem(tr((task->getKernelName() + " - %1").c_str()).arg(task->getId()));
		item->setCheckable(true);
		for (SCHEDULER::Task* taskd : Task->getDependantTasks()) {
			if (taskd->getId() == task->getId())
				item->setCheckState(Qt::CheckState::Checked);
		}
		TasksModel->appendRow(item);
	}
}

void TaskTabWidget::onItemChanged(QStandardItem* item) {
	if (item->checkState() == Qt::Checked) {
		for (SCHEDULER::Task* task : Tasks) {
			QString taskName = tr((task->getKernelName() + " - %1").c_str()).arg(task->getId());
			if (taskName.compare(item->text()) == 0) {
				Task->addDependandTask(task);
			}
		}
	}
	else {
		int i = 0;
		std::vector<SCHEDULER::Task*> tasks = Task->getDependantTasks();
		Task->getDependantTasks().clear();
		for (SCHEDULER::Task* task : tasks) {
			QString taskName = tr((task->getKernelName() + " - %1").c_str()).arg(task->getId());
			if (taskName.compare(item->text()) != 0) {
				Task->addDependandTask(task);
			}
		}
	}
}

void TaskTabWidget::generateGraph() {
	Ui.DiagramPlot->setTitle("Data Plot");
	Ui.DiagramPlot->setCanvasBackground(Qt::white);

	QPolygonF points;
	for (int i = 0; i < Model->rowCount(); i++) {
		points << QPointF(i, Model->item(i, Model->columnCount() - 1)->text().toDouble());
	}
	Ui.DiagramPlot->setAxisTitle(QwtPlot::xBottom, QString::fromUtf8("Data Point"));
	Ui.DiagramPlot->setAxisAutoScale(QwtPlot::xBottom);
	Ui.DiagramPlot->setAxisTitle(QwtPlot::yLeft, QString::fromUtf8("Data"));
	Ui.DiagramPlot->setAxisAutoScale(QwtPlot::yLeft);

	QwtPlotCurve* curve = new QwtPlotCurve();
	curve->setPen(QPen(Qt::blue));
	curve->setSamples(points);
	curve->attach(Ui.DiagramPlot);
	curve->setCurveAttribute(QwtPlotCurve::Fitted, true);
	Ui.DiagramPlot->replot();
}

