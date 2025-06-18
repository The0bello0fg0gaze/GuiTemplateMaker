compile: Main.cpp screen.h ui.h
	g++ Main.cpp -o main.exe -O1 -Wall -std=c++17 -Wno-missing-braces -I include/ -L lib/ -lxl -lraylib -lopengl32 -lgdi32 -lwinmm
