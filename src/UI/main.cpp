//
// Created by moritz on 17.06.19.
//

#include <iostream>
#include <string>
#include "TUI.h"

using namespace std;
using namespace UI;


int main(int count, char** values){
    TUI tui;

    if(count <= 1){
        cout<<"No Arguments given Starting normal."<<endl;
    }else{
        tui.toggleUnitTestMode();
    }

    tui.start();

    return 0;
}