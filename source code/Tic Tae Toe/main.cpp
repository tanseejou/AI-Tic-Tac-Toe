#include "graphics.h"
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <thread>
using namespace std;

int cpu_move_to_row=1, cpu_move_to_col=1;
const int no_of_page = 4;


const string WindowName = "TicTacToe...";
const string MainBackgroundImageName = "images/EntryBG.jpg";


bool isExit = false;

const int width = 780;
const int height = 780;

//total grid will be 9
const int total_row = 3, total_col = 3;


class Point {
public: int x, y; Point(int _x=0, int _y=0) :x(_x), y(_y) {}
};
class Grid {
public: Point topLeft, bottomRight; char occupiedType;
		Grid(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0) :topLeft(Point(x1, y1)), bottomRight(Point(x2, y2)) {occupiedType = '*'; }
		Grid(Point TL, Point BR) : topLeft(TL), bottomRight(BR) { occupiedType = '*'; }
		int getWidth() { return abs(topLeft.x - bottomRight.x); }
		int getHeight() { return abs(topLeft.y - bottomRight.y); }
};

Grid grid[total_row][total_col];

void drawGrid(int width, int height, int margin)
{
	//draw 3 vertical line
	setcolor(WHITE);
	setlinestyle(0, 0, 3);


	int row_width = (width - margin * 2) / 3;
	int col_width = (height - margin * 2) / 3;



	//draw 4 horizontal line
	line(margin, margin, width - margin, margin);

	line(margin, margin + row_width, width - margin, margin + row_width);

	line(margin, margin + row_width * 2, width - margin, margin + row_width * 2);

	line(margin, height - margin, width - margin, height - margin);


	//draw 4 vertical line
	line(margin, margin, margin, height - margin);

	line(margin + col_width, margin, margin + col_width, height - margin);

	line(margin + col_width * 2, margin, margin + col_width * 2, height - margin);

	line(width - margin, margin, width - margin, height - margin);

	for (int i = 0; i < total_row; i++)
		for (int j = 0; j < total_col; j++)
			grid[i][j] = Grid(margin + j * col_width, margin + i * row_width, margin + (j+1) * col_width, margin + (i+1) * row_width);

}


#pragma region Full Condition

int isFull()// grid is full
{
	for (int i=0; i<total_row; i++)
		for (int j=0; j<total_col; j++)
			if (grid[i][j].occupiedType!='X')
				if(grid[i][j].occupiedType != 'O')
					return 0;
	return 1;
}

#pragma endregion


#pragma region WinningCondition

int me_won()
{
	//horizontal win condition
	if (grid[0][0].occupiedType == 'O' && grid[0][1].occupiedType == 'O' && grid[0][2].occupiedType == 'O')                
		return 1;			
	if (grid[1][0].occupiedType == 'O' && grid[1][1].occupiedType == 'O' && grid[1][2].occupiedType == 'O')
		return 1;
	if (grid[2][0].occupiedType == 'O' && grid[2][1].occupiedType == 'O' && grid[2][2].occupiedType == 'O')
		return 1;

	//vertical win condition
	if (grid[0][0].occupiedType == 'O' && grid[1][0].occupiedType == 'O' && grid[2][0].occupiedType == 'O')
		return 1;																							   
	if (grid[0][1].occupiedType == 'O' && grid[1][1].occupiedType == 'O' && grid[2][1].occupiedType == 'O')
		return 1;
	if (grid[0][2].occupiedType == 'O' && grid[1][2].occupiedType == 'O' && grid[2][2].occupiedType == 'O')
		return 1;

	//sliding win condition
	if (grid[0][0].occupiedType == 'O' && grid[1][1].occupiedType == 'O' && grid[2][2].occupiedType == 'O')
		return 1;
	if (grid[0][2].occupiedType == 'O' && grid[1][1].occupiedType == 'O' && grid[2][0].occupiedType == 'O')
		return 1;
	
	return 0;
}

