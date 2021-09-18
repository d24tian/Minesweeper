#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

//Variables----------------------------------------------------------------------------------------------------------------------------
struct fPoint     //Basically a Vector2f variable
{
    int x;
    int y;
};

std::vector<fPoint> bombs;
std::vector<fPoint> revealed;
std::vector<fPoint> flags;
bool inGame = true;
int bombCount;

//Functions----------------------------------------------------------------------------------------------------------------------------
bool CheckRevealed (fPoint b)     //Check if the point at the specified position is revealed
{
    for (int i = 0; i < revealed.size(); i++)
    {
        if (revealed[i].x == b.x && revealed[i].y == b.y)
        {
            return true;
        }
    }
    return false;
}

bool CheckBomb (fPoint b)     //Check if the specified point contains a bomb
{
    for (int i = 0; i < bombs.size(); i++)
    {
        if (bombs[i].x == b.x && bombs[i].y == b.y)
        {
            return true;
        }
    }
    return false;
}

bool CheckFlag (fPoint b)     //Check if the specified point contains a flag
{
    for (int i = 0; i < flags.size(); i++)
    {
        if (flags[i].x == b.x && flags[i].y == b.y)
        {
            return true;
        }
    }
    return false;
}

int GenerateNumber (fPoint b)     //Generate a number for a tile based on the amount of bombs in adjacent tiles
{
    int number = 0;

    for (int i = 0; i < bombs.size(); i++)
    {
        if (bombs[i].x == b.x - 1 && bombs[i].y == b.y + 1)
        {
            number++;
        }

        if (bombs[i].x == b.x && bombs[i].y == b.y + 1)
        {
            number++;
        }

        if (bombs[i].x == b.x + 1 && bombs[i].y == b.y + 1)
        {
            number++;
        }

        if (bombs[i].x == b.x - 1 && bombs[i].y == b.y)
        {
           number++;
        }

        if (bombs[i].x == b.x + 1 && bombs[i].y == b.y)
        {
            number++;
        }

        if (bombs[i].x == b.x - 1 && bombs[i].y == b.y - 1)
        {
            number++;
        }

        if (bombs[i].x == b.x && bombs[i].y == b.y - 1)
        {
            number++;
        }

        if (bombs[i].x == b.x + 1 && bombs[i].y == b.y - 1)
        {
            number++;
        }
    }
    return number;
}

void CleanRevealedList ()     //Clean up lists so that they do not contain duplicate or useless coordinates
{
    for (int i = 0; i < revealed.size(); i++)
    {
        if (revealed[i].x < 1 || revealed[i].x > 10 || revealed[i].y < 1 || revealed[i].y > 10)
        {
            revealed.erase(revealed.begin() + i);
        }

        for (int j = 0; j < revealed.size(); j++)
        {
            if (revealed[j].x < 1 || revealed[j].x > 10 || revealed[j].y < 1 || revealed[j].y > 10)
            {
                revealed.erase(revealed.begin() + j);
            }

            if (j != i && revealed[i].x == revealed[j].x && revealed[i].y == revealed[j].y)
            {
                revealed.erase(revealed.begin() + j);
            }
        }
    }
    return;
}

