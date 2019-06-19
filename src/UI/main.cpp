//
// Created by moritz on 17.06.19.
//

#include <iostream>
#include <string>
#include "TUI.h"

using namespace std;
using namespace UI;


int main(int count, char** values){
    if(count == 0){
        cout<<"No Arguments given Starting normal."<<endl;
    }

    TUI tui;

    tui.start();

    return 0;
}