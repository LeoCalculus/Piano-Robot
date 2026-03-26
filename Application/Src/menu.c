#include <menu.h>
#include <diskio.h>
#include <file_transfer.h>
#include <string.h>
#include <stdio.h>
#include <application.h>

extern PID_t left_motor;
extern PID_t right_motor;

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
        LCD_draw_string(0, r, "                        ", COLOR_BLACK, COLOR_WHITE);
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
        "5. Homing",
        "6. Debug"};
    char line[25];
    for (int i = 0; i < 6; i++)
    {
        snprintf(line, sizeof(line), "%s %s", (i == menu_index) ? ">" : " ", items[i]);
        /* Pad to clear old text */
        int len = strlen(line);
        while (len < 23)
            line[len++] = ' ';
        line[23] = '\0';
        LCD_draw_string(0, 1 + i, line, COLOR_BLACK, COLOR_WHITE);
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
        LCD_draw_string(0, 1, "[Transmit Song]         ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 2, "Waiting for file...     ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 8, "Send :a to go back      ", COLOR_BLACK, COLOR_WHITE);
        page_dirty = 0;
    }

    /* Show live transfer status */
    FT_State_t ft = FT_GetState();
    char buf[25];

    switch (ft)
    {
    case FT_STATE_IDLE:
        LCD_draw_string(0, 3, "Status: IDLE            ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 4, "                        ", COLOR_BLACK, COLOR_WHITE);
        break;
    case FT_STATE_RECEIVING:
    {
        uint8_t pct = FT_GetProgress();
        snprintf(buf, sizeof(buf), "Status: RX  %3d%%        ", pct);
        LCD_draw_string(0, 3, buf, COLOR_BLACK, COLOR_WHITE);

        /* Simple progress bar */
        char bar[21];
        int filled = pct / 5; /* 0-20 */
        for (int i = 0; i < 20; i++)
            bar[i] = (i < filled) ? '#' : '-';
        bar[20] = '\0';
        LCD_draw_string(0, 4, bar, COLOR_BLACK, COLOR_WHITE);
        break;
    }
    case FT_STATE_COMPLETE:
        LCD_draw_string(0, 3, "Status: COMPLETE!       ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 4, "File saved to SD card.  ", COLOR_BLACK, COLOR_WHITE);
        break;
    case FT_STATE_ERROR:
        LCD_draw_string(0, 3, "Status: ERROR           ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 4, "Transfer failed.        ", COLOR_BLACK, COLOR_WHITE);
        break;
    }
}

/* ------------------------------------------------------------------ */
/*  RAM accumulation helper (depreciated)                             */
/* ------------------------------------------------------------------ */

// static void RAM_AccumulateData(uint8_t *data, uint16_t len)
// {
//     if (!ram_rx_started)
//     {
//         const float start_sentinel = RAM_SENTINEL_START;
//         for (int i = 0; i <= (int)len - 4; i++)
//         {
//             if (memcmp(&data[i], &start_sentinel, 4) == 0)
//             {
//                 ram_rx_started = 1;
//                 ram_rx_offset = 0;
//                 int remaining = len - (i + 4);
//                 if (remaining > 0)
//                 {
//                     uint32_t copy_len = ((uint32_t)remaining < RAM_SONG_MAX_BYTES)
//                                         ? (uint32_t)remaining : RAM_SONG_MAX_BYTES;
//                     // find check sum
//                     uint8_t checksum = RAM_CalculateChecksum(&data[i + 4], copy_len-1); // last byte is checksum for verification
//                     if (checksum != data[i + 4 + copy_len - 1]) {
//                         // checksum mismatch, discard data and reset state
//                         // send NAK
//                         HAL_UART_Transmit(&huart1, (uint8_t[]){0xFF}, 1, 50);
//                         return;
//                     }
//                     // send ACK
//                     HAL_UART_Transmit(&huart1, (uint8_t[]){0xAA}, 1, 50);
                    
//                     memcpy((uint8_t *)song_ram, &data[i + 4], copy_len);
//                     ram_rx_offset = copy_len;
//                 }
//                 return;
//             }
//         }
//         /* Not song data — treat as text command (e.g. :a to go back) */
//         data[len] = '\0';
//         if (len > 0 && data[len - 1] == '\n') data[len - 1] = '\0';
//         execute_command(data);
//     }
//     else
//     {
//         uint32_t space = RAM_SONG_MAX_BYTES - ram_rx_offset;
//         uint32_t copy_len = (len < space) ? len : space;
//         memcpy(&((uint8_t *)song_ram)[ram_rx_offset], data, copy_len);
//         ram_rx_offset += copy_len;

//         const float end_sentinel = RAM_SENTINEL_END;
//         if (ram_rx_offset >= 4)
//         {
//             float last_float;
//             memcpy(&last_float, &((uint8_t *)song_ram)[ram_rx_offset - 4], 4);
//             if (last_float == end_sentinel)
//             {
//                 ram_rx_offset -= 4;
//                 ram_rx_complete = 1;
//             }
//         }
//         if (ram_rx_offset >= RAM_SONG_MAX_BYTES)
//             ram_rx_complete = 1;
//     }
// }

/* ------------------------------------------------------------------ */
/*  Dispatch incoming UART data to the right handler                  */
/* ------------------------------------------------------------------ */

void menu_process_message(uint8_t *data, uint16_t len)
{
    if (len == 0) return;

    MenuState_t state = menu_get_state();

    if (state == MENU_STATE_TRANSMIT &&
        data[0] >= 0xF0 && data[0] <= 0xF3)
    {
        /* File transfer packet → SD card */
        FT_ProcessPacket(data, len);
    }
    else if (state == MENU_STATE_TRANSMIT_RAM && data[0] >= 0xE0 && data[0] <= 0xE2)
    {
        /* Raw binary → RAM accumulation */
        // RAM_AccumulateData(data, len);
        RAM_ProcessPacket(data, len);
    }
    else
    {
        /* Text command */
        data[len] = '\0';
        if (len > 0 && data[len - 1] == '\n') data[len - 1] = '\0';
        execute_command(data);
    }
}

/* Try to dispatch accumulated binary bytes (called from main loop).
   Returns 1 if data was consumed, 0 if still waiting for more bytes. */
int menu_try_dispatch_binary(uint8_t *data, uint16_t len)
{
    if (len == 0) return 0;

    // check for MENU in transmit state and waiting for file data packets (0xF0-0xF3)
    if (menu_get_state() == MENU_STATE_TRANSMIT &&
        data[0] >= 0xF0 && data[0] <= 0xF3)
    {
        uint16_t need = 0;
        switch (data[0]) {
          case FT_CMD_FILE_START: need = FT_FILE_START_SIZE; break;
          case FT_CMD_FILE_DATA:  need = (len >= 4) ? (uint16_t)(5 + data[3]) : 0xFFFF; break;
          case FT_CMD_FILE_END:   need = FT_FILE_END_SIZE; break;
          case FT_CMD_FILE_ABORT: need = FT_FILE_ABORT_SIZE; break;
        }
        if (need == 0 || len < need)
            return 0; /* not enough bytes yet */

        menu_process_message(data, len);
        return 1;
    }

    // check for MENU in transmit RAM state and waiting for RAM packets (0xE0-0xE2)
    if (menu_get_state() == MENU_STATE_TRANSMIT_RAM &&
        data[0] >= 0xE0 && data[0] <= 0xE2)
    {
        uint16_t need = 0;
        switch (data[0]) {
          case FT_CMD_RAM_START: need = 2; break;   // [0xE0] [sentinel]
          case FT_CMD_RAM_DATA:  need = 60; break;   // [0xE1] [row_lo] [row_hi] [56 data] [checksum]
          case FT_CMD_RAM_END:   need = 2; break;   // [0xE2] [sentinel]
        }
        if (need == 0 || len < need)
            return 0; /* not enough bytes yet */

        menu_process_message(data, len);
        return 1;
    }

    /* Everything else (text commands) — dispatch immediately */
    menu_process_message(data, len);
    return 1;
}

/* ------------------------------------------------------------------ */
/*  Transmit Song (RAM) page                                          */
/* ------------------------------------------------------------------ */

void menu_draw_transmit_ram(void){
    char buf[32];

    if (page_dirty){
        clear_menu_area();
        LCD_draw_string(0, 1, "[Transmit to RAM]       ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 2, "Waiting for start...    ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 3, "Max 365x14 floats       ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 8, "Send :a to go back      ", COLOR_BLACK, COLOR_WHITE);
        page_dirty = 0;
    }

    /* Show live accumulation status */
    if (ram_rx_started && !ram_rx_complete){
        uint32_t total = RAM_SONG_MAX_BYTES;
        uint8_t pct = (uint8_t)((ram_rx_offset * 100) / total);
        snprintf(buf, sizeof(buf), "RX: %lu/%lu  %3d%%   ",
                 (unsigned long)ram_rx_offset, (unsigned long)total, pct);
        LCD_draw_string(0, 4, buf, COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 2, "Receiving data...       ", COLOR_BLACK, COLOR_WHITE);
    }

    if (ram_rx_complete){
        uint32_t num_floats = ram_rx_offset / 4;
        uint32_t num_chords = num_floats / 14;
        snprintf(buf, sizeof(buf), "Got %lu chords!     ", (unsigned long)num_chords);
        LCD_draw_string(0, 3, buf, COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 4, "Parsing song...         ", COLOR_BLACK, COLOR_WHITE);

        /* Parse the flat float buffer into chord_events struct */
        parsing_song_buffer_to_struct(song_ram, chord_events);

        LCD_draw_string(0, 4, "Song loaded to RAM!     ", COLOR_BLACK, COLOR_WHITE);

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
        sd_list_txt_files();
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
    LCD_draw_string(0, 1, line, COLOR_BLACK, COLOR_WHITE);

    if (fileCount == 0)
    {
        LCD_draw_string(0, 2, "No songs on SD card.    ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 7, "Send :a to go back      ", COLOR_BLACK, COLOR_WHITE);
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
            LCD_draw_string(0, row, line, COLOR_BLACK, COLOR_WHITE);
        }
        else
        {
            LCD_draw_string(0, row, "                        ", COLOR_BLACK, COLOR_WHITE);
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
        LCD_draw_string(0, 7, line, COLOR_BLACK, COLOR_WHITE);
    }

    LCD_draw_string(0, 8, ":d select  :a back      ", COLOR_BLACK, COLOR_WHITE);
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
    LCD_draw_string(0, 0, "ELEC391 Piano Bot       ", COLOR_BLACK, COLOR_WHITE);
    menu_state = MENU_STATE_MAIN;
    menu_index = 0;
    page_dirty = 1;
    menu_draw_main();
}

void menu_update(void)
{

    char line_buf[32];
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
            uart_binary_mode = 0;
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
            if (menu_index > 5)
                menu_index = 0;
        }
        if (menu_move_up)
        {
            menu_index--;
            if (menu_index < 0)
                menu_index = 5;
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
                        LCD_draw_string(0, 9, "Playing song...         ", COLOR_BLACK, COLOR_WHITE);
                        /* TODO: hook into traversal_song with the selected file */
                    }
                    else
                    {
                        LCD_draw_string(0, 9, "No song selected!       ", COLOR_BLACK, COLOR_WHITE);
                    }
                    break;
                case 1: /* Transmit Song to SD card */
                    menu_state = MENU_STATE_TRANSMIT;
                    page_dirty = 1;
                    uart_binary_mode = 1;
                    FT_Init(); /* Reset transfer state */
                    break;
                case 2: /* Transmit Song to RAM */
                    menu_state = MENU_STATE_TRANSMIT_RAM;
                    page_dirty = 1;
                    uart_binary_mode = 1;
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
                    LCD_draw_string(0, 9, "Homing...               ", COLOR_BLACK, COLOR_WHITE);
                    homing_procedure();
                    LCD_draw_string(0, 9, "Homing done!            ", COLOR_BLACK, COLOR_WHITE);
                    break;
                case 5: /* Debug */
                    menu_state = MENU_STATE_DEBUG;
                    page_dirty = 1;
                    break;
            }
        }

        else{
            for(int i = 9; i<14; i++)
                LCD_draw_string(0, i,  "                         ", COLOR_BLACK, COLOR_WHITE);
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
                LCD_draw_string(0, 9, buf, COLOR_BLACK, COLOR_WHITE);
            }
        }

        menu_draw_select();
        break;

    /* ===== DEBUG PAGE ===== */
    case MENU_STATE_DEBUG:
    {
        if (page_dirty) {
            clear_menu_area();
            LCD_draw_string(0, 1, "[Debug] :a to go back   ", COLOR_BLACK, COLOR_WHITE);
            page_dirty = 0;
        }
        char dbg[25];
        snprintf(dbg, sizeof(dbg), "Enc1: %ld  Enc2: %ld      ", left_motor.encoder_cnt, right_motor.encoder_cnt);
        LCD_draw_string(0, 2, dbg, COLOR_BLACK, COLOR_WHITE);
        snprintf(dbg, sizeof(dbg), "Tar1: %.2f Tar2: %.2f  ", left_motor.target_pos, right_motor.target_pos);
        LCD_draw_string(0, 3, dbg, COLOR_BLACK, COLOR_WHITE);
        snprintf(dbg, sizeof(dbg), "Pos1: %.2f Pos2: %.2f  ", left_motor.current_pos, right_motor.current_pos);
        LCD_draw_string(0, 4, dbg, COLOR_BLACK, COLOR_WHITE);
        snprintf(dbg, sizeof(dbg), "L: %.3f %.3f %.3f  ", left_motor.Kp, left_motor.Ki, left_motor.Kd);
        LCD_draw_string(0, 5, dbg, COLOR_BLACK, COLOR_WHITE);
        snprintf(dbg, sizeof(dbg), "R: %.3f %.3f %.3f  ", right_motor.Kp, right_motor.Ki, right_motor.Kd);
        LCD_draw_string(0, 6, dbg, COLOR_BLACK, COLOR_WHITE);
        break;
    }
    }

    /* Reset navigation flags */
    menu_move_down = 0;
    menu_move_up = 0;
}
