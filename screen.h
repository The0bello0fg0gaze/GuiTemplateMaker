#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "ui.h"
#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <iostream>


using namespace std;

void set(Rectangle exit ,bool &tempmenu ,vector<string> &templates, Camera2D &Camera, int *Screen, Texture2D images);
void update(Vector2 mousepos);
void screen1();
void screen2();
void CreateBlankTemp();
void tempmenu(Rectangle temp);
void EditOptions();
void LoadFileFormat(Rectangle sheet);
void CloseFormatFile();

const int screenWidth = GetScreenWidth();
const int screenHeight = GetScreenHeight();
const char* templates_file = "Templates/templates.txt";
const char* format_folder = "Templates/Formats/";

// -------------------------------------- Variable ---------------------------------------------------------
Camera2D camera;
vector<Texture2D> Images;

int *screen;
int *TempPos;

bool TempMenu;
vector<string> Templates; // track the files that we have
Vector2 mousePosWorld;
vector<string> FileFormat; //store the current file loaded

Rectangle TempMenuRect = {0,0,0,0};
Rectangle Exit;

// update variable each loop
void update(Vector2 mousepos){
    mousePosWorld = GetScreenToWorld2D(mousepos, camera);
    mousePosWorld = {mousePosWorld.x, mousePosWorld.y+15};
}
// update varibale once
void set(Rectangle exit ,bool &tempmenu ,vector<string> &templates,vector<string> &fileformat, Camera2D &Camera, int *Screen, int *tempPos, vector<Texture2D> &images){
    TempPos = tempPos;
    Exit = exit;
    TempMenu = tempmenu;
    Templates = templates;
    FileFormat = fileformat;
    camera = Camera;
    screen = Screen;
    Images = images;
}
// ---------------------------------------- screen 1 ----------------------------------------------------
void screen1(){
    Rectangle NewTemplate = {75,120,150,200};
    
    BeginMode2D(camera);
    DrawRectangleRec(NewTemplate, Color {255,255,255,255});
    
    for (int y = 0; y <= int(Templates.size() / 8); y++){
        for (int x = 0; x <= 7; x++){
            if((y!=0 || x!=0)&& (8*y)+x < int(Templates.size())){
                // vector<vector<Rectangle>>
                Rectangle rect = {(float)225*x+75,(float)250*y+120,(float)150,200};
                Rectangle menu = {(float)rect.x+128,(float)rect.y,(float)22,18};
                DrawRectangleRec(rect, WHITE);
                DrawTexture(Images.at(0), menu.x, menu.y, WHITE);
                DrawText(Templates.at(8*y+x).c_str(), rect.x+25, rect.y+100, 20, BLACK);  // temp
            
                //check if the menu button has  been clicked
                if(CheckCollisionPointRec(mousePosWorld, menu)){
                    DrawRectangleRec(menu, Color {0,0,0,20});
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            TempMenu = !TempMenu;
                            TempMenuRect = rect;
                            *TempPos = (8*y)+x;
                    }
                }
                // check if one of the templates has been clicked            
                else if(CheckCollisionPointRec(mousePosWorld, rect) && !TempMenu){
                    DrawRectangleRec(rect, Color {0,0,0,50});
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        cout << "temp has been clicked " << endl;
                    }
                }
            }
        }
    }
    DrawText(" New + ", NewTemplate.x+(NewTemplate.width / 2)-30, NewTemplate.y+(NewTemplate.height/2), 20, GRAY);
    
    if(TempMenu == 1){
                tempmenu(TempMenuRect);
    }
    // Creating a new blank template         
    if(CheckCollisionPointRec(mousePosWorld, NewTemplate)){
        DrawRectangleRec(NewTemplate, Color {0,0,0,50});
        
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            CreateBlankTemp();
        }
    }
    float wheel = GetMouseWheelMove()*40;
    if(wheel != 0 ){
        camera.target.y = Clamp(camera.target.y - wheel, 0, int(Templates.size() / 8)*250);
        NewTemplate.y = Clamp(NewTemplate.y - wheel, 0, int(Templates.size() / 8)*250);
    }
    
    EndMode2D();
    
    //top --------------------------s
    DrawRectangleRec(Rectangle {0,0,1950,35}, Color {220,220,220,255});
    
    DrawRectangleRec(Exit, Color {255,0,0,255});
    DrawText("x", Exit.x+20, Exit.y+10, 20, WHITE);
    
    // bottom -----------------------
    DrawRectangleRec(Rectangle {0,35,1950,35}, Color {210,210,210,255});
}

