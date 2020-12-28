#include "Board.h"
#include "player.h"
#include "pool.h"
#include <fstream>
#include <string>
#define STD_OUTPUT_HANDLE ((DWORD)-11)
#define blue 9
#define red 12
#define lightBlue 3
#define white 15
#define darkGray 8
#define lightGray 7
#define yellow 14
#include <vector>
#include <iomanip>
#include <iostream>
#include <windows.h>
#include <map>
#include <conio.h>
#include <assert.h> 

int Board::xplayer = 0;

Board::Board(ifstream& boardfile, int nrplay)
{
	srand(time(NULL));
	nrplayers = nrplay;
	string  position, word, line;
	int posx, posy;
	char orientation, unuseful;

	boardfile >> sizeLine >> unuseful >> sizeCol;

	// creates a board of tiles 
	for (unsigned int i = 0; i < sizeCol; i++)
	{
		vector<tile> line;
		for (unsigned int u = 0; u < sizeLine; u++)
		{
			tile t1;
			line.push_back(t1);
		}
		BoardVector.push_back(line);
	}

	boardfile.get(unuseful); // verifies emptiness

	while (getline(boardfile, line))
	{
		position = line.substr(0, 2);
		orientation = line[3];
		word = line.substr(5, line.size());
		posx = int(position[0]) - 'A';
		posy = int(position[1]) - 'a';

		switch (orientation)
		{
		case 'H':
			for (unsigned int i = 0; i < word.size(); i++)
			{
				BoardVector[posx][posy + i].letter = word[i];                // fills each board tile with the current character of the word
				BoardVector[posx][posy + i].empty = false;                   // automatically sets that position of the board to occupied
				BoardVector[posx][posy + i].horizvert.first = true;          // the tile belongs to a word whose orientation is horizontal
				BoardVector[posx][posy + i].wordsize.first = word.size();    // length of the horizontal word associated with that tile

				if (i == 0)
				{
					BoardVector[posx][posy].openPlay = true;                 // when the game starts, all first letters are open to be played
					BoardVector[posx][posy].firstLetterHV.first = true;      // if it is the first letter of the word
				}

				if (i == word.size() - 1)
				{
					BoardVector[posx][posy + i].endLetterHV.first = true;     // if it is the last letter of the word
				}

				Pool.addTile(word[i]);          // the correspondent character is added to the pool
			}
			break;
		case 'V':

			for (unsigned int i = 0; i < word.size(); i++)
			{
				BoardVector[posx + i][posy].letter = word[i];                // fills each board tile with the current character of the word
				BoardVector[posx + i][posy].empty = false;                   // automatically sets that position of the board to occupied
				BoardVector[posx + i][posy].horizvert.second = true;         // the tile belongs to a word whose orientation is vertical
				BoardVector[posx + i][posy].wordsize.second = word.size();   // length of the vertical word associated with that tile

				if (i == 0)
				{
					BoardVector[posx][posy].openPlay = true;                 // when the game starts, all first letters are open to be played
					BoardVector[posx][posy].firstLetterHV.second = true;     // if it is the first letter of the word

				}

				if (i == word.size() - 1)
				{
					BoardVector[posx + i][posy].endLetterHV.second = true;   // if it is the last letter of the word  
				}
				Pool.addTile(word[i]);          // the correspondent character is added to the pool
			}
			break;
		}
	}
	removeDuplicates();

	// make sure it is a valid Board (it isn't if there aren't at least 7 tiles per player.
	int totalTiles = Pool.getSize();
	int tilesPerPlayer = 7 * nrplayers;
	assert(tilesPerPlayer <= totalTiles);

	for (int i = 0; i < nrplayers; i++)
	{
		player addplayer;

		// assigns each player 7 randomly drawn tiles
		for (int i = 0; i < 7; i++)
			addplayer.addTile(Pool.getTile());

		players.push_back(addplayer);
	}
}

void Board::clrscr()
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

void Board::setcolor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

