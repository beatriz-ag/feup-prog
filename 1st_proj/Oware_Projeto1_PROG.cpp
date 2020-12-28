// Projeto1_OwareGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#define STD_OUTPUT_HANDLE ((DWORD)-11)
#include <windows.h>
#include <iomanip>
#include <limits>
#include<ios> 
#include <algorithm>
#include <conio.h>

using namespace std;
int a = 4, b = 4, c = 4, d = 4, e = 4, f = 4, g = 4, h = 4, i = 4, k = 4, l = 4, j = 4, x = 0, y = 0, A = 4, B = 4, C = 4, D = 4, E = 4, F = 4, G = 4, H = 4, I = 4, J = 4, K = 4, L = 4, X = 0, Y = 0, xa, xb, xc, xd, xe, xf;
int truepoints[] = { a,b,c,d,e,f,g,h,i,j,k,l }; // an array to achieve the points iteratively
int check_points[] = { A,B,C,D,E,F,G,H,I,J,K,L }; // this array is only used in the function check_errors to verify the legality of the player's choice to play // hence that, after every play, this array is updated check_points = points;
int earned_points[] = { xa,xb,xc,xd,xe,xf };
char table[] = { 'a','b','c','d','e','f','g','h','i','j','k','l' };
int mode;
char dif;
bool keepGoing = true, realplay = true, hardcore = true;
static int player = 0;
string north[] = { "|   North player: ","|   COMPUTER:    " };
int choose_north = 0;

