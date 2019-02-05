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
#define SHIFT_BITS_IN         0xE7     //arguments : 1 = number of bits
#define SHIFT_BITS_IN_BUFFER  0xE8     //arguments : 1 = number of bits
#define SHIFT_BITS_OUT_BUFFER 0xE9     //arguments : 1 = number of bits to shift out
#define SHIFT_BITS_OUT        0xEA     //arguments : 2 = 1 -> number of bits to shift out, 2-> output byte
#define SHIFT_BYTE_IN         0xEB     //arguments : 0
#define SHIFT_BYTE_IN_BUFFER  0xEC     //arguments : 0
#define SHIFT_BYTE_OUT_BUFFER 0xED     //arguments : 0
#define SHIFT_BYTE_OUT        0xEE     //arguments : 1 = byte to be shifted out
#define SET_ICSP_PINS         0xEF     //arguments : 1 = pin states
#define MCLR_TGT_GND_OFF      0xF0     //arguments : 0
#define MCLR_TGT_GND_ON       0xF1     //arguments : 0
#define VPP_PWM_OFF           0xF2     //arguments : 0
#define VPP_PWM_ON            0xF3     //arguments : 0
#define VPP_ON                0xF4     //arguments : 0
#define VPP_OFF               0xF5     //arguments : 0
#define VDD_GND_ON            0xF6     //arguments : 0
#define VDD_GND_OFF           0xF7     //arguments : 0
#define VDD_ON                0xF8     //arguments : 0
#define VDD_OFF               0xF9     //arguments : 0


#rom int8 0x1E00 = {0xF0, 0xE5, 100, 0xF4}
#rom int8 0x1E04 = {0xE5, 100, 0xF0}




#endif 








