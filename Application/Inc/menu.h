#ifndef __MENU_H
#define __MENU_H

#include <ILI9341.h>
#include <hc04bt.h>
#include <application.h>

extern int menu_index;
extern int menu_move_down;
extern int menu_move_up;

void menu_init();
void menu_update();

void menu_index_0();
void menu_index_1();
void menu_index_2();
void menu_index_3();

#endif