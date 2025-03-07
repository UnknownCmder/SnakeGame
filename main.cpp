#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

int len;
int itemColor, snakeColor = 7;
int difficulty, speed = 50;

bool SNAKE;

bool show_rule;

typedef struct snake_ {//■
	int x, y;
	int dir;
	struct snake_ *next;
} Snake;

typedef struct item_ {//☆
	int x, y;
} Item;

typedef struct obstacle_ {//±
	int x, y;
	struct obstacle_ *next;
} Obs;

int obsCnt = 10;
Obs *obsHead, *obsTail;

Item item;

//Snake *snake;
Snake *head, *tail;
int dir;

int *record, rear;

void menu();
/*
0 = stop
1 = up
2 = left
3 = down
4 = right
*/

void setxy(int x, int y)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consoleHandle, pos);
}

void init()
{
	srand(time(NULL));
	system("mode con cols=80 lines=40 | title Snake Game!");//printf("■");

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

void textcolor(int type)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE ), type);
}

int menuObs()
{
	int x = 34, y = 10;
	textcolor(7);
	setxy(x-2, y);
	printf(">");

	setxy(60, 5);
	printf("1개 추가 : d");
	setxy(60, 6);
	printf("1개 감소 : a");

	setxy(x, y + 1);
	printf("장애물 갯수 랜덤");

	setxy(x, y + 2);
	printf("돌아가기");

	while (1)
	{
		setxy(x, 10);
		printf("장애물 수 : %d개  ", obsCnt);

		char key = getch();
		if (key ==  'w' || key == 'W')
		{
			if (y > 10 - 1)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, --y);
				printf(">");
			}
		}
		else if (key == 's' || key == 'S')
		{
			if (y < 12)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, ++y);
				printf(">");
			}
		}
		else if (key == 'a' || key == 'A')
		{
			if (obsCnt > 0)
			{
				if (obsCnt > 100)
				{
					obsCnt = 100;
				}
				obsCnt--;
			}
		}
		else if (key == 'd' || key == 'D')
		{
			if (obsCnt < 100)
			{
				obsCnt++;
			}
		}
		else if (key == ' ')
		{
			return y - 9;
		}
	}
}

void over()
{
	record = (int *)realloc(record, sizeof(int) * (++rear));
	record[rear - 1] = len;

	textcolor(4);
	setxy(25, 10 + 3);
	printf("game over!");
	printf("\n\n\t\t\t");
	textcolor(7);
	system("pause");

	system("cls");

	return;
}

void move()
{
	Snake *node;

	if (GetAsyncKeyState(0x57) & 0x8000) { //UP = w
        if (dir != 3)
        {
			dir = 1;
		}
    }
    else if (GetAsyncKeyState(0x41) & 0x8000) { //LEFT = a
        if (dir != 4)
        {
           	dir = 2;
		}
    }
    else if (GetAsyncKeyState(0x53) & 0x8000) { //DOWN = s
    	if (dir != 1)
       	{
			dir = 3;
		}
    }
	else if (GetAsyncKeyState(0x44) & 0x8000) { //RIGHT = d
       	if (dir != 2)
       	{
			dir = 4;
		}
	}
}

void obsInstall()
{
	bool stop;
	Obs *node, *cur;
//	srand(time(NULL));
	for (int i = 0; i < obsCnt; i++)
	{
	//	printf("1");
		node = (Obs *)malloc(sizeof(Obs));
		node->next = NULL;

		stop = false;

		int x = rand() % (80 - 1);
		int y = rand() % 40;

		if (x % 2 == 1) x++;

		if (x == 0 && y == 0)
		{
			i--;
			continue;
		}

	//	printf("\n\n%d %d\n", x, y);

		if (obsHead == NULL)
		{
			node->x = x;
			node->y = y;
			obsHead = node;
			obsTail = node;
		}
		else
		{
			for (int j = 0; j < i; j++)
			{
				cur = obsHead;

				if (cur->x == x && cur->y == y)
				{
					i--;
					stop = true;
					break;
				}
				cur = cur->next;
			}

			if (stop == false)
			{
				if (x % 2 == 1) x++;

				node->x = x;
				node->y = y;
				obsTail->next = node;
				obsTail = node;
			}
		}

		if ( stop )
		{
			i--;
			continue;
		}
	}
}

