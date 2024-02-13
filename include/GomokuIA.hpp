/*
** EPITECH PROJECT, 2023
** B-AIA-500-PAR-5-1-gomoku-alberick.mahoussi
** File description:
** GomokuIA
*/

#include <iostream>
#include <vector>
#include <limits>
#include <array>
#include <memory>
#include <map>
#include <set>
#include <chrono>

enum TILE_CONTENT {
    EMPTY = 0,
    USER_TRACE = 1,
    OPPONENT_TRACE = 2
};

struct Move {
    int x, y;
    bool operator==(const Move& other) const {
        return x == other.x && y == other.y;
    }
};

constexpr int WIN_COUNT = 5;
constexpr int MAX_DEPTH = 1;

class GomokuAI {
public:
    GomokuAI(int width, int height) : width(width), height(height) {
        map.resize(height, std::vector<TILE_CONTENT>(width, EMPTY));
    }

    void makeMove(int x, int y, TILE_CONTENT player) {
        map[y][x] = player;
    }

    std::chrono::steady_clock::time_point startTime;
    const std::chrono::milliseconds maxDuration{4800};

    Move findBestMove() {
        startTime = std::chrono::steady_clock::now();

        int bestScore = std::numeric_limits<int>::min();
        for (const auto& move : getMoves()) {
            if (isWinningMove(move, USER_TRACE)) {
                return move;
            }
        }

        for (const auto& move : getMoves()) {
            if (isWinningMove(move, OPPONENT_TRACE)) {
                return move;
            }
        }
        Move bestMove;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (map[y][x] == EMPTY) {
                    map[y][x] = USER_TRACE;
                    int score = minimax(0, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
                    map[y][x] = EMPTY;
                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = {x, y};
                    }
                    if (isTimeUp()) return bestMove;
                }
            }
        }
        std::vector<std::vector<int>> scoresMap(height, std::vector<int>(width, std::numeric_limits<int>::min()));
        return bestMove;
    }
    void updateMap(const std::vector<std::vector<TILE_CONTENT>>& newMap) {
        map = newMap;
        // for (size_t y = 0; y < map.size(); y++) {
        //     for (size_t x = 0; x < map[y].size(); x++) {
        //         if (map[y][x] == EMPTY) {
        //             std::cout << " . ";
        //         } else if (map[y][x] == USER_TRACE) {
        //             std::cout << " 1 ";
        //         } else if (map[y][x] == OPPONENT_TRACE) {
        //             std::cout << " 2 ";
        //         }
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << std::endl;
    }

