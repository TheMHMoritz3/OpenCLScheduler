#pragma once
#include <qdialog.h>
#include <ui_ConstantDialog.h>
#include "src/Scheduler/SchedulerNamespace.h"

class ConstantDialog :
	public QDialog
{
public:
	ConstantDialog(QWidget* parent = NULL);

	void setConstantName(std::vector<std::string> names);

	std::string getConstantName();
	SCHEDULER::Type getConstantType();
	void* constantValue();
private:

	void* getDataFromValue(SCHEDULER::Type type, QVariant value);
	void* getDataFromUINTValue(QVariant value);
	void* getDataFromINTValue(QVariant value);
	void* getDataFromCharValue(QVariant value);
	void* getDataFromFloatValue(QVariant value);
	void* getDataFromDoubleValue(QVariant value);

	Ui::Dialog Dialog_;
};


