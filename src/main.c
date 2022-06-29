#include <stdint.h>
#include <stddef.h>

#define TV_NTSC 1
#include "famitone2.h"
#include "nes.h"
#include "reset.h"
#include "rle.h"

#include "data.h"


typedef enum state {
    STATE_TITLE,
    STATE_DAN,
    STATE_TRESSLEY,
    STATE_JEFF,
    STATE_RON,
    STATE_FADE
} state_t;


#pragma bss-name(push, "ZEROPAGE")
uint8_t i;             // loop counter/temp
uint8_t j;             // loop counter/temp
state_t state;
const uint8_t * bg;    // background data
uint8_t pattern_table; // 0 or 1


// used by WritePPU method
uintptr_t       ppu_addr;      // destination PPU address
uint8_t const * ppu_data;      // pointer to data to copy to PPU
uint8_t         ppu_data_size; // # of bytes to copy to PPU
#pragma bss-name(pop)


//TODO: Why is this required?
#pragma bss-name(push, "OAM")
sprite_t text_sprites[1]; // 4x8 grid
#pragma bss-name(pop)


void ResetScroll() {
    PPU_SCROLL = 0x00;
    PPU_SCROLL = 0x00;
}


void EnablePPU() {
    i = PPUCTRL_NAMETABLE_0 | // use nametable 0
        PPUCTRL_NMI_ON      ; // enable NMIs

    if(pattern_table == 0) {
        i |= (PPUCTRL_BPATTERN_0 | PPUCTRL_SPATTERN_0);
    } else {
        i |= (PPUCTRL_BPATTERN_1 | PPUCTRL_SPATTERN_1);
    }

    PPU_CTRL = i;
    PPU_MASK = PPUMASK_COLOR    | // show colors
               PPUMASK_BSHOW    | // show background
               PPUMASK_L8_BSHOW | // show background tiles in leftmost 8px
               PPUMASK_SSHOW    | // show sprites
               PPUMASK_L8_SSHOW ; // show sprites in leftmost 8px
}


void DisablePPU() {
    PPU_CTRL = 0x00;
    PPU_MASK = 0x00;
}


void WritePPU() {
    PPU_ADDRESS = (uint8_t)(ppu_addr >> 8);
    PPU_ADDRESS = (uint8_t)(ppu_addr);
    for ( i = 0; i < ppu_data_size; ++i ) {
        PPU_DATA = ppu_data[i];
    }
}


void DrawBackground() {
    PPU_ADDRESS = (uint8_t)(PPU_NAMETABLE_0 >> 8);
    PPU_ADDRESS = (uint8_t)(PPU_NAMETABLE_0);
    UnRLE(bg);
}


void InitTitle() {
    DisablePPU();

    state = STATE_TITLE;
    pattern_table = 0;

    // write palettes
    ppu_addr = PPU_PALETTE;
    ppu_data = PAL_TITLE;
    ppu_data_size = sizeof(PAL_TITLE);
    WritePPU();

    // draw background
    bg = BG_TITLE;
    DrawBackground();

    // play music
    FamiToneMusicPlay(SONG_TITLE);

    WaitVBlank();
    EnablePPU();
}


void InitDan() {
    DisablePPU();

    state = STATE_DAN;
    pattern_table = 1;

    // write palettes
    ppu_addr = PPU_PALETTE;
    ppu_data = PAL_DAN;
    ppu_data_size = sizeof(PAL_DAN);
    WritePPU();

    // draw background
    bg = BG_DAN;
    DrawBackground();

    // play music
    FamiToneMusicPlay(SONG_DAN);

    WaitVBlank();
    EnablePPU();
}


void InitTressley() {
    DisablePPU();

    state = STATE_TRESSLEY;
    pattern_table = 1;

    // write palettes
    ppu_addr = PPU_PALETTE;
    ppu_data = PAL_TRESSLEY;
    ppu_data_size = sizeof(PAL_TRESSLEY);
    WritePPU();

    // draw background
    bg = BG_TRESSLEY;
    DrawBackground();

    // play music
    FamiToneMusicPlay(SONG_TRESSLEY);

    WaitVBlank();
    EnablePPU();
}


void InitJeff() {
    DisablePPU();

    state = STATE_JEFF;
    pattern_table = 1;

    // write palettes
    ppu_addr = PPU_PALETTE;
    ppu_data = PAL_JEFF;
    ppu_data_size = sizeof(PAL_JEFF);
    WritePPU();

    // draw background
    bg = BG_JEFF;
    DrawBackground();

    // play music
    FamiToneMusicPlay(SONG_JEFF);

    WaitVBlank();
    EnablePPU();
}


void InitRon() {
    DisablePPU();

    state = STATE_RON;
    pattern_table = 1;

    // write palettes
    ppu_addr = PPU_PALETTE;
    ppu_data = PAL_RON;
    ppu_data_size = sizeof(PAL_RON);
    WritePPU();

    // draw background
    bg = BG_RON;
    DrawBackground();

    // play music
    FamiToneMusicPlay(SONG_RON);

    WaitVBlank();
    EnablePPU();
}


void HandleInput() {
    switch(state) {
        case STATE_TITLE:
            if ((InputPort1 & BUTTON_START) && !(InputPort1Prev & BUTTON_START)) {
                InitDan();
                FamiToneSfxPlay(SFX_START);
            }
            if ((InputPort1 & BUTTON_A) && !(InputPort1Prev & BUTTON_A)) {
                InitDan();
                FamiToneSfxPlay(SFX_START);
            }
            break;
        case STATE_DAN:
            if ((InputPort1 & BUTTON_START) && !(InputPort1Prev & BUTTON_START)) {
                InitTressley();
                FamiToneSfxPlay(SFX_START);
            }
            if ((InputPort1 & BUTTON_A) && !(InputPort1Prev & BUTTON_A)) {
                InitTressley();
                FamiToneSfxPlay(SFX_START);
            }
            break;
        case STATE_TRESSLEY:
            if ((InputPort1 & BUTTON_START) && !(InputPort1Prev & BUTTON_START)) {
                InitJeff();
                FamiToneSfxPlay(SFX_START);
            }
            if ((InputPort1 & BUTTON_A) && !(InputPort1Prev & BUTTON_A)) {
                InitJeff();
                FamiToneSfxPlay(SFX_START);
            }
            break;
        case STATE_JEFF:
            if ((InputPort1 & BUTTON_START) && !(InputPort1Prev & BUTTON_START)) {
                InitRon();
                FamiToneSfxPlay(SFX_START);
            }
            if ((InputPort1 & BUTTON_A) && !(InputPort1Prev & BUTTON_A)) {
                InitRon();
                FamiToneSfxPlay(SFX_START);
            }
            break;
        case STATE_RON:
            if ((InputPort1 & BUTTON_START) && !(InputPort1Prev & BUTTON_START)) {
                InitTitle();
                FamiToneSfxPlay(SFX_START);
            }
            if ((InputPort1 & BUTTON_A) && !(InputPort1Prev & BUTTON_A)) {
                InitTitle();
                FamiToneSfxPlay(SFX_START);
            }
            break;
        default:
            break;
    }
}


void Update() {
    switch(state) {
        // case STATE_CREDITS:
        //     UpdateCredits();
        //     break;
        // case STATE_DAN:
        //     break;
    default:
        break;
    }
}


void main(void) {
    FamiToneInit();
    FamiToneSfxInit();
    InitTitle();

    // turn on rendering
    ResetScroll();
    EnablePPU();

    while (1) {
        WaitFrame();
        FamiToneUpdate();
        HandleInput();
        Update();
        ResetScroll();
    };
};