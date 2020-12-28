#include "BoardBuilder.h"
#define STD_OUTPUT_HANDLE ((DWORD)-11)
#define blue 1
#define red 4
#define white 15
#define yellow 14

// Set text color 
void setColor(int c)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, c);
}

void BoardBuilder::clrscr()
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

BoardBuilder::BoardBuilder()
{
    dictFilename = "WORDS.TXT";
    Intro();
    setOuputFile();
    setSize();
    constructBoard();
    readDictionary();
    showBoard();
    processWord();
    saveBoard();
}

void BoardBuilder::Intro()
{
    bool start = false;

    setColor(blue);
    cout << "WELCOME TO SCRABBLE!\n\n";
    setColor(white);
    cout << "This is you Board Builder program.\n"
        << "Give free rein to your imagination and build the ultimate Scrabble Board to play with your family and friends!\n\n"
        << "PRESS ENTER and join this lettery adventure... ";

    while (!start) { if (cin.get() == '\n') start = true; }    
}

/*
---------- VALIDATION FUNCTIONS ----------
*/
bool BoardBuilder::validSize()
{
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000000000, '\n');
        return false;
    }

    if ((size.w < 5 || size.w > 20) || (size.h < 5 || size.h > 20) || (size.w * size.h < 14))
        return false;

    return true;
}

bool BoardBuilder::validWord(string inputWord)
{
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000000000, '\n');
        return false;
    }

    return (searchDictionary(inputWord) != -1);
}

bool BoardBuilder::validOrientation(char orientation)
{  
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000000000, '\n');
        return false;
    }

    if (orientation != 'V' && orientation != 'H')
        return false;

    return true;
}

bool BoardBuilder::validCoordinates(char x, char y)
{
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000000000, '\n');
        return false;
    }

    if ((int(x) - 'a' < 0 || int(x) - 'a' >= size.w) || (int(y) - 'A' < 0 || int(y) - 'A' >= size.h))
        return false;

    return true;
}

bool BoardBuilder::validPlacement()
{
    string word = currentWord.name;
    uint len = currentWord.name.length();
    uint x = int(currentWord.coordinates.first) - 'a';
    uint y = int(currentWord.coordinates.second) - 'A';
    char orientation = currentWord.orientation;

    switch (orientation)
    {
    case 'V':
        if (y + len > size.h)
            return false;

        // check for adjacent words at the begining
        if (y == 0)
        {
            // the word is to be positioned touching the top margin
            // there is no need to check for adjacent words at the begining
        }
        else if (board[y - 1][x] != ' ')
        {
            // the word is starting right after another
            return false;
        }

        // check for adjacent words at the end
        if (y + len == size.h)
        {
            // the word is to be positioned touching the bottom margin
            // there is no need to check for adjacent words at the end
        }
        else if (board[y + len][x] != ' ')
        {
            // the word is starting right before another
            return false;
        }

        for (int i = 0; i < len; i++)
        {
            if (board[y + i][x] == ' ')
            {
                continue;
            }
            else if (board[y + i][x] != toupper(word[i]))
            {
                // mismatched words
                return false;
            }
        }
        // the word can be positioned on the board
        return true;

    case 'H':
        if (x + len > size.w)
            return false;

        // check for adjacent words at the begining
        if (x == 0)
        {
            // the word is to be positioned touching the left margin
            // there is no need to check for adjacent words at the begining
        }
        else if (board[y][x - 1] != ' ')
        {
            // the word is starting right after another
            return false;
        }

        // check for adjacent words at the end
        if (x + len == size.w)
        {
            // the word is to be positioned touching the right margin
            // there is no need to check for adjacent words at the end
        }
        else if (board[y][x + len] != ' ')
        {
            // the word is starting right before another
            return false;
        }

        for (int i = 0; i < len; i++)
        {
            if (board[y][x + i] == ' ')
            {
                continue;
            }
            else if (board[y][x + i] != toupper(word[i]))
            {
                // mismatched words
                return false;
            }
        }
        // the word can be positioned on the board
        return true;
    }
}