void Board::display()
{
	clrscr();
	cout << "SCRABBLE JUNIOR " << endl;
	cout << endl;
	for (int i = 0; i < nrplayers; i++)
	{
		cout << "Player " << i + 1 << " = " << players[i].playerPoints() << " points" << endl;
	}
	cout << endl;
	cout << ' ';
	setcolor(blue);
	for (unsigned int i = 'a'; i < 'a' + sizeCol; i++)
		cout << setw(4) << char(i);
	cout << endl;
	for (unsigned int u = 0; u < sizeCol; u++)
	{
		setcolor(blue);
		cout << char(u + 'A');

		for (unsigned int i = 0; i < sizeLine; i++)
		{
			cout << setw(4);
			setcolor(BoardVector[u][i].color);
			cout << BoardVector[u][i].letter;
			setcolor(white);
		}
		
		cout << endl;

	}
}

/*
if the inputed position is not empty and hasn't been played yet
and, of course, if the letter in the board matches the letter inputed by the player
the move is valid and the function returns true
otherwise, the player is not allowed to make that move and the function returns false 
*/
bool Board::checkPlay(char letter, int posx, int posy)
{
	if (!BoardVector[posx][posy].empty && !BoardVector[posx][posy].played && BoardVector[posx][posy].openPlay)
	{
		if (BoardVector[posx][posy].letter == letter)
			return true;
	}
	return false;
}

/*
checks if the player has conditions to make any move at all
if so, the function returns true and the player is obliged to play
otherwise, the function returns false
*/
bool Board::checkPossibilities(int indexplayer)
{
	int x, y = 0;
	bool possible = false;
	for (unsigned int i = 0; i < sizeCol; i++)
	{
		for (unsigned int u = 0; u < sizeLine; u++)
		{
			if (BoardVector[i][u].openPlay && !BoardVector[i][u].played)
			{
				char letter = BoardVector[i][u].letter;
				possible = players[indexplayer].checkTile(letter);
				if (possible)
				{
					y = u;
					break;
				}
			}
		}
		if (possible)
		{
			x = i;
			players[indexplayer].recordPossiblePlay(char(x + 'A'), char(y + 'a'));
			break;
		}
	}
	return possible;
}

void Board::checkPoints(int posx, int posy, int indexplayer)
{
	int size;
	int posxfinal = 0, posxinit = 0, posyfinal = 0, posyinit = 0;
	bool complete = true, foundinit = false, foundfinal = false;

	// the tile belongs to a word whose orientation is horizontal 
	// (note that a tile can have both orientations set to true)
	if (BoardVector[posx][posy].horizvert.first)
	{
		size = BoardVector[posx][posy].wordsize.first;
		if (!BoardVector[posx][posy].firstLetterHV.first) // not first letter
		{
			// find first letter
			int i = 0;
			while (!BoardVector[posx][posy - i].firstLetterHV.first)
			{
				i++;
			}
			posyinit = posy - i;
			posxinit = posx;
		}
		else
		{
			// define first letter
			posyinit = posy;
			posxinit = posx;
		}

		// check if all the tiles of that word have been played
		for (int i = 0; i < size; i++)
		{
			if (!BoardVector[posxinit][posyinit + i].played)
			{
				complete = false;
				break;
			}
		}

		// if so, the points associated to that tile are added to the player's points
		if (complete)
		{
			players[indexplayer].addPoints(1);
		}
	}

	// the tile belongs to a word whose orientation is vertical 
	// (note that a tile can have both orientations set to true)
	if (BoardVector[posx][posy].horizvert.second)
	{
		complete = true; foundinit = false; foundfinal = false;
		size = BoardVector[posx][posy].wordsize.second;

		if (!BoardVector[posx][posy].firstLetterHV.second)  // not first letter
		{
			// find first letter
			int i = 0;
			while (!BoardVector[posx - i][posy].firstLetterHV.second)
			{
				i++;
			}
			posxinit = posx - i;
			posyinit = posy;
		}
		else
		{
			// define first letter
			posxinit = posx;
			posyinit = posy;
		}

		// check if all the tiles of that word have been played
		for (int i = 0; i < size; i++)
		{
			if (!BoardVector[posxinit + i][posyinit].played)
			{
				complete = false;
				break;
			}
		}

		// if so, the points associated to that tile are added to the player's points
		if (complete)
		{
			players[indexplayer].addPoints(1);
		}
	}
}

