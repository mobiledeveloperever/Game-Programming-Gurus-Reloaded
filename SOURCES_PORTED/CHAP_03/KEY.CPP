
//------------------------------------------
// DOS DEVELOPMENT ENVIRONMENT EMULATION TOOLKIT
//------------------------------------------
#include "stdafx.h"
#include "DOSEmu.h"
//------------------------------------------


// I N C L U D E S ///////////////////////////////////////////////////////////

#include <dos.h>
//#include <bios.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>
//#include <graph.h>


// D E F I N E S  ////////////////////////////////////////////////////////////

// bitmasks for control keys/shift status

#define SHIFT_R             0x0001
#define SHIFT_L             0x0002
#define CTRL                0x0004
#define ALT                 0x0008
#define SCROLL_LOCK_ON      0x0010
#define NUM_LOCK_ON         0x0020
#define CAPS_LOCK_ON        0x0040
#define INSERT_MODE         0x0080
#define CTRL_L              0x0100
#define ALT_L               0x0200
#define CTRL_R              0x0400
#define ALT_R               0x0800
#define SCROLL_LOCK_DWN     0x1000
#define NUM_LOCK_DWN        0x2000
#define CAPS_LOCK_DWN       0x4000
#define SYS_REQ_DWN         0x8000

// scan code values, note keys with two symbols on them are the same so I will
// consistantly try to use the lower symbol for example. the 1 key also has a
// ! above it, but we would just call it the SCAN_1 key.

#define SCAN_ESC          1
#define SCAN_1            2
#define SCAN_2            3
#define SCAN_3            4
#define SCAN_4            5
#define SCAN_5            6
#define SCAN_6            7
#define SCAN_7            8
#define SCAN_8            9
#define SCAN_9            10
#define SCAN_0            11
#define SCAN_MINUS        12
#define SCAN_EQUALS       13
#define SCAN_BKSP         14
#define SCAN_TAB          15
#define SCAN_Q            16
#define SCAN_W            17
#define SCAN_E            18
#define SCAN_R            19
#define SCAN_T            20
#define SCAN_Y            21
#define SCAN_U            22
#define SCAN_I            23
#define SCAN_O            24
#define SCAN_P            25
#define SCAN_LFT_BRACKET  26
#define SCAN_RGT_BRACKET  27
#define SCAN_ENTER        28
#define SCAN_CTRL         29

#define SCAN_A            30
#define SCAN_S            31
#define SCAN_D            32
#define SCAN_F            33
#define SCAN_G            34
#define SCAN_H            35
#define SCAN_J            36
#define SCAN_K            37
#define SCAN_L            38

#define SCAN_SEMI         39
#define SCAN_APOS         40
#define SCAN_TILDE        41

#define SCAN_LEFT_SHIFT   42
#define SCAN_BACK_SLASH   43
#define SCAN_Z            44
#define SCAN_X            45
#define SCAN_C            46
#define SCAN_V            47
#define SCAN_B            48
#define SCAN_N            49
#define SCAN_M            50
#define SCAN_COMMA        51
#define SCAN_PERIOD       52
#define SCAN_FOWARD_SLASH 53
#define SCAN_RIGHT_SHIFT  54
#define SCAN_PRT_SCRN     55
#define SCAN_ALT          56
#define SCAN_SPACE        57
#define SCAN_CAPS_LOCK    58
#define SCAN_F1           59
#define SCAN_F2           60
#define SCAN_F3           61
#define SCAN_F4           62
#define SCAN_F5           63
#define SCAN_F6           64
#define SCAN_F7           65
#define SCAN_F8           66
#define SCAN_F9           67
#define SCAN_F10          68
#define SCAN_F11          133
#define SCAN_F12          134
#define SCAN_NUM_LOCK     69
#define SCAN_SCROLL_LOCK  70
#define SCAN_HOME         71
#define SCAN_UP           72
#define SCAN_PGUP         73
#define SCAN_NUM_MINUS    74
#define SCAN_LEFT         75
#define SCAN_CENTER       76
#define SCAN_RIGHT        77
#define SCAN_NUM_PLUS     78
#define SCAN_END          79
#define SCAN_DOWN         80
#define SCAN_PGDWN        81
#define SCAN_INS          82
#define SCAN_DEL          83

// F U N C T I O N S /////////////////////////////////////////////////////////

unsigned char Get_Ascii_Key(void)

{

// if there is a normal ascii key waiting then return it, else return 0

if (_bios_keybrd(_KEYBRD_READY))
 return(_bios_keybrd(_KEYBRD_READ));
else return(0);

} // end Get_Ascii_Key

//////////////////////////////////////////////////////////////////////////////

unsigned int Get_Control_Keys(unsigned int mask)
{
// return the status of all the requested control key

	return(mask & _bios_keybrd(_KEYBRD_SHIFTSTATUS));

} // end Get_Control_Keys

//////////////////////////////////////////////////////////////////////////////

unsigned char Get_Scan_Code(void)
{
// get the scan code of a key press, since we have to look at status bits
// let's use the inline assembler


// is a key ready?

//------------------------------------------
// NO ANY ASM
//------------------------------------------
/*

__asm
    {
    mov ah,01h          ; function 1: is a key ready?
    int 16h             ; call the interrupt
    jz empty            ; there was no key so exit
    mov ah,00h          ; function 0: get the scan code please
    int 16h             ; call the interrupt
    mov al,ah           ; result was in ah so put into al
    xor ah,ah           ; zero out ah
    jmp done            ; data's in ax...let's blaze!

empty:
     xor ax,ax          ; clear out ax i.e. 0 means no key
done:

    } // end asm
*/
//------------------------------------------


	REGS r;

	r.h.ah = 0x01;
	int86(0x16, &r, &r);
	if(r.w.cflag & I386_FLAG_ZF) return 0;

	r.h.ah = 0x00;
	int86(0x16, &r, &r);
	r.h.al = r.h.ah;
	r.h.ah ^= r.h.ah;

	return r.w.ax;

// since data is in ax it will be returned properly

} // end Get_Scan_Code

//////////////////////////////////////////////////////////////////////////////

void main2(void) // keyboard demo
{
unsigned char key;
int done=0;
unsigned int control;

_clearscreen(_GCLEARSCREEN);

while(!done)
     {

     _settextposition(2,0);

     if ( (key = Get_Scan_Code()) )
        printf("%c %d  ",key,key);

     // test for ctrl and alt keys

     if (Get_Control_Keys(CTRL))
        printf("\ncontrol key pressed");

     if (Get_Control_Keys(ALT))
        printf("\nalt key pressed    ");

     if (key==16) done=1; // 16 is the scan code for 'q'

	 if(!key) printf("               \n                       ");

     } // end main

} // end main