/*
---------- BOARD RELATED FUNCTIONS ----------
*/
void BoardBuilder::setOuputFile()
{
    bool start = false;
    string filesufix;
    clrscr();

    cout << "First of all, there is some details that need to be taken care of.\n\n";
    cout << "Let's start by defining the path where the Board will be saved.\n\n";
    cout << "Suggestion: save the file in the same directory of the ScrabbleJunior game executable is\n";
    cout << "E.g.: C:\\Users\\<username>\\Documents\\2S_PROG\\project_2\\ScrabbleJunior\\ScrabbleJunior\\Board.txt\n";
    cout << "ATTENTION:\n";
    cout << "         - don't forget to include in the path the name of the file\n";
    cout << "         - you must input the path with two backslashes in order it to be processed\n\n";

    ofstream ofboard;
    do {
        setColor(blue);
        cout << "I want to save my Scrabble Board to: ";
        setColor(white);
        getline(cin, boardFilename);

        // checks if the user inputed the name of the file where he wants the board to be saved with ".txt" at the end
        // if the length of the file inputed is lower than 4, it necessarily won't have the ".txt" extension sufix at the end
        if (boardFilename.length() > 4)
        {
            filesufix = boardFilename.substr(boardFilename.length() - 4, 4);
            if (filesufix != ".txt")
                boardFilename = boardFilename + ".txt";
        }
        else
            boardFilename = boardFilename + ".txt";
  
        ofboard.open(boardFilename);
        if (ofboard.fail() || cin.fail())
        {
            setColor(red);
            cout << "Operation failed. Please try again.\n\n";
            setColor(white);
        }
        
    } while (ofboard.fail() || cin.fail());

    ofboard.close();

    setColor(blue);
    cout << "\nIt's time to get to work! PRESS ENTER TO START...";
    setColor(white);
    while (!start) { if (cin.get() == '\n') start = true; }
}

void BoardBuilder::setSize()
{
    clrscr();
    char xchar;

    do {
        cout << "Board size (Width x Height for W, H in [5, 20]): "; 
        cin >> size.w >> xchar >> size.h;
        cin.ignore(10000000000, '\n');
        cout << endl;

        if (!validSize())
            cerr << "Invalid size! Please make sure to enter a valid size.\n\n";

    } while (!validSize());

    clrscr();
}

void BoardBuilder::constructBoard()
{
    vector<char> line(size.w, ' ');
    for (int i = 0; i < size.h; i++)
        board.push_back(line);
}

void BoardBuilder::showBoard()
{
    cout << ' '; 
    setColor(blue);

    for (int i = 'a'; i < 'a' + size.w; i++)
        cout << setw(4) << char(i);

    setColor(white); 
    cout << endl;

    for (int y = 0; y < size.h; y++)
    {
        setColor(blue); 
        cout << char(y + 'A'); 
        setColor(white); 

        for (int x = 0; x < size.w; x++)
        {
            cout << setw(4) << board[y][x];
        }
        cout << endl;
    }
    cout << endl;
}

void BoardBuilder::saveBoard()
{
    ofstream ofboard;

    do {
        ofboard.open(boardFilename);
    } while (ofboard.fail());

    ofboard << size.w << " x " << size.h << endl;
    for (vector<Word>::iterator i = words.begin(); i != words.end(); i++)
    {
        ofboard << i->coordinates.second << i->coordinates.first << ' ' << i->orientation << ' ' << i->name << endl;
    }

    cout << "\nBoard successfully saved in the file. Have fun with Scrabble Junior! \n";
    ofboard.close();
}

/*
---------- DICTIONARY RELATED FUNCTIONS ----------
*/
void BoardBuilder::readDictionary()
{
    ifstream ifdict;
    do { 
        ifdict.open(dictFilename); 
    } while (ifdict.fail());

    string word;
    while (getline(ifdict, word)) 
    { 
        dictionary.push_back(word); 
    }

    ifdict.close();
}

int BoardBuilder::searchDictionary(string word)
{
    int lower = 0;
    int upper = dictionary.size() - 1;
    int middle;
    bool found = false;

    while (!found && lower <= upper)
    {
        middle = (lower + upper) / 2;
        if (word < dictionary[middle])
            upper = middle - 1;
        else if (word > dictionary[middle])
            lower = middle + 1;
        else
            found = true;
    }

    if (found)
        return middle;
    else
        return -1;
}