void food()
{
	Snake *cur = tail;
	Obs *obsCur = obsHead;
	bool stop;

//	srand(time(NULL));
	while (tail != NULL)
	{
		stop = false;
		int x = rand() % (80 - 1);
		int y = rand() % 40;

		if (x % 2 == 1) x++;

		if (x == 0 && y == 0) continue;

	///	printf("done1");
		for (int i = 0; i < len; i++)
		{
			if (cur->x == x && cur->y == y)
			{
				stop = true;
				break;
			}
		}

		if ( stop ) continue;

		for (int i = 0; i < obsCnt; i++)
		{
			if (obsCur == NULL) break;
			if (obsCur->x == x && obsCur->y == y)
			{
				stop = true;
				break;
			}
			obsCur = obsCur->next;
		}

		if ( stop ) continue;

		item.x = x;
		item.y = y;

		return;
	}
}

void add()
{
	Snake *node;
	node = (Snake *)malloc(sizeof(Snake));
	node->next = tail;

	if (tail->dir == 1)//w
	{
		//if (tail->y > 0)
		if (tail->y < 39)
		{
			node->x = tail->x;
			node->y = tail->y - 1;
			node->dir = 1;

			setxy(node->x, node->y);
			if ( SNAKE ) printf("ㆍ");
			else printf("■");

			tail = node;
		}
	}
	if (tail->dir == 2)//a
	{
		//if (tail->x > 1)
		if (tail->x < 79)
		{
			node->x = tail->x - 2;
			node->y = tail->y;
			node->dir = 1;

			setxy(node->x, node->y);
			if ( SNAKE ) printf("ㆍ");
			else printf("■");
			tail = node;
		}
	}
	if (tail->dir == 3)//s
	{
	//	if (tail->y < 39)
		if (tail->y > 0)
		{
			node->x = tail->x;
			node->y = tail->y + 1;
			node->dir = 1;

			setxy(node->x, node->y);
			if ( SNAKE ) printf("ㆍ");
			else printf("■");
			tail = node;
		}
	}
	if (tail->dir == 4)//d
	{
		//if (tail->x < 79)
		if (tail->x > 1)
		{
			node->x = tail->x + 2;
			node->y = tail->y;
			node->dir = 1;

			setxy(node->x, node->y);
			if ( SNAKE ) printf("ㆍ");
			else printf("■");
			tail = node;
		}
	}

	len++;
}