private:
    int width, height;
    std::vector<std::vector<TILE_CONTENT>> map;

    bool isTimeUp() {
        auto currentTime = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime) >= maxDuration;
    }

    bool isWinningMove(const Move& move, TILE_CONTENT player) {
    return checkLine(move, player, 1, 0)
        || checkLine(move, player, 0, 1)
        || checkLine(move, player, 1, 1)
        || checkLine(move, player, 1, -1);
    }

    bool checkLine(const Move& move, TILE_CONTENT player, int dx, int dy) {
        int count = 1;
        for (int i = 1; i < WIN_COUNT; ++i) {
            int x = move.x + i * dx;
            int y = move.y + i * dy;
            if (!isInBounds(x, y) || map[y][x] != player) break;
            count++;
        }
        for (int i = 1; i < WIN_COUNT; ++i) {
            int x = move.x - i * dx;
            int y = move.y - i * dy;
            if (!isInBounds(x, y) || map[y][x] != player) break;
            count++;
        }
        return count >= WIN_COUNT;
    }


    std::vector<Move> getMoves() {
        std::vector<Move> moves;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (map[y][x] == EMPTY && isAdjacentToPiece(x, y)) {
                    moves.push_back({x, y});
                }
            }
        }

        return moves;
    }

    bool isAdjacentToPiece(int x, int y) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;
                if (isInBounds(x + dx, y + dy) && map[y + dy][x + dx] != EMPTY) {
                    return true;
                }
            }
        }
        return false;
    }

    int minimax(int depth, bool isMaximizing, int alpha, int beta) {
        int score = evaluateBoard();
        const int WIN_SCORE = 100000000;

        if (depth == MAX_DEPTH || score == WIN_SCORE || score == -WIN_SCORE) {
            return score;
        }

        if (isMaximizing) {
            int bestScore = std::numeric_limits<int>::min();
            for (const auto& move : getMoves()) {
                map[move.y][move.x] = USER_TRACE;
                bestScore = std::max(bestScore, minimax(depth + 1, false, alpha, beta));
                map[move.y][move.x] = EMPTY;

                alpha = std::max(alpha, bestScore);
                if (beta <= alpha) {
                    break;
                }
            }
            return bestScore;
        } else {
            int bestScore = std::numeric_limits<int>::max();
            for (const auto& move : getMoves()) {
                map[move.y][move.x] = OPPONENT_TRACE;
                bestScore = std::min(bestScore, minimax(depth + 1, true, alpha, beta));
                map[move.y][move.x] = EMPTY;
                beta = std::min(beta, bestScore);
                if (beta <= alpha) {
                    break;
                }
            }
            return bestScore;
        }
    }


    const int WIN_COUNT = 5;

    int evaluateBoard() {
        int score = 0;

        score += evaluateLines();
        score += evaluateColumns();
        score += evaluateDiagonals();

        return score;
    }

    int evaluateLines() {
        int score = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x <= width - WIN_COUNT; x++) {
                score += evaluateSequence(x, y, 1, 0);
            }
        }
        return score;
    }

    int evaluateColumns() {
        int score = 0;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y <= height - WIN_COUNT; y++) {
                score += evaluateSequence(x, y, 0, 1);
            }
        }
        return score;
    }

    int evaluateDiagonals() {
        int score = 0;
        for (int y = 0; y <= height - WIN_COUNT; y++) {
            for (int x = 0; x <= width - WIN_COUNT; x++) {
                score += evaluateSequence(x, y, 1, 1);
            }
        }
        for (int y = 0; y <= height - WIN_COUNT; y++) {
            for (int x = WIN_COUNT - 1; x < width; x++) {
                score += evaluateSequence(x, y, -1, 1);
            }
        }
        return score;
    }

    int evaluateSequence(int startX, int startY, int dx, int dy) {
        int userCount = 0, opponentCount = 0, openEnds = 0;

        for (int i = 0; i < WIN_COUNT; i++) {
            if (map[startY + i * dy][startX + i * dx] == USER_TRACE) {
                userCount++;
            } else if (map[startY + i * dy][startX + i * dx] == OPPONENT_TRACE) {
                opponentCount++;
            }
        }

        if (isInBounds(startX - dx, startY - dy) && map[startY - dy][startX - dx] == EMPTY) {
            openEnds++;
        }
        if (isInBounds(startX + WIN_COUNT * dx, startY + WIN_COUNT * dy) && map[startY + WIN_COUNT * dy][startX + WIN_COUNT * dx] == EMPTY) {
            openEnds++;
        }

        if (userCount > 0 && opponentCount > 0) {
            return 0;
        } else if (userCount > 0) {
            return evaluateUserSequence(userCount, openEnds);
        } else if (opponentCount > 0) {
            return evaluateOpponentSequence(opponentCount, openEnds);
        }


        return 0;
    }

    int evaluateUserSequence(int userCount, int openEnds) {
        if (userCount == 5) return 1000000000;
        if (userCount == 4) return 1000000;
        if (userCount == 3 && openEnds == 2) return 500000;
        return userCount * 100 + openEnds * 10;
    }

    int evaluateOpponentSequence(int opponentCount, int openEnds) {
        if (opponentCount == WIN_COUNT) return -100000000;
        if (opponentCount == WIN_COUNT - 1) {
            if (openEnds > 0) return -500000;
            return -100000;
        }
        if (opponentCount == WIN_COUNT - 2 && openEnds == 2) return -10000;
        return -opponentCount * 100 - openEnds * 10;
    }

    bool isInBounds(int x, int y) {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

};
