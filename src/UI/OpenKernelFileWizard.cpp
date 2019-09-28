#include "OpenKernelFileWizard.h"
#include <QFileDialog>
#include "src/Scheduler/KernelFileParser.h"

using namespace UI;

OpenKernelFileWizard::OpenKernelFileWizard(QWidget* parent) :
	QWizard(parent)
{
	wizard.setupUi(this);
	wizard.retranslateUi(this);

	makeConnections();
}

void OpenKernelFileWizard::makeConnections()
{
	connect(wizard.OpenFileButton, SIGNAL(clicked(bool)), this, SLOT(openFileDialog()));
}

void OpenKernelFileWizard::openFileDialog()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open Kernel", qApp->applicationDirPath(), "*.cl");
	wizard.FileNameLineEdit->setText(fileName);
//	SCHEDULER::KernelFileParser* parser = new SCHEDULER::KernelFileParser();
}