//-------------------------------------screen 2 --------------------------------------------
vector<Rectangle> UiObjects = {
    {50,150,200,59}, //text_field 0
    {50,250,200,59}, //dropdown 1
    {50,350,200,59}, //checkbox 2
    {50,450,200,59}, //radio 3
    {50,550,200,59}, //toggle 4
    {50,650,200,59}, //date 5
    {50,750,200,59}  //uplode 6
};
Vector2 clicked = {0,0};
void screen2(){
    if( WindowShouldClose() ){
        *screen = 1;
        camera.zoom = 1.0f;
        camera.target = Vector2{0,0};
        camera.offset = Vector2{(float)screenWidth/2,(float)screenHeight/2};
        CloseFormatFile();
    }
    Rectangle sheet =  {700,200,600,800};   
    string Name = Templates.at(*TempPos);
    BeginMode2D(camera);
        // click and move the template on the screen  
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {clicked = mousePosWorld;}
        DrawRectangleRec(sheet, Color {255,255,255,255}); // edit mode
        
        DrawText(Name.c_str(), sheet.x+350-Name.size()*10, sheet.y, 30, BLACK);  // name
        
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(clicked, Rectangle {700,200,600,800})){
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }else{ // zoom in and out of the template
            float wheel = GetMouseWheelMove();
            if (IsKeyDown(KEY_LEFT_CONTROL) && wheel != 0 )
            {
                // Get the world point that is under the mouse
                Vector2 mouseWorldPos = GetScreenToWorld2D(mousePosWorld, camera);

                camera.offset = mousePosWorld;

                // Set the target to match, so that the camera maps the world space point 
                camera.target = mouseWorldPos;

                // Zoom increment
                // Uses log scaling to provide consistent zoom speed
                float scale = 0.2f*wheel;
                camera.zoom = Clamp(expf(logf(camera.zoom)+scale), 0.5f, 1.8f);
            }
        }
        LoadFileFormat(sheet);
                    
    EndMode2D();
    
    //top ----------------------------------
    DrawRectangleRec(Rectangle {0,0,1950,35}, Color {210,210,210,255});
    DrawRectangleRec(Exit, Color {255,0,0,255});
    DrawText("x", Exit.x+20, Exit.y+10, 20, WHITE);

    //bottom ------------------------------
    DrawRectangleRec(Rectangle {0,35,1950,35}, Color {220,220,220,255}); 

    DrawRectangleRec(Rectangle {0,75,300,900}, Color {200,200,200,255}); // tool bar
    DrawTexture(Images.at(1), UiObjects.at(0).x, UiObjects.at(0).y, WHITE);
    DrawTexture(Images.at(2), UiObjects.at(1).x, UiObjects.at(1).y, WHITE);
    DrawTexture(Images.at(3), UiObjects.at(2).x, UiObjects.at(2).y, WHITE);
    DrawTexture(Images.at(4), UiObjects.at(3).x, UiObjects.at(3).y, WHITE);
    DrawTexture(Images.at(5), UiObjects.at(4).x, UiObjects.at(4).y, WHITE);
    DrawTexture(Images.at(6), UiObjects.at(5).x, UiObjects.at(5).y, WHITE);
    DrawTexture(Images.at(7), UiObjects.at(6).x, UiObjects.at(6).y, WHITE);
    
    EditOptions();
}

//------------------------------ tepmlate menu-------------------------------------------------
void LoadFileFormat(Rectangle sheet){
    vector<int> Data;
    string format;
    updateui(mousePosWorld);
    for(int j=0; j < (int)FileFormat.size(); j++){
        for(int i=0; i < (int)FileFormat.at(j).size(); i++){
            if(FileFormat.at(j).at(i) != ','){
                format.push_back(FileFormat.at(j).at(i));
            }else if(format == "text_field"){
                Data.push_back(0);
                format.clear();
            }else if(format == "dropdown"){
                Data.push_back(1);
                format.clear();
            }else if(format == "checkbox"){
                Data.push_back(2);
                format.clear();
            }else if(format == "radio"){
                Data.push_back(3);
                format.clear();
            }else if(format == "toggle"){
                Data.push_back(4);
                format.clear();
            }else if(format == "date"){
                Data.push_back(5);
                format.clear();
            }else if(format == "uplode"){
                Data.push_back(6);
                format.clear();
            }else{
                Data.push_back(std::stoi(format));
                format.clear();
            }
        }
        int pos = Data.at(1);
        if(Data.at(0) == 0){
            string Text_field = "Student Names";
            Text_Field(sheet, pos, Text_field);
        }else if(Data.at(0) == 1){
            string Text_field = "State";
            DropDown(sheet, pos, Text_field);
        }else if(Data.at(0) == 2){
            DrawRectangle(sheet.x, sheet.y+pos*25, sheet.width, 25, BLUE);
        }else if(Data.at(0) == 3){
            DrawRectangle(sheet.x, sheet.y+pos*25, sheet.width, 25, YELLOW);
        }else if(Data.at(0) == 4){
            DrawRectangle(sheet.x, sheet.y+pos*25, sheet.width, 25, GRAY);
        }else if(Data.at(0) == 5){
            DrawRectangle(sheet.x, sheet.y+pos*25, sheet.width, 25, BLACK);
        }else if(Data.at(0) == 6){
            DrawRectangle(sheet.x, sheet.y+pos*25, sheet.width, 25, GRAY);
        }
        format.clear();
        Data.clear();
    }
}