void Board::replaceTiles(int indexplayer, int xplay)
{
	if (xplay % 2 == 0 && !Pool.isEmpty()) // if it is the second play and there are still tiles available
	{
		if (players[indexplayer].handSize() == 5)
		{
			char tile1 = Pool.getTile();
			if (!Pool.isEmpty())  //in case there is only one tile available
			{
				char tile2 = Pool.getTile();
				players[indexplayer].addTile(tile2);
			}
			players[indexplayer].addTile(tile1);
		}
		else if (players[indexplayer].handSize() == 6) // this happens when tile is picked up from the pool in the second play
		{
			char tile1 = Pool.getTile();
			players[indexplayer].addTile(tile1);

		}
	} // replace played tiles
}

void Board::removeDuplicates()
{
	for (unsigned int i = 0; i < sizeCol; i++)
	{
		for (unsigned int u = 0; u < sizeLine; u++)
		{
			if (BoardVector[i][u].horizvert.first && BoardVector[i][u].horizvert.second)
			{
				char letter = BoardVector[i][u].letter;
				Pool.removeDuplicate(letter);
			}
		}
	}
}

/*
if a player can't play any of his tiles
the player will use his turn to exchange any two of his tiles with two tiles from the pool
*/
void Board::swapTiles(int indexplayer)
{
	// removes random tiles from user 
	char tile1FromPlayer = players[indexplayer].removeTile();
	char tile2FromPlayer = players[indexplayer].removeTile();

	// gets random tiles from the poll
	char tile1 = Pool.getTile();
	char tile2 = Pool.getTile();

	// returns tiles to the pool
	Pool.addTile(tile1FromPlayer);
	Pool.addTile(tile2FromPlayer);

	// gives the user the random tiles
	players[indexplayer].addTile(tile1);
	players[indexplayer].addTile(tile2);
}

void Board::openPlaysWord(int posx, int posy)
{
	int newposy = posy;
	if (BoardVector[posx][posy].horizvert.first) // the tile belongs to a word whose orientation is horizontal 
	{

		if (!BoardVector[posx][posy].firstLetterHV.first) // not first letter 
		{
			// finds first letter
			int i = 0;
			while (!BoardVector[posx][posy - i].firstLetterHV.first)
			{
				i++;
			}
			newposy = posy - i;
		}
		int i = 0;
		while (!BoardVector[posx][newposy + i].endLetterHV.first) // from first tile to last tile
																  // opens next tile if all tiles before have been played
		{
			if ((unsigned int)(newposy + i + 1) >= sizeLine) // avoid limit errors
				break;
			if (!BoardVector[posx][newposy + i].played) // next tile won't be open if current tile isn't played
				break;
			if (!BoardVector[posx][newposy + i + 1].played) // played tiles shouldn't be open
			{
				BoardVector[posx][newposy + i + 1].openPlay = true;
				break;
			}
			i++;
		}
	}
	if (BoardVector[posx][posy].horizvert.second) // the tile belongs to a word whose orientation is vertical 
	{
		if (!BoardVector[posx][posy].firstLetterHV.second) // not first letter
		{
			// find first letter
			int i = 0;
			while (!BoardVector[posx - i][posy].firstLetterHV.second)
			{
				i++;
			}
			posx = posx - i;
		}
		int i = 0;
		while (!BoardVector[posx + i][posy].endLetterHV.second) // from first tile to last tile
																// opens next tile if all tiles before have been played
		{
			if ((unsigned int)(posx + i + 1) >= sizeLine) // avoid limit errors
				break;
			if (!BoardVector[posx + i][posy].played) // next tile won't be open if current tile isn't played
				break;
			else if (!BoardVector[posx + i + 1][posy].played) // played tiles shouldn't be open
			{
				BoardVector[posx + i + 1][posy].openPlay = true;
				break;
			}
			i++;
		}
	}
}

/*
checks if a player has, indeed, the tile that is trying to play
if so, the fucntion returns true
otherwise, the player is not allowed to play that tile and the fucntion returns false
*/
bool Board::cheating(char letter, int indexplayer)
{
	bool valid;
	valid = players[indexplayer].checkTile(letter);
	return (!valid);
}

