#include <menu.h>

int menu_index = 0;
int menu_move_down = 0; // should update in bluetooth command
int menu_move_up = 0;

// initialize menu after LCD is initialized
void menu_init(){
    LCD_draw_string(&lcd_config, 0, 0, "ELEC391 Piano Bot", COLOR_BLACK, COLOR_WHITE);
    menu_index_0();
}

void menu_update(){
    if (menu_move_down == 1){
        menu_index++;
        if (menu_index > 3) menu_index = 0;
    } else if (menu_move_up == 1){
        menu_index--;
        if (menu_index < 0) menu_index = 3;
    }
    switch (menu_index){
        case 0: menu_index_0(); break;
        case 1: menu_index_1(); break;
        case 2: menu_index_2(); break;
        case 3: menu_index_3(); break;
        default: break;
    }
    // reset all flags:
    menu_move_down = 0;
    menu_move_up = 0;
}

void menu_index_0(){
    LCD_draw_string(&lcd_config, 0, 1, "> 1. Play Song", COLOR_BLACK, COLOR_WHITE);
    LCD_draw_string(&lcd_config, 0, 2, "  2. Transmit Song", COLOR_BLACK, COLOR_WHITE); 
    LCD_draw_string(&lcd_config, 0, 3, "  3. Select Song", COLOR_BLACK, COLOR_WHITE); 
    LCD_draw_string(&lcd_config, 0, 4, "  4. Homing", COLOR_BLACK, COLOR_WHITE);
}
void menu_index_1(){
    LCD_draw_string(&lcd_config, 0, 1, "  1. Play Song", COLOR_BLACK, COLOR_WHITE);
    LCD_draw_string(&lcd_config, 0, 2, "> 2. Transmit Song", COLOR_BLACK, COLOR_WHITE); 
    LCD_draw_string(&lcd_config, 0, 3, "  3. Select Song", COLOR_BLACK, COLOR_WHITE); 
    LCD_draw_string(&lcd_config, 0, 4, "  4. Homing", COLOR_BLACK, COLOR_WHITE);
}
void menu_index_2(){
    LCD_draw_string(&lcd_config, 0, 1, "  1. Play Song", COLOR_BLACK, COLOR_WHITE);
    LCD_draw_string(&lcd_config, 0, 2, "  2. Transmit Song", COLOR_BLACK, COLOR_WHITE); 
    LCD_draw_string(&lcd_config, 0, 3, "> 3. Select Song", COLOR_BLACK, COLOR_WHITE); 
    LCD_draw_string(&lcd_config, 0, 4, "  4. Homing", COLOR_BLACK, COLOR_WHITE);
}
void menu_index_3(){
    LCD_draw_string(&lcd_config, 0, 1, "  1. Play Song", COLOR_BLACK, COLOR_WHITE);
    LCD_draw_string(&lcd_config, 0, 2, "  2. Transmit Song", COLOR_BLACK, COLOR_WHITE); 
    LCD_draw_string(&lcd_config, 0, 3, "  3. Select Song", COLOR_BLACK, COLOR_WHITE); 
    LCD_draw_string(&lcd_config, 0, 4, "> 4. Homing", COLOR_BLACK, COLOR_WHITE);
}