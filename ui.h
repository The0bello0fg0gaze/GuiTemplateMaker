#include "raylib.h"
#include "libxl.h"
#pragma once
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

static void SetOneTrue(std::vector<int> &int_data, int pos);

Color bg = {68,68,68,255};
Color top = {57,57,57,255};
Color ui = {113,113,113,255};
Color red = {255,105,97,255};
Color green = {144,238,144,255};
Color slt = {255,255,255,50};

std::vector<std::vector<std::string>> SheetData;
std::string previousvalue = "common";
int streditmaxval = 20;
bool stredit = false;
std::string* streditvalue = &previousvalue; // pointer to the string that is being edited
int streditlowerlimit = 32;
int streditupperlimit = 125;


class UiElements{
    public:
    std::vector<std::string> options;
    std::vector<std::string> str_data = {"NULL","1","1","1"};
    std::vector<int> int_data = {0, 0, 0}; // default size for dropdowns and checkboxes
    std::string tag;

    UiElements(std::string Tag, std::vector<std::string> Options = {"NULL"}){
        tag = Tag;
        options = Options;
        int size = Options.size();
        if(size > 4){
            int_data.clear();
            for(int i=0; i < size; i++){
                int_data.push_back(0);
            }
        }
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
    DrawText(data.str_data[0].c_str(), x+15, y+20, 20, BLACK);
    float thickness = 1.0f;
    if(CheckCollisionPointRec(MousePosWorld, Text_Box)){ 
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        thickness = 1.5f;                                                     
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            stredit = true;
            streditvalue = &data.str_data[0];
            streditmaxval = 40;
            streditlowerlimit = 32;
            streditupperlimit = 125;
        }
    }
    DrawRectangleLinesEx(Text_Box, thickness, BLACK);
}

void DropDown(Rectangle Sheet, int Pos, std::string Title, UiElements &data, bool edit = false){
    float x = Sheet.x,  y = Sheet.y + Pos*30,   Width = Sheet.width-20-Title.size()*13;
    Rectangle dropdown = {x+15+Title.size()*13, y+15, Width, 25};
    DrawText(Title.c_str(), x+15, y+15, 20, BLACK);
    float thickness = 1.0f;
    if(CheckCollisionPointRec(MousePosWorld, dropdown)){ 
        thickness = 1.5f;                                                     
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) data.int_data[0] = !data.int_data[0];
    }
    if(data.int_data[0]){
        DrawRectangle(x+15+Title.size()*13, y+15+25, Width, 25*(data.options.size()-1), (Color){200,200,200,255});
    }
    for(int i=1; i< (int)data.options.size(); i++){
        if(data.int_data[0]){
            Rectangle Dropdown = {x+15+Title.size()*13, y+15+25*i, Width, 25};
            DrawText(data.options[i].c_str(), Dropdown.x+(Width/2)-data.options[i].size()*12,Dropdown.y, 20, BLACK);
            if(CheckCollisionPointRec(MousePosWorld, Dropdown)){ 
                DrawRectangleRec(Dropdown, (Color){0,0,0,150});
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) SetOneTrue(data.int_data, i);
                else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && edit){
                    stredit = true;
                    streditvalue = &data.options[i];
                    streditmaxval = 40;
                    streditlowerlimit = 32;
                    streditupperlimit = 125;
                }
            }
        }
        if(data.int_data[i]){
            DrawText(data.options[i].c_str() ,dropdown.x+(Width/2)-data.options[i].size()*12 ,dropdown.y ,20 ,BLACK);
        }
    }
    x = dropdown.x+dropdown.width ;                                           
    DrawRectangleLinesEx(dropdown, thickness, BLACK);
    DrawTriangle(Vector2 {x-10, y+22}, Vector2 {x-30, y+22}, Vector2 {x-20, y+32}, BLACK); //draw the upsided down triangle and the box
}

void CheckBox(Rectangle Sheet, int Pos, std::string Title, UiElements &data, bool edit = false){
    float x = Sheet.x+15,  y = Sheet.y + Pos*30 +15;
    DrawText(Title.c_str(), x, y, 20, BLACK);
    x += Title.size()*13;
    for(int i = 1; i < (int)data.options.size(); i++ ){
        x += 20;
        DrawText( data.options[i].c_str(), x, y, 20, BLACK);
        x += data.options[i].size()*13;
        float thickness = 1.0f;
        Rectangle checkboxrect = {x-10, y, 20, 20};
        if(CheckCollisionPointRec(MousePosWorld, checkboxrect)){
            thickness = 1.5f;                                                 // add functioning here
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) data.int_data[i] = !data.int_data[i];
            else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && edit){
                    stredit = true;
                    streditvalue = &data.options[i];
                    streditmaxval = 40;
                    streditlowerlimit = 32;
                    streditupperlimit = 125;
                }
        }
        if(data.int_data[i]){
            DrawCircle(x, y+10, 5, BLACK);    
        }
        DrawRectangleLinesEx(checkboxrect, thickness, BLACK);
    }
}

