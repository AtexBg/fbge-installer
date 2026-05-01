#include <3ds.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h> //mkdir
//#include <3ds/services/ptmsysm.h> //PTMSYSM service to check console model

#include "utils.h" //flushFramebufferAndWaitForVBlank();, waitForInput(); and copyFile();
#include "help.h" //showHelpMenu();

#define LINE(x) "\x1b[" #x ";1H" //specific line encoding using ANSI sequences
#define FBGE_BINARY_VERSION "0.6.0"
#define FBGE_BINARY_SIZE "2.1MB"

//bool isNew3ds;
bool hasHelpMenuBeenRead = false;
//char* text[1024]; //buffer used to load and display text
char* menuEntry[] = {"1- Help", "2- Install 3DS-FBGE", "3- Install Linux ONLY", "4- Quit"};
char* linuxFiles[] = {"luma/payloads/linux_bootloader.firm", "linux/zImage", "linux/arm9linuxfw.bin", "linux/nintendo3ds_ctr.dtb"};
char* linuxFilesDescriptions[] = {"Bootloader", "Kernel image", "ARM9 Firmware", "Device Tree"}; //the same device tree works for both OLD and NEW 3DS
char* fbgeBinaryPaths[] = {"romfs:/sdContents/fbge", "sdmc:/fbge"};
int currentSelectedOption = 0;
int linuxFilesDescriptionsEntriesAmount = sizeof(linuxFilesDescriptions) / sizeof(linuxFilesDescriptions[0]);
int entriesAmount = sizeof(menuEntry) / sizeof(menuEntry[0]);

void linuxOnlyInstall(bool hasHelpMenuBeenRead, bool doNeedToClearConsole){
    consoleClear();

    if(hasHelpMenuBeenRead == false){
        printf("\x1b[31m/!\\\x1b[0m Please read the \"Help\" page before\nproceeding to install.\n");
        flushFramebufferAndWaitForVBlank();
        waitForInput();
        consoleClear();
        return;
    }
    
    mkdir("sdmc:/linux", 0777);
    mkdir("sdmc:/luma/payloads", 0777); //just it case, but should already been made by CFW
    printf("\x1b[32mInstalling Linux...\x1b[0m");

    //buffer containing the src/dst paths
    char buf[256];
    char buf2[256];

    for(int i=0; i < linuxFilesDescriptionsEntriesAmount; i++){
        printf("\x1b[%d;1HCopying %s at \x1b[36m%s\x1b[0m.", i+2, linuxFilesDescriptions[i], linuxFiles[i]);
        
        snprintf(buf, sizeof(buf), "romfs:/sdContents/%s", linuxFiles[i]);
        snprintf(buf2, sizeof(buf2), "sdmc:/%s", linuxFiles[i]);

        flushFramebufferAndWaitForVBlank();
        copyFile(buf, buf2); //copy the files
    }

    printf("\x1b[6;1H\x1b[0mLinux is installed, press \x1b[32many key\x1b[0m to continue...");
    flushFramebufferAndWaitForVBlank();

    waitForInput();

    if(doNeedToClearConsole){ //clear screen if needed
        consoleClear();
    }
}


void proceedToInstall(bool hasHelpMenuBeenRead){
    consoleClear();

    if(hasHelpMenuBeenRead == false){
        printf("\x1b[31m/!\\\x1b[0m Please read the \"Help\" page before\nproceeding to install.\n");
        flushFramebufferAndWaitForVBlank();
        waitForInput();
        return;
    }

    linuxOnlyInstall(true, false);
    
    printf("\x1b[7;1HInstalling binary to \x1b[36m%s\x1b[0m...\nVersion " FBGE_BINARY_VERSION ".\nSize : " FBGE_BINARY_SIZE ".\n", fbgeBinaryPaths[1]);
    flushFramebufferAndWaitForVBlank();
    copyFile(fbgeBinaryPaths[0], fbgeBinaryPaths[1]);
    printf("FBGE binary installed. Press \x1b[32many key\x1b[0m to go back.");
    flushFramebufferAndWaitForVBlank();
    waitForInput();
    consoleClear();
}


int main(){
    //init
	gfxInitDefault();
    romfsInit();
    consoleInit(GFX_TOP, NULL);

    //doesn't work, may need a fix
    // PTMSYSM_CheckNew3DS(&isNew3ds); //Check whether the system is a NEW or OLD 3DS

    // if(!isNew3ds){ //shows warning if not running on an OLD 3DS model
    //     printf(LINE(3) "\x1b[32mOLD 3DS SYSTEM DETECTED, THE PROGRAM MAY NOT WORK /!\\");
    //     printf(LINE(4) "\x1b[32mIF IT DOESN'T WORKS AFTER TESTING, GO TO https://github.com/AtexBg/3ds-fbge/issues/new AND EXPLAIN WHAT HAPPENED!");
    // }

	while (aptMainLoop()){
        //consoleClear();
		hidScanInput();
		u32 keys = hidKeysDown();
    
        printf(LINE(1) "-- 3DS-FBGE INSTALLER -- By AtexBg");
        printf(LINE(2) "Please check \"help\" for more information.\n");
        
        if(keys & KEY_DOWN){currentSelectedOption++;}
        if(keys & KEY_UP){currentSelectedOption--;}
        
        if(currentSelectedOption == entriesAmount){currentSelectedOption--;} //block currentSelectedOption to go further than the amount of menu entries
        if(currentSelectedOption == -1){currentSelectedOption++;}

        if(keys & KEY_A){
            switch(currentSelectedOption){
                case 0: showHelpMenu(&hasHelpMenuBeenRead); break;
                case 1: proceedToInstall(hasHelpMenuBeenRead); break;
                case 2: linuxOnlyInstall(hasHelpMenuBeenRead, true); break;
                case 3: goto quit; break;
            }
        }
        

        //display options
        for(int i=0; i < entriesAmount; i++){
            if(currentSelectedOption == i){
                printf("\x1b[%d;1H \x1b[32m", i+4);
            }
            printf("\x1b[%d;2H%s\x1b[0m\n", i+4, menuEntry[i]);
        }

        printf(LINE(25) "                         \x1b[33mUP\x1b[0m/\x1b[33mDOWN\x1b[0m : Navigate");
        printf(LINE(26) "                         \x1b[33mA\x1b[0m : Select option");
        printf(LINE(27) "                         \x1b[33mL\x1b[0m+\x1b[33mR\x1b[0m : Quit FBGE-INSTALLER");
        printf(LINE(30) "\x1b[33mhttps://github.com/AtexBg/3ds-fbge @ 2026\x1b[0m");

		if((keys & KEY_L) && (keys & KEY_R)) break; //quit if L and R are pressed
        flushFramebufferAndWaitForVBlank();
	}
    quit:

    //exit
    romfsExit();
	gfxExit();
	return 0;
}
