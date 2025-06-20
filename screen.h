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

void set(Rectangle exit ,bool &tempmenu ,vector<string> &templates,vector<string> &fileformat, Camera2D &Camera, int *Screen, int *tempPos, vector<Texture2D> &images);
void update(Vector2 mousepos);
void screen1();
void screen2();
void CreateBlankTemp();
void tempmenu(Rectangle temp);
void EditOptions();
void LoadFileFormat(Rectangle sheet);
void CloseFormatFile();
void ShowFileFormatEditMode(Rectangle sheet);
void CopyUiObjects();

const int screenWidth = GetScreenWidth();
const int screenHeight = GetScreenHeight();
const char* templates_file = "Templates/templates.txt";
const char* format_folder = "Templates/Formats/";

// -------------------------------------- Variable ---------------------------------------------------------
Camera2D camera;
vector<Texture2D> Images;

int *screen;
int *TempPos;

bool ReadEnable = true;
bool TempMenu;

vector<UiElements> SheetUiData; 
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
    DrawRectangleRec(NewTemplate, top);
    
    for (int y = 0; y <= int(Templates.size() / 8); y++){
        for (int x = 0; x <= 7; x++){
            if((y!=0 || x!=0)&& (8*y)+x < int(Templates.size())){
                // vector<vector<Rectangle>>
                Rectangle rect = {(float)225*x+75,(float)250*y+120,(float)150,200};
                Rectangle menu = {(float)rect.x+128,(float)rect.y,(float)22,18};
                DrawRectangleRec(rect, top);
                DrawTexture(Images.at(0), menu.x, menu.y, top);
                DrawText(Templates.at(8*y+x).c_str(), rect.x+25, rect.y+100, 20, WHITE);  // temp
            
                //check if the menu button has  been clicked
                if(CheckCollisionPointRec(mousePosWorld, menu)){
                    DrawRectangleRec(menu, slt);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            TempMenu = !TempMenu;
                            TempMenuRect = rect;
                            *TempPos = (8*y)+x;
                    }
                }
                // check if one of the templates has been clicked            
                else if(CheckCollisionPointRec(mousePosWorld, rect) && !TempMenu){
                    DrawRectangleRec(rect, slt);
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        cout << "temp has been clicked " << endl;                         // feed data to the exel using templte son click here 
                    }
                }
            }
        }
    }
    DrawText(" New + ", NewTemplate.x+(NewTemplate.width / 2)-30, NewTemplate.y+(NewTemplate.height/2), 20, WHITE);
    
    if(TempMenu == 1){
                tempmenu(TempMenuRect);
    }
    // Creating a new blank template         
    if(CheckCollisionPointRec(mousePosWorld, NewTemplate)){
        DrawRectangleRec(NewTemplate, slt);
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
    DrawRectangleRec(Rectangle {0,0,1950,35}, ui);
    
    DrawRectangleRec(Exit, red);
    DrawText("x", Exit.x+20, Exit.y+10, 20, WHITE);
    
    // bottom -----------------------
    DrawRectangleRec(Rectangle {0,35,1950,35}, top);
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
        ReadEnable = true;
        CopyUiObjects();
        CloseFormatFile();
    }
    Rectangle sheet =  {700,200,600,800};   
    string Name = Templates[*TempPos];
    BeginMode2D(camera);
        // click and move the template on the screen  
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {clicked = mousePosWorld;}
        DrawRectangleRec(sheet, slt); // edit mode
        
        DrawText(Name.c_str(), sheet.x+350-Name.size()*10, sheet.y, 30, BLACK);  // name
        
        if(ReadEnable){LoadFileFormat(sheet);}
        ShowFileFormatEditMode(sheet);
        updateui(mousePosWorld);
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
        float wheel = GetMouseWheelMove() * 20;
        if (wheel != 0 )
        {
            camera.target.y += wheel; // up and down movement
        }


        if(stredit){                                // take input from key board to a string and replace it with str
            int key = GetCharPressed();

            while (key > 0) {
                if ((key >= streditlowerlimit) && (key <= streditupperlimit) && ((int)streditvalue->size() < streditmaxval)) {
                    streditvalue->push_back((char)key);
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && streditvalue->size() > 0) {
                streditvalue->pop_back();  // Removes last character
            }
            if(IsKeyDown(KEY_ENTER)){
                stredit = false;
                streditvalue = nullptr;
                CopyUiObjects();
            }
        }
        
        
                    
    EndMode2D();
    
    //top ----------------------------------
    DrawRectangleRec(Rectangle {0,0,1950,35}, bg);
    DrawRectangleRec(Exit, red);
    DrawText("x", Exit.x+20, Exit.y+10, 20, WHITE);

    //bottom ------------------------------
    DrawRectangleRec(Rectangle {0,35,1950,35}, top); 

    DrawRectangleRec(Rectangle {0,75,300,900}, slt); // tool bar
    DrawTexture(Images.at(1), UiObjects.at(0).x, UiObjects.at(0).y, (Color){150,150,150,255});
    DrawTexture(Images.at(2), UiObjects.at(1).x, UiObjects.at(1).y, (Color){150,150,150,255});
    DrawTexture(Images.at(3), UiObjects.at(2).x, UiObjects.at(2).y, (Color){150,150,150,255});
    DrawTexture(Images.at(4), UiObjects.at(3).x, UiObjects.at(3).y, (Color){150,150,150,255});
    DrawTexture(Images.at(5), UiObjects.at(4).x, UiObjects.at(4).y, (Color){150,150,150,255});
    DrawTexture(Images.at(6), UiObjects.at(5).x, UiObjects.at(5).y, (Color){150,150,150,255});
    DrawTexture(Images.at(7), UiObjects.at(6).x, UiObjects.at(6).y, (Color){150,150,150,255});
    
    EditOptions();
}

