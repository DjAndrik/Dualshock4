#pragma once

#include <string>
#include "gamepad.hpp"

using namespace std;

int main(){

    TypeGamepadStruct *GamepadStruct = new TypeGamepadStruct;

    DS4 Gamepad("/dev/input/js1");

    while((Gamepad).GetGamepadStatus()){

	(Gamepad).GetGamepadStruct(GamepadStruct);

	cout<<(*GamepadStruct).L2<<" "<<(*GamepadStruct).R2 <<"\n";

	if((*GamepadStruct).PS){break;}
    }

    Gamepad.DS4::~DS4();

    cout<<"THE END\n";
    cin.get();

    return 0;
}