void Radio(Rectangle Sheet, int Pos, std::string Title, UiElements &data, bool edit = false){
    float x = Sheet.x+15,  y = Sheet.y + Pos*30 +15;
    DrawText(Title.c_str(), x, y, 20, BLACK);
    x += Title.size()*13;
    for(int i = 1; i < (int)data.options.size(); i++ ){
        x += 20;
        DrawText( data.options[i].c_str(), x, y, 20, BLACK);
        x += data.options[i].size()*13;
        DrawCircleLines( x, y+7, 10, BLACK);
        if(CheckCollisionPointCircle(MousePosWorld, (Vector2){x, y+7}, 10)){
            DrawCircleLines( x, y+7, 8, BLACK);                          // add functioning here
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) SetOneTrue(data.int_data, i);
            else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && edit){
                    stredit = true;
                    streditvalue = &data.options[i];
                    streditmaxval = 40;
                    streditlowerlimit = 32;
                    streditupperlimit = 125;
                }
        }
        if(data.int_data[i]){
            DrawCircle(x, y+7, 5, BLACK);    
        }
    }
}

void Toggle(Rectangle Sheet, int Pos, std::string Title, UiElements &data, bool edit = false){
    float x = Sheet.x,  y = Sheet.y + Pos*30;
    DrawText(Title.c_str(), x+15, y+15, 20, BLACK);
    Rectangle Text_Box = {x+15+Title.size()*13, y+15, 80, 25.0f};
    float thickness = 1.0f;
    if(CheckCollisionPointRec(MousePosWorld, Text_Box)){ 
        thickness = 1.5f;                                       // add functioning here
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            data.int_data.at(0) = !data.int_data.at(0);
        }
    }
    if(data.int_data.at(0)){
        DrawRectangle(Text_Box.x, Text_Box.y, 80, 25, green);
        DrawText(" TRUE ", Text_Box.x, Text_Box.y+3, 20, BLACK);
    }else{
        DrawRectangle(Text_Box.x, Text_Box.y, 80, 25, red);
        DrawText(" FALSE ", Text_Box.x, Text_Box.y+3, 20, BLACK);
    }
    DrawRectangleLinesEx(Text_Box, thickness, BLACK);
}

void Date(Rectangle Sheet, int Pos, std::string Title, UiElements &data){
    float x = Sheet.x,  y = Sheet.y + Pos*30+15;
    DrawText(Title.c_str(), x+15, y, 20, BLACK);
    x +=  Title.size()*16;
    Rectangle Day = {x ,y ,30 ,25};
    Rectangle Month = {x+40  + 15,y ,30 ,25};
    Rectangle Year = {x+80 + 30,y ,50 ,25};
    if(CheckCollisionPointRec(MousePosWorld, Day)){
        DrawRectangleRec(Day,slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            stredit = true;
            streditlowerlimit = 45;
            streditupperlimit = 57;
            streditvalue = &data.str_data[1];
            streditmaxval = 2;
        }
    }else if(CheckCollisionPointRec(MousePosWorld, Month)){
        DrawRectangleRec(Month,slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            stredit = true;
            streditlowerlimit = 45;
            streditupperlimit = 57;
            streditvalue = &data.str_data[2];
            streditmaxval = 2;
        }
    }else if(CheckCollisionPointRec(MousePosWorld, Year)){
        DrawRectangleRec(Year,slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            stredit = true;
            streditlowerlimit = 45;
            streditupperlimit = 57;
            streditvalue = &data.str_data[3];
            streditmaxval = 4;
        }
    }
    DrawRectangleLinesEx(Day, 1, BLACK);
    DrawText("/", Month.x-20, y, 20, BLACK);
    DrawRectangleLinesEx(Month, 1, BLACK);
    DrawText("/", Year.x-20, y, 20, BLACK);
    DrawRectangleLinesEx(Year, 1, BLACK);
    DrawText(data.str_data[1].c_str(), Day.x+5, y, 20, BLACK);           // DD
    DrawText(data.str_data[2].c_str(), Month.x+5, y, 20, BLACK);           // MM
    DrawText(data.str_data[3].c_str(), Year.x+5, y, 20, BLACK);           // YYYY
}

void Uplode(Rectangle Sheet, int Pos, std::string Title, UiElements &data){
    float x = Sheet.x,  y = Sheet.y + Pos*30,   Width = Sheet.width-20-Title.size()*13;
    DrawText(Title.c_str(), x+15, y+15, 20, BLACK);
    Rectangle Text_Box = {x+15+Title.size()*13, y+15, Width, 25.0f};
    float thickness = 1.0f;
    if(CheckCollisionPointRec(MousePosWorld, Text_Box)){
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        thickness = 1.5f;                                                     
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            stredit = true;
            streditvalue = &data.str_data[0];
            streditmaxval = 80;
        }
    }
    DrawRectangleLinesEx(Text_Box, thickness, BLACK);
}

// private functions :---
static void SetOneTrue(std::vector<int> &int_data, int pos){
    for(int i=1; i < (int)int_data.size(); i++){
        if(i != pos){
            int_data[i] = 0;
        }else{
            int_data[i] = !int_data[i];
        }
    }
}
