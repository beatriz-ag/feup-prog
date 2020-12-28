// ScrabbleJunior.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include "Board.h"
using namespace std;

int main()
{
    cout << "SCRABBLE JUNIOR\n\n";

    bool error = true;
    string filename;
    int nrplayers;
    ifstream boardfile;
    
    while (error)
    {
        error = false;
        cout << "Enter the Board's textfile name: ";
        cin >> filename;
        if (filename.size() > 5)
        {
            if (filename.substr(filename.size() - 4, filename.size()) != ".txt")
                filename += ".txt";
        }
        else
            filename += ".txt";
        boardfile.open(filename);
        if (boardfile.fail())
        {
            cerr << "\nERROR: Board file not found. \n\n";
            error = true;
        }
        if (!error)
        {
            cout << endl << "Enter the number of players [2, 4]: ";
            cin >> nrplayers;
            if (nrplayers > 4 || nrplayers < 2)
            {
                cerr << "ERROR: number of players out of range. Only 2-4 players allowed. \n\n";
                error = true;
                boardfile.close();
            }
        }
    }

    Board b(boardfile, nrplayers);
    boardfile.close();
    b.play();
}

