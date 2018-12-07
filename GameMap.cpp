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
	BLACK,			/*  0 : ��� */
	DARK_BLUE,		/*  1 : ��ο� �Ķ� */
	DARK_GREEN,		/*  2 : ��ο� �ʷ� */
	DARK_SKY_BLUE,	/*  3 : ��ο� �ϴ� */
	DARK_RED,		/*  4 : ��ο� ���� */
	DARK_VIOLET,	/*  5 : ��ο� ���� */
	DARK_YELLOW,	/*  6 : ��ο� ��� */
	GRAY,			/*  7 : ȸ�� */
	DARK_GRAY,		/*  8 : ��ο� ȸ�� */
	BLUE,			/*  9 : �Ķ� */
	GREEN,			/* 10 : �ʷ� */
	SKY_BLUE,		/* 11 : �ϴ� */
	RED,			/* 12 : ���� */
	VIOLET,			/* 13 : ���� */
	YELLOW,			/* 14 : ��� */
	WHITE,			/* 15 : �Ͼ� */
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
/* ȭ���� Ŀ���� ���̰ų� ����� */
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
/* x, y ��ǥ�� Ŀ���� �����̴� �Լ� */
void gotoxy(int x, int y) /*Ŀ���̵� �Լ� �κ�*/
{
    COORD XY = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), XY);
}

/* ========================================================================== */
/* �ܼ� ������ �����ϴ� �Լ� */
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
	case	'0':	SetColor(BLACK);	printf("  ");	break;	/* ����� */
	case	'1':	SetColor(YELLOW);	printf("��");	break;	/* ������ */
	case	'2':	SetColor(GREEN);	printf("��");	break;	/* ��ó�� */
	case	'3':	SetColor(RED);		printf("��");	break;	/* ������ �ڽ� */
	case	'4':	SetColor(RED);		printf("��");	break;	/* �ϼ��� ������ */
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
	SetColor(SKY_BLUE);	printf("��");
}

/* ========================================================================== */

void PlayerDraw1(void)
{
	gotoxy(dx1 * 2, dy1);
	SetColor(RED);	printf("��");
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

	/* Esc�� ������ ������ �����Ѵ�. */
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
		SetColor(YELLOW);	printf("��");
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
				SetColor(RED);	printf("��");
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
				SetColor(RED);	printf("��");
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
				SetColor(RED);	printf("��");
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
				SetColor(RED);	printf("��");
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
			SetColor(RED);	printf("��");
			map[player_y][player_x]= '1';
			gotoxy(player_x * 2, player_y);
			SetColor(YELLOW);	printf("��");
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
		if (data == 0) break;	/* �������� �������� ���� üũ */
		if (data == 'n') {	/* �ٹٲ� ó�� */
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
	int max_x = 0;	/* ���� ū ����ũ�� ã�� */
	int x = 0;	/* ���������� ���� ũ�� */
	int y = 0;	/* ���������� ���� ũ�� */
	char data;

	for (;;) {
		data = *pnt++;
		if (data == 0) break;	/* �������� �������� ���� üũ */
		if (data == 'n') {	/* �ٹٲ� ó�� */
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
