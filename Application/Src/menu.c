#include <menu.h>
#include <sdcard.h>
#include <file_transfer.h>
#include <string.h>
#include <stdio.h>

/* Menu state */
static MenuState_t menu_state = MENU_STATE_MAIN;

/* Navigation flags */
int menu_index = 0;
int menu_move_down = 0;
int menu_move_up = 0;
int menu_enter = 0;
int menu_back = 0;

/* Select-song page state */
int select_song_index = 0;
int select_scroll_offset = 0;
int active_song_index = -1;

/* Track if page needs full redraw */
static int page_dirty = 1;

/* ------------------------------------------------------------------ */
/*  Helpers                                                           */
/* ------------------------------------------------------------------ */

static void clear_menu_area(void)
{
    /* Clear rows 1-9 (leave row 0 = title, rows 10+ = debug info) */
    for (int r = 1; r <= 9; r++)
    {
        LCD_draw_string(&lcd_config, 0, r, "                        ", COLOR_BLACK, COLOR_WHITE);
    }
}

/* ------------------------------------------------------------------ */
/*  Main menu page                                                    */
/* ------------------------------------------------------------------ */

void menu_draw_main(void)
{
    const char *items[] = {
        "1. Play Song",
        "2. Transmit Song(SD)",
        "3. Transmit Song(RAM)",
        "4. Select Song",
        "5. Homing"};
    char line[25];
    for (int i = 0; i < 5; i++)
    {
        snprintf(line, sizeof(line), "%s %s", (i == menu_index) ? ">" : " ", items[i]);
        /* Pad to clear old text */
        int len = strlen(line);
        while (len < 23)
            line[len++] = ' ';
        line[23] = '\0';
        LCD_draw_string(&lcd_config, 0, 1 + i, line, COLOR_BLACK, COLOR_WHITE);
    }
}

/* ------------------------------------------------------------------ */
/*  Transmit Song (SD) page                                           */
/* ------------------------------------------------------------------ */

