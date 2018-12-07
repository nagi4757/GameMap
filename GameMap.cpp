#include <stdio.h>
#include <conio.h>
#include <windows.h>

/* ========================================================================== */

#define SIZE_MAP_X	40
#define SIZE_MAP_Y	24

#define ONE_SEC		60

#define	ESC			27
#define UP			72
#define DOWN		80
#define LEFT		75
#define RIGHT		77

enum {HIDDEN, SHOW};

enum {
	BLACK,			/*  0 : 까망 */
	DARK_BLUE,		/*  1 : 어두운 파랑 */
	DARK_GREEN,		/*  2 : 어두운 초록 */
	DARK_SKY_BLUE,	/*  3 : 어두운 하늘 */
	DARK_RED,		/*  4 : 어두운 빨강 */
	DARK_VIOLET,	/*  5 : 어두운 보라 */
	DARK_YELLOW,	/*  6 : 어두운 노랑 */
	GRAY,			/*  7 : 회색 */
	DARK_GRAY,		/*  8 : 어두운 회색 */
	BLUE,			/*  9 : 파랑 */
	GREEN,			/* 10 : 초록 */
	SKY_BLUE,		/* 11 : 하늘 */
	RED,			/* 12 : 빨강 */
	VIOLET,			/* 13 : 보라 */
	YELLOW,			/* 14 : 노랑 */
	WHITE,			/* 15 : 하양 */
};

/* ========================================================================== */

const char *stage = "040322222222n20010002n20030002n22200312n21300222n20003002n20001002n22222222n";

/* ========================================================================== */

char map[SIZE_MAP_Y][SIZE_MAP_X];

int stage_x;
int stage_y;

int player_x;
int player_y;

int dx1; 
int	dy1;

int exit_flag;

/* ========================================================================== */
/* 화면의 커서를 보이거나 숨기기 */
void CursorView(char show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(hConsole , &ConsoleCursor);
}

/* ========================================================================== */
/* x, y 좌표로 커서를 움직이는 함수 */
void gotoxy(int x, int y) /*커서이동 함수 부분*/
{
    COORD XY = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), XY);
}

/* ========================================================================== */
/* 콘솔 색깔을 지정하는 함수 */
void SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/* ========================================================================== */

void MapDraw(int x, int y)
{
	char data;

	gotoxy(x * 2, y);
	data = map[y][x];
	switch (data) {
	case	'0':	SetColor(BLACK);	printf("  ");	break;	/* 빈공간 */
	case	'1':	SetColor(YELLOW);	printf("♀");	break;	/* 목적지 */
	case	'2':	SetColor(GREEN);	printf("▩");	break;	/* 벽처리 */
	case	'3':	SetColor(RED);		printf("♥");	break;	/* 움직일 박스 */
	case	'4':	SetColor(RED);		printf("♀");	break;	/* 완성된 목적지 */
	}
}

/* ========================================================================== */

void PlayerErase(void)
{
	MapDraw(player_x, player_y);
}

/* ========================================================================== */

void PlayerDraw(void)
{
	gotoxy(player_x * 2, player_y);
	SetColor(SKY_BLUE);	printf("★");
}

/* ========================================================================== */

void PlayerDraw1(void)
{
	gotoxy(dx1 * 2, dy1);
	SetColor(RED);	printf("♥");
}

/* ========================================================================== */

void GameDraw(void)
{
	int loopx, loopy;

	for (loopy = 0; loopy < SIZE_MAP_Y; loopy++) {
		for (loopx = 0; loopx < SIZE_MAP_X; loopx++) {
			MapDraw(loopx, loopy);
		}
	}
}

/* ========================================================================== */

