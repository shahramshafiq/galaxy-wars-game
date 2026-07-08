#include "GameEngine.h"
#include "Managers.h"
#include "GameException.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using namespace GalaxyWars;

// static enemy counter
int Enemy::enemycount = 0;

int main()
{
    srand(static_cast<unsigned>(time(0)));

    try
    {
        GameEngine game;
        game.run();

        // load scores from binary file
        cout << endl;
        cout << "=== Loading Binary Scores ===" << endl;
        ScoreManager::loadBinaryScores();
    }
    catch (GameException& err)
    {
        cout << "Game Error: " << err.getMessage() << endl;
    }
    catch (...)
    {
        cout << "Unknown critical error occurred" << endl;
    }

    return 0;
}
