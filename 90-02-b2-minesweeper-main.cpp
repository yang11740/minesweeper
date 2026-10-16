/* 2352475 计科 易阳 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h> 
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../include/cmd_console_tools.h"
#include "../include/cmd_gmw_tools.h"
using namespace std;

#define ROW_MAX 16
#define COL_MAX 30

struct RAND {
	char number = 0;
	bool mine_sign = 0;
	bool sel_sign = 0;
	int show_sign = 0;
};

void input(int& row, int& col, int& num)
{
	char sel;
	while (1) {
		cct_gotoxy(0, 0);
		cout << "请输入扫雷游戏的等级（数字），初级\\中级\\高级（1\\2\\3）";
		sel = _getche();
		if ('1' == sel || '2' == sel || '3' == sel) {
			break;
		}
		else {
			cout << "\b" << " " << "\b";
		}
	}
	if ('1' == sel) {
		row = 9;
		col = 9;
		num = 10;
	}
	else if ('2' == sel) {
		row = 16;
		col = 16;
		num = 40;
	}
	else if ('3' == sel) {
		row = 16;
		col = 30;
		num = 99;
	}
}
void produce(int row, int col, int num, struct RAND str[ROW_MAX][COL_MAX])
{
	int x, y, n, i, j;

	for (n = 0; n < num; n++) {
		x = rand() % col;
		y = rand() % row;
		if (str[y][x].mine_sign == 0) {
			str[y][x].mine_sign = 1;
			str[y][x].number = '*';
		}
		else {
			--n;
		}
	}
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			n = 0;
			if (str[i][j].number != '*') {
				if (i >= 1) {
					if (j >= 1) {
						if (str[i - 1][j - 1].number == '*')
							++n;
					}
					if (str[i - 1][j].number == '*')
						++n;
					if (j < col - 1) {
						if (str[i - 1][j + 1].number == '*')
							++n;
					}
				}
				if (j >= 1) {
					if (str[i][j - 1].number == '*')
						++n;
					if (i < row - 1) {
						if (str[i + 1][j - 1].number == '*')
							++n;
					}
				}
				if (j < col - 1) {
					if (str[i][j + 1].number == '*')
						++n;
				}
				if (i < row - 1) {
					if (str[i + 1][j].number == '*')
						++n;
					if (str[i + 1][j + 1].number == '*')
						++n;
				}
				if (n) {
					str[i][j].number = n;
				}
				else {
					str[i][j].number = n + '0';
				}
			}
			else {
				continue;
			}
		}
	}
}
void judge(int row, int col, int t_row, int t_col, struct RAND str[ROW_MAX][COL_MAX])
{
	if (str[t_row][t_col].number != '0') {
		str[t_row][t_col].show_sign = 1;
		return;
	}
	if (t_col != col) {
		if (str[t_row][t_col].number == '0') {
			str[t_row][t_col].show_sign = 1;
			if (str[t_row][t_col + 1].show_sign == 0) {
				judge(row, col, t_row, t_col + 1, str);
			}
		}
	}
	if (t_col != 0) {
		if (str[t_row][t_col].number == '0') {
			str[t_row][t_col].show_sign = 1;
			if (str[t_row][t_col - 1].show_sign == 0) {
				judge(row, col, t_row, t_col - 1, str);
			}
		}
	}
	if (t_row != row) {
		if (str[t_row][t_col].number == '0') {
			str[t_row][t_col].show_sign = 1;
			if (str[t_row + 1][t_col].show_sign == 0) {
				judge(row, col, t_row + 1, t_col, str);
			}
		}
	}
	if (t_row != 0) {
		if (str[t_row][t_col].number == '0') {
			str[t_row][t_col].show_sign = 1;
			if (str[t_row - 1][t_col].show_sign == 0) {
				judge(row, col, t_row - 1, t_col, str);
			}
		}
	}
}
int play_by_mouse(const CONSOLE_GRAPHICS_INFO* const pCGI, struct RAND str[ROW_MAX][COL_MAX], const BLOCK_DISPLAY_INFO* const bdi,int num, LARGE_INTEGER tick, LARGE_INTEGER begin)
{
	int i, j, n = num, n_man = 0, n_true = 0, n_show = 0, time_sign = 0;
	int maction, keycode1, keycode2, mrow, mcol, ret;
	char temp[256], temp1[256];
	LARGE_INTEGER end;

	cct_setcursor(CURSOR_INVISIBLE);
	cct_enable_mouse();
	while (1) {
		sprintf(temp, "ESC退出，空格显示时间");
		if (time_sign) {
			sprintf(temp1, "当前时间：%.6lf秒，", double(end.QuadPart - begin.QuadPart) / tick.QuadPart);
			time_sign = 0;
		}
		else {
			sprintf(temp1, "剩余雷数：%d，", n);
		}
		gmw_status_line(pCGI, TOP_STATUS_LINE, temp, temp1);
		if (n_true == num && n_show == pCGI->row_num * pCGI->col_num - num) {
			return 1;
		}
		ret = gmw_read_keyboard_and_mouse(pCGI, maction, mrow, mcol, keycode1, keycode2, true);
		if (ret == CCT_KEYBOARD_EVENT) {
			if (keycode1 == 0x1B) {
				return -1;
			}
			if (keycode1 == 32) {
				QueryPerformanceCounter(&end);//获得终止硬件定时器计数
				time_sign = 1;
			}
		}
		else if (ret == CCT_MOUSE_EVENT) {
			switch (maction) {
				case MOUSE_ONLY_MOVED:
					break;
				case MOUSE_LEFT_BUTTON_CLICK:
					if (str[mrow][mcol].show_sign == -1) {
						break;
					}
					else {
						judge(pCGI->row_num, pCGI->col_num, mrow, mcol, str);
						for (i = 0; i < pCGI->row_num; i++) {
							for (j = 0; j < pCGI->col_num; j++) {
								if (str[i][j].show_sign == 1) {
									gmw_draw_block(pCGI, i, j, str[i][j].number, bdi);
									str[i][j].show_sign = -1;
									n_show++;
									if (str[i][j].number == '*') {
										return 0;
									}
								}
							}
						}
						break;
					}
				case MOUSE_RIGHT_BUTTON_CLICK:
					if (str[mrow][mcol].sel_sign == 0 && str[mrow][mcol].show_sign == 0) {
						gmw_draw_block(pCGI, mrow, mcol, '#', bdi);
						str[mrow][mcol].sel_sign = 1;
						if (str[mrow][mcol].mine_sign == 1) {
							n_true++;
						}
						n_man++;
						if (n > 0) {
							n--;
						}
					}
					else if (str[mrow][mcol].sel_sign == 1 && str[mrow][mcol].show_sign == 0) {
						gmw_draw_block(pCGI, mrow, mcol, 0, bdi);
						str[mrow][mcol].sel_sign = 0;
						if (str[mrow][mcol].mine_sign == 1) {
							n_true--;
						}
						if (n >= 0 && n < num && n_man <= num) {
							n++;
						}
						n_man--;
					}
					break;
				default:
					break;
			}
		}
	}
}

int main()
{
	int row, col, i, j, num, ret;
	CONSOLE_GRAPHICS_INFO MyCGI;
	char temp[256], temp1[256];
	RAND mine[ROW_MAX][COL_MAX];
	LARGE_INTEGER tick, begin, end;

	srand((unsigned int)(time(0)));

	gmw_init(&MyCGI);
	cct_setconsoleborder(90, 30, 120, 9000);
	input(row, col, num);
	gmw_set_rowcol(&MyCGI, row, col);
	gmw_set_frame_color(&MyCGI, COLOR_HWHITE, COLOR_BLACK);
	gmw_set_font(&MyCGI, "新宋体", 16, 0);
	gmw_set_frame_style(&MyCGI, 4, 2, true);
	gmw_set_rowno_switch(&MyCGI, true);
	gmw_set_colno_switch(&MyCGI, true);
	gmw_set_delay(&MyCGI, DELAY_OF_DRAW_FRAME, 0);
	gmw_set_block_border_switch(&MyCGI, false);

	
	const BLOCK_DISPLAY_INFO bdi[] = {
		{BDI_VALUE_BLANK,COLOR_YELLOW, COLOR_YELLOW, "  "},
		{'0', COLOR_WHITE, COLOR_WHITE, "  "},
		{1, COLOR_WHITE, 1, NULL},
		{2, COLOR_WHITE, 2, NULL},
		{3, COLOR_WHITE, 3, NULL},
		{4, COLOR_WHITE, 4, NULL},
		{5, COLOR_WHITE, 5, NULL},
		{6, COLOR_WHITE, 6, NULL},
		{7, COLOR_WHITE, 7, NULL},
		{8, COLOR_WHITE, 8, NULL},
		{'*', COLOR_WHITE, 9, "*"},
		{'#', COLOR_HRED, COLOR_BLACK, "#"},
		{BDI_VALUE_END, -1, -1, NULL}
	};
	
	while (1) {
		produce(MyCGI.row_num, MyCGI.col_num, num, mine);
		gmw_draw_frame(&MyCGI);
		for (i = 0; i < MyCGI.row_num; i++) {
			for (j = 0; j < MyCGI.col_num; j++) {
				gmw_draw_block(&MyCGI, i, j, 0, bdi);
			}
		}
		sprintf(temp, "ESC退出，空格显示时间");
		gmw_status_line(&MyCGI, TOP_STATUS_LINE, temp);

		QueryPerformanceFrequency(&tick);	//获得时钟频率
		QueryPerformanceCounter(&begin);	//获得初始硬件定时器计数

		ret = play_by_mouse(&MyCGI, mine, bdi, num, tick, begin);
		if (ret == -1) {
			break;
		}
		else if (ret == 0) {
			QueryPerformanceCounter(&end);//获得终止硬件定时器计数
			sprintf(temp, "你输了，游戏结束");
			sprintf(temp1, "共用时：%.6lf秒，", double(end.QuadPart - begin.QuadPart) / tick.QuadPart);
			gmw_status_line(&MyCGI, LOWER_STATUS_LINE, temp,temp1);
			break;
		}
		else if (ret == 1) {
			QueryPerformanceCounter(&end);//获得终止硬件定时器计数
			sprintf(temp, "恭喜胜利！游戏结束");
			sprintf(temp1, "共用时：%.6lf秒，", double(end.QuadPart - begin.QuadPart) / tick.QuadPart);
			gmw_status_line(&MyCGI, LOWER_STATUS_LINE, temp, temp1);
			break;
		}
	}
	cct_setcursor(CURSOR_VISIBLE_NORMAL);
	cct_setcolor();
	cct_gotoxy(MyCGI.SLI.lower_start_x, MyCGI.SLI.lower_start_y + 1);
	return 0;
}