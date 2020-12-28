#ifndef BOARDBUILDER_H
#define BOARDBUILDER_H

#define STD_OUTPUT_HANDLE ((DWORD)-11)
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
typedef unsigned int uint;
typedef vector<vector<char>> _board;
typedef vector<string> _dict;

struct Word { 
	string name;
	char orientation;
	pair<char, char> coordinates;
};

struct Measure {
	uint w; // width
	uint h; // height
};

class BoardBuilder
{
private:
	bool exit;
	Measure size;
	Word currentWord;
	_board board;
	_dict dictionary;
	vector<Word> words;
	string dictFilename;
	string boardFilename;

public:
	BoardBuilder();  
	void Intro();
	void clrscr();

	bool validSize();                                   // checks if the size is valid 
	bool validWord(string word);                        // checks if the word is valid 
	bool validOrientation(char orientation);            // checks if the orientation is either vertical or horizontal
	bool validCoordinates(char x, char y);              // checks if the coordinates Y and x are valid
	bool validPlacement();                              // passing all previous conditions, checks if the word can be positioned in the board

	void setOuputFile();                                // sets the path to the file where the board data will be saved
	void setSize();                                     // requests user to select the board size LxH          
	void constructBoard();                              // allocates space for the board given its size  
	void showBoard();                                   // shows board on the screen         
	void saveBoard();                                   // saves board information in the file            
	
	void readDictionary();                              // reads dictionary from a file
	int searchDictionary(string word);                  // searches in the dictionary for a given word using binary search algorithm
	
	void getWord();                                     // requests user to input the word to be placed on the board
	void getOrientation();                              // requests user to input the orientation of the given word
	void getCoordinates();                              // requests user to input the coordinates of the given word
	void positionWord();                                // positions the given word on the board     
	void processWord();                                 // handles word related functions
};

#endif 