void game()
{
	dir = 0;
	//snake = NULL;
	head = tail = NULL;
	item.x = -1;
	item.y = -1;

	Snake *remove;
	len = 1;

	Snake *node, *cur;
	node = (Snake *)malloc(sizeof(Snake));
	node->x = 0;
	node->y = 0;
	node->next = NULL;
	head = node;
	tail = node;

	obsHead = obsTail = NULL;

	setxy(25, 13);
	printf("맵 생성 중...");
	Sleep(50);

	obsInstall();
	food();

	system("cls");
	setxy(25, 13);
	printf("맵 생성 완료!");
	Sleep(10);

	system("cls");

	textcolor(12);
	Obs *obsCur = obsHead;
	for (int i = 0; i < obsCnt; i++)
	{
		setxy(obsCur->x, obsCur->y);
		printf("±");
		obsCur = obsCur->next;
	}
	textcolor(7);

	setxy(0, 0);
	if ( SNAKE ) printf("ㆍ");
	else printf("■");

	item.x = 30;
	item.y = 0;

	while (1)
	{
		//food print
		setxy(item.x, item.y);
		if (itemColor == 15)
		{
			itemColor = 0;
		}
		textcolor(++itemColor);
		printf("☆");
		if (snakeColor == 15)
			textcolor(itemColor);
		else
			textcolor(snakeColor);

		//snake move
		node = (Snake *)malloc(sizeof(Snake));
		move();

		if (dir == 1)
		{
			if (head->y <= 0)
			{
				over();
				return;
			}
			else
			{
    	       	head->dir = 1;

       	    	node->x = head->x;
       	    	node->y = head->y - 1;
       	    	node->dir = 1;
       	    	node->next = NULL;
       	    	head->next = node;
       	    	head = node;

       	    	setxy(head->x, head->y);
       	    	if ( SNAKE ) printf("ㆍ");
				else printf("■");
       	    	setxy(tail->x, tail->y);
       	    	printf("  ");

       	    	remove = tail;
       	    	tail = tail->next;
       	    	free(remove);
			}

		}
		else if (dir == 2)
		{
			if (head->x <= 0)
			{
				over();
				return;
			}
			else
			{
				head->dir = 2;

				node->x = head->x - 2;
          	 	node->y = head->y;
        	   	node->dir = 2;
        	   	node->next = NULL;
           		head->next = node;
           		head = node;

           		setxy(head->x, head->y);
           		if ( SNAKE ) printf("ㆍ");
				else printf("■");
           		setxy(tail->x, tail->y);
           		printf("  ");

         	  	remove = tail;
           		tail = tail->next;
           		free(remove);
			}

		}
		else if (dir == 3)
		{
			if (head->y >= 39)
			{
				over();
				return;
			}
			else
			{
				head->dir = 3;

    	   		node->x = head->x;
       	    	node->y = head->y + 1;
       	    	node->dir = 3;
       	    	node->next = NULL;
       	    	head->next = node;
       	    	head = node;

       	    	setxy(head->x, head->y);
       	    	if ( SNAKE ) printf("ㆍ");
				else printf("■");
       	    	setxy(tail->x, tail->y);
       	    	printf("  ");

       	    	remove = tail;
       	    	tail = tail->next;
       	    	free(remove);
			}
		}
		else if (dir == 4)
		{
			if (head->x >= 79)
			{
				over();
				return;
			}
			else
			{
				head->dir = 4;

   	    		node->x = head->x + 2;
   	        	node->y = head->y;
   	        	node->dir = 4;
   	        	node->next = NULL;
   	        	head->next = node;
   	        	head = node;

   	        	setxy(head->x, head->y);
   	        	if ( SNAKE ) printf("ㆍ");
				else printf("■");
   	        	setxy(tail->x, tail->y);
   	        	printf("  ");

   	        	remove = tail;
   	        	tail = tail->next;
   	        	free(remove);
			}
		}

		if (tail != NULL)
		{
			cur = tail;
    	    while (cur->next != NULL)
			{
				if (head->x == cur->x && head->y == cur->y)
				{
					over();
					return;
				}
				cur = cur->next;
			}
		}

		if (item.y == head->y)
		{
			if (item.x == head->x)
			{
				setxy(item.x - 1, item.y);
				printf(" ");
				add();
				food();
			}
	/*		else if (item.x + 1 == head->x)
			{
				setxy(item.x, item.y);
				printf(" ");
				add();
				food();
			}*/
		}

		obsCur = obsHead;
		for (int i = 0; i < obsCnt; i++)
		{
			if (obsCur->y == head->y)
			{
				if (obsCur->x == head->x)
				{
					over();
					return;
				}
			}

			obsCur = obsCur->next;
		}

        Sleep(speed);
	}
}

void menu()
{
		textcolor(snakeColor);
		setxy(0, 0);
		if ( SNAKE ) printf("ㆍ");
		else printf("■");

		textcolor(12);
		setxy(47, 0);
		printf("*절대 화면크기를 조절하지 마세요!");
		textcolor(7);

		setxy(25, 10 - 2);
		printf("snake");
		setxy(25, 10);
		printf("#####  ####   ## ##  #####");
		setxy(25, 10 + 1);
		printf("#   #  #  #  #  #  # #    ");
		setxy(25, 10 + 2);
		printf("#      #  #  #  #  # #####");
		setxy(25, 10 + 3);
		printf("#  ##  ####  #  #  # #    \t올리기 : w");
		setxy(25, 10 + 4);
		printf("#   #  #  #  #  #  # #    \t내리기 : s");
		setxy(25, 10 + 5);
		printf("#####  #  #  #  #  # #####\t선택 : space");
		return;
}

