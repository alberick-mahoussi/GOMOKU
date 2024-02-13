## EPITECH PROJECT, 2023
## B-AIA-500-PAR-5-1-gomoku-alberick.mahoussi
## File description:
## Makefile

# Source files
SRC =	src/Protocol.cpp	\
		src/Brain.cpp	\
		src/main.cpp

OBJ = $(SRC:.cpp=.o)

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -g3

# Executable name
NAME =  pbrain-gomoku-ai

# Build rule
all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(CXXFLAGS)

# Clean rule
clean:
	rm -f $(OBJ)

# Full clean rule
fclean: clean
	rm -f $(NAME)

# Rebuild rule
re: fclean all

# Phony targets
.PHONY: all clean fclean re