void menu_draw_transmit(void)
{
    if (page_dirty)
    {
        clear_menu_area();
        LCD_draw_string(&lcd_config, 0, 1, "[Transmit Song]         ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(&lcd_config, 0, 2, "Waiting for file...     ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(&lcd_config, 0, 8, "Send :a to go back      ", COLOR_BLACK, COLOR_WHITE);
        page_dirty = 0;
    }

    /* Show live transfer status */
    FT_State_t ft = FT_GetState();
    char buf[25];

    switch (ft)
    {
    case FT_STATE_IDLE:
        LCD_draw_string(&lcd_config, 0, 3, "Status: IDLE            ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(&lcd_config, 0, 4, "                        ", COLOR_BLACK, COLOR_WHITE);
        break;
    case FT_STATE_RECEIVING:
    {
        uint8_t pct = FT_GetProgress();
        snprintf(buf, sizeof(buf), "Status: RX  %3d%%        ", pct);
        LCD_draw_string(&lcd_config, 0, 3, buf, COLOR_BLACK, COLOR_WHITE);

        /* Simple progress bar */
        char bar[21];
        int filled = pct / 5; /* 0-20 */
        for (int i = 0; i < 20; i++)
            bar[i] = (i < filled) ? '#' : '-';
        bar[20] = '\0';
        LCD_draw_string(&lcd_config, 0, 4, bar, COLOR_BLACK, COLOR_WHITE);
        break;
    }
    case FT_STATE_COMPLETE:
        LCD_draw_string(&lcd_config, 0, 3, "Status: COMPLETE!       ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(&lcd_config, 0, 4, "File saved to SD card.  ", COLOR_BLACK, COLOR_WHITE);
        break;
    case FT_STATE_ERROR:
        LCD_draw_string(&lcd_config, 0, 3, "Status: ERROR           ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(&lcd_config, 0, 4, "Transfer failed.        ", COLOR_BLACK, COLOR_WHITE);
        break;
    }
}

/* ------------------------------------------------------------------ */
/*  Transmit Song (RAM) page                                          */
/* ------------------------------------------------------------------ */

void menu_draw_transmit_ram(void){
    char buf[32];

    if (page_dirty){
        clear_menu_area();
        LCD_draw_string(&lcd_config, 0, 1, "[Transmit to RAM]       ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(&lcd_config, 0, 2, "Waiting for start...    ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(&lcd_config, 0, 3, "Max 365x14 floats       ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(&lcd_config, 0, 8, "Send :a to go back      ", COLOR_BLACK, COLOR_WHITE);
        page_dirty = 0;
    }

    /* Show live accumulation status */
    if (ram_rx_started && !ram_rx_complete){
        uint32_t total = RAM_SONG_MAX_BYTES;
        uint8_t pct = (uint8_t)((ram_rx_offset * 100) / total);
        snprintf(buf, sizeof(buf), "RX: %lu/%lu  %3d%%   ",
                 (unsigned long)ram_rx_offset, (unsigned long)total, pct);
        LCD_draw_string(&lcd_config, 0, 4, buf, COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(&lcd_config, 0, 2, "Receiving data...       ", COLOR_BLACK, COLOR_WHITE);
    }

    if (ram_rx_complete){
        uint32_t num_floats = ram_rx_offset / 4;
        uint32_t num_chords = num_floats / 14;
        snprintf(buf, sizeof(buf), "Got %lu chords!     ", (unsigned long)num_chords);
        LCD_draw_string(&lcd_config, 0, 3, buf, COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(&lcd_config, 0, 4, "Parsing song...         ", COLOR_BLACK, COLOR_WHITE);

        /* Parse the flat float buffer into piano_song struct */
        parsing_song_buffer_to_struct(song_ram, piano_song);

        LCD_draw_string(&lcd_config, 0, 4, "Song loaded to RAM!     ", COLOR_BLACK, COLOR_WHITE);

        /* Reset state so we don't re-parse every loop */
        ram_rx_complete = 0;
        ram_rx_started = 0;
        ram_rx_offset = 0;
        redirect_to_ram = 0;
    }
}

/* ------------------------------------------------------------------ */
/*  Select Song page                                                  */
/* ------------------------------------------------------------------ */

void menu_draw_select(void)
{
    char line[48];

    if (page_dirty)
    {
        clear_menu_area();
        /* Scan SD card for TXT files */
        SD_ListTxtFiles();
        select_song_index = 0;
        select_scroll_offset = 0;
        page_dirty = 0;
    }

    /* Header */
    snprintf(line, sizeof(line), "[Select] %d songs", (int)fileCount);
    /* Pad to 24 chars */
    int hlen = strlen(line);
    while (hlen < 24)
        line[hlen++] = ' ';
    line[24] = '\0';
    LCD_draw_string(&lcd_config, 0, 1, line, COLOR_BLACK, COLOR_WHITE);

    if (fileCount == 0)
    {
        LCD_draw_string(&lcd_config, 0, 2, "No songs on SD card.    ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(&lcd_config, 0, 7, "Send :a to go back      ", COLOR_BLACK, COLOR_WHITE);
        return;
    }

    /* Show up to 5 files with scroll */
    for (int i = 0; i < 5; i++)
    {
        int fileIdx = select_scroll_offset + i;
        int row = 2 + i;

        if (fileIdx < fileCount)
        {
            char marker = ' ';
            if (fileIdx == select_song_index)
                marker = '>';
            /* Show active song indicator */
            if (fileIdx == active_song_index)
            {
                snprintf(line, sizeof(line), "%c*%-12s %5luB", marker,
                         fileList[fileIdx].name, (unsigned long)fileList[fileIdx].size);
            }
            else
            {
                snprintf(line, sizeof(line), "%c %-12s %5luB", marker,
                         fileList[fileIdx].name, (unsigned long)fileList[fileIdx].size);
            }
            /* Pad to clear */
            int len = strlen(line);
            while (len < 23)
                line[len++] = ' ';
            line[23] = '\0';
            LCD_draw_string(&lcd_config, 0, row, line, COLOR_BLACK, COLOR_WHITE);
        }
        else
        {
            LCD_draw_string(&lcd_config, 0, row, "                        ", COLOR_BLACK, COLOR_WHITE);
        }
    }

    /* Scroll indicator */
    if (fileCount > 5)
    {
        int lo = select_scroll_offset + 1;
        int hi = (select_scroll_offset + 5 < fileCount) ? select_scroll_offset + 5 : fileCount;
        snprintf(line, sizeof(line), "  [%d-%d of %d]", lo, hi, (int)fileCount);
        int slen = strlen(line);
        while (slen < 24)
            line[slen++] = ' ';
        line[24] = '\0';
        LCD_draw_string(&lcd_config, 0, 7, line, COLOR_BLACK, COLOR_WHITE);
    }

    LCD_draw_string(&lcd_config, 0, 8, ":d select  :a back      ", COLOR_BLACK, COLOR_WHITE);
}

/* ------------------------------------------------------------------ */
/*  Public API                                                        */
/* ------------------------------------------------------------------ */

MenuState_t menu_get_state(void)
{
    return menu_state;
}

void menu_init(void)
{
    LCD_draw_string(&lcd_config, 0, 0, "ELEC391 Piano Bot       ", COLOR_BLACK, COLOR_WHITE);
    menu_state = MENU_STATE_MAIN;
    menu_index = 0;
    page_dirty = 1;
    menu_draw_main();
}

void menu_update(void)
{

    /* ---- Handle BACK in any sub-page ---- */
    if (menu_back)
    {
        menu_back = 0;
        if (menu_state != MENU_STATE_MAIN)
        {
            /* If a transfer is in progress, abort it before leaving */
            if (menu_state == MENU_STATE_TRANSMIT && FT_GetState() == FT_STATE_RECEIVING)
            {
                FT_Abort();
            }
            if (menu_state == MENU_STATE_TRANSMIT_RAM)
            {
                ram_rx_started = 0;
                ram_rx_complete = 0;
                ram_rx_offset = 0;
                redirect_to_ram = 0;
            }
            menu_state = MENU_STATE_MAIN;
            menu_index = 0;
            page_dirty = 1;
            clear_menu_area();
            menu_draw_main();
            /* Consume stale nav flags */
            menu_move_down = 0;
            menu_move_up = 0;
            menu_enter = 0;
            return;
        }
    }

    /* ---- State-specific update ---- */
    switch (menu_state)
    {

    /* ===== MAIN MENU ===== */
    case MENU_STATE_MAIN:
        /* Navigation */
        if (menu_move_down)
        {
            menu_index++;
            if (menu_index > 4)
                menu_index = 0;
        }
        if (menu_move_up)
        {
            menu_index--;
            if (menu_index < 0)
                menu_index = 4;
        }

        /* Enter */
        if (menu_enter)
        {
            menu_enter = 0;
            switch (menu_index)
            {
                case 0: /* Play Song */
                    /* Only play if a song is selected */
                    if (active_song_index >= 0 && active_song_index < fileCount)
                    {
                        LCD_draw_string(&lcd_config, 0, 9, "Playing song...         ", COLOR_BLACK, COLOR_WHITE);
                        /* TODO: hook into traversal_song with the selected file */
                    }
                    else
                    {
                        LCD_draw_string(&lcd_config, 0, 9, "No song selected!       ", COLOR_BLACK, COLOR_WHITE);
                    }
                    break;
                case 1: /* Transmit Song to SD card */
                    menu_state = MENU_STATE_TRANSMIT;
                    page_dirty = 1;
                    FT_Init(); /* Reset transfer state */
                    break;
                case 2: /* Transmit Song to RAM */
                    menu_state = MENU_STATE_TRANSMIT_RAM;
                    page_dirty = 1;
                    redirect_to_ram = 1;
                    ram_rx_started = 0;
                    ram_rx_complete = 0;
                    ram_rx_offset = 0;
                    break;
                case 3: /* Select Song */
                    menu_state = MENU_STATE_SELECT;
                    page_dirty = 1;
                    break;
                case 4: /* Homing */
                    LCD_draw_string(&lcd_config, 0, 9, "Homing...               ", COLOR_BLACK, COLOR_WHITE);
                    homing_procedure();
                    LCD_draw_string(&lcd_config, 0, 9, "Homing done!            ", COLOR_BLACK, COLOR_WHITE);
                    break;
            }
        }

        menu_draw_main();
        break;

    /* ===== TRANSMIT SONG PAGE ===== */
    case MENU_STATE_TRANSMIT:
        /* No navigation needed; just show transfer status */
        menu_draw_transmit();
        /* Check transfer timeout */
        FT_TimeoutCheck();
        break;

    case MENU_STATE_TRANSMIT_RAM:
        /* No navigation needed; just show transfer status */
        menu_draw_transmit_ram();
        break;

    /* ===== SELECT SONG PAGE ===== */
    case MENU_STATE_SELECT:
        /* Navigation through file list */
        if (menu_move_down)
        {
            select_song_index++;
            if (select_song_index >= fileCount)
                select_song_index = 0;
            /* Adjust scroll window */
            if (select_song_index >= select_scroll_offset + 5)
                select_scroll_offset = select_song_index - 4;
            if (select_song_index < select_scroll_offset)
                select_scroll_offset = select_song_index;
            /* Handle wrap-around */
            if (select_song_index == 0)
                select_scroll_offset = 0;
        }
        if (menu_move_up)
        {
            select_song_index--;
            if (select_song_index < 0)
                select_song_index = fileCount - 1;
            /* Adjust scroll window */
            if (select_song_index < select_scroll_offset)
                select_scroll_offset = select_song_index;
            if (select_song_index >= select_scroll_offset + 5)
                select_scroll_offset = select_song_index - 4;
            /* Handle wrap-around */
            if (select_song_index == fileCount - 1)
                select_scroll_offset = (fileCount > 5) ? fileCount - 5 : 0;
        }

        /* Select a song */
        if (menu_enter)
        {
            menu_enter = 0;
            if (fileCount > 0)
            {
                active_song_index = select_song_index;
                char buf[25];
                snprintf(buf, sizeof(buf), "Selected: %-12s  ", fileList[active_song_index].name);
                LCD_draw_string(&lcd_config, 0, 9, buf, COLOR_BLACK, COLOR_WHITE);
            }
        }

        menu_draw_select();
        break;
    }

    /* Reset navigation flags */
    menu_move_down = 0;
    menu_move_up = 0;
}
