/*
** EPITECH PROJECT, 2023
** B-AIA-500-PAR-5-1-gomoku-alberick.mahoussi
** File description:
** Protocol
*/

#include "Protocol.hpp"
#include <vector>
#include <string>


Protocol::Protocol()
{
}


Protocol::~Protocol()
{
}

bool checkInt(const std::string& var) {
    try {
        std::stoi(var);
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

void Protocol::Start(std::string const &arg)
{
    std::string arguments;
    if (arg[0] == ' ')
        arguments = arg.substr(1);
    if (std::stoi(arguments) < 5 || !checkInt(arguments)) {
        Error("BAD MAP SIZE");
        return;
    }

    width = std::stoi(arguments);
    height = std::stoi(arguments);
    map.clear();
    InitMap(width);
    brainPtr = std::make_shared<GomokuAI>(width, height);
    //brainPtr = std::make_shared<Brain>();
    brainPtr->updateMap(map);
    //brainPtr->start();
    std::cout << "OK" << std::endl;
}

void Protocol::Restart(std::string const &arg)
{
    (void)arg;
    map.clear();
    InitMap(width);
    std::cout << "OK" << std::endl;
}

void Protocol::Rectstart(std::string const &arg)
{
    std::string arguments;
    if (arg[0] == ' ')
        arguments = arg.substr(1);
    std::vector<std::string> args = Split(",", arguments);

    if (args.size() != 2 || !checkInt((args[0])) || !checkInt(args[1])) {
        Error("RECTSTART - BAD ARGUMENT");
        return;
    } else if (std::stoi(args[0]) < 5 || std::stoi(args[1]) < 5) {
        Error("RECTSTART - BAD SIZE MAP");
        return;
    }
    width = std::stoi(args[0]);
    height = std::stoi(args[1]);

    map.clear();
    InitMap(width, height);
}

void Protocol::About(std::string const &arg)
{
    (void)arg;
    std::cout << "name=\"pbrainGOMOKOU\", version=\"1.0\", author=\"pbrainGOMOKOU team\", country=\"FR\"" << std::endl;
}

int Protocol::CheckKey(std::string key)
{
    auto it = Info_values.find(key);
    if (it != Info_values.end()) {
        return 0;
    } else {
        return -1;
    }
}

void Protocol::Info(std::string const &arg)
{
    std::string arguments;
    if (arg[0] == ' ')
        arguments = arg.substr(1);
    std::vector<std::string> args = Split(" ", arguments);

    if (args.size() != 2) {
        Error("Info - BAD ARGUMENT");
        return;
    } else if (CheckKey(args[0]) == -1) {
        Error("Info - KEY INFORMATION DON'T EXIST");
        return;
    }

    if (args[0] == "game_type" && (std::stoi(args[1]) < 0 || std::stoi(args[1]) > 3)) {
        Error("Info - GAME-RTYPE ARGUMENT");
        return;
    } else if (args[0] == "rule" && (std::stoi(args[1]) != 1 && std::stoi(args[1]) != 2 && std::stoi(args[1]) != 4 && std::stoi(args[1]) != 8)) {
        Error("Info - RULE BAD ARGUMENT");
        return;
    }
    Info_values[args[0]] = args[1];
}

void Protocol::Begin(std::string const &arg)
{
    (void)arg;
    int x = width / 2;
    int y = height / 2;
    std::cout << x << "," << y << std::endl;
    InsertMap(x, y, 1);
    brainPtr->updateMap(map);
}

void Protocol::Turn(std::string const &arg)
{
    int pos_x;
    int pos_y;
    std::string arguments;
    std::vector<std::string> args;

    if (arg[0] == ' ')
        arguments = arg.substr(1);
    
    args = Split(",", arguments);
    
    if (args.size() != 2 || !checkInt(args[0]) || !checkInt(args[1])) {
        Error("BAD TURN POSITION");
        return;
    }

    pos_x = std::stoi(args.at(0));
    pos_y = std::stoi(args.at(1));

    if ((pos_x < 0 || pos_y < 0) || (pos_x > width - 1 || pos_y > height - 1)) {
        Error("BAD TURN POSITION");
        return;
    }

    if (map[pos_y][pos_x] != EMPTY) {
        Error("BAD POSITION CASE NOT EMPTY");
        return;
    }

    InsertMap(pos_x, pos_y, 2);
    /// GAme implement
    brainPtr->updateMap(map);
    std::cout << brainPtr->findBestMove().x << "," <<  brainPtr->findBestMove().y << std::endl;
    InsertMap(brainPtr->findBestMove().x, brainPtr->findBestMove().y, 1);
    brainPtr->updateMap(map);
}

void Protocol::Board(std::string const &arg)
{
    (void)arg;
    std::string args_board = "";
    std::vector<std::string> info;
    if (bord_tmp == false) {
        bord_tmp = true;
    }

    while (bord_tmp) {
        getline(std::cin, args_board);

        if (args_board.compare("DONE") == 0) {
            bord_tmp = false;
            break;
        }

        info = Split(",", args_board);

        if (info.size() != 3 || ((std::stoi(info[0]) > width - 1 || std::stoi(info[0]) < 0) || (std::stoi(info[1]) > height - 1 || std::stoi(info[1]) < 0) || (std::stoi(info[2]) != 1 && std::stoi(info[2]) != 2 && std::stoi(info[2]) != 3))) {
            Error("BAD BOARD ARGUMENT");
            //return;
        } else {
            if (map[std::stoi(info[1])][std::stoi(info[0])] != EMPTY) {
                Error("BAD POSITION CASE NOT EMPTY");
                continue;
            }
            //:: cout << std::stoi(info[0]) << " " << std::stoi(info[1]) << " " << std::stoi(info[2]) << std::endl;
            InsertMap(std::stoi(info[0]), std::stoi(info[1]), std::stoi(info[2]));
        }
    }
    brainPtr->updateMap(map);
    std::cout << brainPtr->findBestMove().x << "," <<  brainPtr->findBestMove().y << std::endl;
    InsertMap(brainPtr->findBestMove().x, brainPtr->findBestMove().y, 1);
    brainPtr->updateMap(map);
}

void Protocol::InsertMap(int x, int y, int type)
{
    switch (type) {
        case 1:
            map[y][x] = USER_TRACE;
            break;
        case 2:
            map[y][x] = OPPONENT_TRACE;
            break;
        case 3:
            break;

    }
}

void Protocol::Takeback(std::string const &arg)
{
    (void)arg;
}

void Protocol::Play(std::string const &arg)
{
    (void)arg;
}

int Protocol::checkCmd(std::string arg)
{
    int x = -1;
    std::vector<std::pair<std::string, COMMAND_SEND>> command = {
        std::make_pair("START", &Protocol::Start),
        std::make_pair("RESTART", &Protocol::Restart),
        std::make_pair("RECTSTART", &Protocol::Rectstart),
        std::make_pair("ABOUT", &Protocol::About),
        std::make_pair("BEGIN", &Protocol::Begin),
        std::make_pair("TURN", &Protocol::Turn),
        std::make_pair("BOARD", &Protocol::Board),
        std::make_pair("INFO", &Protocol::Info),
        std::make_pair("TAKEBACK", &Protocol::Takeback),
        std::make_pair("END", &Protocol::End),
        std::make_pair("PLAY", &Protocol::Play)
    };

    for (size_t i = 0; i < command.size(); i++) {
        if (command[i].first.compare(arg) == 0) {
            //std::cout << command[i].first << std::endl;
            x = i;
            break;
        }
    }
    return x;
}

std::vector<std::string> Protocol::Split(std::string del, std::string cmd)
{
    std::string space_delimiter = del;
    std::vector<std::string> args;

    size_t pos = cmd.find(space_delimiter);

    // Vérifier si le délimiteur espace est présent
    if (pos != std::string::npos) {
        while (pos != std::string::npos) {
            args.push_back(cmd.substr(0, pos));
            cmd.erase(0, pos + space_delimiter.length());
            pos = cmd.find(space_delimiter);
        }
        args.push_back(cmd);  // Ajouter la dernière partie de la chaîne
    } else {
        // S'il n'y a pas de délimiteur espace, ajouter la chaîne entière
        args.push_back(cmd);
    }

    return args;
}



void Protocol::ManagRequest(std::string cmd)
{
    switch (checkCmd(Split(" ", cmd)[0])) {
        case 0:
            if (Split(" ", cmd).size() != 2) {
                Error("ERROR Start command - Invalid arguments");
                return; 
                }
            Start(cmd.substr(Split(" ", cmd)[0].length()));
            break;
        case 1:
            if (Split(" ", cmd).size() != 1) {
                Error("COMMAND RESTART - Invalid arguments");
                return; 
            }
            Restart(cmd.substr(Split(" ", cmd)[0].length()));
            break;
        case 2:
            if (Split(" ", cmd).size() != 2) {
                Error("COMMAND RECTSTART - Invalid arguments");
                return; 
            }
            Rectstart(cmd.substr(Split(" ", cmd)[0].length()));
            break;
        case 3:
            if (Split(" ", cmd).size() != 1) {
                Error("COMMAND ABOUT - Invalid arguments");
                return; 
            }
            About("");
            break;
        case 4:
            if (Split(" ", cmd).size() != 1) {
                Error("COMMAND BEGIN - Invalid arguments");
                return; 
            }
            Begin(" ");
            break;
        case 5:
            if (Split(" ", cmd).size() != 2) {
                Error("COMMAND TURN - Invalid arguments");
                return; 
            }
            Turn(cmd.substr(Split(" ", cmd)[0].length()));
            break;
        case 6:
            if (Split(" ", cmd).size() != 1) {
                Error("COMMAND BOARD - Invalid arguments");
                return; 
            }
            Board(cmd.substr(Split(" ", cmd)[0].length()));
            break;
        case 7:
            if (Split(" ", cmd).size() != 3) {
                Error("INFO - BAD ARGUMENT COMMAND INFO");
                return;
            }
            Info(cmd.substr(Split(" ", cmd)[0].length()));
            break;
        case 8:
            if (Split(" ", cmd).size() != 2) {
                Error("COMMAND TAKEBACK - Invalid arguments");
                return; 
            }
            Takeback(cmd.substr(Split(" ", cmd)[0].length()));
            break;
        case 9:
            if (Split(" ", cmd).size() != 1) {
                Error("COMMAND END - Invalid arguments");
                return; 
            }
            End(" ");
            break;
        case 10:
            if (Split(" ", cmd).size() != 2) {
                Error("COMMAND PLAY - Invalid arguments");
                return; 
            }
            Play(cmd.substr(Split(" ", cmd)[0].length()));
    }
}


void Protocol::ReadLoop()
{
    int pos = -1;
    std::vector<std::string> args;
    std::string recv = "", cmd = "";
    while (1) {
        getline(std::cin, recv);
       // std::cout << recv << std::endl;
        if (recv.compare("") == 0)
            continue;
        args = Split(" ", recv);
        cmd += args[0];
        pos = checkCmd(cmd);
        if (pos == -1) {
            Error("THIS COMMAND DON'T EXIST");
            cmd = "";
        } else {
            ManagRequest(recv);
            cmd = "";
        }
    }
}

 void Protocol::Error(std::string const &arg)
 {
    std::cout << "ERROR " << arg << "\r\n";
 }

void Protocol::Unknow(std::string const &arg)
{
    std::cout << "UNKNOWN " << arg << "\r\n";
}

void Protocol::Debug(std::string const &arg)
{
    std::cout << "DEBUG " << arg << "\r\n";
}

void Protocol::Message(std::string const &arg)
{
    std::cout << "MESSAGE " << arg;
}

 void Protocol::InitMap(int size)
 {
    std::vector<TILE_CONTENT> empty_cointain;
    
    for (int i = 0; i < size; i++) {
        empty_cointain.push_back(EMPTY);
    }

    for (int i = 0; i < size; i++) {
        map.push_back(empty_cointain);
    }
 }

 void Protocol::InitMap(int width, int height)
 {
    std::vector<TILE_CONTENT> empty_cointain;

     for (int i = 0; i < width; i++) {
         empty_cointain.push_back(EMPTY);
     }

     for (int i = 0; i < height; i++) {
         map.push_back(empty_cointain);
     }
 }
 void Protocol::End(std::string const &arg)
 {
    (void)arg;
    exit (0);
 }