void EditOptions(){
    int size = FileFormat.size()+1;
    if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(0))){
        DrawRectangleRec(UiObjects.at(0), Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            FileFormat.push_back(TextFormat("text_field,%i,",size));
          }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(1))){
        DrawRectangleRec(UiObjects.at(1), Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            FileFormat.push_back(TextFormat("dropdown,%i,",size));
        }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(2))){
        DrawRectangleRec(UiObjects.at(2), Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            FileFormat.push_back(TextFormat("checkbox,%i,",size));
        }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(3))){
        DrawRectangleRec(UiObjects.at(3), Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            FileFormat.push_back(TextFormat("radio,%i,",size));
        }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(4))){
        DrawRectangleRec(UiObjects.at(4), Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            FileFormat.push_back(TextFormat("toggle,%i,",size));
        }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(5))){
        DrawRectangleRec(UiObjects.at(5), Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            FileFormat.push_back(TextFormat("date,%i,",size));
        }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(6))){
        DrawRectangleRec(UiObjects.at(6), Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            FileFormat.push_back(TextFormat("uplode,%i,",size));
        }
    }
}

void tempmenu(Rectangle temp){
    Rectangle view = {temp.x+90,temp.y+20,60,30};
    Rectangle edit = {temp.x+90,temp.y+50,60,30};
    Rectangle del = {temp.x+90,temp.y+80,60,30};
    Color transparent = {0,0,0,10};
    DrawRectangleRec(view, transparent);
    DrawText("View", view.x+15, view.y+10, 15, BLACK);
    DrawRectangleRec(edit, transparent);
    DrawText("Edit", edit.x+20, edit.y+10, 15, BLACK);
    DrawRectangleRec(del, transparent);
    DrawText("Delete", del.x+10, del.y+10, 15, BLACK);
    if(CheckCollisionPointRec(mousePosWorld, view)){
        DrawRectangleRec(view, Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ // VIEW
            // ADD VIEW FUNCTION HERE
        }
    }else if(CheckCollisionPointRec(mousePosWorld, edit)){ // EDIT
        DrawRectangleRec(edit, Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){  
            *screen = 2;
            camera.zoom = 1.0f;
            camera.target = Vector2{0,0};
            camera.offset = Vector2{(float)screenWidth/2,(float)screenHeight/2};
            ifstream Format((format_folder+Templates.at(*TempPos)).c_str());
            string s;
            FileFormat.clear();
            while(getline(Format, s)){
                FileFormat.push_back(s);
            }
            Format.close();
        }
    }else if(CheckCollisionPointRec(mousePosWorld, del)){ //DELETE
        DrawRectangleRec(del, Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if (remove((format_folder+Templates.at(*TempPos)).c_str()) == 0) { //delete the file
                std::cout << "File deleted successfully." << std::endl;
            } else {
                perror("Failed to delete file");
            }
            
            Templates.erase(Templates.begin()+*TempPos);
            TempMenu = false;
        }
    }//check if the menu button has not been clicked
    else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(mousePosWorld, (Rectangle){temp.x+90,temp.y-15,70,125})) { 
        TempMenu = false; 
    }
}

// create a blank template
void CreateBlankTemp(){
    //check of the value
    string temp = TextFormat("Unames%i.txt",(int)Templates.size());
    for(int i=0; i<(int)Templates.size(); i++){
        if(!Templates.at(i).compare(temp)){
            temp = TextFormat("Unames%i.txt",(int)(Templates.size()+1));
        }
    }
    Templates.push_back(temp);
    
    fstream file; // Object of fstream class
    file.open(format_folder+temp, ios::out); // Open file "test.txt" in out(write) mode
    if (!file) { // If file is not created, return error
        std::cout << "Error in file creation!" << format_folder+temp << std::endl;
    } else {
        std::cout << "File Creation successful." << format_folder+temp << std::endl;
    }
    file.close();
    
}

void CloseFormatFile(){
    std::fstream file;
    file.open(TextFormat("Templates/Formats/%s",Templates.at(*TempPos).c_str()), std::ios::out | std::ios::trunc); // Open file for writing and truncate if it exists

    if (file.is_open()) {
        for(int i=0; i<(int)FileFormat.size(); i++){
            file << FileFormat.at(i) << "\n";
        }
        file.close(); // Close the file after writing
    } else {
        std::cout << "Unable to open file FileFormat\n";
    }
}