/*
---------- WORD RELATED FUNCTIONS ----------
*/
void BoardBuilder::getWord()
{
    string inputWord;

    do {
        cout << "Word to be added to the board (CTRL-Z TO END): ";
        getline(cin, inputWord);
        transform(inputWord.begin(), inputWord.end(), inputWord.begin(), tolower); // since in the dictionary all the words are written in lowercase

        // if the user asks to leave, the function immediately returns
        if (cin.eof())
        {
            exit = true;
            return;
        }
        cout << endl;

        if (!validWord(inputWord))
        {
            setColor(red);
            cerr << "Word not found in the dictionary! Please make sure to enter a valid word.\n\n";
            setColor(white);
        }
    } while (!validWord(inputWord));

    transform(inputWord.begin(), inputWord.end(), inputWord.begin(), toupper);
    currentWord.name = inputWord;
}

void BoardBuilder::getOrientation()
{
    char orientation;
    bool valid = true;

    do {
        cout << "Orientation [V, H]: ";
        cin >> orientation;
        orientation = toupper(orientation); // orientation inputed in lowercase is also valid

        if (cin.peek() != '\n')
        {
            valid = false;
            cin.clear();
            cin.ignore(10000000000, '\n');
        }
        else
            valid = true;

        cout << endl;

        if (!validOrientation(orientation) || !valid)
        {
            setColor(red);
            cerr << "Invalid orientation! Please make sure to enter 'V', for vertical, or 'H', for horizontal.\n\n";
            setColor(white);
        }
    } while (!validOrientation(orientation) || !valid);

    currentWord.orientation = orientation;
}

void BoardBuilder::getCoordinates()
{
    char y, x;
    bool valid = true;

    do {
        cout << "Coordinates (Yx): ";
        cin >> y >> x;
        if (cin.peek() != '\n')
        {
            valid = false;
            cin.clear();
            cin.ignore(10000000000, '\n');
        }
        else
            valid = true;

        cout << endl;

        if (!validCoordinates(x, y) || !valid)
        {
            setColor(red);
            cerr << "Invalid coordinates! Please make sure to enter valid coordinates.\n\n";
            setColor(white);
        }

    } while (!validCoordinates(x, y) || !valid);

    cin.ignore(10000000000, '\n');
    currentWord.coordinates.first = x;
    currentWord.coordinates.second = y;
}

void BoardBuilder::positionWord()
{
    string w = currentWord.name;
    uint len = currentWord.name.length();
    uint x = int(currentWord.coordinates.first) - 97;
    uint y = int(currentWord.coordinates.second) - 65;
    char orientation = currentWord.orientation;

    switch (orientation)
    {
    case 'V':
        for (int i = 0; i < len; i++)
        {
            board[y + i][x] = toupper(w[i]);
        }
        break;
    case 'H':
        for (int i = 0; i < len; i++)
        {
            board[y][x + i] = toupper(w[i]);
        }
        break;
    }

    // word is added to the vector containing all the words of the board
    words.push_back(currentWord);
}

void BoardBuilder::processWord()
{
    bool placed = false;
    unsigned int numOfLetters = 0;

    do {
        do {
            exit = false;
            for (size_t i = 0; i < words.size(); i++)
                numOfLetters += (words[i].name.length() - 1);

            getWord();
            // the user keeps asking to leave but there are less than 14 letters in the board
            // the insertion of a new word will continue to be required
            while(exit && numOfLetters < 14)
            {
                setColor(red);
                cerr << "There is not enough words in the board yet. Please insert a few more.\n\n";
                setColor(white);
                exit = false;
                cin.clear();
                getWord();
            }
            if (exit && numOfLetters >= 14)
            {
                return;
            }
            getOrientation();
            getCoordinates();
            if (validPlacement())
            {
                positionWord();
                placed = true;
                clrscr();
                showBoard();
            }
            else
            {
                setColor(red);
                cerr << "Invalid position for the given word! Please try again.\n\n";
                setColor(white);
            } 
        } while (!placed);

    } while (!exit);
}