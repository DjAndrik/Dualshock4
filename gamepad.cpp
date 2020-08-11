#include "gamepad.hpp"

struct TypeEventStruct{
    uint32_t Time = 0;
    int16_t Value = 0;
    char ObjectType = 0;
    char ObjectNumber = 0;
};

void DS4::GetGamepadStruct(TypeGamepadStruct* GamepadStruct){
    this->GamepadStructMutex.lock();
    *GamepadStruct = *(this->GamepadStruct);
    this->GamepadStructMutex.unlock();
}

void DS4::SetGamepadStruct(TypeGamepadStruct* GamepadStruct){
    this->GamepadStructMutex.lock();
    *(this->GamepadStruct) = *GamepadStruct;
    this->GamepadStructMutex.unlock();
}

void DS4::SetGamepadStatus(bool GamepadStatus){
    this->GamepadStatusMutex.lock();
    this->GamepadStatus=GamepadStatus;
    this->GamepadStatusMutex.unlock();
}

bool DS4::GetGamepadStatus(){
    this->GamepadStatusMutex.lock();
    bool GamepadStatus=this->GamepadStatus;
    this->GamepadStatusMutex.unlock();
    return GamepadStatus;
}

DS4::DS4(string GamepadPath){
    this->GamepadDescriptor = open(GamepadPath.c_str(), O_RDONLY);
    if (this->GamepadDescriptor==-1){this->SetGamepadStatus(false);}
    thread ReadGamepadThread(&DS4::ReadGamepad,this);
    ReadGamepadThread.detach();
}

DS4::~DS4(){
    this->SetGamepadStatus(false);
    close(this->GamepadDescriptor);
    delete this->GamepadStruct;
}

void DS4::ReadGamepad(){
    TypeEventStruct EventStruct;
    TypeGamepadStruct *GamepadStruct = new TypeGamepadStruct;

    int GamepadStatus = 0;

    while(1){

	this->GetGamepadStruct(GamepadStruct);

	GamepadStatus = read(this->GamepadDescriptor,&EventStruct,8);
	    
	if (GamepadStatus==-1){this->SetGamepadStatus(false);break;}

	if (EventStruct.ObjectType==2){
	    if(EventStruct.ObjectNumber==0){(*GamepadStruct).LX = (float) EventStruct.Value/MagicNumber;}
	    else if(EventStruct.ObjectNumber==1){(*GamepadStruct).LY = (float) EventStruct.Value/MagicNumber;}
            else if(EventStruct.ObjectNumber==2){(*GamepadStruct).L2 = (float) (EventStruct.Value+MagicNumber)/(2*MagicNumber);}
	    else if(EventStruct.ObjectNumber==3){(*GamepadStruct).RX = (float) EventStruct.Value/MagicNumber;}
	    else if(EventStruct.ObjectNumber==4){(*GamepadStruct).RY = (float) EventStruct.Value/MagicNumber;}
            else if(EventStruct.ObjectNumber==5){(*GamepadStruct).R2 = (float) (EventStruct.Value+MagicNumber)/(2*MagicNumber);}
	}
	else if (EventStruct.ObjectType==1){
            if(EventStruct.ObjectNumber==0){(*GamepadStruct).Cross = (bool) EventStruct.Value;}
            else if(EventStruct.ObjectNumber==1){(*GamepadStruct).Round = (bool) EventStruct.Value;}
            else if(EventStruct.ObjectNumber==2){(*GamepadStruct).Triangle = (bool) EventStruct.Value;}
            else if(EventStruct.ObjectNumber==3){(*GamepadStruct).Square = (bool) EventStruct.Value;}

            else if(EventStruct.ObjectNumber==4){(*GamepadStruct).L1 = (bool) EventStruct.Value;}
            else if(EventStruct.ObjectNumber==5){(*GamepadStruct).R1 = (bool) EventStruct.Value;}

            else if(EventStruct.ObjectNumber==8){(*GamepadStruct).Share = (bool) EventStruct.Value;}
            else if(EventStruct.ObjectNumber==9){(*GamepadStruct).Options = (bool)EventStruct.Value;}
            else if(EventStruct.ObjectNumber==10){(*GamepadStruct).PS = (bool) EventStruct.Value;}

	    else if(EventStruct.ObjectNumber==11){(*GamepadStruct).L3 = (bool) EventStruct.Value;}
            else if(EventStruct.ObjectNumber==12){(*GamepadStruct).R3 = (bool) EventStruct.Value;}

	}
	this->SetGamepadStruct(GamepadStruct);
    }
    delete GamepadStruct;
}
