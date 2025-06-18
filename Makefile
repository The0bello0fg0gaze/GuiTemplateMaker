compile: Main.cpp screen.h ui.h
	g++ Main.cpp -o main.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lxl -lraylib -lopengl32 -lgdi32 -lwinmm

test:
	g++ test.cpp -o test.exe -std=c++17 -Iinclude/ -Llib/ -lxl -lraylib -lopengl32 -lgdi32 -lwinmm