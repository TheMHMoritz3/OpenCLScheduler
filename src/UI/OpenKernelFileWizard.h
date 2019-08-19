#pragma once

#include "ui_OpenTaskWizard.h"
#include <QWizard>

namespace UI {
	class OpenKernelFileWizard : public QWizard
	{
		Q_OBJECT
	public:
		OpenKernelFileWizard(QWidget* parent);

	private:
		Ui::OpenTaskWizard  wizard;

		void makeConnections();
	private slots:
		void openFileDialog();
	};
}