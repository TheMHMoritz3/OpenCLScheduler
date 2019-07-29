/*
Embedded Systems Project 2019
Copyright (C) 2019  Moritz Herzog, Philip Lersch

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
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
		cout << values[1]<<endl;
        tui.toggleUnitTestMode();
    }

    tui.start();

    return 0;
}