#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <thread>
#include <vector>
#include <utility>
#include <mutex>
#include <limits>
#include <algorithm>
#include <map>
#include <iostream>
#include <memory>

enum TILE_CONTENT {
    EMPTY = 0,
    USER_TRACE = 1,
    OPPONENT_TRACE = 2
};

class Brain {
    public:
        Brain() {};
        ~Brain() = default;

        void start();
        void stop();
        bool is_valid_position(int x, int y) ;
        void defensive_strategy();
        void offensive_strategy();
        int calculate_score(int lineLength, int openEnds);
        std::pair<int, int> find_best_defensive_move();
        int static_eval(const std::pair<int, int>& pos, TILE_CONTENT tileType);
        int evaluate_direction(const std::pair<int, int>& pos, const std::vector<int>& dir, TILE_CONTENT tileType);
        int find_patterns(const std::pair<int, int>& pos, const std::map<std::string, int>& patterns);
        std::pair<int, int> get_best_move_from_eval();
        int evaluate_board();
        bool should_defend(int current_score, const std::pair<int, int>& best_defensive_move);
        void placeTile(int x, int y, TILE_CONTENT tile);
        void updateMap(int wdth, int hght, std::vector<std::vector<TILE_CONTENT>> mapProtocole) {
            width = wdth;
            height = hght;
            map = mapProtocole;
        }
    private:
        int width, height;
        std::vector<std::vector<TILE_CONTENT>> map;
        // Protocol& protocol;
        std::thread defensive_thread;
        std::thread offensive_thread;
        bool running;
        std::mutex mutex;
        TILE_CONTENT OPPONENT_TRACE;
};

#endif // BRAIN_HPP