int cpu_won()
{
	//horizontal win condition
	if (grid[0][0].occupiedType == 'X' && grid[0][1].occupiedType == 'X' && grid[0][2].occupiedType == 'X')
		return 1;
	if (grid[1][0].occupiedType == 'X' && grid[1][1].occupiedType == 'X' && grid[1][2].occupiedType == 'X')
		return 1;
	if (grid[2][0].occupiedType == 'X' && grid[2][1].occupiedType == 'X' && grid[2][2].occupiedType == 'X')
		return 1;

	//vertical win condition
	if (grid[0][0].occupiedType == 'X' && grid[1][0].occupiedType == 'X' && grid[2][0].occupiedType == 'X')
		return 1;
	if (grid[0][1].occupiedType == 'X' && grid[1][1].occupiedType == 'X' && grid[2][1].occupiedType == 'X')
		return 1;
	if (grid[0][2].occupiedType == 'X' && grid[1][2].occupiedType == 'X' && grid[2][2].occupiedType == 'X')
		return 1;

	//sliding win condition
	if (grid[0][0].occupiedType == 'X' && grid[1][1].occupiedType == 'X' && grid[2][2].occupiedType == 'X')
		return 1;
	if (grid[0][2].occupiedType == 'X' && grid[1][1].occupiedType == 'X' && grid[2][0].occupiedType == 'X')
		return 1;

	return 0;
}

#pragma endregion


//here is the minimax for computer brain
int minimax(bool flag)// The minimax function
{

	int max_val = -1000, min_val = 1000;     //-100 is our assumed infinite max value, 100 is our assumed infinite min value
	int i, j, value = 1;
	if (cpu_won() == 1)
	{

		return 10;
	}
	else if (me_won() == 1)
	{


		return -10;
	}
	else if (isFull() == 1)
	{


		return 0;
	}

	int score[total_row][total_col] = { {1,1,1}, {1,1,1}, {1,1,1} };//if score[i]=1 then it is empty

	for (i = 0; i < total_row; i++)
		for (j=0; j<total_col; j++)
		{
			if (grid[i][j].occupiedType == '*')
			{
				if (min_val > max_val) // reverse of pruning condition
				{
					if (flag == true)
					{
						grid[i][j].occupiedType = 'X';
						value = minimax(false);
					}
					else
					{
						grid[i][j].occupiedType = 'O';
						value = minimax(true);
					}
					grid[i][j].occupiedType = '*';
					score[j][i] = value;
				}
			}
		}

		if (flag == true)
		{
			max_val = -1000;
			for (int i = 0; i < total_row; i++)
				for (int j = 0; j < total_col; j++)
				{
					if (score[j][i] > max_val && score[j][i] != 1)
					{
						max_val = score[j][i];
						cpu_move_to_row = i;
						cpu_move_to_col = j;
					}
				}
			return max_val;
		}
		if (flag == false)
		{
			
			min_val = 1000;

			for (i=0; i<total_row; i++)
				for (j = 0; j < total_col; j++)
				{
					if (score[j][i] < min_val && score[j][i] != 1)
					{
						min_val = score[j][i];
						cpu_move_to_row = i;
						cpu_move_to_col = j;
					}
				}
			return min_val;
		}
}



class Player{
public:
	bool hasMoved;
	Player() { hasMoved = false; }
	void Control() 
	{
		int x, y;
		getmouseclick(WM_LBUTTONDOWN, x, y);

		int crop_image_to_offset = 5;  //we need to reduce the image size becoz it hide our lines.

		for (int i = 0; i < total_row; i++) {
			for (int j = 0; j < total_col; j++)
			{
				//first, check for a valid grid
				if (grid[i][j].occupiedType == '*') {
					if (x >= grid[i][j].topLeft.x
						&& x <= grid[i][j].bottomRight.x
						&& y >= grid[i][j].topLeft.y
						&& y <= grid[i][j].bottomRight.y
						)
					{

						cout << endl << endl;
						cout << "\nYou press grid [" << i << "][" << j << "]\n";
						readimagefile("images/O.jpg", grid[i][j].topLeft.x + crop_image_to_offset
							, grid[i][j].topLeft.y + crop_image_to_offset
							, grid[i][j].bottomRight.x - crop_image_to_offset
							, grid[i][j].bottomRight.y - crop_image_to_offset);

						grid[i][j].occupiedType = 'O';

						PlaySound("sounds/Button.wav", NULL, SND_FILENAME);
						hasMoved = true;
						return;
					}


				}
				else
				{
					hasMoved = false;
				}

			}

		}
	}


	
};

