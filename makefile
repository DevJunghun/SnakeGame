run:
	g++ -o game ./src/map.cpp ./src/position.cpp ./src/item.cpp ./src/stage.cpp ./src/vector.cpp ./src/snakeGame.cpp -lncurses
	./game