#ifndef SCRIPTS_H
#define SCRIPTS_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/******                       SCRIPT COMMANDS                           ******/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define READ_N_BITS           0xD5     //arguments : 1 = number of bits
#define READ_BYTE             0xD6     //arguments : 0
#define VISI24                0xD7     //arguments : 0
#define NOP24                 0xD8     //arguments : 0
#define COREINST24            0xD9     //arguments : 3 = low byte, mid byte, upper byte
#define COREINST18            0xDA     //arguments : 2 = low byte, upper byte
#define POP_DOWNLOAD_BUFFER   0xDB     //arguments : 0
#define READ_ICSP_STATES      0xDC     //arguments : 0
#define LOOP_BUFFER           0xDD     //arguments : 1 = index offset to loop to 
#define WRITE_SFR             0xDE     //arguments : 2 = 1 -> lower byte of SFR addr, 2 -> data to be written
#define READ_SFR              0xDF     //arguments : 1 = lower byte of SFR addr
#define EXIT_SCRIPT           0xE0     //arguments : 0
#define GOTO_IDX              0xE1     //arguments : 1 = script command index to go to
#define IF_GT_GOTO            0xE2     //arguments : 2 = 1->byte to compare with head of DATA_Out_Buffer, 2->signed idx to branch to
#define IF_EQ_GOTO            0xE3     //arguments : 2 = 1->byte to compare with head of DATA_Out_Buffer, 2->signed idx to branch to
#define SHORT_DELAY           0xE4     //arguments : 1 = duration
#define LONG_DELAY            0xE5     //arguments : 1 = duration
#define LOOP                  0xE6     //arguments : 2 = 1 -> index offset, 2 -> iterations
#define SHIFT_BITS_IN_CMD     0xE7     //arguments : 1 = number of bits
#define SHIFT_BITS_IN_BUFFER  0xE8     //arguments : 1 = number of bits
#define SHIFT_BITS_OUT_BUFFER 0xE9     //arguments : 1 = number of bits to shift out
#define SHIFT_BITS_OUT_CMD    0xEA     //arguments : 2 = 1 -> number of bits to shift out, 2-> output byte
#define SHIFT_BYTE_IN         0xEB     //arguments : 0
#define SHIFT_BYTE_IN_BUFFER  0xEC     //arguments : 0
#define SHIFT_BYTE_OUT_BUFFER 0xED     //arguments : 0
#define SHIFT_BYTE_OUT        0xEE     //arguments : 1 = byte to be shifted out
#define SET_ICSP_PINS_CMD     0xEF     //arguments : 1 = pin states
#define MCLR_TGT_GND_OFF      0xF0     //arguments : 0
#define MCLR_TGT_GND_ON       0xF1     //arguments : 0
#define VPP_PWM_OFF           0xF2     //arguments : 0
#define VPP_PWM_ON            0xF3     //arguments : 0
#define VPP_ON_CMD            0xF4     //arguments : 0
#define VPP_OFF               0xF5     //arguments : 0
#define VDD_GND_ON            0xF6     //arguments : 0
#define VDD_GND_OFF           0xF7     //arguments : 0
#define VDD_ON                0xF8     //arguments : 0
#define VDD_OFF               0xF9     //arguments : 0



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/******                        ACTUAL SCRIPTS                           ******/
/******             Starting address in ROM is 0x006000 (Block 3)       ******/
/******              PIC18F2550 Programming Specifications, P:9         ******/
/******                      Number Of Scripts = 5                      ******/
/******                     Total Size = 159 Bytes                      ******/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



/*********** ProgEntry PIC24 ***************/
/***           Address = 0x6000          ***/
/***           Length = 30               ***/
#rom int8 0x6000 = {VPP_OFF, MCLR_TGT_GND_ON, VPP_PWM_ON, SET_ICSP_PINS_CMD, 0x00, LONG_DELAY, 0x14, MCLR_TGT_GND_OFF, VPP_ON_CMD, SHORT_DELAY, 0x17, VPP_OFF, MCLR_TGT_GND_ON, SHORT_DELAY, 0x2F, SHIFT_BYTE_OUT, 0xB2, SHIFT_BYTE_OUT, 0xC2, SHIFT_BYTE_OUT, 0x12, SHIFT_BYTE_OUT, 0x8A, MCLR_TGT_GND_OFF, VPP_ON_CMD, LONG_DELAY, 0x09, SHIFT_BITS_OUT_CMD, 0x05, 0}
 