class Computer {
public:
	Computer() { }
	void Control()
	{
		int crop_image_to_offset = 5;
			cout<<"\nMinimax: "<<minimax(true);
			if (grid[cpu_move_to_row][cpu_move_to_col].occupiedType == '*')
			{
				grid[cpu_move_to_row][cpu_move_to_col].occupiedType = 'X';
				readimagefile("images/X.jpg", grid[cpu_move_to_row][cpu_move_to_col].topLeft.x + crop_image_to_offset
					, grid[cpu_move_to_row][cpu_move_to_col].topLeft.y + crop_image_to_offset
					, grid[cpu_move_to_row][cpu_move_to_col].bottomRight.x - crop_image_to_offset
					, grid[cpu_move_to_row][cpu_move_to_col].bottomRight.y - crop_image_to_offset);

				cout << "\nCPU select grid[" << cpu_move_to_row << "][" << cpu_move_to_col << "]";


				PlaySound("sounds/CPUSelect.wav", NULL, SND_FILENAME);
			}
	}
};



class PageController {
private:
	bool isMeFirst;
	int currPageIndex;
public:
	PageController()
	{
		currPageIndex = 0;
		srand(time(NULL));
		cpu_move_to_row = rand() % total_row;
		cpu_move_to_col = rand() % total_col;
		isMeFirst = false;
	}

	void switchPage()
	{
		currPageIndex = (currPageIndex + 1) % no_of_page;
	}

	void displayCurrentPage()
	{
		cout << "Displaying page: " << currPageIndex<<endl;

		DisplayPage(currPageIndex);
	}

	void DisplayPage(int index)
	{
			
			switch (index)
			{
			case 0:
			{
				EntryPageScene(width, height);
				break;
			}
			case 1:
			{

				SelectFirstPageScene(width, height);
				break;
			}
			case 2:
			{

				PlayPageScene(width, height);
				break;
			}
			case 3:
				ResultPageScene(width, height);
				break;
		
			}
	}

	void EntryPageScene(int width, int height)
	{
		int marginX = 100;
		readimagefile(MainBackgroundImageName.c_str(), 0, 0, width, height / 2);

		readimagefile("images/playBtn.jpg", width / 2 - marginX, height / 12*7, width / 2 + marginX, height / 12 *8);
		readimagefile("images/exitBtn.jpg", width / 2 - marginX, height / 12*10 , width / 2 + marginX, height / 12*11);
		setmousequeuestatus(WM_LBUTTONDOWN);

	//while (!ismouseclick(WM_LBUTTONDOWN)) { /*wait for press*/}
		while (1)
		{
			int x, y;
			getmouseclick(WM_LBUTTONDOWN, x, y);

			if (x >= width / 2 - marginX
				&& x <= width / 2 + marginX
				&& y >= height / 12 * 7
				&& y <= height / 12 * 8)
			{
				PlaySound("sounds/Button.wav", NULL, SND_FILENAME);

				cleardevice();
				switchPage();
				break;
			}
			if (x >= width / 2 - marginX
				&& x <= width / 2 + marginX
				&& y >= height / 12 * 10
				&& y <= height / 12 * 11)
			{
				PlaySound("sounds/Button.wav", NULL, SND_FILENAME);

				cleardevice();
				closegraph();
				isExit = true;
				break;
			}

		}
	}

	void SelectFirstPageScene(int width, int height)
	{
		mciSendString("play sounds/bgm.wav", NULL, 0, 0);
		int buttonWidth = 200;


		readimagefile("images/WhoFirstImg.jpg", width / 2 - buttonWidth, (1 / 12.0) * height, width / 2 + buttonWidth, (3 / 12.0) * height);
		readimagefile("images/computerBtn.jpg", width / 2 - buttonWidth, (5 / 12.0) * height, width / 2 + buttonWidth, (7 / 12.0) * height);
		readimagefile("images/meBtn.jpg", width / 2 - buttonWidth, (8 / 12.0) * height, width / 2 + buttonWidth, (10 / 12.0) * height);


		while (1)
		{
			int x, y;
			getmouseclick(WM_LBUTTONDOWN, x, y);

			if (x >= width / 2 - buttonWidth
				&& x <= width / 2 + buttonWidth
				&& y >= (5 / 12.0) * height
				&& y <= (7 / 12.0) * height)
			{
				PlaySound("sounds/Button.wav", NULL, SND_FILENAME);

				cleardevice();
				switchPage();
				isMeFirst = false;
				break;
			}
			
			if (x >= width / 2 - buttonWidth
				&& x <= width / 2 + buttonWidth
				&& y >= (8 / 12.0) * height
				&& y <= (10 / 12.0) * height)
			{
				PlaySound("sounds/Button.wav", NULL, SND_FILENAME);

				cleardevice();
				switchPage();
				isMeFirst = true;
				break;
			}

		}


	}

