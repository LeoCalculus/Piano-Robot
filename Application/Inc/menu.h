#ifndef __MENU_H
#define __MENU_H

#include <ILI9225.h>
#include <hc04.h>
#include <application.h>

/* Menu page states */
typedef enum
{
    MENU_STATE_MAIN,     /* Main menu with 4 items */
    MENU_STATE_TRANSMIT, /* Transmit Song (SD) page - receives files via BT */
    MENU_STATE_TRANSMIT_RAM, /* Transmit Song (RAM) */
    MENU_STATE_SELECT,   /* Select Song page - list files on SD card */
    MENU_STATE_DEBUG     /* Debug info page */
} MenuState_t;

/* Navigation flags (set by command parser, consumed by menu_update) */
extern int menu_index; /* Highlighted item in current page */
extern int menu_move_down;
extern int menu_move_up;
extern int menu_enter; /* Enter/confirm action */
extern int menu_back;  /* Back to main menu */

/* Select-song page state */
extern int select_song_index;    /* Highlighted song in file list */
extern int select_scroll_offset; /* Scroll offset when >5 songs */
extern int active_song_index;    /* Currently selected song for playback (-1 = none) */

/* Public API */
void menu_init(void);
void menu_update(void);
MenuState_t menu_get_state(void);
void menu_process_message(uint8_t *data, uint16_t len);
int menu_try_dispatch_binary(uint8_t *data, uint16_t len);

/* Page renderers */
void menu_draw_main(void);
void menu_draw_transmit(void);
void menu_draw_transmit_ram(void);
void menu_draw_select(void);

#endif
