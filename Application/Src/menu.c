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

/* Heartbeat timer (main page only) */
static uint32_t last_heartbeat_tick = 0;
#define HEARTBEAT_INTERVAL_MS 500

/* UI state sync — send on every state/index change */
static uint8_t last_sent_state = 0xFF;
static uint8_t last_sent_index = 0xFF;

/* File list send state: 0=not started, 1..N=sending entry N-1, N+1=done */
static int file_list_send_idx = 0;
static int file_list_sent = 0;

/* Transfer status tracking */
static uint8_t last_ft_state = 0xFF;
static uint8_t last_ram_sent_status = 0xFF;

/* ------------------------------------------------------------------ */
/*  Helpers                                                           */
/* ------------------------------------------------------------------ */

static void clear_menu_area(void)
{
    for (int r = 1; r <= 9; r++)
    {
        LCD_draw_string(0, r, "                        ", COLOR_BLACK, COLOR_WHITE);
    }
}

/* Send UI state packet if changed */
static void sync_ui_state(void)
{
    uint8_t st = (uint8_t)menu_state;
    uint8_t idx = (uint8_t)menu_index;
    uint8_t ex1 = 0, ex2 = 0;

    if (menu_state == MENU_STATE_SELECT) {
        idx = (uint8_t)select_song_index;
        ex1 = (uint8_t)fileCount;
        ex2 = (uint8_t)(active_song_index + 1); /* 0 = none */
    }

    if (st != last_sent_state || idx != last_sent_index) {
        pkt_send_ui_state(st, idx, ex1, ex2);
        last_sent_state = st;
        last_sent_index = idx;
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
        pkt_send_transfer_status(0); /* begin */
        last_ft_state = 0xFF;
        page_dirty = 0;
    }

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
        char bar[21];
        int filled = pct / 5;
        for (int i = 0; i < 20; i++)
            bar[i] = (i < filled) ? '#' : '-';
        bar[20] = '\0';
        LCD_draw_string(0, 4, bar, COLOR_BLACK, COLOR_WHITE);
        break;
    }
    case FT_STATE_COMPLETE:
        LCD_draw_string(0, 3, "Status: COMPLETE!       ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 4, "File saved to SD card.  ", COLOR_BLACK, COLOR_WHITE);
        if (last_ft_state != (uint8_t)FT_STATE_COMPLETE) {
            pkt_send_transfer_status(1); /* complete */
        }
        break;
    case FT_STATE_ERROR:
        LCD_draw_string(0, 3, "Status: ERROR           ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 4, "Transfer failed.        ", COLOR_BLACK, COLOR_WHITE);
        if (last_ft_state != (uint8_t)FT_STATE_ERROR) {
            pkt_send_transfer_status(2); /* error */
        }
        break;
    }
    last_ft_state = (uint8_t)ft;
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
        pkt_send_transfer_status(0); /* begin */
        last_ram_sent_status = 0;
        page_dirty = 0;
    }

    if (ram_rx_started && !ram_rx_complete){
        uint8_t pct = (uint8_t)((ram_rx_offset * 100) / MAX_CHORD_EVENTS);
        snprintf(buf, sizeof(buf), "RX: %lu/%d rows %3d%%  ",
                 (unsigned long)ram_rx_offset, MAX_CHORD_EVENTS, pct);
        LCD_draw_string(0, 4, buf, COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 2, "Receiving data...       ", COLOR_BLACK, COLOR_WHITE);
    }

    if (ram_rx_complete){
        uint32_t num_chords = ram_rx_offset;
        snprintf(buf, sizeof(buf), "Got %lu chords!     ", (unsigned long)num_chords);
        LCD_draw_string(0, 3, buf, COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 4, "Song loaded to RAM!     ", COLOR_BLACK, COLOR_WHITE);

        if (last_ram_sent_status != 1) {
            pkt_send_transfer_status(1); /* complete */
            last_ram_sent_status = 1;
        }

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
        sd_list_txt_files();
        select_song_index = 0;
        select_scroll_offset = 0;
        file_list_sent = 0;
        file_list_send_idx = 0;
        page_dirty = 0;
    }

    /* Send file list to PC one entry per main-loop iteration (non-blocking) */
    if (!file_list_sent) {
        if (file_list_send_idx < fileCount) {
            pkt_send_file_entry(fileList[file_list_send_idx].name,
                                fileList[file_list_send_idx].size);
            file_list_send_idx++;
        } else {
            pkt_send_file_list_end((uint8_t)fileCount);
            file_list_sent = 1;
        }
    }

    /* Header */
    snprintf(line, sizeof(line), "[Select] %d songs", (int)fileCount);
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

    for (int i = 0; i < 5; i++)
    {
        int fileIdx = select_scroll_offset + i;
        int row = 2 + i;

        if (fileIdx < fileCount)
        {
            char marker = ' ';
            if (fileIdx == select_song_index)
                marker = '>';
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
/*  Dispatch incoming UART data to the right handler                  */
/* ------------------------------------------------------------------ */

void menu_process_message(uint8_t *data, uint16_t len)
{
    if (len == 0) return;

    MenuState_t state = menu_get_state();

    if (state == MENU_STATE_TRANSMIT &&
        data[0] >= 0xF0 && data[0] <= 0xF3)
    {
        FT_ProcessPacket(data, len);
    }
    else if (state == MENU_STATE_TRANSMIT_RAM && data[0] >= 0xE0 && data[0] <= 0xE2)
    {
        RAM_ProcessPacket(data, len);
    }
    else
    {
        data[len] = '\0';
        if (len > 0 && data[len - 1] == '\n') data[len - 1] = '\0';
        execute_command(data);
    }
}

int menu_try_dispatch_binary(uint8_t *data, uint16_t len)
{
    if (len == 0) return 0;

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
            return 0;

        menu_process_message(data, len);
        return 1;
    }

    if (menu_get_state() == MENU_STATE_TRANSMIT_RAM &&
        data[0] >= 0xE0 && data[0] <= 0xE2)
    {
        uint16_t need = 0;
        switch (data[0]) {
          case FT_CMD_RAM_START: need = 2; break;
          case FT_CMD_RAM_DATA:  need = 4 + sizeof(ChordEvent_t); break;
          case FT_CMD_RAM_END:   need = 2; break;
        }
        if (need == 0 || len < need)
            return 0;

        menu_process_message(data, len);
        return 1;
    }

    menu_process_message(data, len);
    return 1;
}

/* ------------------------------------------------------------------ */
/*  Public API                                                        */
/* ------------------------------------------------------------------ */

MenuState_t menu_get_state(void)
{
    return menu_state;
}

void menu_set_state(MenuState_t s)
{
    menu_state = s;
    page_dirty = 1;
    last_sent_state = 0xFF; /* force re-sync */
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
    /* ---- Handle BACK in any sub-page ---- */
    if (menu_back)
    {
        menu_back = 0;
        if (menu_state != MENU_STATE_MAIN)
        {
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
            menu_move_down = 0;
            menu_move_up = 0;
            menu_enter = 0;
            last_sent_state = 0xFF;
            sync_ui_state();
            return;
        }
    }

    /* ---- State-specific update ---- */
    switch (menu_state)
    {

    /* ===== MAIN MENU ===== */
    case MENU_STATE_MAIN:
    {
        pwm_mode = PWM_STOP;
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
                    menu_state = MENU_STATE_PLAYING;
                    last_sent_state = 0xFF;
                    sync_ui_state();
                    traversal();
                    menu_state = MENU_STATE_MAIN;
                    page_dirty = 1;
                    last_sent_state = 0xFF;
                    clear_menu_area();
                    break;
                case 1: /* Transmit Song to SD card */
                    menu_state = MENU_STATE_TRANSMIT;
                    page_dirty = 1;
                    uart_binary_mode = 1;
                    FT_Init();
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
    }

    /* ===== PLAYING (handled by traversal in command.c) ===== */
    case MENU_STATE_PLAYING:
        /* traversal() is blocking, so we won't normally reach here */
        break;

    /* ===== TRANSMIT SONG PAGE ===== */
    case MENU_STATE_TRANSMIT:
        menu_draw_transmit();
        FT_TimeoutCheck();
        break;

    case MENU_STATE_TRANSMIT_RAM:
        menu_draw_transmit_ram();
        break;

    /* ===== SELECT SONG PAGE ===== */
    case MENU_STATE_SELECT:
        if (menu_move_down)
        {
            select_song_index++;
            if (select_song_index >= fileCount)
                select_song_index = 0;
            if (select_song_index >= select_scroll_offset + 5)
                select_scroll_offset = select_song_index - 4;
            if (select_song_index < select_scroll_offset)
                select_scroll_offset = select_song_index;
            if (select_song_index == 0)
                select_scroll_offset = 0;
        }
        if (menu_move_up)
        {
            select_song_index--;
            if (select_song_index < 0)
                select_song_index = fileCount - 1;
            if (select_song_index < select_scroll_offset)
                select_scroll_offset = select_song_index;
            if (select_song_index >= select_scroll_offset + 5)
                select_scroll_offset = select_song_index - 4;
            if (select_song_index == fileCount - 1)
                select_scroll_offset = (fileCount > 5) ? fileCount - 5 : 0;
        }

        if (menu_enter)
        {
            menu_enter = 0;
            if (fileCount > 0)
            {
                active_song_index = select_song_index;
                char buf[25];
                snprintf(buf, sizeof(buf), "Selected: %-12s  ", fileList[active_song_index].name);
                LCD_draw_string(0, 9, buf, COLOR_BLACK, COLOR_WHITE);
                LCD_draw_string(0, 10, "Parsing song...         ", COLOR_BLACK, COLOR_WHITE);
                sd_parse_array(fileList[active_song_index].name);
                LCD_draw_string(0, 10, "Song ready to play!    ", COLOR_BLACK, COLOR_WHITE);
            }
        }

        menu_draw_select();
        break;

    /* ===== DEBUG PAGE ===== */
    case MENU_STATE_DEBUG:
    {
        pwm_mode = PWM_PID;
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

    /* Heartbeat on every page (keeps PC BT-alive indicator green) */
    {
        uint32_t now = HAL_GetTick();
        if (now - last_heartbeat_tick >= HEARTBEAT_INTERVAL_MS) {
            pkt_send_heartbeat();
            last_heartbeat_tick = now;
        }
    }

    /* Sync UI state to PC */
    sync_ui_state();

    /* Reset navigation flags */
    menu_move_down = 0;
    menu_move_up = 0;
}