/***********  ProgExit PIC24 ***************/
/***           Address = 0x603C          ***/
/***           Length = 7                ***/
#rom  int8 0x603C = {VPP_OFF, MCLR_TGT_GND_ON, VPP_PWM_OFF, SET_ICSP_PINS_CMD, 0x03, LONG_DELAY, 0x0A}


/*********** DevIdScript PIC24 *************/
/***           Address = 0x604A          ***/
/***           Length = 50               ***/
#rom int8 0x604A = {COREINST24, 0x00, 0x02, 0x04, COREINST24, 0x00, 0x02, 0x04, NOP24, NOP24, NOP24, COREINST24, 0x00, 0x02, 0x04, NOP24, COREINST24, 0xF0, 0x0F, 0x20, COREINST24, 0x90, 0x01, 0x88, COREINST24, 0x06, 0x00, 0x20, NOP24, NOP24, COREINST24, 0x07, 0x00, 0x20, COREINST24, 0xB6, 0x0B, 0xBA, NOP24, NOP24, COREINST24, 0x20, 0x3C, 0x88, NOP24, VISI24, NOP24, LOOP, 0x0D, 0x01} 


/********** ProgEntry PIC18F_J_ ************/
/***           Address = 0x60AE          ***/
/***           Length = 27               ***/
#rom int8 0x60AE = {VPP_OFF, MCLR_TGT_GND_ON, VPP_PWM_ON, SET_ICSP_PINS_CMD, 0x00, LONG_DELAY, 0x14, MCLR_TGT_GND_OFF, VPP_ON_CMD,  SHORT_DELAY, 0x17, VPP_OFF, MCLR_TGT_GND_ON, SHORT_DELAY, 0x2F, SHIFT_BYTE_OUT, 0xB2, SHIFT_BYTE_OUT, 0xC2, SHIFT_BYTE_OUT, 0x12, SHIFT_BYTE_OUT, 0x0A, MCLR_TGT_GND_OFF, VPP_ON_CMD, SHORT_DELAY, 0xEB}



/********** ReadDevID PIC18F_J_ ************/
/***           Address = 0x60E4          ***/
/***           Length = 45               ***/
#rom int8 0x60E4 = {COREINST18, 0x2A, 0x0E, COREINST18, 0x15, 0x09, COREINST18, 0x00, 0x00, COREINST18, 0xF8, 0x6E, COREINST18, 0xAA, 0x0E, COREINST18, 0x55, 0x0A, COREINST18, 0xF7, 0x6E, COREINST18, 0xAA, 0x0E, COREINST18, 0x54, 0x0A, COREINST18, 0x00, 0x00, COREINST18, 0xF6, 0x6E, SHIFT_BITS_OUT_CMD, 0x04, 0x09, SHIFT_BYTE_OUT, 0x00, SHIFT_BYTE_IN_BUFFER, COREINST18, 0xFF, 0xFF, LOOP, 0x09, 0x01}


/********** ProgEntry PIC18F_K_ ************/
/***           Address = 0x613E          ***/
/***           Length = 17               ***/
#rom int8 0x613E = {VPP_OFF, MCLR_TGT_GND_ON, VPP_PWM_ON, SET_ICSP_PINS_CMD, 0x00, LONG_DELAY, 0x14, MCLR_TGT_GND_OFF, VPP_ON_CMD, SHORT_DELAY, 0x7F, MCLR_TGT_GND_ON, VPP_OFF, VPP_ON_CMD, MCLR_TGT_GND_OFF, LONG_DELAY, 0x13}


