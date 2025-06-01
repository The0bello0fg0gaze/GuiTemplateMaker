#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

void set(Rectangle exit ,bool &tempmenu ,vector<string> &templates, Camera2D &Camera, int *Screen, Texture2D Templatemenu);
void update(Vector2 mousepos);
void screen1();
void screen2();
void CreateBlankTemp();
void tempmenu(Rectangle temp);

// -------------------------------------- Variable ---------------------------------------------------------
int TempPos;
Rectangle TempMenuRect = {0,0,0,0};
Rectangle Exit;
bool TempMenu;
vector<string> Templates;
Camera2D camera;
int *screen;
Texture2D templatemenu;
Vector2 mousePosWorld;

// update variable each loop
void update(Vector2 mousepos){
    mousePosWorld = GetScreenToWorld2D(mousepos, camera);
}
// update varibale once
void set(Rectangle exit ,bool &tempmenu ,vector<string> &templates, Camera2D &Camera, int *Screen, Texture2D Templatemenu){
    Exit = exit;
    TempMenu = tempmenu;
    Templates = templates;
    camera = Camera;
    screen = Screen;
    templatemenu = Templatemenu;
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
                Rectangle rect = {225*x+75,250*y+120,150,200};
                Rectangle menu = {rect.x+128,rect.y,22,18};
                DrawRectangleRec(rect, WHITE);
                DrawTexture(templatemenu, menu.x, menu.y, WHITE);
                DrawRectangleRec(menu, (Color){0,0,0,0});
                DrawText(Templates.at(8*y+x).c_str(), rect.x+25, rect.y+100, 20, BLACK);  // temp
            
                //check if the menu button has  been clicked
                if(CheckCollisionPointRec(mousePosWorld, (Rectangle){menu.x-15,menu.y-15,menu.width+15,menu.height+15})){
                    DrawRectangleRec(menu, Color {0,0,0,20});
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            TempMenu = !TempMenu;
                            TempMenuRect = rect;
                            TempPos = (8*y)+x;
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

void screen2(){
                
    BeginMode2D(camera);        
        DrawRectangleRec(Rectangle {700,200,600,800}, Color {255,255,255,255}); // edit mode
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosWorld, Rectangle {700,200,600,800})){
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }else{
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
                    
    EndMode2D();
    
    //top ----------------------------------
    DrawRectangleRec(Rectangle {0,0,1950,35}, Color {210,210,210,255});
    DrawRectangleRec(Exit, Color {255,0,0,255});
    DrawText("x", Exit.x+20, Exit.y+10, 20, WHITE);

    //bottom ------------------------------
    DrawRectangleRec(Rectangle {0,35,1950,35}, Color {220,220,220,255}); 

    DrawRectangleRec(Rectangle {0,75,300,900}, Color {200,200,200,255}); // tool bar
}

//------------------------------ tepmlate menu-------------------------------------------------

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
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            // ADD VIEW FUNCTION HERE
        }
    }else if(CheckCollisionPointRec(mousePosWorld, edit)){
        DrawRectangleRec(edit, Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            *screen = 2;
        }
    }else if(CheckCollisionPointRec(mousePosWorld, del)){
        DrawRectangleRec(del, Color {0,0,0,20});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Templates.erase(Templates.begin()+TempPos);
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
    
}