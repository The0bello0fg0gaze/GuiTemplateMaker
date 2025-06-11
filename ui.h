#include "raylib.h"
#pragma once
#include <iostream>
#include <string>

class Select{
    public:
    string name;
    bool selected;
    int data;
};

Vector2 MousePosWorld;
void updateui(Vector2 mousepos){
    MousePosWorld = mousepos;
}

// float x = Sheet.x,  y = Sheet.y + Pos,   Width = Sheet.width,  Height = 25;
void Text_Field(Rectangle Sheet, int Pos, std::string Title){
    float x = Sheet.x,  y = Sheet.y + Pos*30,   Width = Sheet.width-20-Title.size()*13;
    DrawText(Title.c_str(), x+15, y+15, 20, BLACK);
    DrawRectangleLines(x+15+Title.size()*13, y+15, Width, 25, BLACK);
}

void DropDown(Rectangle Sheet, int Pos, std::string Title){
    float x = Sheet.x,  y = Sheet.y + Pos*30,   Width = Sheet.width-20-Title.size()*13;
    Rectangle dropdown = {x+15+Title.size()*13, y+15, Width, 25};
    DrawText(Title.c_str(), x+15, y+15, 20, BLACK);
    float thickness = 1.0f;
    if(CheckCollisionPointRec(MousePosWorld, dropdown)){ thickness = 1.5f;}
    DrawRectangleLinesEx(dropdown, thickness, BLACK);                         //draw the upsided down triangle and the box
    DrawTriangle(Vector2 {x+Width+70, y+22}, Vector2 {x+Width+50, y+22}, Vector2 {x+Width+60, y+32}, BLACK);
}

void Radio(Rectangle Sheet, int Pos, std::string Title, vector<Select> Data){
    float x = Sheet.x+15,  y = Sheet.y+ 15 + Pos*30,   Width = Sheet.width-20-Title.size()*13;
    DrawText(Title.c_str(), x, y, 20, BLACK);
    for(int i=0; i< (int)Data.size(); i++){        // for each value of Data 
        DrawText(Data.at(i).name.c_str(), x, y, 20, BLACK);
        x += Data.name.size + i*;
        y = ;
        DrawCircleLines(x, y, 10, BLACK);
        if(Data.at(i).selected){                                               // draw the tick if selected                     
            DrawCircle(x, y, 5, BLACK);
        }
    }
}