#include "raylib.h"
#include "screen.h"
#include "rlgl.h"
#include "raymath.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;
Rectangle MakeRectangle(string s);
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    int screen = 2;
    vector<Rectangle> Templates;

    InitWindow(screenWidth, screenHeight, "Main File");
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    
    ifstream templates("templates.txt");
    string s;
    while(getline(templates, s)){
        Rectangle rect = MakeRectangle(s); // convert x-y-width-height- from string to rectangle;
        Templates.push_back(rect);
    }
    templates.close();
    
    //permanents
    Rectangle Exit = {1870,3,60,30};
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(Color {180,180,180,255});
            
            if(screen == 1){// Screen 1 is being loaded onto the screen
                
                screen1(Exit, Templates);
                
            }else if(screen == 2){
                
                screen2 (Exit, camera);
   
            }
            
        if(CheckCollisionPointRec(GetMousePosition(), Exit)){
            DrawRectangleRec(Exit, Color {0,0,0,50});
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                CloseWindow();
            }
        }

        EndDrawing();
        
        //----------------------------------------------------------------------------------
    }

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

Rectangle MakeRectangle(string str){
    Rectangle temp = {0,0,0,0};
    vector<int> temprect;
    string rect;
    for(int i=0; i <= int(str.length()); i++){
        if(str[i] != '-'){
            rect.push_back(str[i]);
        }else if(str[i] == '\n'){
            temprect.push_back(stoi(rect));
            temp.x = temprect[0];
            temp.y = temprect[1];
            temp.width = temprect[2];
            temp.height = temprect[3];
            return temp;
        }else{
            temprect.push_back(stoi(rect));
            rect.clear();
        }
    }
    temp.x = temprect[0];
    temp.y = temprect[1];
    temp.width = temprect[2];
    temp.height = temprect[3];
    return temp;
}