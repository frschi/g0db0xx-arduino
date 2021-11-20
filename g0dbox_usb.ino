#include <Joystick.h>

// Definitions
////////////////////////////////////////
typedef struct ControllerState {
    bool a = false;
    bool b = false;
    bool x = false;
    bool y = false;
    bool z = false;
    bool start = false;
    bool controlUp = false;
    bool controlDown = false;
    bool controlLeft = false;
    bool controlRight = false;
    bool cUp = false;
    bool cDown = false;
    bool cLeft = false;
    bool cRight = false;
    bool l = false;
    bool r = false;
    bool modX = false;
    bool modY = false;
    bool modZ = false;
} ControllerState;

enum Game {
    Plus,
    Melee
};

enum Socd {
    Neutral,
    TwoInputs
};

typedef struct PlusModifierCoordinates {
    //modX
    uint8_t upX[] = {128, 187};
    uint8_t downX[] = {128, 69};
    uint8_t leftX[] = {60, 128};
    uint8_t rightX[] = {196, 128};

    uint8_t leftDownX[] = {60, 95};
    uint8_t rightDownX[] = {196, 95};
    uint8_t leftUpX[] = {60, 161};
    uint8_t rightUpX[] = {196, 161};
    //modY
    uint8_t upY[] = {128, 161};
    uint8_t downY[] = {128, 95};
    uint8_t leftY[] = {100, 128};
    uint8_t rightY[] = {156, 128};

    uint8_t leftDownY[] = {100, 71};
    uint8_t rightDownY[] = {156, 71};
    uint8_t leftUpY[] = {100, 185};
    uint8_t rightUpY[] = {156, 185};
    //modZ
    uint8_t upZ[] = {128, 255};
    uint8_t downZ[] = {128, 0};
    uint8_t leftZ[] = {0, 128};
    uint8_t rightZ[] = {255, 128};

    uint8_t leftDownZ[] = {0, 90};
    uint8_t rightDownZ[] = {255, 90};
    uint8_t leftUpZ[] = {0, 166};
    uint8_t rightUpZ[] = {255, 166};
} PlusModifierCoordinates;

typedef struct MeleeModifierCoordinates {
    //modX
    uint8_t upX[] = {128, 180};
    uint8_t downX[] = {128, 76};
    uint8_t leftX[] = {69, 128};
    uint8_t rightX[] = {187, 128};

    uint8_t leftDownX[] = {69, 105};
    uint8_t rightDownX[] = {187, 105};
    uint8_t leftUpX[] = {69, 151};
    uint8_t rightUpX[] = {187, 151};
    //modY
    uint8_t upY[] = {128, 187};
    uint8_t downY[] = {128, 69};
    uint8_t leftY[] = {105, 128};
    uint8_t rightY[] = {151, 128};

    uint8_t leftDownY[] = {105, 69};
    uint8_t rightDownY[] = {151, 69};
    uint8_t leftUpY[] = {105, 187};
    uint8_t rightUpY[] = {151, 187};
    //modZ
    uint8_t upZ[] = {128, 255};
    uint8_t downZ[] = {128, 0};
    uint8_t leftZ[] = {0, 128};
    uint8_t rightZ[] = {255, 128};

    uint8_t leftDownZ[] = {0, 89};
    uint8_t rightDownZ[] = {255, 89};
    uint8_t leftUpZ[] = {0, 167};
    uint8_t rightUpZ[] = {255, 167};
} MeleeModifierCoordinates;

// State
////////////////////////////////////////
ControllerState boxState;
PlusModifierCoordinates plusMods;
MeleeModifierCoordinates meleeMods;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   20, 0,                       // Buttons,
                   true, true, false,           // X, Y, Z
                   true, true, false,           // RX, RY, RZ
                   false, false,                // Rudder, Throttle
                   false, false, false);        // Accelerator, Brake, Steering

Game currentGame = Plus;
Socd currentSOCD = Neutral;

uint8_t X = 128;
uint8_t Y = 128;
uint8_t RX = 128;
uint8_t RY = 128;

/*
 * 0 = None
 * 1 = Left Down
 * 2 = Right Down
 * 3 = Left Up
 * 4 = Right Up
 * 5 = Up
 * 6 = Down
 * 7 = Left
 * 8 = Right
*/
uint8_t activeDirection = 0;

bool previousUp = false;
bool previousLeft = false;

// Pin Numbers
////////////////////////////////////////
int a = A1;
int b = 14;
int x;
int y = A8;
int z = A9;
int start = 16;
int controlUp = A10;
int controlDown = A6;
int controlLeft = 5;
int controlRight = 3;
int cUp = A0;
int cDown = A3;
int cLeft = A2;
int cRight = 15;
int l = A7;
int r = 7;
int modX = 2;
int modY = 1;
int modZ = 0;

//standard cardinal coordinates
uint8_t none[2] = {128, 128};

uint8_t up[] = {128, 255};
uint8_t down[] = {128, 0};
uint8_t left[] = {0, 128};
uint8_t right[] = {255, 128};