/*
investigates if there is still open tiles to be played
if so, the function returns true and the game has conditions to continue
otherwise, the function returns false and the game ends
*/
bool Board::lookForOpenTiles()
{
	for (unsigned int line = 0; line < sizeCol; line++)
	{
		for (unsigned int col = 0; col < sizeLine; col++)
		{
			if (BoardVector[line][col].openPlay && !BoardVector[line][col].played && !BoardVector[line][col].empty)
				return true;
		}
	}
	return false;
}

void Board::play()
{
	bool avance = true, showHint = false;
	int xplay = 0, xplayer = 0, attemptsToSwap = 0; // each player has 2 plays, hence the xplay, if (xplay % 2 == 0) it is the first play else it is the second play
								                    // xplayer is increased 1 whenever players switch turns, (xplayer % nrplayers) defines the nr of the player [0-4]
	clrscr();
	rules();
	clrscr();

	while (lookForOpenTiles()) // as long as there are open tiles to be played, the game must continue
	{
		display();
		char letter;
		string position;
		int indexplayer = xplayer % nrplayers;
		avance = true;
		bool valid = false;

		cout << endl;
		cout << "Player " << indexplayer + 1 << endl;

		if (Pool.isEmpty())
		{
			setcolor(lightBlue); 
			cout << "POOL IS EMPTY!\n"; 
			setcolor(white);
		}

		players[indexplayer].showHand();
		players[indexplayer].showAvailableHints();

		if (showHint)  //Shows a possible play position
		{
			setcolor(yellow);
			players[indexplayer].showHints();
			setcolor(white);
		}
		showHint = false;

		if (players[indexplayer].handSize() == 0)  //the player has no letters to play
		{
			setcolor(yellow);
			cout << "You have no letters available.\n";
			cout << "Your player will be passed on to the next player.\n";
			setcolor(white);
			char timeUserRead = _getch();

			xplayer++;
			xplay = 0;
			attemptsToSwap = 0;
			avance = false;
		}  //if the player has no letters
		else
		{
			cout << "Enter a letter and its corresponding position (e.g. T bD): ";
			cin >> letter >> position;
			if (cin.fail())
			{
				avance = false;
				if (cin.eof())
				{
					cin.clear();

					bool possible = checkPossibilities(indexplayer);

					if (!possible && !Pool.isEmpty() && (xplay % 2 == 0)) // impossible to play in the first play but there are tiles available
					{
						if (attemptsToSwap >= 3)
						{
							setcolor(yellow);
							cout << "You only have 3 swaps per play.\nYour turn will be passed.\n";
							setcolor(white);
							char timeUserRead = _getch();

							attemptsToSwap = 0;
							xplayer++;
							xplay = 0;
						}
						else
						{
							swapTiles(indexplayer);

							setcolor(yellow);
							cout << "Two tiles will be swapped!\n";
							setcolor(white);
							char timeUserRead = _getch();

							attemptsToSwap++;
						}
					}
					else if (!possible && xplay % 2 == 1) // the player can only play once
					{
						xplayer++;
						xplay = 0;
						attemptsToSwap = 0;

						if (!Pool.isEmpty())
						{
							char tile = Pool.getTile();
							players[indexplayer].addTile(tile);
						} // replace played tile
					}
					else if (!possible && Pool.isEmpty())  //the player can't make a move in his first play and there aren't tiles to swap
					{
						setcolor(yellow);
						cout << "Once the pool is empty and you can't play, your turn will be passed.\n";
						setcolor(white);
						char timeUserRead = _getch();

						xplayer++;
						xplay = 0;
						attemptsToSwap = 0;
					}
					else if (possible)
					{
						char useHint;
						setcolor(yellow);
						cout << "You are obliged to play.\n";
						setcolor(white);

						if (players[indexplayer].hintsAvailable() > 0)
						{
							while (1)
							{
								cout << "Do you want to use a hint? Y/y or N/n \n";
								cin >> useHint;

								if (cin.fail())
								{
									cin.clear();
									cin.ignore(1000, '\n');
								}
								else if (useHint == 'y' || useHint == 'Y')
								{
									showHint = true;
									players[indexplayer].decreaseHints();
									break;
								}
								else if (useHint == 'n' || useHint == 'N')
									break;
								else
									setcolor(red); cerr << "A wrong command has been entered!\n"; setcolor(white);
							}
						}
						char timeUserRead = getchar();
					}
				}
				else
				{
					setcolor(red); cerr << "INPUT ERROR\n"; setcolor(white);
					cin.clear();
					char timeUserRead = _getch();
				}
			}
			else if (position.size() == 1)
			{
				setcolor(red);
				cerr << "INPUT ERROR\n";
				setcolor(white);
			}
			else if (position[0] < 'A' || (unsigned int)(position[0]) > sizeLine + 'A' || position[1] < 'a' || (unsigned int)(position[1]) > sizeLine + 'a' || letter < 'A' || letter > 'Z')
			{
				avance = false;
				setcolor(red); cerr << "INPUT ERROR\nCareful with the capsLock and board limits!\n"; setcolor(white);
				char timeUserRead = _getch();
			}
		}

		if (avance)
		{
			int posx, posy;
			posx = int(position[0]) - 'A';
			posy = int(position[1]) - 'a';
			bool isCheating = cheating(letter, indexplayer);

			if (!isCheating)
				valid = checkPlay(letter, posx, posy);

			if (valid)
			{
				xplay++;
				players[indexplayer].removeTile(letter);
				replaceTiles(indexplayer, xplay);
				BoardVector[posx][posy].openPlay = false;
				BoardVector[posx][posy].color = darkGray;
				BoardVector[posx][posy].played = true;
				checkPoints(posx, posy, indexplayer);
				openPlaysWord(posx, posy);
				if (xplay % 2 == 0)
				{
					xplayer++; //next player
					xplay = 0; //set first play
					attemptsToSwap = 0;
				}
			}
			else
			{
				setcolor(red);
				cerr << "Invalid play. Try again!\n";
				setcolor(white);
				char timeUserRead = _getch();
			}
		}
	}

	// when there is no longer any open tiles
	clrscr();
	end();
}

