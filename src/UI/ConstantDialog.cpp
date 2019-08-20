#include "ConstantDialog.h"

ConstantDialog::ConstantDialog(QWidget* parent) :
	QDialog(parent)
{
	Dialog_.setupUi(this);
	Dialog_.retranslateUi(this);
}

void ConstantDialog::setConstantName(std::vector<std::string> names)
{
	for (std::string name : names)
	{
		Dialog_.ConstantNameComboBox->addItem(QString(name.c_str()).split(" ").last());
	}
}

std::string ConstantDialog::getConstantName()
{
	return Dialog_.ConstantNameComboBox->currentText().toStdString();
}

SCHEDULER::Type ConstantDialog::getConstantType()
{
	return (SCHEDULER::Type)Dialog_.TypeComboBox->currentIndex();
}

void* ConstantDialog::constantValue()
{
	return getDataFromValue(getConstantType(),QVariant(Dialog_.ValueLineEidt->text()));
}

void* ConstantDialog::getDataFromValue(SCHEDULER::Type type, QVariant value)
{
	switch (type) {
	case SCHEDULER::INT:
		return getDataFromINTValue(value);
	case SCHEDULER::UINT:
		return  getDataFromUINTValue(value);
	case SCHEDULER::CHAR:
		return getDataFromCharValue(value);
	case SCHEDULER::FLOAT: 
		return getDataFromFloatValue(value);
	case SCHEDULER::DOUBLE: break;
	case SCHEDULER::STRING:
	default:
		return nullptr;
	}
}

void* ConstantDialog::getDataFromUINTValue(QVariant value)
{
	unsigned int* intValue = new unsigned int[1];
	intValue[0] = value.toUInt();
	return intValue;
}

void* ConstantDialog::getDataFromINTValue(QVariant value)
{
	int* intValue = new int[1];
	intValue[0] = value.toInt();
	return intValue;
}

void* ConstantDialog::getDataFromCharValue(QVariant value)
{
	char* charValue = new char[1];
	charValue[0] = value.toChar().toLatin1();
	return charValue;
}

void* ConstantDialog::getDataFromFloatValue(QVariant value)
{
	float* floatValue = new float[1];
	floatValue[0] = value.toFloat();
	return floatValue;
}

void* ConstantDialog::getDataFromDoubleValue(QVariant value)
{
	double* floatValue = new double[1];
	floatValue[0] = value.toDouble();
	return floatValue;
}