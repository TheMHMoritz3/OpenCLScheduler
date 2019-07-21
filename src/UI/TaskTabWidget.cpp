#include "TaskTabWidget.h"


using namespace SCHEDULER;


TaskTabWidget::TaskTabWidget(QWidget* parent):
QWidget(parent)
{
	ui.setupUi(this);
	ui.retranslateUi(this);
	makeConnections();
}

void TaskTabWidget::makeConnections()
{
	
}

