#include <iostream>
#include <cstdlib>			//标准库
#include <windows.h>			//延时函数
#include <stdio.h>			//getchar	
#include <ctime>			
#include <conio.h>
#include <fstream>
//#include <ofstream>
//#include <ifstream>
using namespace std;

bool flag = false;
bool slow = false;
bool autogame = true;
int row, col;
int exitx, exity;
int** maze;// = new int* [];// [MAX_X] [MAX_Y] ;		//迷宫
void mazeRowCol();
void mazeSave();
void createMaze();
void printMaze();
void check();
void menu();
void autoMove();
void gamestart();
void mazeLoad();
#define mazeSavePath  "D:\\maze.txt"


//路线栈
class stack_of_maze {
private:
	//记录迷宫坐标
	struct node
	{
		int x;
		int y;
		char direction;  	//上一步路径（如何来的）
		node* next;
	};
	node* head;
public:
	stack_of_maze() {
		head = NULL;
	}

	~stack_of_maze() {
		node* p = head;
		while (head != NULL) {
			head = head->next;
			delete p;
			p = head;
		}
	}

	//压栈
	void push(int xx, int yy, char ddirection) {
		node* new_node = new node;
		if (new_node != NULL) {
			new_node->x = xx;
			new_node->y = yy;
			new_node->direction = ddirection;
			new_node->next = NULL;

			if (head == NULL)
				head = new_node;
			else {
				new_node->next = head;
				head = new_node;
			}
		}
		else
			cout << "Memmory Error!" << endl;

	}

	//出栈
	node* pop(int& xx, int& yy) {
		if (head != NULL) {
			node* p = head;
			head = head->next;
			xx = p->x;
			yy = p->y;
			delete p;
		}
		return head;
	}

	void print() {
		if (head != NULL) {
			node* p = head;
			while (p != NULL) {
				cout << " " << p->x << " " << p->y << " " << p->direction << endl;
				p = p->next;
			}
		}
		else
			cout << "Print Error (NULL Stuck!)" << endl;
	}
};

//创建迷宫
void createMaze() {
	int maxway = row * col;		//最大通路
	int x, y;

	for (x = 0; x < row; x++)
		for (y = 0; y < col; y++)
			maze[x][y] = 1;			//先填充迷宫

	srand((unsigned)time(NULL));	//随机函数种子，以时间为参数
	for (int i = 0; i < maxway; i++)		//随机构建迷宫通路
	{
		x = rand() % (row - 2) + 1;
		y = rand() % (col - 2) + 1;
		maze[x][y] = 0;
	}

	//maze[1][1] = 0; 				//入口
	//maze[row - 2][col - 2] = 0;		
	maze[exitx][exity] = 0;		//出口
	maze[row / 2][col / 2] = 8;
}

//输出迷宫
void printMaze() {
	int x, y;
	system("cls");
	//cout<<endl;
	for (x = 0; x < row; x++)
	{
		for (y = 0; y < col; y++)
		{
			if (maze[x][y] == 0) { cout << "  "; continue; }		//通路
			if (maze[x][y] == 1) { cout << " X"; continue; }		//墙
			if (maze[x][y] == 2) { cout << "×"; continue; }		//死胡同
			if (maze[x][y] == 3) { cout << " o"; continue; }
			if (maze[x][y] == 4) { cout << " o"; continue; }
			if (maze[x][y] == 5) { cout << " o"; continue; }
			if (maze[x][y] == 6) { cout << " o"; continue; }
			if (maze[x][y] == 7) { cout << " E"; continue; }		//当前站立位置
			if (maze[x][y] == 8) { cout << " S"; continue; }
		}
		cout << endl;
	}
	
	
	if (slow) {
		Sleep(1*1000);											//延时函数
	}
}