uint8_t leftDown[] = {0, 0};
uint8_t rightDown[] = {255, 0};
uint8_t leftUp[] = {0, 255};
uint8_t rightUp[] = {255, 255};


unsigned long myTime = 0;

void setup() {
    pinMode(a, INPUT_PULLUP);
    pinMode(b, INPUT_PULLUP);
    pinMode(x, INPUT_PULLUP);
    pinMode(y, INPUT_PULLUP);
    pinMode(z, INPUT_PULLUP);
    pinMode(start, INPUT_PULLUP);
    pinMode(controlUp, INPUT_PULLUP);
    pinMode(controlDown, INPUT_PULLUP);
    pinMode(controlLeft, INPUT_PULLUP);
    pinMode(controlRight, INPUT_PULLUP);
    pinMode(cUp, INPUT_PULLUP);
    pinMode(cDown, INPUT_PULLUP);
    pinMode(cLeft, INPUT_PULLUP);
    pinMode(cRight, INPUT_PULLUP);
    pinMode(l, INPUT_PULLUP);
    pinMode(r, INPUT_PULLUP);
    pinMode(modX, INPUT_PULLUP);
    pinMode(modY, INPUT_PULLUP);
    pinMode(modZ, INPUT_PULLUP);

    Joystick.begin(false);
    Joystick.setXAxisRange(0, 255);
    Joystick.setYAxisRange(0, 255);
    Joystick.setRxAxisRange(0, 255);
    Joystick.setRyAxisRange(0, 255);
}

void loop() {
    boxState.a = (digitalRead(a) == LOW);
    boxState.b = (digitalRead(b) == LOW);
    boxState.x = (digitalRead(x) == LOW);
    boxState.y = (digitalRead(y) == LOW);
    boxState.z = (digitalRead(z) == LOW);
    boxState.start = (digitalRead(start) == LOW);
    boxState.controlUp = (digitalRead(controlUp) == LOW);
    boxState.controlDown = (digitalRead(controlDown) == LOW);
    boxState.controlLeft = (digitalRead(controlLeft) == LOW);
    boxState.controlRight = (digitalRead(controlRight) == LOW);
    boxState.cUp = (digitalRead(cUp) == LOW);
    boxState.cDown = (digitalRead(cDown) == LOW);
    boxState.cLeft = (digitalRead(cLeft) == LOW);
    boxState.cRight = (digitalRead(cRight) == LOW);
    boxState.l = (digitalRead(l) == LOW);
    boxState.r = (digitalRead(r) == LOW);
    boxState.modX = (digitalRead(modX) == LOW);
    boxState.modY = (digitalRead(modY) == LOW);
    boxState.modZ = (digitalRead(modZ) == LOW);

    checkSwitchGame();
    updateControllerState();
}

// Only possible to switch game every 3 seconds, else it would spam switching games when hotkey combination pressed
void checkSwitchGame() {
    if (boxState.l && boxState.controlLeft && boxState.controlDown && boxState.controlRight && boxState.a && boxState.cLeft && boxState.cUp && boxState.cRight && boxState.cDown) {
        if (millis() > myTime + 3000) {
            if (currentGame == Melee)
                currentGame = Plus;
            else if (currentGame == Plus) {
                currentGame = Melee;
            }
            myTime = millis();
        }
    }
}

void updateControllerState() {
    setActiveDirection();
    isModifierPressed() ? handleModifiers() : setUnmodifiedCoordinates();
    setCStickCoordinates();
    updateJoystickAxis();
    updateButtons();
    Joystick.sendState();
}

void setUnmodifiedCoordinates() {
    setCoordinatesForDirection(leftDown, rightDown, leftUp, rightUp, up, down, left, right);
}

void setXModifierCoordinates() {
    if (currentGame == Plus) {
        setCoordinatesForDirection(plusMods.leftDownX, plusMods.rightDownX, plusMods.leftUpX, plusMods.rightUpX, plusMods.upX, plusMods.downX, plusMods.leftX, plusMods.rightX);
    } else if (currentGame == Melee) {
        setCoordinatesForDirection(meleeMods.leftDownX, meleeMods.rightDownX, meleeMods.leftUpX, meleeMods.rightUpX, meleeMods.upX, meleeMods.downX, meleeMods.leftX, meleeMods.rightX);
    }
}

void setYModifierCoordinates() {
    if (currentGame == Plus) {
        setCoordinatesForDirection(plusMods.leftDownY, plusMods.rightDownY, plusMods.leftUpY, plusMods.rightUpY, plusMods.upY, plusMods.downY, plusMods.leftY, plusMods.rightY);
    } else if (currentGame == Melee) {
        setCoordinatesForDirection(meleeMods.leftDownY, meleeMods.rightDownY, meleeMods.leftUpY, meleeMods.rightUpY, meleeMods.upY, meleeMods.downY, meleeMods.leftY, meleeMods.rightY);
    }
}

