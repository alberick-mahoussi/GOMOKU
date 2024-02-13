/*
** EPITECH PROJECT, 2023
** B-AIA-500-PAR-5-1-gomoku-alberick.mahoussi
** File description:
** Protocol
*/

#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <string>
// #include "Brain.hpp"
#include "GomokuIA.hpp"
#include <map>

#define MIN_SIZE_X 5
#define MIN_SIZE_Y 5

class Protocol {
    public:
        Protocol();
        //Protocol(Game*);
        ~Protocol();
        std::vector<std::string> Split(std::string del, std::string);
        void ManagRequest(std::string);
        int checkCmd(std::string cmd);
        void ReadLoop();
        void Start(std::string const &arg);
        void Restart(std::string const &arg);
        void Rectstart(std::string const &arg);
        void About(std::string const &arg);
        void Info(std::string const &arg);
        void Begin(std::string const &arg);
        void Turn(std::string const &arg);
        void Board(std::string const &arg);
        void Takeback(std::string const &arg);
        void Error(std::string const &arg);
        void Unknow(std::string const &arg);
        void Debug(std::string const &arg);
        void Message(std::string const &arg);
        void Play(std::string const &arg);
        void Suggest(std::string const &arg);
        void InitMap(int size);
        void InitMap(int width, int height);
        void End(std::string const &arg);
        void InsertMap(int x, int y, int type);
        int CheckKey(std::string key);

    protected:
        bool bord_tmp = false;
        int width, height; /* the board size */
        //int map[50][50];
        std::vector<std::vector<TILE_CONTENT>> map;

        std::map<std::string, std::string> Info_values = {
                {"timeout_turn", "0"},
                {"timeout_match", "0"},
                {"max_memory", "0"},
                {"time_left", "0"},
                {"game_type", "0"},
                {"rule", "0"},
                {"evaluate", "0"},
                {"folder", "./"}
        };
        std::shared_ptr<GomokuAI> brainPtr;
};

typedef void(Protocol::*COMMAND_SEND)(std::string const &);