int button()
{
	int x = 34, y = 25;
	setxy(x-2, y);
	printf("> 게임시작");
	setxy(x, y + 1);
	printf("게임규칙");
	setxy(x, y + 2);
	printf("게임설정");
	setxy(x, y + 3);
	printf("게임기록");

	textcolor(4);
	setxy(x, y + 4);
	printf("  종료  ");

	textcolor(7);

	while (1)
	{
		char key = getch();
		if (key ==  'w' || key == 'W')
		{
			if (y > 25)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, --y);
				printf(">");
			}
		}
		else if (key == 's' || key == 'S')
		{
			if (y < 29)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, ++y);
				printf(">");
			}
		}
		else if (key == ' ')
		{
			return y - 24;
		}
	}
}

void rule()
{
	setxy(25, 10 + 2);
	printf("뱀을 길어지게 만드는 게임!");
	setxy(20, 10 + 3);
	printf("☆(아이템)을 먹을수로 한칸씩 길어집니다");
	setxy(13, 10 + 4);
	printf("화면 밖으로 나가거나 머리가 자기 몸통에 닿으면 탈락입니다");
	setxy(23, 10 + 5);
	printf("±(장애물)을 먹어도 탈락합니다");

	setxy(32, 10 + 7);
	printf("뱀 1칸 = 1점");

	setxy(35, 10 + 9);
	printf("<조작>");
	setxy(30 , 10 + 10);
	printf("위로 이동 : w");
	setxy(30 , 10 + 11);
	printf("왼쪽으로 이동 : a");
	setxy(30 , 10 + 12);
	printf("아래로 이동 : s");
	setxy(30 , 10 + 13);
	printf("오른쪽으로 이동 : d");

	setxy(15, 10 + 15);
	printf("* 장애물이 많을수록 별을 못 먹을 가능성이 증가합니다!");

	setxy(23, 10 + 16);
	system("pause");
	system("cls");
}

int snake_color()
{
	int x = 34, y = 10;
	textcolor(7);
	setxy(x - 2, y);
	printf(">");

	textcolor(1);
	setxy(x, y);
	printf("파란색");

	textcolor(2);
	setxy(x, y + 1);
	printf("초록색");

	textcolor(3);
	setxy(x, y + 2);
	printf("옥색");

	textcolor(4);
	setxy(x, y + 3);
	printf("빨간색");

	textcolor(5);
	setxy(x, y + 4);
	printf("자주색");

	textcolor(6);
	setxy(x, y + 5);
	printf("노란색");

	textcolor(7);
	setxy(x, y + 6);
	printf("흰색");

	textcolor(8);
	setxy(x, y + 7);
	printf("회색");

	textcolor(9);
	setxy(x, y + 8);
	printf("연한 파란색");

	textcolor(10);
	setxy(x, y + 9);
	printf("연한 초록색");

	textcolor(11);
	setxy(x, y + 10);
	printf("연한 옥색");

	textcolor(12);
	setxy(x, y + 11);
	printf("연한 빨간색");

	textcolor(13);
	setxy(x, y + 12);
	printf("연한 자주색");

	textcolor(14);
	setxy(x, y + 13);
	printf("연한 노란색");

	textcolor(0);
	setxy(x, y + 14);
	printf("무지개(?) 색");

	textcolor(0);
	setxy(x, y + 15);
	printf("투명(?) 색");

	textcolor(7);
	setxy(x, y + 16);
	printf("돌아가기");

	while (1)
	{
		char key = getch();
		if (key ==  'w' || key == 'W')
		{
			if (y > 10)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, --y);
				printf(">");
			}
		}
		else if (key == 's' || key == 'S')
		{
			if (y < 26)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, ++y);
				printf(">");
			}
		}
		else if (key == ' ')
		{
			return y - 9;
		}
	}
}