void PlayerMoveAction(void)
{
	int dx, dy;
	char key;
	char data, data2;

	key = getch();

	/* Esc를 누르면 게임을 종료한다. */
	if (key == ESC) {
		exit_flag = 1;
		return;
	}

	if (key == -32)	key = getch();

	switch (key) {
	case	UP: dx =  0; dy = -1; break;
	case	DOWN:	dx =  0; dy =  1;	break;
	case	LEFT:	dx = -1; dy =  0;	break;
	case	RIGHT:	dx =  1; dy =  0;	break;
	}

	data = map[player_y + dy][player_x + dx];
	data2= map[player_y + dy + dy][player_x + dx +dx];

	if (data != '2') {
		if (data == '3' && data2 == '2')
			dx = 0 , dy = 0;
		if (data == '3' && data2 == '3')
			dx = 0 , dy = 0;
		if (data == '3' && data2 == '4')
			dx = 0 , dy = 0;
		if (data == '4' && data2 == '2')
			dx = 0 , dy = 0;
		if (data == '4' && data2 == '3')
			dx = 0 , dy = 0;
		PlayerErase();
		player_x += dx;
		player_y += dy;
		PlayerDraw();
	}

	if (data == '1') {
		gotoxy(player_x * 2, player_y);
		SetColor(YELLOW);	printf("♀");
		map[player_y][player_x]= '1';
	}
	
	if (data == '3') {
		if(data2 != '3')
			map[player_y][player_x]= '0';

		switch (key) {
		case UP:
			dx1 = player_x;		
			dy1 = player_y - 1;
			if (map[player_y - 1][player_x] != '1')
			{
			map[dy1][dx1]= '3';
			PlayerDraw1();
			}
			if (map[player_y - 1][player_x]== '1')
			{
				gotoxy(player_x * 2, player_y - 1);
				SetColor(RED);	printf("♀");
				map[player_y - 1][player_x]= '4';
			}
		
			break;

		case DOWN:	
			dx1 = player_x;		
			dy1 = player_y + 1;
			if (map[player_y + 1][player_x] != '1')
			{
			map[dy1][dx1]= '3';
			PlayerDraw1();
			}
			if (map[player_y + 1][player_x]== '1')
			{
				gotoxy(player_x * 2, player_y + 1);
				SetColor(RED);	printf("♀");
				map[player_y + 1][player_x]= '4';
			}
			break;
		case LEFT:	
			dx1 = player_x - 1;	
			dy1 = player_y;
			if (map[player_y][player_x - 1] != '1')
			{
			map[dy1][dx1]= '3';
			PlayerDraw1();
			}
			if (map[player_y][player_x - 1]== '1')
			{
				gotoxy((player_x - 1) * 2, player_y);
				SetColor(RED);	printf("♀");
				map[player_y][player_x - 1]= '4';
			}
			
			break;
		case RIGHT:	
			dx1 = player_x + 1;	
			dy1 = player_y;
			if (map[player_y][player_x + 1] != '1')
			{
			map[dy1][dx1]= '3';
			PlayerDraw1();
			}
			if (map[player_y][player_x + 1]== '1')
			{
				gotoxy((player_x + 1) * 2, player_y);
				SetColor(RED);	printf("♀");
				map[player_y][player_x +1]= '4';
			}
			break;
		}
	}
	
	if (data == '4')
	{
		if (data2 == '0')
		{
			map[player_y + dy][player_x + dx]= '3';
			gotoxy((player_x + dx) * 2, player_y + dy);
			SetColor(RED);	printf("♥");
			map[player_y][player_x]= '1';
			gotoxy(player_x * 2, player_y);
			SetColor(YELLOW);	printf("♀");
		}
		
	}
}

/* ========================================================================== */

void GameMapClear(void)
{
	int loopx, loopy;

	for (loopy = 0; loopy < SIZE_MAP_Y; loopy++) {
		for (loopx = 0; loopx < SIZE_MAP_X; loopx++) {
			map[loopy][loopx] = 0;
		}
	}
}

/* ========================================================================== */

void GameStageToMap(const char *pnt)
{
	int x = stage_x;
	int y = stage_y;
	char data;

	GameMapClear();

	for (;;) {
		data = *pnt++;
		if (data == 0) break;	/* 스테이지 데이터의 끝을 체크 */
		if (data == 'n') {	/* 줄바꿈 처리 */
			x = stage_x;
			y++;
			continue;
		}

		map[y][x] = data;

		x++;
	}
}

/* ========================================================================== */

void GameStageSizeSet(const char *pnt)
{
	int max_x = 0;	/* 가장 큰 가로크기 찾기 */
	int x = 0;	/* 스테이지의 가로 크기 */
	int y = 0;	/* 스테이지의 세로 크기 */
	char data;

	for (;;) {
		data = *pnt++;
		if (data == 0) break;	/* 스테이지 데이터의 끝을 체크 */
		if (data == 'n') {	/* 줄바꿈 처리 */
			if (x > max_x)	max_x = x;
			x = 0;
			y++;
			continue;
		}

		x++;
	}

	stage_x = (SIZE_MAP_X - max_x) / 2;
	stage_y = (SIZE_MAP_Y - y) / 2;
}

/* ========================================================================== */

int GameGetPosition(const char *pnt)
{
	int ret;
	char data;

	data = *pnt++;
	ret = data - '0';
	data = *pnt++;
	ret *= 10;
	ret += data - '0';

	return ret;
}

/* ========================================================================== */

void GamePlayerPositionSet(const char *pnt)
{
	player_x = GameGetPosition(pnt);
	player_y = GameGetPosition(pnt + 2);
}

/* ========================================================================== */

void GameStageInit(void)
{
	const char *pnt = stage;

	GamePlayerPositionSet(pnt);
	pnt += 4;

	GameStageSizeSet(pnt);

	player_x += stage_x;
	player_y += stage_y;

	GameStageToMap(pnt);
}

/* ========================================================================== */

int main(void)
{
	exit_flag = 0;

	CursorView(HIDDEN);

	GameStageInit();
	GameDraw();
	PlayerDraw();

	for (;;) {
		PlayerMoveAction();
		
		if (exit_flag != 0) break;
	}

	gotoxy(0, 24);
	CursorView(SHOW);

	return 0;
}

/* ========================================================================== */