void check(stack_of_maze& s) {
	int **temp;

	temp = new int* [row];
	for (int i = 0; i < row; i++)
	{
		temp[i] = new int[col];

	}

	for (int x = 0; x < row; x++)
		for (int y = 0; y < col; y++)
			temp[x][y] = maze[x][y];

	int x = row / 2, y = col / 2;						
	while (1) {
		temp[x][y] = 2;

		//向下
		if (temp[x + 1][y] == 0) {
			s.push(x, y, 'D');
			temp[x][y] = 3;			//在当前位置做一个向下的标志
			x = x + 1;
			temp[x][y] = 7;			//当前位置
			if ((x == exitx) && (y == exity)) {
				flag = true;
				return;
			}
			else
				continue;
		}

		//向右
		if (temp[x][y + 1] == 0) {
			s.push(x, y, 'R');
			temp[x][y] = 4;			//在当前位置做一个向右的标志
			y = y + 1;
			temp[x][y] = 7;
			if ((x == exitx) && (y == exity)) {
				flag = true;
				return;
			}
			else
				continue;
		}

		//向上
		if (temp[x - 1][y] == 0) {
			s.push(x, y, 'U');
			temp[x][y] = 6;			//在当前位置做一个向上的标志
			x = x - 1;
			temp[x][y] = 7;
			if ((x == exitx) && (y == exity)) {
				flag = true;
				return;
			}
			else
				continue;
		}

		//向左
		if (temp[x][y - 1] == 0) {
			s.push(x, y, 'L');
			temp[x][y] = 5;			//在当前位置做一个向右的标志
			y = y - 1;
			temp[x][y] = 7;
			if ((x == exitx) && (y == exity)) {
				flag = true;
				return;
			}
			else
				continue;
		}

		//上下左右不通，则回退
		if (s.pop(x, y) == NULL && temp[x - 1][y] != 0 && temp[x][y - 1] != 0 && temp[x][y + 1] != 0 && temp[x + 1][y] != 0) {
			temp[0][1] = 7;
			if (temp[1][1] != 1)
				temp[1][1] = 2;
			return;
		}
	}
}


void autoMove(stack_of_maze& s) {
	int x = row/2, y = col/2;					//出发点	
	
	
	while (1) {
		maze[x][y] = 2;

		//向下
		if (maze[x + 1][y] == 0) {
			s.push(x, y, 'D');
			maze[x][y] = 3;			//在当前位置做一个向下的标志
			x = x + 1;
			maze[x][y] = 7;			//当前位置
			if (slow)
				printMaze();
			if ((x == exitx) && (y == exity)) {
				s.push(x, y, '*');
				cout << "\n\n              Get out Successful!" << endl;
				return;
			}
			else
				continue;
		}

		//向右
		if (maze[x][y + 1] == 0) {
			s.push(x, y, 'R');
			maze[x][y] = 4;			//在当前位置做一个向右的标志
			y = y + 1;
			maze[x][y] = 7;
			if (slow)
				printMaze();
			if ((x == exitx) && (y == exity)) {
				s.push(x, y, '*');
				cout << "\n\n              Get out Successful!" << endl;
				return;
			}
			else
				continue;
		}

		//向上
		if (maze[x - 1][y] == 0) {
			s.push(x, y, 'U');
			maze[x][y] = 6;			//在当前位置做一个向上的标志
			x = x - 1;
			maze[x][y] = 7;
			if (slow)
				printMaze();
			if ((x == exitx) && (y == exity)) {
				s.push(x, y, '*');
				cout << "\n\n              Get out Successful!" << endl;
				return;
			}
			else
				continue;
		}

		//向左
		if (maze[x][y - 1] == 0) {
			s.push(x, y, 'L');
			maze[x][y] = 5;			//在当前位置做一个向右的标志
			y = y - 1;
			maze[x][y] = 7;
			if (slow)
				printMaze();
			if ((x == exitx) && (y == exity)) {
				s.push(x, y, '*');
				cout << "\n\n              Get out Successful!" << endl;
				return;
			}
			else
				continue;
		}

		//上下左右不通，则回退
		if (s.pop(x, y) == NULL && maze[x - 1][y] != 0 && maze[x][y - 1] != 0 && maze[x][y + 1] != 0 && maze[x + 1][y] != 0) {
			cout << "\n\n              There is no Path to get out..." << endl;
			maze[0][1] = 7;
			if (maze[1][1] != 1)
				maze[1][1] = 2;
			return;
		}
		
	}
}


