//
// Created by Coma on 22/11/2023.
//

#include "Brain.hpp"
#include <limits>
#include <algorithm>
#include <map>

const int NEG_INF = std::numeric_limits<int>::min();
const int POS_INF = std::numeric_limits<int>::max();
const int MAX_DEPTH = 3; // Adjust to check performances !!!!



void Brain::start() {
    running = true;

    // Launch a separate thread to continuously evaluate the best strategy
    std::thread decision_making_thread = std::thread([this]() {
        while (running) {
            std::lock_guard<std::mutex> lock(mutex);
            // Evaluate the current state of the board
            int current_score = evaluate_board();

            // Simulate defensive and offensive scenarios
            auto best_defensive_move = find_best_defensive_move();
            auto best_offensive_move = get_best_move_from_eval();

            // Decide whether to focus on offense or defense
            if (should_defend(current_score, best_defensive_move)) {
                // Prioritize defense
                placeTile(best_defensive_move.first, best_defensive_move.second, USER_TRACE);
                break;
            } else {
                // Focus on offense
                placeTile(best_offensive_move.first, best_offensive_move.second, USER_TRACE);
                break;
            }

            // Add delay to prevent high CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // Defensive and offensive strategies can still run in their dedicated threads
    defensive_thread = std::thread(&Brain::defensive_strategy, this);
    offensive_thread = std::thread(&Brain::offensive_strategy, this);

    // Join the decision-making thread in the stop method
    decision_making_thread.join();
}

bool Brain::should_defend(int current_score, const std::pair<int, int>& best_defensive_move) {
    // Simple logic: defend if the opponent has a significant threat
    // You can add more complex heuristics based on the current_score
    // and other factors of the game state.
    return best_defensive_move.first != -1;
}

std::pair<int, int> Brain::find_best_defensive_move() {
    int max_threat = 0;
    std::pair<int, int> best_defensive_move = {-1, -1};

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (map[y][x] == EMPTY) {
                int threat = static_eval({x, y}, OPPONENT_TRACE);
                if (threat > max_threat) {
                    max_threat = threat;
                    best_defensive_move = {x, y};
                }
            }
        }
    }

    return best_defensive_move;
}

void Brain::stop() {
    running = false;
    if (defensive_thread.joinable()) defensive_thread.join();
    if (offensive_thread.joinable()) offensive_thread.join();
}

void Brain::defensive_strategy() {
    while (running) {
        std::lock_guard<std::mutex> lock(mutex);
        int max_threat = 0;
        std::pair<int, int> best_move = {-1, -1};

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (map[y][x] == EMPTY) {
                    int threat = static_eval({x, y}, OPPONENT_TRACE);
                    if (threat > max_threat) {
                        max_threat = threat;
                        best_move = {x, y};
                    }
                }
            }
        }

        if (best_move.first != -1) {
            // Place a tile to block the threat
            placeTile(best_move.first, best_move.second, USER_TRACE);
        }

        // Add a small delay to prevent the thread from consuming too much CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Brain::offensive_strategy() {
    while (running) {
        std::lock_guard<std::mutex> lock(mutex);
        std::pair<int, int> best_move = get_best_move_from_eval();

        if (best_move.first != -1) {
            // Place a tile for the best offensive move
            placeTile(best_move.first, best_move.second, USER_TRACE);
        }

        // Add a small delay to prevent the thread from consuming too much CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int Brain::static_eval(const std::pair<int, int>& pos, TILE_CONTENT tileType) {
    int score = 0;
    std::vector<std::vector<int>> directions = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    for (const auto& dir : directions) {
        score += evaluate_direction(pos, dir, tileType);
    }
    return score;
}

int Brain::evaluate_direction(const std::pair<int, int>& pos, const std::vector<int>& dir, TILE_CONTENT tileType) {
    int lineLength = 0;
    int openEnds = 0;
    int x = pos.first;
    int y = pos.second;

    // Check in one direction
    for (int i = 0; i < 5; ++i) {
        x += dir[0];
        y += dir[1];
        if (!is_valid_position(x, y) || map[y][x] != tileType) {
            break;
        }
        if (map[y][x] == EMPTY) {
            openEnds++;
            break;
        }
        lineLength++;
    }

    // Reset and check in the opposite direction
    x = pos.first;
    y = pos.second;
    for (int i = 0; i < 5; ++i) {
        x -= dir[0];
        y -= dir[1];
        if (!is_valid_position(x, y) || map[y][x] != tileType) {
            break;
        }
        if (map[y][x] == EMPTY) {
            openEnds++;
            break;
        }
        lineLength++;
    }

    return calculate_score(lineLength, openEnds);
}

int Brain::calculate_score(int lineLength, int openEnds) {
    int score = 0;

    // Basic scoring based on the length of the line
    // The longer the line, the higher the score
    switch (lineLength) {
        case 4: score = 100000; break; // 4 in a line is a high priority
        case 3: score = 10000; break;  // 3 in a line
        case 2: score = 1000; break;   // 2 in a line
        case 1: score = 100; break;    // 1 in a line
        default: score = 0;
    }
    // Additional scoring for open ends
    // An open end means potential to extend the line
    if (openEnds == 2) {
        score *= 2; // Both ends open, higher potential
    } else if (openEnds == 1) {
        score = static_cast<int>(score * 1.5); // One end open, some potential
    }
    return score;
}

int Brain::find_patterns(const std::pair<int, int>& pos, const std::map<std::string, int>& patterns)
{
    std::string lines[4];
    int range = 4; // Look 4 tiles in each direction

    // Horizontal and Vertical
    for (int i = -range; i <= range; ++i) {
        int x = pos.first + i, y = pos.second + i;
        if (x >= 0 && x < width) {
            lines[0] += map[pos.second][x] == EMPTY ? '_' : (map[pos.second][x] == USER_TRACE ? 'x' : 'o');
        }
        if (y >= 0 && y < height) {
            lines[1] += map[y][pos.first] == EMPTY ? '_' : (map[y][pos.first] == USER_TRACE ? 'x' : 'o');
        }
    }

    // Diagonals
    for (int i = -range; i <= range; ++i) {
        int x1 = pos.first + i, y1 = pos.second + i, x2 = pos.first + i, y2 = pos.second - i;
        if (x1 >= 0 && y1 >= 0 && x1 < width && y1 < height) {
            lines[2] += map[y1][x1] == EMPTY ? '_' : (map[y1][x1] == USER_TRACE ? 'x' : 'o');
        }
        if (x2 >= 0 && y2 >= 0 && x2 < width && y2 < height) {
            lines[3] += map[y2][x2] == EMPTY ? '_' : (map[y2][x2] == USER_TRACE ? 'x' : 'o');
        }
    }

    // Calculate score
    int ret_val = 0;
    for (const auto& line : lines) {
        for (const auto& pattern : patterns) {
            size_t found = line.find(pattern.first);
            while (found != std::string::npos) {
                ret_val += pattern.second;
                found = line.find(pattern.first, found + 1);
            }
        }
    }
    return ret_val;
}

std::pair<int, int> Brain::get_best_move_from_eval() {
    int best_value = NEG_INF;
    std::pair<int, int> best_move;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (map[y][x] == EMPTY) {
                int score = static_eval({x, y}, map[y][x]);
                if (score > best_value) {
                    best_value = score;
                    best_move = {x, y};
                }
            }
        }
    }
    return best_move;
}

bool Brain::is_valid_position(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void Brain::placeTile(int x, int y, TILE_CONTENT tile) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        map[y][x] = tile;
        std::cout << y << std::endl;
        std::cout << x << std::endl;
    }
}

int Brain::evaluate_board() {
    int score = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (map[y][x] != EMPTY) {
                score += static_eval({x, y}, map[y][x]);
            }
        }
    }
    return score;
}
