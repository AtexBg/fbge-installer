#include <3ds.h>
#include <stdio.h>

void flushFramebufferAndWaitForVBlank(){
    gfxFlushBuffers();
	gfxSwapBuffers();

	gspWaitForVBlank();
}

void waitForInput(){
    while(true){ 
        hidScanInput();
        u32 keys = hidKeysDown();

        flushFramebufferAndWaitForVBlank();

        if(keys){ //no bitmask operation = any key can be detected
            //consoleClear();
            return;
        }
    }
}
    
void notImplemented(){ //USE WHEN A FEATURE IS MISSING, TO DELETE BEFORE REAL RELEASE
    consoleClear();
    printf("\x1b[1;1Hs-sowwy, that thing is.. n-not implemented y-yet~\n");
    flushFramebufferAndWaitForVBlank();
    waitForInput();
}

void copyFile(const char* src, const char* dst){
    uint8_t buffer[16384]; //16KB temporary buffer
    size_t bytes;
    FILE* inputFile = fopen(src, "rb");
    FILE* outputFile = fopen(dst, "wb");

    while((bytes = fread(buffer, 1, sizeof(buffer), inputFile)) > 0){
        fwrite(buffer, 1, bytes, outputFile);
    }
    fclose(inputFile);
    fclose(outputFile);
}