void gamestart() {
	flag = false;
	while (!flag) {
		stack_of_maze stack;			//定义一个栈的对象，用来记录行走路线	
		createMaze();
		check(stack);
		system("cls");
		cout << "\t*                loading.              *" << endl;
		system("cls");
		cout << "\t*                loading..             *" << endl;
		system("cls");
		cout << "\t*                loading...            *" << endl;
	}
	printMaze();						//输出当前迷宫的初始状态
	cout << "\n\n              Press Enter to Continue" << endl;
	getchar();

	stack_of_maze stack1;
	autoMove(stack1);

	printMaze();							//输出迷宫的最终状态
	cout << "\n\n              Press Enter to Continue" << endl;
	getchar();
	menu();
}

void menu() {
	//system("cls");
	//int num;
	cout << "\t*******************************************************************" << endl;
	cout << "\t*                                                                 *" << endl;
	cout << "\t*         1.View the Complete Maze and Path (Show Exit Only)      *" << endl;
	cout << "\t*                                                                 *" << endl;
	cout << "\t*    2.View the Step By Step Maze and Path(Show Start & Exit)     *" << endl;
	cout << "\t*                                                                 *" << endl;
	cout << "\t*               3.          Exit                                  *" << endl;
	cout << "\t*                                                                 *" << endl;
	cout << "\t*               4.         Save Maze                              *" << endl;
	cout << "\t*                                                                 *" << endl;
	cout << "\t*               5.         Load Maze                              *" << endl;
	cout << "\t*                                                                 *" << endl;
	cout << "\t*******************************************************************" << endl;
	slow = false;
	
	switch (getchar()) {
	case '1':
		autogame = true;
		mazeRowCol();
		gamestart(); break;
	case '2':
		autogame = true;
		slow = true;
		mazeRowCol();
		gamestart();
		break;
	case '3':
		exit(1); break;
	case '4':
		mazeSave();
		break;
	case '5':
		mazeLoad();
		break;
	default:
		cout << "\n\n              Error input! Press Enter to Back!" << endl;
		getchar();
		menu();
	}
	getchar();
}

void mazeRowCol()
{
	cout << "\n              Please input rows and colums with Enter to record:" << endl;
	cin >> row >> col;

	maze = new int* [row];
	for (int i = 0; i < row; i++)
	{
		maze[i] = new int[col];

	}
	cout << "\n              Please input Exit Coord(x,y) with Enter to recort: " << endl;
	cout << "\n              The Coord(x,y) start from 0. If you got a 5x5 map, the max Coord is (4x4)!  " << endl;
	cin >> exitx >> exity;

	if ((exitx == 0 || exity == 0 || exitx == row - 1 || exity == col - 1) && exitx >= 0 && exitx < col && exity >= 0 && exity < row)
	{
		cout << "\n              Input Exit Coord Successful! " << endl;
	}
	else
	{
		cout << "\n              Input Exit Coord Error! " << endl;
		cout << "\n              Please Retry to Input Coord again! " << endl;
		cin >> exitx >> exity;
	}
}

void mazeSave()
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp,mazeSavePath,"w");
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			fprintf(fp, "%d ", maze[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	cout << "\n               Maze Save Successful! Path: D:\\maze.txt " << endl;
	menu();
}

void mazeLoad()
{
	maze = new int* [row];
	for (int i = 0; i < row; i++)
	{
		maze[i] = new int[col];

	}
	int i, j;

	FILE* fp;
	errno_t err;
	//err = fopen_s(&fp, readDataPath, "w");
	err = fopen_s(&fp, mazeSavePath, "r"); //打开文件  
	if (fp == NULL)
	{
		printf("File Open Error!");
		return ;
	}
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			fscanf_s(fp, "%d", &maze[i][j]); /*每次读取一个数，fscanf函数遇到空格或者换行结束*/
		}
		fscanf_s(fp, "\n");
	}
	fclose(fp);
	printMaze();
	menu();

	
}

int main(int argc, char** argv) {
 	menu();
	return 0;
}
