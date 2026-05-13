#pragma once

class Screen;

// =============================================================================
// SwitchManager - manages on/off switches that control doors
// In screen files: / = ON, \ = OFF
// =============================================================================
class SwitchManager {
public:
    static constexpr int MAX_SWITCHES = 20;
    static constexpr char SWITCH_ON = '/';
    static constexpr char SWITCH_OFF = '\\';

private:
    struct SwitchInfo {
        int x = -1;
        int y = -1;
        int room = -1;
        bool isOn = false;
        bool wasToggled = false;
        int linkedDoorId = 0;
        bool exists = false;
    };

    SwitchInfo switches[MAX_SWITCHES];
    int switchCount = 0;

public:
    SwitchManager() = default;

    void scanAndInit(Screen rooms[], int numRooms);
    int findSwitchAt(int x, int y, int room) const;
    bool toggleSwitch(int index, Screen& screen);

    int getLinkedDoorId(int index) const {
        if (index >= 0 && index < switchCount && switches[index].exists) {
            return switches[index].linkedDoorId;
        }
        return 0;
    }

    int countOnSwitchesForDoor(int doorId) const;
    int countTotalSwitchesForDoor(int doorId) const;
    void reset();
    int getCount() const { return switchCount; }
};