void setZModifierCoordinates() {
    if (currentGame == Plus) {
        setCoordinatesForDirection(plusMods.leftDownZ, plusMods.rightDownZ, plusMods.leftUpZ, plusMods.rightUpZ, plusMods.upZ, plusMods.downZ, plusMods.leftZ, plusMods.rightZ);
    } else if (currentGame == Melee) {
        setCoordinatesForDirection(meleeMods.leftDownZ, meleeMods.rightDownZ, meleeMods.leftUpZ, meleeMods.rightUpZ, meleeMods.upZ, meleeMods.downZ, meleeMods.leftZ, meleeMods.rightZ);
    }
}

void handleModifiers() {
    if (boxState.modX && boxState.modY) {
        updateDPad();
    } else if (boxState.modX) {
        setXModifierCoordinates();
    } else if (boxState.modY) {
        setYModifierCoordinates();
    } else if (boxState.modZ) {
        setZModifierCoordinates();
    }
}

void updateDPad() {
    Joystick.setButton(8, boxState.cUp);
    Joystick.setButton(9, boxState.cDown);
    Joystick.setButton(10, boxState.cLeft);
    Joystick.setButton(11, boxState.cRight);
}

void setCoordinatesForDirection(uint8_t leftDown[2], uint8_t rightDown[2], uint8_t leftUp[2], uint8_t rightUp[2], uint8_t up[2], uint8_t down[2], uint8_t left[2], uint8_t right[2]) {
    switch (activeDirection) {
        case 0:
            setXY(none[0], none[1]);
            break;
        case 1:
            setXY(leftDown[0], leftDown[1]);
            break;
        case 2:
            setXY(rightDown[0], rightDown[1]);
            break;
        case 3:
            setXY(leftUp[0], leftUp[1]);
            break;
        case 4:
            setXY(rightUp[0], rightUp[1]);
            break;
        case 5:
            setXY(up[0], up[1]);
            break;
        case 6:
            setXY(down[0], down[1]);
            break;
        case 7:
            setXY(left[0], left[1]);
            break;
        case 8:
            setXY(right[0], right[1]);
            break;
    }
}

void setCStickCoordinates() {
    if (boxState.modX && boxState.modY)
        return
    if (boxState.cDown) {
        setRxRy(down[0], down[1]);
    } else if (boxState.cUp) {
        setRxRy(up[0], up[1]);
    } else if (boxState.cLeft) {
        setRxRy(left[0], left[1]);
    } else if (boxState.cRight) {
        setRxRy(right[0], right[1]);
    } else {
        setRxRy(none[0], none[0]);
    }
}

void updateJoystickAxis() {
    Joystick.setXAxis(X);
    Joystick.setYAxis(Y);
    Joystick.setRxAxis(RX);
    Joystick.setRyAxis(RY);
}

void setActiveDirection() {
    checkSocd(boxState.controlUp, boxState.controlDown, previousUp);
    checkSocd(boxState.controlLeft, boxState.controlRight, previousLeft);

    if (boxState.controlDown && boxState.controlLeft) {
        setPreviousDirection(false, true);
        activeDirection = 1;
    } else if (boxState.controlDown && boxState.controlRight) {
        setPreviousDirection(false, false);
        activeDirection = 2;
    } else if (boxState.controlUp && boxState.controlLeft) {
        setPreviousDirection(true, true);
        activeDirection = 3;
    } else if (boxState.controlUp && boxState.controlRight) {
        setPreviousDirection(true, false);
        activeDirection = 4;
    } else if (boxState.controlUp) {
        setPreviousDirection(true, previousLeft);
        activeDirection = 5;
    } else if (boxState.controlDown) {
        setPreviousDirection(false, previousLeft);
        activeDirection = 6;
    } else if (boxState.controlLeft) {
        setPreviousDirection(previousUp, true);
        activeDirection = 7;
    } else if (boxState.controlRight) {
        setPreviousDirection(previousUp, false);
        activeDirection = 8;
    } else {
        activeDirection = 0;
    }
}

void checkSocd(bool &a, bool &b, bool previous) {
    if (a && b) {
        if (currentSOCD == Neutral) {
            a = b = false;
        } else if (currentSOCD == TwoInputs) {
            previous ? a = false : b = false;
        }
        return setActiveDirection();
    }
}

void updateButtons() {
    Joystick.setButton(0, boxState.a);
    Joystick.setButton(1, boxState.b);
    Joystick.setButton(2, boxState.x);
    Joystick.setButton(3, boxState.y);
    Joystick.setButton(4, boxState.z);
    Joystick.setButton(5, boxState.start);
    Joystick.setButton(6, boxState.l);
    Joystick.setButton(7, boxState.r);
}

void setXY(uint8_t x, uint8_t y) {
    X = x;
    Y = y;
}

void setRxRy(uint8_t Rx, uint8_t Ry) {
    RX = Rx;
    RY = Ry;
}

void setPreviousDirection(bool up, bool left) {
    previousUp = up;
    previousLeft = left;
}

bool isModifierPressed() {
    return (boxState.modX || boxState.modY || boxState.modZ);
}