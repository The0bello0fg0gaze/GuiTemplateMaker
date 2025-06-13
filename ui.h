#include "raylib.h"
#pragma once
#include <iostream>
#include <vector>
#include <string>

class Select{
    public:
    std::string name = "Unamed";
    bool selected = false;
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
    Rectangle Text_Box = {x+15+Title.size()*13, y+15, Width, 25.0f};
    float thickness = 1.0f;
    if(CheckCollisionPointRec(MousePosWorld, Text_Box)){ 
        thickness = 1.5f;                                                     // add functioning here
    }
    DrawRectangleLinesEx(Text_Box, thickness, BLACK);
}

void DropDown(Rectangle Sheet, int Pos, std::string Title){
    float x = Sheet.x,  y = Sheet.y + Pos*30,   Width = Sheet.width-20-Title.size()*13;
    Rectangle dropdown = {x+15+Title.size()*13, y+15, Width, 25};
    DrawText(Title.c_str(), x+15, y+15, 20, BLACK);
    float thickness = 1.0f;
    if(CheckCollisionPointRec(MousePosWorld, dropdown)){ 
        thickness = 1.5f;                                                     // add functioning here
    }
    x = dropdown.x+dropdown.width ;                                           // change value of x for triangle
    DrawRectangleLinesEx(dropdown, thickness, BLACK);                         //draw the upsided down triangle and the box
    DrawTriangle(Vector2 {x-10, y+22}, Vector2 {x-30, y+22}, Vector2 {x-20, y+32}, BLACK);
}

void CheckBox(Rectangle Sheet, int Pos, std::string Title, std::vector<Select> data){
    float x = Sheet.x+15,  y = Sheet.y + Pos*30 +15,   Width = Sheet.width-20-Title.size()*13;
    DrawText(Title.c_str(), x, y, 20, BLACK);
    x += Title.size()*13;
    for(int i = 0; i < (int)data.size(); i++ ){
        x += 20;
        DrawText( data.at(i).name.c_str(), x, y, 20, BLACK);
        x += data.at(i).name.size()*14;
        float thickness = 1.0f;
        Rectangle checkboxrect = {x, y+5, 10, 10};
        if(CheckCollisionPointRec(MousePosWorld, checkboxrect)){
            thickness = 1.5f;                                                 // add functioning here
        }
        DrawRectangleLinesEx(checkboxrect, thickness, BLACK);
    }
}

void Radio(Rectangle Sheet, int Pos, std::string Title, std::vector<Select> data){
    float x = Sheet.x+15,  y = Sheet.y + Pos*30 +15,   Width = Sheet.width-20-Title.size()*13;
    DrawText(Title.c_str(), x, y, 20, BLACK);
    x += Title.size()*13;
    for(int i = 0; i < (int)data.size(); i++ ){
        x += 20;
        DrawText( data.at(i).name.c_str(), x, y, 20, BLACK);
        x += data.at(i).name.size()*14;
        DrawCircleLines( x, y+10, 5, BLACK);
        if(CheckCollisionPointCircle(MousePosWorld, (Vector2){x, y+10}, 5)){
            DrawCircleLines( x, y+10, 4, BLACK);                          // add functioning here
        }
    }
}
