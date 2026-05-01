#include <3ds.h>
#include <stdio.h>

#include "utils.h"

#define LINES_AMOUNT 43 //may add more in a future update

//yes, the entire text is in a single buffer structured with 50 chars per line
char* helpingTextBuffer[] = 
{"         \x1b[32m[3DS-FBGE PROJECT HELPING PAGE]\x1b[0m          ",
"This is the helping text for the 3ds-fbge project,",
"please read it before installing and using it.    ",
"                                                  ",
"\x1b[33m#What is it?\x1b[0m :                                    ",
"The 3DS-FBGE program (the name is a short for     ",
"\"\x1b[32m3DS-FrameBufferGraphicalEnvironnement\x1b[0m\") is a     ",
"custom desktop environnement for the 3DS when     ",
"running under a GNU/Linux operating system. It's  ",
"just a proof of concept and many features will be ",
"added later on.                                   ",
"                                                  ",
"\x1b[33m#How to install?\x1b[0m :                                ",
"To install the FBGE files, select the option      ",
"\"\x1b[32mInstall 3DS-FBGE\x1b[0m\" in the previous menu, it will  ",
"automatically download and install the binary and ",
"the Linux OS files. If you only want Linux3DS and ",
"you don't care about my FBGE project (which i can ",
"understand lol) you would select the option called",
"\"\x1b[32mInstall Linux ONLY\x1b[0m\".                             ",
"                                                  ", 
"\x1b[33m#How to use it?\x1b[0m (\x1b[31mREAD CAREFULLY!\x1b[0m) :               ",
"I can't make this program automatically boot the  ",
"Linux OS and enter every command directly, so     ",
"here is the EXACT steps you have to follow,       ",
"don't worry it's not that hard ^^                 ",
"                                                  ",
" 1- Poweroff your console and power it while      ",
"    holding the SELECT button, it will show the   ",
"    Luma3DS bootloader, now to load Linux you     ",
"    need to select the \"\x1b[32mlinux_bootloader.firm\x1b[0m\"    ",
"    file, a Linux terminal will load.             ", 
" 2- Now that you have Linux running on the console",
"    you need to mount the SD card to access the   ",
"    'fbge' binary, to do that run a script by     ",
"    typing the \"\x1b[32m./switch.sh\x1b[0m\" command.             ",
" 3- Then your SD card should be mounted at \"\x1b[32m/mnt\x1b[0m\",",
"    you can now run the desktop environnement by  ",
"    typing the \"\x1b[32m/mnt/fbge\x1b[0m\" command.               ",
"                                                  ",
"\x1b[33m#Need more info?\x1b[0m :                                ", 
"To get more information, go read the Github README",
"at https://github.com/AtexBg/3ds-fbge.            "};

void showHelpMenu(bool* hasHelpMenuBeenRead){
    consoleClear();
    int textBufferOffset = 0;

    while(true){ //loop of the "text engine"
        hidScanInput();
        u32 keys = hidKeysDown();

        if(keys & KEY_DOWN){textBufferOffset++;}
        if(keys & KEY_UP){textBufferOffset--;}
        if(keys & KEY_START){break;}

        if(textBufferOffset < 0){textBufferOffset++;}
        if(textBufferOffset >= LINES_AMOUNT - 27){textBufferOffset--;}

        
        for(int i=0; i < 28; i++){
            printf("\x1b[%d;1H%s", i+1, helpingTextBuffer[i+textBufferOffset]);
        }
        printf("\x1b[29;1H--------------------------------------------------");
        printf("\x1b[30;1H\x1b[33mUP\x1b[0m/\x1b[33mDOWN\x1b[0m : Navigate                 \x1b[33mSTART\x1b[0m : Go Back");

        flushFramebufferAndWaitForVBlank();
    }

    *hasHelpMenuBeenRead = true;
    consoleClear();
}


