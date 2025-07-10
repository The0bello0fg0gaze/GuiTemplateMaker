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
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        
        if(stredit){                                // take input from key board to a string and replace it with str
            int key = GetCharPressed();
            previousvalue = *streditvalue;
            while (key > 0) {
                if ((key >= streditlowerlimit) && (key <= streditupperlimit) && ((int)streditvalue->size() < streditmaxval)) {
                    streditvalue->push_back((char)key);
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && streditvalue->size() > 0) {
                streditvalue->pop_back();  // Removes last character
            }
            if(previousvalue != *streditvalue){
                previousvalue = *streditvalue;
                CopyUiObjects();
            }
            else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                stredit = false;  // Exit edit mode
            }
        }

        BeginDrawing();
            update(GetMousePosition());
            ClearBackground(bg);
            if(screen == 1){// Screen 1 is being loaded onto the screen
                
                screen1();
                
            }else if(screen == 2){
                
                screen2 ();     
   
            }else if(screen == 3){
                
                screen3 ();     
   
            }else if(screen == 4){
                
                screen4 ();     
   
            }
            
        if(CheckCollisionPointRec(GetMousePosition(), Exit)){ 
            DrawRectangleRec(Exit, Color {0,0,0,50});
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(screen == 1){
                    CloseFiles();
                    CloseFormatFile();
                    CloseWindow();
                }else{
                    screen = 1;
                    camera.zoom = 1.0f; 
                    camera.target = Vector2{0,0};
                    camera.offset = Vector2{(float)screenWidth/2,(float)screenHeight/2};
                    ReadEnable = true;
                    CopyUiObjects();
                    CloseFormatFile();
                    if(Name != Templates[TempPos]){ // if the name has been changed
                        remove((format_folder+Templates[TempPos]+".txt").c_str()); // delete the old file
                        Templates[TempPos] = Name; // update the name in the templates vector
                    }
                }
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

// close all the template files 
void CloseFiles(){
    std::fstream file;
    file.open(templates_file, std::ios::out | std::ios::trunc); // Open file for writing and truncate if it exists

    if (file.is_open()) {
        for(int i=0; i<(int)Templates.size(); i++){
            file << Templates.at(i) << "\n";
        }
        file.close(); // Close the file after writing
    } else {
        std::cout << "---Unable to open file Templates\n";
    }
}