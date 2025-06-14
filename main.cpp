#include "raylib.h"
#include "screen.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

// functions we are using
Rectangle MakeRectangle(string);
void CloseFiles();
void CloseFormatFile();


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    bool TempMenu = false;
    int screen = 1;
    int TempPos;
    vector<string> Templates;
    vector<Texture2D> Images;
    InitWindow(screenWidth, screenHeight, "Main File");
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    
    
    ifstream templates(templates_file);
    string s;
    while(getline(templates, s)){
        Templates.push_back(s);
    }
    templates.close();
    
    Image TemplateMenu = LoadImage("Resources/TemplateMenu.png"); //0
    Texture2D templatemenu = LoadTextureFromImage(TemplateMenu);
    Images.push_back(templatemenu);
    UnloadImage(TemplateMenu);
    
    Image Text_Field = LoadImage("Resources/Text_Field.png"); //1
    Texture2D text_field = LoadTextureFromImage(Text_Field);
    Images.push_back(text_field);
    UnloadImage(Text_Field);

    Image Dropdown = LoadImage("Resources/Dropdown.png"); //2
    Texture2D dropdown = LoadTextureFromImage(Dropdown);
    Images.push_back(dropdown);    
    UnloadImage(Dropdown);

    Image Checkbox = LoadImage("Resources/Checkbox.png"); //3
    Texture2D checkbox = LoadTextureFromImage(Checkbox);
    Images.push_back(checkbox);
    UnloadImage(Checkbox);

    Image Radio = LoadImage("Resources/Radio.png"); //4
    Texture2D radio = LoadTextureFromImage(Radio);
    Images.push_back(radio);
    UnloadImage(Radio);

    Image Toggle = LoadImage("Resources/Toggle.png"); //5
    Texture2D toggle = LoadTextureFromImage(Toggle);
    Images.push_back(toggle);
    UnloadImage(Toggle);
 
    Image Date = LoadImage("Resources/Date.png"); //6
    Texture2D date = LoadTextureFromImage(Date);
    Images.push_back(date);
    UnloadImage(Date);

    Image Uplode = LoadImage("Resources/Uplode.png"); //7
    Texture2D uplode = LoadTextureFromImage(Uplode);
    Images.push_back(uplode);
    UnloadImage(Uplode);
    
    
    vector<string> FileFormat;
    
    //permanents
    Rectangle Exit = {1870,3,60,30};
    // Main game loop
    set(Exit, TempMenu ,Templates, FileFormat, camera, &screen, &TempPos, Images);
    while (true)    // Detect window close button or ESC key
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            update(GetMousePosition());
            ClearBackground(Color {180,180,180,255});
            if(screen == 1){// Screen 1 is being loaded onto the screen
                
                screen1();
                
            }else if(screen == 2){
                
                screen2 ();     
   
            }
            
        if(CheckCollisionPointRec(GetMousePosition(), Exit)){ 
            DrawRectangleRec(Exit, Color {0,0,0,50});
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                CloseFiles();
                CloseFormatFile();
                CloseWindow();
            }
        }

        EndDrawing();
        
        //----------------------------------------------------------------------------------
    }
    CloseFiles();
    CloseFormatFile();
    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// close all the files 
void CloseFiles(){
    std::fstream file;
    file.open(templates_file, std::ios::out | std::ios::trunc); // Open file for writing and truncate if it exists

    if (file.is_open()) {
        for(int i=0; i<(int)Templates.size(); i++){
            file << Templates.at(i) << "\n";
        }
        file.close(); // Close the file after writing
    } else {
        std::cout << "Unable to open file Templates\n";
    }
}



//convert string from x-y-width-height- format to rectangle
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