all: build 

build: game_of_life

game_of_life: game_of_life.c
		gcc -Wall -Werror -Wextra game_of_life.c -o game_of_life
		./game_of_life

clean:
		rm ./game_of_life