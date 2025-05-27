#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <iostream>
using namespace std;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    int screen = 2;

    InitWindow(screenWidth, screenHeight, "Main File");
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
             BeginMode2D(camera);
             DrawRectangleRec(Rectangle {700,200,600,800}, Color {0,0,255,255}); // edit mode
             if(IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                Vector2 delta = GetMouseDelta();
                delta = Vector2Scale(delta, -1.0f/camera.zoom);
                camera.target = Vector2Add(camera.target, delta);
             }else{
                camera.target.y += 0-(GetMouseWheelMove()*30);
             }
             EndMode2D();
            
            if(screen == 1){
                DrawRectangleRec(Rectangle {0,35,1950,35}, Color {0,255,0,255}); //top
                DrawRectangleRec(Rectangle {0,0,1950,35}, Color {255,0,0,255}); // bottom
                
                //temp ----
                for (int y = 0; y < 10; y++){
                    for (int x = 0; x < 8; x++){ // vector<vector<Rectangle>> 
                        DrawRectangleRec(Rectangle {(75*x+150*x)+75,(125*y+200*y)+125,150,200}, Color {0,0,255,255});
                    }
                }
            }else if(screen == 2){
                DrawRectangleRec(Rectangle {0,0,1950,35}, Color {255,0,0,255}); // top
                DrawRectangleRec(Rectangle {0,35,1950,35}, Color {0,255,0,255}); //bottom
                
                DrawRectangleRec(Rectangle {0,75,300,900}, Color {255,0,0,255}); // tool bar
                
            }
 
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}