void revealAround (fPoint b, bool blank)      //Reveal all tiles around a specified point
{
    if (blank)
    {
        if (GenerateNumber(b) != 0)
        {
            return;
        }
    }

    fPoint tmpReveal;

    for (int i = 0; i < 8; i++)
    {
        if (i == 0)
        {
            tmpReveal.x = b.x - 1;
            tmpReveal.y = b.y - 1;
        }
        else if (i == 1)
        {
            tmpReveal.x = b.x;
            tmpReveal.y = b.y - 1;
        }
        else if (i == 2)
        {
            tmpReveal.x = b.x + 1;
            tmpReveal.y = b.y - 1;
        }
        else if (i == 3)
        {
            tmpReveal.x = b.x - 1;
            tmpReveal.y = b.y;
        }
        else if (i == 4)
        {
            tmpReveal.x = b.x + 1;
            tmpReveal.y = b.y;
        }
        else if (i == 5)
        {
            tmpReveal.x = b.x - 1;
            tmpReveal.y = b.y + 1;
        }
        else if (i == 6)
        {
            tmpReveal.x = b.x;
            tmpReveal.y = b.y + 1;
        }
        else if (i == 7)
        {
            tmpReveal.x = b.x + 1;
            tmpReveal.y = b.y + 1;
        }

        if (CheckRevealed(tmpReveal) == false)
        {
            if (!CheckRevealed(tmpReveal))
            {
                if (blank == false)
                {
                    if (!CheckFlag(tmpReveal))
                    {
                        if (CheckBomb(tmpReveal))
                        {
                            inGame = false;
                            cout << "You have hit a bomb! Game over." << endl;
                        }
                        else
                        {
                            revealed.push_back(tmpReveal);
                        }
                    }
                }
                else
                {
                    if (CheckBomb(tmpReveal))
                    {
                        inGame = false;
                        cout << "You have hit a bomb! Game over." << endl;
                    }
                    else
                    {
                        revealed.push_back(tmpReveal);
                    }
                }
            }
        }
    }

    CleanRevealedList();

    return;
}

bool CheckBlanks ()     //Check whether or not there are still blanks existing
{
    fPoint tmpReveal;

    for (int i = 0; i < revealed.size(); i ++)
    {
        tmpReveal.x = revealed[i].x - 1;
        tmpReveal.y = revealed[i].y - 1;
        if (GenerateNumber(tmpReveal) == 0)
        {
            return true;
        }

        tmpReveal.x = revealed[i].x;
        tmpReveal.y = revealed[i].y - 1;
        if (GenerateNumber(tmpReveal) == 0)
        {
            return true;
        }

        tmpReveal.x = revealed[i].x + 1;
        tmpReveal.y = revealed[i].y - 1;
        if (GenerateNumber(tmpReveal) == 0)
        {
            return true;
        }

        tmpReveal.x = revealed[i].x - 1;
        tmpReveal.y = revealed[i].y;
        if (GenerateNumber(tmpReveal) == 0)
        {
            return true;
        }

        tmpReveal.x = revealed[i].x + 1;
        tmpReveal.y = revealed[i].y;
        if (GenerateNumber(tmpReveal) == 0)
        {
            return true;
        }

        tmpReveal.x = revealed[i].x - 1;
        tmpReveal.y = revealed[i].y - 1;
        if (GenerateNumber(tmpReveal) == 0)
        {
            return true;
        }

        tmpReveal.x = revealed[i].x;
        tmpReveal.y = revealed[i].y + 1;
        if (GenerateNumber(tmpReveal) == 0)
        {
            return true;
        }

        tmpReveal.x = revealed[i].x + 1;
        tmpReveal.y = revealed[i].y + 1;
        if (GenerateNumber(tmpReveal) == 0)
        {
            return true;
        }
    }
    return false;
}

void RevealBlanks ()      //Reveal all tiles around blanks
{
    int tmpSize = revealed.size();

    for (int i = 0; i < revealed.size(); i++)
    {
        if (GenerateNumber(revealed[i]) == 0)
        {
            revealAround(revealed[i], true);
        }
        CleanRevealedList();
    }

    return;
}

