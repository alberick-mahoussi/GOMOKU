/*
** EPITECH PROJECT, 2023
** B-AIA-500-PAR-5-1-gomoku-alberick.mahoussi
** File description:
** IGame
*/

#ifndef IGAME_HPP_
#define IGAME_HPP_

class Protocol;

class IGame {
    public:
        virtual void init(Protocol*) = 0;
        virtual void end() = 0;
        ~IGame() {};
};

#endif /* !IGAME_HPP_ */