void Board::rules()
{
	cin.ignore(1000, '\n');
	setcolor(blue);
	cout << "SCRABBLE JUNIOR\n";
	setcolor(lightGray);
	cout << "\nThe Game Rules:\n\n";
	cout << " 1. The player must play, if possible, 2 tiles in each play.\n";
	cout << " 2. Throughout the game each player has 3 hints that should be used when the player has a tile to play\n";
	cout << "    but can't find the position.\n";
	cout << " 3. If the player can't play any tiles, CTRL-Z must be used in order to swap two tiles from the Pool.\n";
	cout << "    If the pool is empty, your play will be passed.\n";
	cout << " 4. In the case only the first play can be made, CTRL-Z must be also used to pass.\n";
	cout << " 5. Letters can only be covered in the correct spelling order.\n";
	cout << " 6. A point will be earned for every word the player completes.\n";
	cout << " 7. When there are no more tiles left in the pool, keep playing with the tiles\n";
	cout << "    you have until all the tiles have been placed on the board. This ends the game.\n";
	cout << " 8. The player with the most points wins.\n";
	cout << "\n\n\tENJOY THE GAME!\n\n Press ENTER to start... ";
	cin.ignore(1000, '\n');
}

/*
declares the winner(s) and the game ends
*/
void Board::end()
{
	setcolor(blue);
	cout << "SCRABBLE JUNIOR\n" << endl;
	setcolor(white);

	int winnerPoints = 0;

	for (unsigned int n = 0; n < players.size(); n++)
	{
		if (players[n].playerPoints() > winnerPoints)
			winnerPoints = players[n].playerPoints();
	}

	cout << "The winners are:\n";
	for (unsigned int n = 0; n < players.size(); n++)
	{
		if (players[n].playerPoints() == winnerPoints)
			cout << "Player " << n + 1 << endl;
	}

	cout << endl;
	cout << "The game has just ended. Congratulations to the winner(s)!\nHope you had a good time playing Scrabble!\n\n";
}