void clrscr(void)
{
	COORD coordScreen = { 0, 0 }; // upper left corner
	DWORD cCharsWritten;
	DWORD dwConSize;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hCon, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	// fill with spaces
	FillConsoleOutputCharacter(hCon, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hCon, &csbi);
	FillConsoleOutputAttribute(hCon, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	// cursor to upper left corner
	SetConsoleCursorPosition(hCon, coordScreen);
}

void setcolor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

void display()
{
	cout << "		        OWARE GAME" << endl;
	cout << endl;
	cout << "      f          e          d          c          b          a" << endl;
	cout << "  _________________________________________________________________" << endl;
	cout << "||          |          |          |          |          |          ||" << endl;
	cout << "||";
	for (int i = 5; i >= 0; i--)
	{
		cout << "    ";
		setcolor(9);
		cout << setfill('0') << setw(2) << truepoints[i];
		setcolor(7);
		cout << "    ";
		cout << "|";
	}
	cout << north[choose_north] << x << " points." << endl;
	cout << "||__________|__________|__________|__________|__________|__________||" << endl;
	cout << "||          |          |          |          |          |          ||" << endl;
	cout << "||";
	for (int i = 6; i < 12; i++)
	{
		cout << "    ";
		setcolor(9);
		cout << setfill('0') << setw(2) << truepoints[i];
		setcolor(7);
		cout << "    ";
		cout << "|";
	}
	cout << "|   South player: " << y << " points." << endl;
	cout << "||__________|__________|__________|__________|__________|__________||" << endl;
	cout << endl;
	cout << "      g          h          i          j          k          l" << endl;

}

bool fplayer(int n)
{
	return (n % 2 == 0);
}

void update_check_points()
{
	X = x;
	Y = y;
	for (int i = 0; i < 12; i++)
		check_points[i] = truepoints[i];
}

void remove_seeds(bool player) //if the player manages to empty is side of the board all the seeds will be returned to the respective player
{
	if (player)
	{
		for (int i = 0; i < 6; i++)
		{
			x = x + truepoints[i];
			truepoints[i] = 0;
		}
	}
	else
	{
		for (int i = 6; i < 12; i++)
		{
			y = y + truepoints[i];
			truepoints[i] = 0;
		}
	}
}

void check_mate(bool player) // verify if all the player's holes stayed empty, the check mate that ends the game
{
	bool checkmate = true;
	if (player)
	{
		for (int i = 6; i < 12; i++)
		{
			if (truepoints[i] != 0)
			{
				checkmate = false;
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			if (truepoints[i] != 0)
			{
				checkmate = false;
				break;
			}
		}
	}
	if (checkmate)
		remove_seeds(player);
}

void checkPoints(bool player)
{
	check_mate(player);

	if (x == 24 && y == 24)
	{
		clrscr();
		display();
		cout << "It's a draw!";
		if (mode == 1)
		{
			cout << " Congratulations for both of the players." << endl;
		}
		keepGoing = false;
	}
	else if (x >= 24)
	{
		clrscr();
		display();
		if (mode == 1)
		{
			cout << "NORTH PLAYER IS THE WINNER!" << endl << " Congratulations North player." << endl;
			cout << "Better luck next South player..." << endl;
		}
		else
		{
			cout << "YOU LOST!" << endl;
		}
		keepGoing = false;
	}

	else if (y >= 24)
	{
		clrscr();
		display();
		keepGoing = false;
		if (mode == 1)
		{
			cout << "SOUTH PLAYER IS THE WINNER!" << endl << " Congratulations South player." << endl;
			cout << "Better luck next North player..." << endl;
		}
		else
		{
			cout << "YOU WIN!" << endl;
		}

	}
	else
		clrscr();
	update_check_points();
}

void check_oponent(bool player, int seeds) //only called in debug, it's used to check if the oponent will be left with no seeds (if so it is an invalid play)
{
	int zeros = 0;
	if (player) // south player's turn, i have to check the north player's board
	{
		Y = y;
		y = y - seeds; // check_oponent is only called in the check_points version (aka debug version) so i'm subtracting the points i added in the function before
		for (i = 0; i < 6; i++)
		{
			if (check_points[i] == 0)
				zeros++;
		}

		if (zeros == 6)
			realplay = false;
	}
	else // north player's turn, i have to check the South player's board
	{
		X = x;
		x = x - seeds;
		for (i = 6; i < 12; i++)
		{
			if (check_points[i] == 0)
				zeros++;
		}

		if (zeros == 6)
			realplay = false;
	}
}

void getsPoints(int* points, int n, bool player)
{
	if (n == -1) //error in the function before
		check_oponent(player, 0);

	int table[] = { a,b,c,d,e,f,g,h,i,j,k,l };
	int seeds, summed = 0;
	n = n % 12;
	if (player && n < 6) // if the south player ended its play in an oponent's hole, I'll check if he wins seeds
	{

		while (points[n] == 2 || points[n] == 3)
		{
			seeds = points[n];
			summed = summed + seeds;
			y = y + seeds;
			points[n] = 0;
			n--;
			if (n < 0)
				break;
		}
	}
	else if ((!player) && (n > 5)) //if the north player ended its play in an oponent's hole
	{
		while (points[n] == 2 || points[n] == 3)
		{
			seeds = points[n];
			summed = summed + seeds;
			x = x + seeds;
			points[n] = 0;
			n--;
			if (n < 6)
				break;
		}
	}
	if (points == check_points) //debug
		check_oponent(player, summed);
	else  //real play
		checkPoints(player);
}

void play(int* points, char hole, int n)
{
	int seeds, lastHole;
	bool error = false;


	if (fplayer(n)) //south player
	{
		char holes[] = { 'g', 'h', 'i', 'j', 'k', 'l' };
		for (int i = 0; i < 6; i++)
		{
			if (holes[i] == hole)
			{
				i = i + 6;
				if (points[i] == 0) //invalid play
				{
					clrscr();
					if (mode == 1) //Bot version doesn't output the bot's errors
					{
						setcolor(12); cout << "Impossible play. That hole is empty!" << endl; setcolor(7);
					}
					error = true;
				}
				else
				{ //valid play
					seeds = points[i];
					points[i] = 0; //empties hole

					//the next houses will be filled with the seeds
					for (int u = 1; u <= seeds; u++)
					{
						if ((u + i) % 12 != i)
							points[(u + i) % 12] = points[(u + i) % 12] + 1;
						if (u == seeds)
							lastHole = i + u;
					}
				}
				break;
			}
		}
	}
	else
	{ //North player
		char holes[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
		for (int i = 0; i < 6; i++)
		{
			if (holes[i] == hole)
			{
				if (points[i] == 0)
				{
					clrscr();
					if (mode == 1)
					{
						setcolor(12); cout << "Impossible play. That hole is empty!" << endl; setcolor(7);
					}
					error = true;
				}
				else
				{
					seeds = points[i];
					points[i] = 0;

					for (int u = 1; u <= seeds; u++)
					{
						if ((u + i) % 12 != i)
							points[(u + i) % 12] = points[(u + i) % 12] + 1;
						if (u == seeds)
							lastHole = i + u;
					}
				}
				break;
			}
		}
	}
	if (error)
	{
		realplay = false;
		getsPoints(check_points, -1, fplayer(n)); // I use -1 (an impossible value for lastHole) for the next function to stop at the beginning, its an error, there is no point in running
	}
	else if (points == check_points) // debug 
		getsPoints(check_points, lastHole, fplayer(n));
	else  //real play 
		getsPoints(truepoints, lastHole, fplayer(n));
}

int find_index(char hole)
{
	char possibilities[] = { 'a','b','c','d','e','f','g','h','i','j','k','l' };
	for (int i = 0; i < 12; i++)
	{
		if (hole == possibilities[i])
		{
			return i;
		}
	}

}

bool check_error(char hole, int n) //Yhis is the function that checks if a play is valid (debug function)
{
	int i;
	realplay = true;
	play(check_points, hole, n); //I use the array check_points as a parameter, the true_points array is only used in the real play
	i = find_index(hole); //only used in the hard automatic version
	earned_points[i] = (X - x); //"""
	update_check_points();//after checking the play, i put the variables as they were, as the name suggests they are only checking variables/functions
	return realplay; //true if valid

}

void init()
{
	setcolor(9); cout << "             OWARE GAME             "; setcolor(7); cout << endl;
	cout << endl;
	cout << "Oware is a strategy game for two players.It belongs to the wider family of mancala games, consisting" << endl;
	cout << "in the distribution of game pieces around the game board and its removal from the board when certain conditions are met. In this version you can choose if " << endl;
	cout << "you want to play 1 vs 1 or against me (auto mode).";
	cout << "The following are the game rules : " << endl;
	setcolor(9); cout << "1. "; setcolor(7); cout << "The first player owns the south row. (Auto mode, the user is always the first.)" << endl;
	setcolor(9); cout << "2. "; setcolor(7); cout << "The player who is to move chooses one of the holes (a,b,c,d,..) on his own side of the board,leaving the hole empty." << endl;
	cout << "   Subsequently, these seeds will be distributed counterclockwise on the board during the sowing phase. " << endl;
	setcolor(9); cout << "3. "; setcolor(7); cout << "A player may collect the seeds from any of the holes that belong to him if it contains one or more seeds, only with " << endl;
	cout << "   the exception that after making the move his opponent must be able to play. Seeds will be collected automatically." << endl;
	setcolor(9); cout << "4. "; setcolor(7); cout << "A player will never sow on the holes used for storage." << endl;
	setcolor(9); cout << "5. "; setcolor(7); cout << "After each play, if the last hole where seed was put contains exactly two or three seeds, the player will capture them and earn those points." << endl;
	setcolor(9); cout << "6. "; setcolor(7); cout << "The game will end when a player reaches 24 points. There can be draws." << endl;
	setcolor(9); cout << "7. "; setcolor(7); cout << "In order to stop the game, enter ctr-Z in your turn." << endl;
	cout << endl;
	setcolor(11); cout << endl << " Press Enter to continue.";
	setcolor(7);
	cin.ignore(10, '\n');
	clrscr();
	while (1)
	{
		cout << endl << endl << "\t \t \t ___________________________________________" << endl;
		cout << "\t \t \t|					    |" << endl;
		cout << "\t \t \t|   1. 2 player		2. Bot mode         |" << endl;
		cout << "\t \t \t|___________________________________________|" << endl;
		setcolor(9);  cout << "\t \t \t Which mode do you want to play? "; setcolor(7);
		cin >> mode;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			clrscr();
			if (cin.eof())
			{
				exit(0);
			}
		}
		else
		{
			if (mode == 1 || mode == 2)
			{
				break;
			}
		}
	}
	cin.ignore(1000, '\n');
	if (mode == 2)
	{
		choose_north = 1;
		while (1)
		{
			setcolor(2); cout << "\t \t \t            Easy"; setcolor(7); cout << " or "; setcolor(12); cout << "hard ? "; setcolor(7); cout << "(E/H) ";
			cin >> dif;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(1000, '\n');

				if(cin.eof())
				{
					exit(0);
				}
			}
			else if (dif == 'H' || dif == 'E')
			{

				dif = toupper(dif);
				if (dif == 'E')
				{
					hardcore = false;
					break;
				}
				if (dif == 'H')
				{
					hardcore = true;
					break;
				}
			}
		}
	}
	cout << endl << endl;
	setcolor(11); cout << endl << " Press Enter to play. May the best win.";
	setcolor(7);
	cin.ignore(10, '\n');
	clrscr();
}

int find_max()
{ // function used to find the index of the maximum value in an array
	int max = 0, index = 0;
	for (int i = 0; i < 6; i++)
	{
		if (earned_points[i] > max)
		{
			max = earned_points[i];
			index = i;
		}
	}
	if (index == 0 && xa == 0) //if the array only has zeros, which happens early in the game, the computer would always choose the hole 'a'. To avoid that, the computer chooses randomly.
	{
		srand(time(0));
		return rand() % (6);
	}
	return index;
}

int main()
{

	char answerSouth, answerNorth;

	init(); // function that displays the rules and asks which mode of play (bot or manual) the user wants

	while (keepGoing)
	{
		bool noerror = true;
		display();
		cout << endl;
		if (player % 2 == 0)     // I use the variable player to decide whose turn it is. Even numbers belong to the South and odd to the North. Variable is incremented in each play.
		{
			setcolor(8);
			cout << "[South Player]"; setcolor(7); cout << " Which hole do you want to empty (g,h,i,j,k,l) ? ";
			cin >> answerSouth;
			answerSouth = tolower(answerSouth);
			if (cin.fail())
			{
				if (cin.eof())
					return 0;
				else
				{
					cin.clear();
					cin.ignore(1000, '\n');
					clrscr();
				}
			}
			else if (answerSouth != 'g' && answerSouth != 'h' && answerSouth != 'i' && answerSouth != 'j' && answerSouth != 'k' && answerSouth != 'l')
			{
				cin.clear();
				cin.ignore(1000, '\n');
				clrscr();
				setcolor(12); cerr << "A wrong command has been entered. Try again." << endl; setcolor(7);
			}
			else
			{
				cin.ignore(1000, '\n');
				noerror = check_error(answerSouth, player);  //Check if it is a valid play (debug mode)
				if (noerror)
				{
					play(truepoints, answerSouth, player); //The actual play
					player++;
				}
				else //not a valid play
				{
					clrscr();
					setcolor(12); cerr << "The play you tried to make is against the rules." << endl; setcolor(7);
				}
			}

		}
		else if (player % 2 == 1 && mode == 1)   // North's player turn is divided in the manual and bot version
		{ // manual
			setcolor(8);
			cout << "[North Player]"; setcolor(7); cout << " Which hole do you want to empty (f,e,d,c,b,a) ? ";
			cin >> answerNorth;
			answerNorth = tolower(answerNorth);
			if (cin.fail())
			{
				if (cin.eof())
					return 0;
				else
				{
					cin.clear();
					cin.ignore(1000, '\n');
					clrscr();

				}
			}
			else if (answerNorth != 'a' && answerNorth != 'b' && answerNorth != 'c' && answerNorth != 'd' && answerNorth != 'e' && answerNorth != 'f')
			{
				cin.clear();
				cin.ignore(1000, '\n');
				clrscr();
				setcolor(12); cerr << "A wrong command has been entered. Try again." << endl; setcolor(7);
			}
			else
			{
				cin.ignore(1000, '\n');
				noerror = check_error(answerNorth, player);
				if (noerror)
				{
					play(truepoints, answerNorth, player);
					player++;
				}
				else
				{
					clrscr();
					setcolor(12); cerr << "The play you tried to make is against the rules." << endl; setcolor(7);
				}
			}
		}
		else
		{ //bot
			char possibilities[] = { 'a','b','c','d','e','f' };
			if (hardcore) //True if the user chose hard mode, in this case the computer will the choose the hole based on points he earns after the play
			{
				bool nothing; //In order to avoid creating another function that checks the plays, and returns true if valid, i created a bool variable with no meaning to figure out
								//which play gives more points


				for (int i = 0; i < 6; i++)
				{
					nothing = check_error(possibilities[i], player); //each possiility will be considered
				}
				while (1)
				{
					int max = find_max(); //the max of the array earned points will be used to play, if valid
					noerror = check_error(possibilities[max], player);
					if (noerror)
					{
						play(truepoints, possibilities[max], player);
						player++;
						break;
					}
					earned_points[max] = 0; //if the hole that gives the most points is an invalid play, it will be chosen the second highest earned points hole 
				}
			}
			else
			{//This is the easy version, the computer's play will be chosen randomly, only checking if it's valid
				int randm;
				srand(time(NULL));
				while (1)
				{
					randm = rand() % (6);
					answerNorth = possibilities[randm];
					noerror = check_error(answerNorth, player);
					if (noerror)
					{
						play(truepoints, answerNorth, player);
						player++;
						break;
					}
				}
			}
		}
	}
	return 0;
}
