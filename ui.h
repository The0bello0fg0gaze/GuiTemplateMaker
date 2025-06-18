#include "raylib.h"
#pragma once
#include <iostream>
#include <vector>
#include <string>

int streditmaxval = 20;
bool stredit = false;
std::string* streditvalue;

class UiElements{
    public:
    std::vector<std::string> options;
    std::string str_data = "NULL";
    std::vector<int> int_data = {-1,0,0};
    std::string tag;

    UiElements(std::string Tag, std::vector<std::string> Options = {"NULL"}){
        tag = Tag;
        options = Options;
    }

    std::string GetName(){
        return options.at(0);
    }
};

Vector2 MousePosWorld;
void updateui(Vector2 mousepos){
    MousePosWorld = mousepos;
}

// float x = Sheet.x,  y = Sheet.y + Pos,   Width = Sheet.width,  Height = 25;
void Text_Field(Rectangle Sheet, int Pos, std::string Title, UiElements &data){
    float x = Sheet.x,  y = Sheet.y + Pos*30,   Width = Sheet.width-20-Title.size()*13;
    DrawText(Title.c_str(), x+15, y+15, 20, BLACK);
    x = x+15+Title.size()*13;
    Rectangle Text_Box = {x, y+15, Width, 25.0f};
    DrawText(data.str_data.c_str(), x, y+15, 20, BLACK);
    float thickness = 1.0f;
    if(CheckCollisionPointRec(MousePosWorld, Text_Box)){ 
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        thickness = 1.5f;                                                     // add functioning here
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            stredit = true;
            streditvalue = &data.str_data;
            streditmaxval = 40;
        }
    }
    DrawRectangleLinesEx(Text_Box, thickness, BLACK);
}

void DropDown(Rectangle Sheet, int Pos, std::string Title, UiElements &data){
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

void CheckBox(Rectangle Sheet, int Pos, std::string Title, UiElements &data){
    float x = Sheet.x+15,  y = Sheet.y + Pos*30 +15;
    DrawText(Title.c_str(), x, y, 20, BLACK);
    x += Title.size()*13;
    for(int i = 1; i < (int)data.options.size(); i++ ){
        x += 20;
        DrawText( data.options.at(i).c_str(), x, y, 20, BLACK);
        x += data.options.at(i).size()*14;
        float thickness = 1.0f;
        Rectangle checkboxrect = {x, y+5, 10, 10};
        if(CheckCollisionPointRec(MousePosWorld, checkboxrect)){
            thickness = 1.5f;                                                 // add functioning here
        }
        DrawRectangleLinesEx(checkboxrect, thickness, BLACK);
    }
}

void Radio(Rectangle Sheet, int Pos, std::string Title, UiElements &data){
    float x = Sheet.x+15,  y = Sheet.y + Pos*30 +15;
    DrawText(Title.c_str(), x, y, 20, BLACK);
    x += Title.size()*13;
    for(int i = 1; i < (int)data.options.size(); i++ ){
        x += 20;
        DrawText( data.options.at(i).c_str(), x, y, 20, BLACK);
        x += data.options.at(i).size()*14;
        DrawCircleLines( x, y+10, 5, BLACK);
        if(CheckCollisionPointCircle(MousePosWorld, (Vector2){x, y+10}, 5)){
            DrawCircleLines( x, y+10, 4, BLACK);                          // add functioning here
        }
    }
}

void Toggle(Rectangle Sheet, int Pos, std::string Title, UiElements &data){
    float x = Sheet.x,  y = Sheet.y + Pos*30;
    DrawText(Title.c_str(), x+15, y+15, 20, BLACK);
    Rectangle Text_Box = {x+15+Title.size()*13, y+15, 200, 25.0f};
    float thickness = 1.0f;
    if(CheckCollisionPointRec(MousePosWorld, Text_Box)){ 
        thickness = 1.5f;                                       // add functioning here
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            data.int_data.at(0) = 0-data.int_data.at(0);
        }
    }
    if(data.int_data.at(0)>0){
        DrawRectangle(Text_Box.x+100, Text_Box.y+3, 100, 20, BLACK);
    }else{
        DrawRectangle(Text_Box.x, Text_Box.y+3, 100, 20, RED);
    }
    DrawRectangleLinesEx(Text_Box, thickness, BLACK);
}

void Date(Rectangle Sheet, int Pos, std::string Title, UiElements &data){
    float x = Sheet.x,  y = Sheet.y + Pos*30;
    DrawText(Title.c_str(), x+15, y+15, 20, BLACK);
    DrawText(TextFormat("%d/%d/%d",data.int_data.at(0),data.int_data.at(1),data.int_data.at(2)), x+15+Title.size()*13, y+15, 20, BLACK);           // make it take data from date format
}

void Uplode(Rectangle Sheet, int Pos, std::string Title){
    float x = Sheet.x,  y = Sheet.y + Pos*30,   Width = Sheet.width-20-Title.size()*13;
    DrawText(Title.c_str(), x+15, y+15, 20, BLACK);
    Rectangle Text_Box = {x+15+Title.size()*13, y+15, Width, 25.0f};
    float thickness = 1.0f;
    if(CheckCollisionPointRec(MousePosWorld, Text_Box)){ 
        thickness = 1.5f;                                                     // add functioning here
    }
    DrawRectangleLinesEx(Text_Box, thickness, BLACK);
}

