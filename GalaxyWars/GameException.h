#pragma once
#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <string>
using namespace std;

namespace GalaxyWars {

    class GameException {
        string errormsg;
    public:
        GameException(const string& msg) : errormsg(msg) {}

        string getMessage() const {
            return errormsg;
        }
    };

}

#endif