	void PlayPageScene(int width, int height)
	{
		drawGrid(width, height, 100);
		int font_size = 1;
		settextstyle(10, HORIZ_DIR, font_size);
		char cpu_say[] = "CPU: Don't click so fast, let me think...";
		outtextxy(20, height-60, cpu_say);
		delay(1000);
		char me_reply[] = "Me: okay...";
		outtextxy(20, height - 40, me_reply);




		Player me;
		Computer enemy;

		int isMyTurn = false;
	
		cout << "\n\nMe first? " << ((isMeFirst) ? "\nYa, me first\n\n" : "\nNo, cpu first\n\n")<<endl;
		
		cout << "----------Game Start---------\n\n";

		if (isMeFirst)
		{
			while (1)
			{
				me.Control();
				if (me.hasMoved)
				{
					isMyTurn = false;
					break;
				}

			}

		}



		//looping the game until sb win
		while (1)
		{
			//computer turn
			if (!isMyTurn)
			{
				enemy.Control();
				isMyTurn = true;
			}

			if (cpu_won() || isFull())
			{
				cout << "\nLoading Result...\n\n";
				
				int font_size = 3;

				settextstyle(10, HORIZ_DIR, 3);
				setcolor(WHITE);


				outtextxy((width - 100) / 2 - 17 * 3, 0, "\nLoading Result...");

				delay(1000);
				cleardevice();
				switchPage();
				break;
			}



			if (isMyTurn)//here is our gameplay
			{
				me.Control();
				if (me.hasMoved)
				{
					me.hasMoved = false;
					isMyTurn = false;

				}


			}

			if (me_won()||cpu_won()||isFull())
			{
				settextstyle(10, HORIZ_DIR, 3);
				setcolor(WHITE);


				outtextxy((width-100)/2-17*3, 0, "\nLoading Result...");
				delay(1000);
				cleardevice();
				switchPage();
				break;
			}

			

		}
	}
	void ResultPageScene(int width, int height)
	{
		int buttonWidth = 120;
		mciSendString("close sounds/bgm.wav", NULL, 0, 0);

		readimagefile("images/restartBtn.jpg", width / 2 - buttonWidth, (5 / 12.0) * height, width / 2 + buttonWidth, (7 / 12.0) * height);
		readimagefile("images/exitBtn.jpg", width / 2 - buttonWidth, (8 / 12.0) * height, width / 2 + buttonWidth, (10 / 12.0) * height);

		if (me_won() == 1)
		{
			cout << "\n\n---------Result---------\nMe Win!\n\n";
			readimagefile("images/youWinImg.jpg", 50, (1 / 12.0) * height, width - 50, (3 / 12.0) * height);
			PlaySound("sounds/Win.wav", NULL, SND_FILENAME);
		}
		else if (cpu_won() == 1)
		{

			cout << "\n\n---------Result---------\nCPU Win!\n\n";
			
			readimagefile("images/youLoseImg.jpg", 50, (1 / 12.0) * height, width - 50, (3 / 12.0) * height);

			PlaySound("sounds/Lose.wav", NULL, SND_FILENAME);
		}
		else if (isFull() == 1)
		{

			cout << "\n\n---------Result---------\nDraw!\n\n";

			readimagefile("images/drawImg.jpg", 50, (1 / 12.0) * height, width - 50, (3 / 12.0) * height);

			PlaySound("sounds/Draw.wav", NULL, SND_FILENAME);
		}




		while (1)
		{
			int x, y;
			getmouseclick(WM_LBUTTONDOWN, x, y);

			if (x >= width / 2 - buttonWidth
				&& x <= width / 2 + buttonWidth
				&& y >= (5 / 12.0) * height
				&& y <= (7 / 12.0) * height)
			{
				PlaySound("sounds/Button.wav", NULL, SND_FILENAME);


				cleardevice();
				currPageIndex = 1;
				system("cls");
				break;
			}

			if (x >= width / 2 - buttonWidth
				&& x <= width / 2 + buttonWidth
				&& y >= (8 / 12.0) * height
				&& y <= (10 / 12.0) * height)
			{
				PlaySound("sounds/Button.wav", NULL, SND_FILENAME);

				cleardevice();
				closegraph();
				isExit = true;
				break;
			}



		}

	}



};





int main()
{

	int winPosX = (getmaxwidth() - width) / 2;
	int winPosY = (getmaxheight() - height) / 2;


	//centre our window at the middle
	initwindow(width, height, WindowName.c_str(), winPosX, winPosY);

	PageController c;

	while (!isExit)
	{
		c.displayCurrentPage();
	}
	return 0;
}
	