/********* ProgEntryVpp PIC18F_K_ **********/
/***           Address = 0x6160          ***/
/***           Length = 21               ***/
#rom int8 0x6160 = {VPP_OFF, MCLR_TGT_GND_ON, VDD_OFF, VDD_GND_ON, VPP_PWM_ON, SET_ICSP_PINS_CMD, 0x00, LONG_DELAY, 0x14, MCLR_TGT_GND_OFF, VPP_ON_CMD, VDD_GND_OFF, VDD_ON, SHORT_DELAY, 0x7F, MCLR_TGT_GND_ON, VPP_OFF, VPP_ON_CMD, MCLR_TGT_GND_OFF, LONG_DELAY, 0x13} 


/********** ReadDevId MR/1.8V Min **********/
/***           Address = 0x18A          ***/
/***              Length = 18            ***/
#rom int8 0x618A = {SHIFT_BITS_OUT_CMD, 0x06, 0x00, SHIFT_BYTE_OUT, 0x00, SHIFT_BYTE_OUT, 0x00, SHIFT_BITS_OUT_CMD, 0x06, 0x06, LOOP, 0x03, 0x05, SHIFT_BITS_OUT_CMD, 0x06, 0x04, SHIFT_BYTE_IN_BUFFER, SHIFT_BYTE_IN_BUFFER}


/********** ProgEntry PIC18F_K80 ***********/
/***           Address = 0x61AE          ***/
/***              Length = 27            ***/
#rom int8 0x61AE = {VPP_OFF, MCLR_TGT_GND_ON, VPP_OFF, MCLR_TGT_GND_ON, VPP_PWM_ON, SET_ICSP_PINS_CMD, 0x00, LONG_DELAY, 0x14, MCLR_TGT_GND_OFF, MCLR_TGT_GND_OFF, VPP_ON_CMD, VPP_ON_CMD, SHORT_DELAY, 0x2F, SHIFT_BYTE_OUT, 0xB2, SHIFT_BYTE_OUT, 0xC2, SHIFT_BYTE_OUT, 0x12, SHIFT_BYTE_OUT, 0x0A, MCLR_TGT_GND_OFF, VPP_ON_CMD, SHORT_DELAY, 0xEB}


/********** ProgEntryVpp PIC18F  ***********/
/***           Address = 0x61E4          ***/
/***              Length = 21            ***/
#rom int8 0x61E4 = {VPP_OFF, MCLR_TGT_GND_ON, VDD_OFF, VDD_GND_ON, VPP_PWM_ON, SET_ICSP_PINS_CMD, 0x00, LONG_DELAY, 0x3C, VDD_GND_OFF, VDD_ON, MCLR_TGT_GND_OFF, VPP_ON_CMD, SHORT_DELAY, 0x7F, MCLR_TGT_GND_ON, VPP_OFF, VPP_ON_CMD, MCLR_TGT_GND_OFF, LONG_DELAY, 0x13}



/*********** ProgEntryVpp PIC32 ************/
/***           Address = 0x60C5          ***/
/***    Length = MISSING COMMANDS        ***/
//#rom int8 0x60C5 = {VPP_OFF, MCLR_TGT_GND_ON, VPP_PWM_ON, SET_ICSP_PINS_CMD, 0x00, LONG_DELAY, 0x14, MCLR_TGT_GND_OFF, VPP_ON_CMD, SHORT_DELAY, 0x17, VPP_OFF, MCLR_TGT_GND_ON, SHORT_DELAY, 0x2F, SHIFT_BYTE_OUT, 0xB2, SHIFT_BYTE_OUT, 0xC2, SHIFT_BYTE_OUT, 0x12, SHIFT_BYTE_OUT, 0x0A, MCLR_TGT_GND_OFF, VPP_ON_CMD, SHORT_DELAY, 0xEB, SET_ICSP_PINS_CMD, 0x02}   



#rom int8 0x3000 = {SET_ICSP_PINS_CMD, 0x00, SHIFT_BITS_OUT_CMD, 0x06, 0x00, SHIFT_BYTE_OUT, 0x00, SHIFT_BYTE_OUT, 0x00, SHIFT_BITS_OUT_CMD, 0x06, 0x06, LOOP, 0x03, 0x05}//, SHIFT_BITS_OUT_CMD, 0x06, 0x04, SHIFT_BYTE_IN_BUFFER, SHIFT_BYTE_IN_BUFFER}






#endif 



















