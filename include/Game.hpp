/*
** EPITECH PROJECT, 2023
** B-AIA-500-PAR-5-1-gomoku-alberick.mahoussi
** File description:
** Game
*/
#pragma once

#include <string>
//#include "Protocol.hpp"
#include "IGame.hpp"

class Game: public IGame {
    public:
        Game() {
            rfs = nullptr;
        };

        void init(Protocol *pr) {
            rfs = pr;
        }

        void end()
        {
            
        }


    private:
        Protocol *rfs;
};