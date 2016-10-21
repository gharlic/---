#ifndef PuzzleTest_Common
#define PuzzleTest_Common
#include "cocos2d.h"
#include <string.h>
#include <iomanip>
#include <locale>
#include "ui/CocosGUI.h"

USING_NS_CC;

// 좌표 관련
#define DESIGN_WIDTH	1080.0f
#define DESIGN_HEIGHT	1920.0f

#define CELL_WIDTH		175.0f
#define CELL_HEIGHT		105.0f

#define BUTTOM_HEIGHT	200.0f

#define MAX_COLS		50
#define MAX_ROWS		50

// 터치 관련
#define MAIN_RECT			1
#define SLIDER_COLS_RECT	2
#define SLIDER_ROWS_RECT	3
#define RESET_POSITION_RECT	4

#define SLIDER_COLS_TYPE	0
#define SLIDER_ROWS_TYPE	1

#define NORMAL_STATE		0
#define SCANE_STATE			1

// 기타
#define OUT_PEAPLE			0
#define JOIN_PEAPLE			1
#define PAY_PEAPLE			2
#define CALC_MODE			3

// 입력 창
#define NAME_MODE			0
#define MENU_MODE			1
#define BACK				0
#define OKAY				1

#define TOUCH_RANGE			50

class Common {
public:
	// 숫자 format - 콤마
	static std::string FormatWithCommas(int value) {
		std::string strValueTS;

		int j, i = 0, k = 0;
		int temp = value;
		int p = 1;
		while (temp > 0) {
			temp /= 10;
			p *= 10;
			i++;
		}
		j = i % 3;
		p /= 10;

		while (i > 0) {
			strValueTS += char((value / p) + '0');
			value %= p;
			p /= 10;
			i--;
			k++;
			j--;
			if ((k % 3 == 0 && i > 0) || (j == 0 && i > 2)) {
				strValueTS += ",";
				k = 0;
			}
		}
		return strValueTS;
	}


};

#endif