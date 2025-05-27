#include "raylib.h"
#include <iostream>

void Button();
// A simple button

void Button(Rectangle rect, Color rcolor, const char *text, bool function, bool enable, Color tcolor){
    if(function && enable){
        DrawRectangleRec( rect, rcolor);
        DrawText( text, rect.x, rect.y, 30 ,  tcolor);
    }
}

bool function(Rectangle button, bool condition){
    if(CheckCollisionPointRec(GetMousePosition(), button)){
        if(condition){
            DrawRectangleRec( Rectangle {111,111,60,59}, RED);
        }
    }return 1;
}

"Resource/Movistar Text Regular.ttf"