//------------------------------ tepmlate menu-------------------------------------------------
void LoadFileFormat(Rectangle sheet){
    string tag;
    vector<string> data;
    string format;
    ReadEnable = false;
    SheetUiData.clear();
     // read the format file to print the data on the screen each time its being updated or when a screen is opened
    for(int j=0; j < (int)FileFormat.size(); j++){
        for(int i=0; i < (int)FileFormat.at(j).size(); i++){
            if(FileFormat.at(j).at(i) != ','){
                format.push_back(FileFormat.at(j).at(i));
            }else if(format == "text_field" || format == "dropdown" || format == "checkbox" || format == "radio" || format == "toggle" 
                || format == "date" || format == "uplode"){
                tag = format;
                format.clear();
            }else{
                data.emplace_back(format);
                format.clear();
            }
        }
        // store all the data on how the ui are placed and options in the object
        SheetUiData.emplace_back(tag,data);
        format.clear();
        data.clear();
    }
}

void ShowFileFormatEditMode(Rectangle sheet){
    for (int pos = (int)SheetUiData.size()-1; pos >= 0; pos--) {
        UiElements& data = SheetUiData.at(pos);
        std::string tag = data.tag;
        std::string name = data.GetName();  
        float x = sheet.x + 15,  y = sheet.y+ 15 + (pos+1)*30;
        Rectangle deleterect = {x-50,y,25,25};
        Rectangle namebox = {x, y, (float)name.size()*12,25};

        if (tag == "text_field") {
            Text_Field(sheet, pos+1, name, data);
        } else if (tag == "dropdown") {
            DropDown(sheet, pos+1, name, data);
        } else if (tag == "checkbox") {
            CheckBox(sheet, pos+1, name, data);
        } else if (tag == "radio") {
            Radio(sheet, pos+1, name, data);
        } else if (tag == "toggle") {
            Toggle(sheet, pos+1, name, data);
        } else if (tag == "date") {
            Date(sheet, pos+1, name, data);
        } else if (tag == "uplode") {
            Uplode(sheet, pos+1, name, data);
        }
        DrawRectangleRec(deleterect,red);
        DrawText("-",x-40,y+3,20,BLACK);
        if(CheckCollisionPointRec(MousePosWorld, deleterect)){
            DrawRectangleRec(deleterect,(Color){0,0,0,100});
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                ReadEnable = true;
                FileFormat.erase(FileFormat.begin()+pos);
            }
        }
        if(CheckCollisionPointRec(MousePosWorld, namebox)){
            DrawRectangleRec(namebox,slt);
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
                stredit = true;
                streditvalue = &SheetUiData[pos].options[0];
                streditmaxval = 20;
            }
        }
    }
}

void EditOptions(){
    if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(0))){
        DrawRectangleRec(UiObjects[0], slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            ReadEnable = true;
            FileFormat.push_back("text_field,Text,");
          }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(1))){
        DrawRectangleRec(UiObjects[1], slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            ReadEnable = true;
            FileFormat.push_back("dropdown,DropDown,op1,op2,");
        }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(2))){
        DrawRectangleRec(UiObjects[2], slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            ReadEnable = true;
            FileFormat.push_back("checkbox,CheckBox,option1,conclusion2,");
        }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(3))){
        DrawRectangleRec(UiObjects[3], slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            ReadEnable = true;
            FileFormat.push_back("radio,Radio,option1,conclusion2,");
        }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(4))){
        DrawRectangleRec(UiObjects[4], slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            ReadEnable = true;
            FileFormat.push_back("toggle,Text,");
        }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(5))){
        DrawRectangleRec(UiObjects[5], slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            ReadEnable = true;
            FileFormat.push_back("date,Date,8,11,2006,");
        }
    }else if(CheckCollisionPointRec(GetMousePosition(), UiObjects.at(6))){
        DrawRectangleRec(UiObjects[6], slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            ReadEnable = true;
            FileFormat.push_back("uplode,Text,");
        }
    }
}

void tempmenu(Rectangle temp){
    Rectangle view = {temp.x+90,temp.y+20,60,30};
    Rectangle edit = {temp.x+90,temp.y+50,60,30};
    Rectangle del = {temp.x+90,temp.y+80,60,30};
    Color transparent = {0,0,0,10};
    DrawRectangleRec(view, transparent);
    DrawText("View", view.x+15, view.y+10, 15, WHITE);
    DrawRectangleRec(edit, transparent);
    DrawText("Edit", edit.x+20, edit.y+10, 15, WHITE);
    DrawRectangleRec(del, transparent);
    DrawText("Delete", del.x+10, del.y+10, 15, WHITE);
    if(CheckCollisionPointRec(mousePosWorld, view)){
        DrawRectangleRec(view, slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ // VIEW
            // ADD VIEW FUNCTION HERE
        }
    }else if(CheckCollisionPointRec(mousePosWorld, edit)){ // EDIT
        DrawRectangleRec(edit, slt);
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
        DrawRectangleRec(del, slt);
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
        if(!Templates[i].compare(temp)){
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
            file << FileFormat[i] << "\n";
        }
        file.close(); // Close the file after writing
    } else {
        std::cout << "Unable to open file FileFormat\n";
    }

    SheetUiData.clear();
}

void CopyUiObjects(){
    FileFormat.clear();
    for (int pos = 0; pos < (int)SheetUiData.size(); pos++) {
        UiElements& data = SheetUiData.at(pos);
        std::string templatedata;
        for(int x=0; x < (int)data.options.size(); x++){
            templatedata += data.options[x]+",";
        }
        FileFormat.push_back(data.tag+","+templatedata);
    }
}