#pragma once

#include "ui_MainWindow.h"
#include <QMainWindow>

class MainWindow : public QMainWindow 
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = NULL);

private:
	Ui::MainWindow ui;
};

