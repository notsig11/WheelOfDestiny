#pragma once

// 1.5oz shots
// pour base: 1/8 oz


#define DISPLAY_ADDRESS     0x27

#define PUMP_CONTROL_0      0x10
#define PUMP_CONTROL_1      0x11
#define PUMP_CONTROL_2      0x12
#define PUMP_CONTROL_3      0x13
#define PUMP_CONTROL_4      0x14
#define PUMP_CONTROL_5      0x15

#define PRIME               0xEE
#define DRAIN               0xFF
#define DISPENSE            0xDE


// Message format: COMMAND PUMP_IDX POUR_COUNT = 0xDE 0x01 0x08 // Pour 1oz from pump 1
// example shot 3 way dirty pirate: spiced rum, coconut rum, pineapple, grenadine
//      PUMP_CONTROL_0 -> 0xDE (DISPENSE) 0x3 (pump 4) 0x2 (two 1/8oz pours)
//      PUMP_CONTROL_1 -> 0xDE (DISPENSE) 0x1 (pump 1) 0x2 (two 1/8oz pours)
//      PUMP_CONTROL_5 -> 0xDE (DISPENSE) 0x2 (pump 3) 0x2 (two 1/8oz pours)
//      PUMP_CONTROL_5 -> 0xDE (DISPENSE) 0x3 (pump 4) 0x2 (two 1/8oz pours)
