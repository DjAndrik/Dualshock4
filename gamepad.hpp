#pragma once

#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstring>
#include <string>
#include <cstdio>
#include <mutex>
#include <thread>

#define MagicNumber 0x7FFF

using namespace std;

struct TypeGamepadStruct{

    bool Triangle = false;
    bool Square = false;
    bool Round = false;
    bool Cross = false;

    bool DPadUp = false;
    bool DPadDown = false;
    bool DPadLeft = false;
    bool DPadRight = false;

    bool PS = false;
    bool Share = false;
    bool Options = false;

    bool L1 = false;
    bool R1 = false;
    bool L3 = false;
    bool R3 = false;

    float RX = 0.0f;
    float RY = 0.0f;
    float LX = 0.0f;
    float LY = 0.0f;
    float L2 = 0.0f;
    float R2 = 0.0f;
};

class DS4{
private:
    mutex GamepadStructMutex;
    mutex GamepadStatusMutex;

    TypeGamepadStruct *GamepadStruct = new TypeGamepadStruct;

    int GamepadDescriptor = 0;

    bool GamepadStatus = true;

    void SetGamepadStruct(TypeGamepadStruct* GamepadStruct);

    void ReadGamepad();

    void SetGamepadStatus(bool GamepadStatus);

public:
    DS4(string GamepadPath);

    ~DS4();

    bool GetGamepadStatus();

    void GetGamepadStruct(TypeGamepadStruct* GamepadStruct);
};