int setDifficulty()
{
	int x = 34, y = 10;
	textcolor(7);
	setxy(x-2, y);
	printf(">");

	textcolor(10);
	setxy(x, y);
	printf("쉬움");

	textcolor(6);
	setxy(x, y + 1);
	printf("보통");

	textcolor(12);
	setxy(x, y + 2);
	printf("어려움");

	textcolor(5);
	setxy(x, y + 3);
	printf("크레이지");

	textcolor(7);
	setxy(x, y + 4);
	printf("돌아가기");

	while (1)
	{
		char key = getch();
		if (key ==  'w' || key == 'W')
		{
			if (y > 10)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, --y);
				printf(">");
			}
		}
		else if (key == 's' || key == 'S')
		{
			if (y < 14)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, ++y);
				printf(">");
			}
		}
		else if (key == ' ')
		{
			return y - 9;
		}
	}
}

int setting()
{
	int x = 34, y = 10;
	setxy(x-2, y);
	printf("> 난이도");

	setxy(x, y + 1);
	printf("뱀 색깔");

	setxy(x, y + 2);
	printf("장애물");

	setxy(x, y + 3);
	printf("돌아가기");
	textcolor(7);

	while (1)
	{
		char key = getch();
		if (key ==  'w' || key == 'W')
		{
			if (y > 10)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, --y);
				printf(">");
			}
		}
		else if (key == 's' || key == 'S')
		{
			if (y < 13)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, ++y);
				printf(">");
			}
		}
		else if (key == 'd' || key == 'D')
		{
			if (y == 11)
			{
				if ( SNAKE ) SNAKE = false;
				else SNAKE = true;
				snakeColor = 12;
			}
		}
		else if (key == ' ')
		{
			return y - 9;
		}
	}
}

void view_record()
{
	int num = rear - 1;

	setxy(60, 5);
	printf("다음 판 : d");
	setxy(60, 6);
	printf("이전 판 : a");

	int x = 24, y = 10;
	setxy(x-2, y);
	printf(">");

	setxy(x, y + 1);
	printf("돌아가기");
	textcolor(7);

	setxy(72, 39);
	printf(__TIME__);

	while (1)
	{
		setxy(x, 10);
		if (rear > 0)
			printf("%d 번째 판 : %d점            ", num + 1, record[num]);
		else
			printf("기록없음");

		char key = getch();
		if (key ==  'w' || key == 'W')
		{
			if (y > 10)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, --y);
				printf(">");
			}
		}
		else if (key == 's' || key == 'S')
		{
			if (y < 11)
			{
				setxy(x - 2, y);
				printf(" ");
				setxy(x - 2, ++y);
				printf(">");
			}
		}
		else if (key == 'a' || key == 'A')
		{
			if (num > 0)
			{
				num--;
			}
		}
		else if (key == 'd' || key == 'D')
		{
			if (num < rear - 1)
			{
				num++;
			}
		}
		else if (key == ' ')
		{
			return;
		}
	}
}

int main(void)
{
	init();
	int maincode = 0;
	while (1)
	{
		textcolor(7);
		system("cls");
		menu();

		maincode = button();
		if (maincode == 1)
		{
			system("cls");
			game();
			if (obsCnt > 100)
			{
				obsCnt = 10;
			}
		}
		else if (maincode == 2)
		{
			system("cls");
			rule();
		}
		else if (maincode == 3)
		{
			system("cls");
			int set = setting();

			if (set == 1)
			{
				system("cls");
				difficulty = setDifficulty();
				if (difficulty == 1)//easy
				{
					speed = 80;
				}
				else if (difficulty == 2)//nomal
				{
					speed = 50;
				}
				else if (difficulty == 3)//hard
				{
					speed = 30;
				}
				else if (difficulty == 4)//crazy
				{
					speed = 10;
				}
			}
			else if (set == 2)
			{
				system("cls");
				snakeColor = snake_color();
				if (snakeColor == 16)
					snakeColor = 0;
			}
			else if (set == 3)
			{
				system("cls");
				int hidObs = menuObs();
				if (hidObs == 0)
				{
					obsCnt = 1000;
				}
				else if (hidObs == 2)
				{
					obsCnt = rand() % 101;
				}
			}
		}
		else if (maincode == 4)
		{
			system("cls");
			view_record();
		}
		else if (maincode == 5)
		{
			return 0;
		}
	}

	return 1;
}