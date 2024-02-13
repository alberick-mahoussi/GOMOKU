/*
** main.cpp for B-AIA-500-PAR-5-1-gomoku-alberick.mahoussi in c:\Users\mahou\B-AIA-500-PAR-5-1-gomoku-alberick.mahoussi\src
**
** Made by 
** Login   <>
**
*/

#include <iostream>
#include "Protocol.hpp"
#include "Game.hpp"

int main(void)
{
    try
    {
        Protocol *rfs = new Protocol();
        rfs->ReadLoop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}