#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <vector>
#include <iostream>
using namespace std;

void screen1(Rectangle Exit, vector<Rectangle> &Templates, Camera2D &camera);
void screen2(Rectangle Exit, Camera2D &camera);

// ---------------------------------------- screen 1 ----------------------------------------------------

void screen1(Rectangle Exit, vector<Rectangle> &Templates, Camera2D &camera){
    
    Rectangle NewTemplate = {75,120,150,200};
    
    BeginMode2D(camera);
    DrawRectangleRec(NewTemplate, Color {255,255,255,255});
    
    for (int y = 0; y <= int(Templates.size() / 8); y++){
        for (int x = 0; x < 8; x++){ // vector<vector<Rectangle>>
            if((8*y)+x < Templates.size()){
                Rectangle rect = Templates.at((8*y)+x);
                DrawRectangleRec(rect, WHITE);
                        
            if(CheckCollisionPointRec(GetMousePosition(), rect)){
                DrawRectangleRec(rect, Color {0,0,0,50});
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        CloseWindow();
                    }
                }
            }
        }
    }
    DrawText(" New + ", NewTemplate.x+(NewTemplate.width / 2)-30, NewTemplate.y+(NewTemplate.height/2), 20, GRAY);
    
    if(CheckCollisionPointRec(GetMousePosition(), NewTemplate)){
        DrawRectangleRec(NewTemplate, Color {0,0,0,50});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            int y = int(Templates.size() / 8);
            int x = int(Templates.size() % 8);
            Rectangle temp = {225*x+75,250*y+125,150,200};
            Templates.push_back(temp);

        }
    }
    float wheel = GetMouseWheelMove()*15;
    if(wheel != 0 ){
        camera.target.y = Clamp(camera.target.y + wheel, 150, 500);
        NewTemplate.y = Clamp(NewTemplate.y + wheel, 150, 500);
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

void screen2(Rectangle Exit, Camera2D &camera){
                
    BeginMode2D(camera);        
        DrawRectangleRec(Rectangle {700,200,600,800}, Color {255,255,255,255}); // edit mode
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }else{
            float wheel = GetMouseWheelMove();
            if (IsKeyDown(KEY_LEFT_CONTROL) && wheel != 0 )
            {
                // Get the world point that is under the mouse
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

                // Set the offset to where the mouse is
                camera.offset = GetMousePosition();

                // Set the target to match, so that the camera maps the world space point 
                // under the cursor to the screen space point under the cursor at any zoom
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