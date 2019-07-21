#pragma once


#include <QWidget>
#include "src/Scheduler/Task.h"
#include "ui_TaskTabWidget.h"

class TaskTabWidget : public QWidget
{
	Q_OBJECT
public:
	TaskTabWidget(QWidget* parent = NULL);

private:
	void makeConnections();
	Ui::TaskTabWidget ui;
};