bool Win ()     //Check win condition
{
    if (revealed.size() == 100 - bombCount)
    {
        for (int i = 0; i < bombs.size(); i++)
        {
            if (CheckRevealed(bombs[i]))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

//MAIN--------------------------------------------------------------------------------------------------------------------------------
int main ()
{
    //Initilization--------------------------------------------------
    fPoint tmpPos;
    fPoint checkPos;
    int action;

    srand((unsigned)time(0));

    cout << "Welcome to Minesweeper!" << endl;

    do      //Prompt the user to input how many bombs they would like to start the game with
    {
        cout << "How many bombs would you like to play the board with?" << endl;
        cin >> bombCount;

        while (cin.fail())
        {
            cout << "Please enter a number from 1 to 99." << endl;
            cin.clear();
            cin.ignore(255,'\n');
            cin >> bombCount;
        }

        if (bombCount < 1 || bombCount > 99)
        {
            cout << "Please enter a number from 1 to 99." << endl;
        }
    }
    while (bombCount < 1 || bombCount > 99);

    for (int i = 0; i < bombCount; i++)     //Generate bombs randomly
    {
        fPoint b;

        b.x = (rand()%10)+1;
        b.y = (rand()%10)+1;

        while (CheckBomb(b))
        {
            b.x = (rand()%10)+1;
            b.y = (rand()%10)+1;
        }

        bombs.push_back(b);
    }

    //Main game loop--------------------------------------------------
    while (inGame)
    {
        for (int i = 0; i < 10; i++)      //Draw board
        {
            for (int j = 0; j < 10; j++)
            {
                tmpPos.x = j + 1;
                tmpPos.y = i + 1;

                if (CheckFlag(tmpPos))
                {
                    cout << "F ";
                }
                else if (CheckRevealed(tmpPos))
                {
                    if (CheckBomb(tmpPos))
                    {
                        cout << "X ";
                    }
                    else
                    {
                        if (GenerateNumber(tmpPos) == 0)
                        {
                            cout << "  ";
                        }
                        else
                        {
                            cout << GenerateNumber(tmpPos) << " ";
                        }
                    }
                }
                else
                {
                    cout << "- ";
                }
            }
            cout << i + 1 << endl;
        }
        for (int i = 0; i < 10; i++)
        {
            cout << i + 1 << " ";
        }
        cout << endl;

        if (revealed.size() > 0)      //Prompt the user to input what action they would like to perform, inputs are as follows:
                                        /*
                                        1 = Reveal a Tile
                                        2 = Place a flag on a tile or remove an existing flag
                                        3 = Reveal all tiles around a specified point
                                        */
        {
            do
            {
                cout << "What action would you like to perform?\n 1 = Reveal Tile\n 2 = Place/Remove Flag\n 3 = Reveal Around Tile" << endl;
                cin >> action;

                while (cin.fail())
                {
                    cout << "Please enter a number from 1 to 3." << endl;
                    cin.clear();
                    cin.ignore(255,'\n');
                    cin >> action;
                }

                if (action < 1 || action > 3)
                {
                    cout << "Please enter a number from 1 to 3." << endl;
                }
            }
            while (action < 1 || action > 3);
        }
        else
        {
            do
            {
                cout << "What action would you like to perform?\n 1 = Reveal Tile\n 2 = Place/Remove Flag\n 3 = Reveal Around Tile" << endl;
                cin >> action;

                while (cin.fail())
                {
                    cout << "Please enter a number from 1 to 3." << endl;
                    cin.clear();
                    cin.ignore(255,'\n');
                    cin >> action;
                }

                if (action == 3)
                {
                    cout << "You have not revealed any tiles yet!" << endl;
                }
                else if (action < 1 || action > 2)
                {
                    cout << "Please enter a number from 1 to 3." << endl;
                }
            }
            while (action < 1 || action > 2);
        }

        if (action == 1)  //If the user wants to reveal a tile
        {
            cout << "What are the X and Y positions of the tile you would like to reveal?" << endl;

            do
            {
                do
                {
                    cout << "X position: " << endl;
                    cin >> checkPos.x;

                    while (cin.fail())
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                        cin.clear();
                        cin.ignore(255,'\n');
                        cin >> checkPos.x;
                    }

                    if (checkPos.x > 10 || checkPos.x < 1)
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                    }
                }
                while (checkPos.x > 10 || checkPos.x < 1);

                do
                {
                    cout << "Y position: " << endl;
                    cin >> checkPos.y;

                    while (cin.fail())
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                        cin.clear();
                        cin.ignore(255,'\n');
                        cin >> checkPos.y;
                    }

                    if (checkPos.y > 10 || checkPos.y < 1)
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                    }
                }
                while (checkPos.y > 10 || checkPos.y < 1);

                if (CheckRevealed(checkPos))
                {
                    cout << "You have already revealed that position." << endl;
                }

                if (CheckFlag(checkPos))
                {
                    cout << "There is a flag on that position!" << endl;
                }
            }
            while (CheckRevealed(checkPos) || CheckFlag(checkPos));

            if (CheckBomb(checkPos))
            {
                cout << "You have hit a bomb! Game over." << endl;
                inGame = false;
            }

            if (!CheckRevealed(checkPos))
            {
                revealed.push_back(checkPos);
            }

            if (GenerateNumber(checkPos) == 0)
            {
                revealAround(checkPos, true);
            }

            CleanRevealedList();

            RevealBlanks();
        }
        else if (action == 2)    //If the user wants to place/remove a flag
        {
            cout << "What are the X and Y positions of the tile you would like to place/remove a flag on?" << endl;

            do
            {
                do
                {
                    cout << "X position: " << endl;
                    cin >> checkPos.x;

                    while (cin.fail())
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                        cin.clear();
                        cin.ignore(255,'\n');
                        cin >> checkPos.x;
                    }

                    if (checkPos.x > 10 || checkPos.x < 1)
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                    }
                }
                while (checkPos.x > 10 || checkPos.x < 1);

                do
                {
                    cout << "Y position: " << endl;
                    cin >> checkPos.y;

                    while (cin.fail())
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                        cin.clear();
                        cin.ignore(255,'\n');
                        cin >> checkPos.y;
                    }

                    if (checkPos.y > 10 || checkPos.y < 1)
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                    }
                }
                while (checkPos.y > 10 || checkPos.y < 1);

                if (CheckRevealed(checkPos))
                {
                    cout << "You have already revealed that position." << endl;
                }
            }
            while (CheckRevealed(checkPos));

            if (CheckFlag(checkPos))
            {
                for (int i = 0; i < flags.size(); i++)
                {
                    if (checkPos.x == flags[i].x && checkPos.y == flags[i].y)
                    {
                        flags.erase(flags.begin() + i);
                    }
                }
            }
            else
            {
                flags.push_back(checkPos);
            }
        }
        else if (action == 3)    //If the user wants to reveal all tiles around a point
        {
            cout << "What are the X and Y positions of the tile you would like to reveal around?" << endl;

            do
            {
                do
                {
                    cout << "X position: " << endl;
                    cin >> checkPos.x;

                    while (cin.fail())
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                        cin.clear();
                        cin.ignore(255,'\n');
                        cin >> checkPos.x;
                    }

                    if (checkPos.x > 10 || checkPos.x < 1)
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                    }
                }
                while (checkPos.x > 10 || checkPos.x < 1);

                do
                {
                    cout << "Y position: " << endl;
                    cin >> checkPos.y;

                    while (cin.fail())
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                        cin.clear();
                        cin.ignore(255,'\n');
                        cin >> checkPos.y;
                    }

                    if (checkPos.y > 10 || checkPos.y < 1)
                    {
                        cout << "Please enter a position from 1 to 10." << endl;
                    }
                }
                while (checkPos.y > 10 || checkPos.y < 1);

                if (!CheckRevealed(checkPos))
                {
                    cout << "You have not yet revealed that position." << endl;
                }
            }
            while (!CheckRevealed(checkPos));

            revealAround(checkPos, false);

            CleanRevealedList();

            RevealBlanks();
        }

        if (Win())      //Check for win condition
        {
            cout << "Congratulations! You have won!" << endl;
            inGame = false;
        }
    }

    //Game has ended--------------------------------------------------
    for (int i = 0; i < 10; i++)     //Draw fully revealed board after game ends
    {
        for (int j = 0; j < 10; j++)
        {
            tmpPos.x = j + 1;
            tmpPos.y = i + 1;

            if (CheckBomb(tmpPos))
            {
                cout << "X ";
            }
            else
            {
                if (GenerateNumber(tmpPos) == 0)
                {
                    cout << "  ";
                }
                else
                {
                    cout << GenerateNumber(tmpPos) << " ";
                }
            }
        }
        cout << i + 1 << endl;
    }
    for (int i = 0; i < 10; i++)
    {
        cout << i + 1 << " ";
    }
    cout << endl;

    return 0;
}
