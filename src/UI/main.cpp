//
// Created by moritz on 17.06.19.
//

#include <iostream>
#include <string>
#include "TUI.h"
#include "MainWindow.h"

using namespace std;
using namespace UI;


int main(int count, char** values){
   /* TUI tui;

    if(count <= 1){
        cout<<"No Arguments given Starting normal."<<endl;
    }else{
		cout << values[1]<<endl;
        tui.toggleUnitTestMode();
    }

    tui.start();*/
	QApplication appl(count, values);

	MainWindow* mainWindow = new MainWindow();
	mainWindow->show();

	return appl.exec();
}