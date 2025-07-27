#include "raylib.h"
#include "raymath.h"
#include "ui.h"
#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
static int GetTruePos(std::vector<int> &int_data);

void set(Rectangle exit ,bool &tempmenu ,vector<string> &templates,vector<string> &fileformat, Camera2D &Camera, int *Screen, int *tempPos, vector<Texture2D> &images);
void screen1();
void screen2();
void screen3();
void screen4();
void update(Vector2 mousepos);
void screen1();
void screen2();
void CreateBlankTemp();
void tempmenu(Rectangle temp);
void EditOptions();
void LoadFileFormat();
void CloseFormatFile();
void ShowFileFormatEditMode(Rectangle sheet);
void CopyUiObjects();
void ShowFileFormatDataEntryMode(Rectangle sheet);
void CommitChanges();
void GetSheetData();
void CloseSheetData();

const int screenWidth = GetScreenWidth();
const int screenHeight = GetScreenHeight();
const char* templates_file = "Templates/templates.txt";
const char* format_folder = "Templates/Formats/";
const char* data_folder = "Templates/Data/";

// -------------------------------------- Variable ---------------------------------------------------------
Camera2D camera;
vector<Texture2D> Images;

int *screen;
int *TempPos;

bool delperm = false; // delete permission
bool commit = false; // commit changes
bool ReadEnable = true;
bool TempMenu;

string Name;

vector<UiElements> SheetUiData; 
vector<string> Templates; // track the files that we have
Vector2 mousePosWorld;
vector<string> FileFormat; //store the current file loaded

Rectangle TempMenuRect = {0,0,0,0};
Rectangle Exit;

vector<Rectangle> UiObjects = {
    {50,150,200,59}, //text_field 0
    {50,250,200,59}, //dropdown 1
    {50,350,200,59}, //checkbox 2
    {50,450,200,59}, //radio 3
    {50,550,200,59}, //toggle 4
    {50,650,200,59}, //date 5
    {50,750,200,59}  //uplode 6
};
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

void screen1(){  //Main menu screenf    l    
    Rectangle NewTemplate = {75,120,150,200};
    updateui(GetMousePosition());
    BeginMode2D(camera);
    DrawRectangleRec(NewTemplate, ui);
    
    for (int y = 0; y <= int(Templates.size() / 8); y++){
        for (int x = 0; x <= 7; x++){
            if((y!=0 || x!=0)&& (8*y)+x < int(Templates.size())){
                // vector<vector<Rectangle>>
                Rectangle rect = {(float)225*x+75,(float)250*y+120,(float)150,200};
                Rectangle menu = {(float)rect.x+128,(float)rect.y,(float)22,18};
                DrawRectangleRec(rect, ui);
                DrawTexture(Images.at(0), menu.x, menu.y, ui);
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
                        *TempPos = (8*y)+x;
                        *screen = 3;                         // feed data to the exel using templte son click here 
                        ifstream Format((format_folder+Templates.at(*TempPos)+".txt").c_str());
                        string s;
                        FileFormat.clear();
                        while(getline(Format, s)){
                            FileFormat.push_back(s);
                        }
                        Format.close();
                        LoadFileFormat();
                        GetSheetData();
                    }
                }
            }
        }
    }
    DrawText(" New + ", NewTemplate.x+(NewTemplate.width / 2)-30, NewTemplate.y+(NewTemplate.height/2), 20, WHITE);
    
    if(delperm){
        Rectangle del = {800,450,350,200};
        Rectangle Yes = {del.x+25,del.y+125,125,50};
        Rectangle No = {del.x+200,del.y+125,125,50};
        DrawRectangleRec(del, GRAY);
        DrawText("Are you sure you want to \n delete this template?", del.x+45, del.y+25, 20, BLACK);
        DrawRectangleRec(Yes, slt);
        DrawText("Yes", Yes.x+40, Yes.y+10, 20, BLACK);
        DrawRectangleRec(No, slt);
        DrawText("No", No.x+40, No.y+10, 20, BLACK);
        if(CheckCollisionPointRec(mousePosWorld, Yes)){
            DrawRectangleRec(Yes, (Color){0,0,0,200});
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if (remove((format_folder+Templates.at(*TempPos)+".txt").c_str()) == 0) { //delete the file
                    std::cout << "File deleted successfully." << std::endl;
                } else {
                    perror("Failed to delete file");
                }
                
                Templates.erase(Templates.begin()+*TempPos);
                TempMenu = false;
                delperm = false;
            }
        }else if(CheckCollisionPointRec(mousePosWorld, No)){
            DrawRectangleRec(No, (Color){0,0,0,200});
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                delperm = false;
            }
        }
    }

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
    DrawRectangleRec(Rectangle {0,0,1950,35}, top);
    
    DrawRectangleRec(Exit, red);
    DrawText("x", Exit.x+20, Exit.y+10, 20, WHITE);
    
    // bottom -----------------------
    DrawRectangleRec(Rectangle {0,35,1950,35}, ui);
}

void screen2(){ // Edit template screen
    if( WindowShouldClose() ){
        *screen = 1;
        camera.zoom = 1.0f;
        camera.target = Vector2{0,0};
        camera.offset = Vector2{(float)screenWidth/2,(float)screenHeight/2};
        ReadEnable = true;
        CopyUiObjects();
        CloseFormatFile();
        if(Name != Templates[*TempPos]){ // if the name has been changed
            remove((format_folder+Templates[*TempPos]+".txt").c_str()); // delete the old file
            Templates[*TempPos] = Name; // update the name in the templates vector
            
        }
    }
    Rectangle sheet =  {700,200,600,800};   
    BeginMode2D(camera);
        DrawRectangleRec(sheet, slt); // edit mode
        
        DrawText(Name.c_str(), sheet.x+350-Name.size()*10, sheet.y, 30, BLACK);  // name
        if(CheckCollisionPointRec(mousePosWorld, (Rectangle){sheet.x,sheet.y,sheet.width,30})){
            DrawRectangle(sheet.x,sheet.y,sheet.width,30, slt);
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                stredit = true;
                streditvalue = &Name;
                streditmaxval = 20;
                streditlowerlimit = 32;
                streditupperlimit = 125;
            }
        }
        
        if(ReadEnable){LoadFileFormat();}
        ShowFileFormatEditMode(sheet);
        updateui(mousePosWorld);

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
        if (wheel != 0 )
        {
            camera.target.y -= wheel * 20; // up and down movement
        }

    EndMode2D();
    
    //top ----------------------------------
    DrawRectangleRec(Rectangle {0,0,1950,35}, top);
    DrawRectangleRec(Exit, red);
    DrawText("x", Exit.x+20, Exit.y+10, 20, WHITE);

    //bottom ------------------------------
    DrawRectangleRec(Rectangle {0,35,1950,35}, ui); 

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

void screen3(){ // Data entry screen

    if( WindowShouldClose() ){
        *screen = 1;
        camera.zoom = 1.0f; 
        camera.target = Vector2{0,0};
        camera.offset = Vector2{(float)screenWidth/2,(float)screenHeight/2};
        ReadEnable = true;
        CopyUiObjects();
        CloseFormatFile();
        CloseSheetData();
            
    }

    Rectangle sheet =  {700,200,600,800};   
    string Name = Templates[*TempPos];

    if(ReadEnable){
        LoadFileFormat();
    }
    updateui(mousePosWorld);

    BeginMode2D(camera);
        DrawRectangleRec(sheet, slt); // edit mode
        
        DrawText(Name.c_str(), sheet.x+350-Name.size()*10, sheet.y, 30, BLACK);  // name
        if(CheckCollisionPointRec(mousePosWorld, (Rectangle){sheet.x,sheet.y,sheet.width,30})){
            DrawRectangle(sheet.x,sheet.y,sheet.width,30, slt);
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                stredit = true;
                streditvalue = &Templates[*TempPos];
                streditmaxval = 20;
                streditlowerlimit = 32;
                streditupperlimit = 125;
            }
        }
        
        float wheel = GetMouseWheelMove();
        if (IsKeyDown(KEY_LEFT_CONTROL) && wheel != 0 )
        {
            // Get the world point that is under the mouse
            // Vector2 mouseWorldPos = GetScreenToWorld2D(mousePosWorld, camera);

            camera.offset = Vector2{sheet.x + sheet.width/2, sheet.y + sheet.height/2};

            // Set the target to match, so that the camera maps the world space point 
            camera.target = Vector2{sheet.x + sheet.width/2, sheet.y + sheet.height/2};

            // Zoom increment
            // Uses log scaling to provide consistent zoom speed
            float scale = 0.2f*wheel;
            camera.zoom = Clamp(expf(logf(camera.zoom)+scale), 0.5f, 1.8f);
        }else if (wheel != 0 )
        {   
            camera.target.y -= wheel * 20; // up and down movement
        }
        
        if(commit){
        Rectangle del = {1400,450,350,200};
        Rectangle Yes = {del.x+25,del.y+125,125,50};
        Rectangle No = {del.x+200,del.y+125,125,50};
        DrawRectangleRec(del, GRAY);
        DrawText("Are you sure you want to \n Commit this template?", del.x+45, del.y+25, 20, BLACK);
        DrawRectangleRec(Yes, slt);
        DrawText("Yes", Yes.x+40, Yes.y+10, 20, BLACK);
        DrawRectangleRec(No, slt);
        DrawText("No", No.x+40, No.y+10, 20, BLACK);
        if(CheckCollisionPointRec(mousePosWorld, Yes)){
            DrawRectangleRec(Yes, (Color){0,0,0,200});
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                try{
                    CommitChanges();
                } catch (const std::exception& e) {
                    std::cerr << "Error committing changes: " << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "Error: While Committing Unknown" << std::endl;
                }
                commit = false;
            }
        }else if(CheckCollisionPointRec(mousePosWorld, No)){
            DrawRectangleRec(No, (Color){0,0,0,200});
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                commit = false;
            }
        }
    }
        
    ShowFileFormatDataEntryMode(sheet);         
    EndMode2D();
    //top --------------------------
    DrawRectangleRec(Rectangle {0,0,1950,35}, top);
    DrawRectangleRec(Exit, red);
    DrawText("x", Exit.x+20, Exit.y+10, 20, WHITE);

    // bottom -----------------------
    DrawRectangleRec(Rectangle {0,35,1950,35}, ui);

    // sheet ui ------------------------
    DrawRectangleRec(Rectangle {1775,950,125,50}, green);
    DrawText("Commit", 1775+20, 950+10, 30, BLACK);
    if( CheckCollisionPointRec(mousePosWorld, Rectangle {1775,950,125,50})){
        DrawRectangleRec(Rectangle {1775,950,125,50}, (Color){0,0,0,100});
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            DrawRectangleRec(Rectangle {1775,950,125,50}, (Color){0,0,0,150});
            commit = true; // ask for commit permission
        }
    }

    //lower bottom
    DrawRectangleRec(Rectangle {0,1010,1950,35}, ui);     
    
}

void screen4(){ // Data view screen
    if( WindowShouldClose() ){
        *screen = 1;
        camera.zoom = 1.0f; 
        camera.target = Vector2{0,0};
        camera.offset = Vector2{(float)screenWidth/2,(float)screenHeight/2};
        CloseSheetData();        
    }

    for(vector<string> &row : SheetData){
        for(int x = 0; x < (int)row.size(); x++){
            float x_pos = 100 + x * 200;
            float y_pos = 100 + (&row - &SheetData[0]) * 30; // calculate y position based on row index
            DrawText(row.at(x).c_str(), x_pos, y_pos, 20, BLACK);
        }
    }

    //top --------------------------
    DrawRectangleRec(Rectangle {0,0,1950,35}, top);
    DrawRectangleRec(Exit, red);
    DrawText("x", Exit.x+20, Exit.y+10, 20, WHITE);

    // bottom -----------------------
    DrawRectangleRec(Rectangle {0,35,1950,35}, ui);
}

// Functions in use
//load the file format from the file and store it in a vector of objects
void CommitChanges() {
    std::vector<string> data;
    // Save the current state of the UI elements
    for (const auto& element : SheetUiData) {
        // Save each element's data
        if(element.tag == "text_field") {
            // Save text field data
            data.push_back(element.str_data[0]);

        } else if(element.tag == "dropdown") {
            // Save dropdown data
            std::vector<int> int_data = element.int_data;
            int pos = GetTruePos(int_data);
            if(pos == -1){
                data.push_back("NULL");
            }else{
                data.push_back(element.options[pos]);
            }

        } else if(element.tag == "checkbox") {
            // Save checkbox data
            std::vector<int> int_data = element.int_data;
            for(int i=1; i < (int)int_data.size(); i++){
                if(int_data[i]){
                    data.push_back(element.options[i-1]);
                }
            }

        } else if(element.tag == "radio") {
            // Save radio button data
            std::vector<int> int_data = element.int_data;
            int pos = GetTruePos(int_data);
            if(pos == -1){
                data.push_back("NULL");
            }else{
                data.push_back(element.options[pos]);
            }

        } else if(element.tag == "toggle") {
            // Save toggle switch data
            std::vector<int> int_data = element.int_data;
            if(int_data[0]){
                data.push_back("TRUE");
            } else {
                data.push_back("FALSE");
            }
        } else if(element.tag == "date") {
            // Save date picker date
            string date = element.str_data[1] + "-" + element.str_data[2] + "-" + element.str_data[3];
            data.push_back(date);

        } else if(element.tag == "uplode") {
            // Save upload data
            data.push_back(element.str_data[0]);

        }
    }
    SheetData.push_back(data); // Store the data in the SheetData vector
}

void GetSheetData() {
    cout << "Running GetSheetData()";
    libxl::Book* book = xlCreateXMLBook();
    vector<string> Data;
    string filePath = main_filepath + data_folder + Templates[*TempPos] + ".xlsx";
    if (book) {
        if(book->load(filePath.c_str())){
            libxl::Sheet* Sheet = book->getSheet(0);
            for (int pos = 0; pos < (int)SheetUiData.size(); pos++) {
                Data.push_back(SheetUiData.at(pos).tag);
            }
            SheetData.clear(); // Clear previous data
            SheetData.push_back(Data); // Store the header row
            for (int row = Sheet->firstRow()+1; row < Sheet->lastRow(); ++row) {
                Data.clear();
                for (int col = Sheet->firstCol(); col < Sheet->lastCol(); ++col) {
                    if(Sheet->readStr(row, col) != nullptr){
                        string val = Sheet->readStr(row, col);
                        Data.push_back(val);
                    }
                }
                SheetData.push_back(Data); // Store the data in the SheetData vector
            }
        }else{
            cout << "-- Failed to load the file: " << book->errorMessage() << endl;
            libxl::Sheet* sheet = book->addSheet("Sheet1");
            book->save(filePath.c_str());
        }
    }else {
        cout << " -- Failed to create book" << endl;
        book->release();
        return;
    }
    book->release();
    cout << "-- successful." << endl;
}

void CloseSheetData() {
    cout << "Running CloseSheetData()";
    string filePath = data_folder + Templates[*TempPos] + ".xlsx";
    libxl::Book* book = xlCreateXMLBook();
    vector<string> Data;
    libxl::Sheet* sheet = book->addSheet(Templates[*TempPos].c_str());
    if(book == nullptr){
        cout << " -- CloseSheetData(): Failed to create book: " << book->errorMessage() << endl;
        return;
    }
    if(sheet != nullptr){
        for(int i = 0; i < (int)SheetData.size(); i++){
            for(int j = 0; j < (int)SheetData.at(i).size(); j++){
                sheet->writeStr(i+1, j+1, SheetData.at(i).at(j).c_str());
            }
        }
    }else{
        cout << " -- CloseSheetData(): Failed to get sheet: " << book->errorMessage() << endl;
    }
    book->save(filePath.c_str());
    book->release();
    SheetData.clear();
    cout << " -- successfull." << endl;
}

// Create a vector of sheet ui objects to store the file format that will run when adding the data to exel sheet. <Runs when ReadEnable is true>
void LoadFileFormat(){ 
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

// show ui elements in edit mode screen 3

void ShowFileFormatDataEntryMode(Rectangle sheet){
    for (int pos = (int)SheetUiData.size()-1; pos >= 0; pos--) {
        UiElements& data = SheetUiData.at(pos);
        std::string tag = data.tag;
        std::string name = data.GetName();  
        //float x = sheet.x + 15,  y = sheet.y+ 15 + (pos+1)*30;

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
    }
}

// show ui elements in edit mode screen 2
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
            DropDown(sheet, pos+1, name, data, true);
            Rectangle add = {x+sheet.width,y,25,25};
            DrawRectangleRec(add,GREEN);
            DrawText("+",add.x+5,add.y,20,BLACK);
            if(CheckCollisionPointRec(MousePosWorld, add)){
                DrawRectangleRec(add,(Color){0,0,0,100});
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    ReadEnable = true;
                    SheetUiData.at(pos).options.push_back( "option" );
                    CopyUiObjects();
                }
            }
            for(int i=1; i< (int)data.options.size(); i++){
                if(data.int_data[0]){
                    Rectangle del = {sheet.x+sheet.width, y+25*i, 25, 25};
                    DrawRectangleRec(del,red);
                    DrawText("-",del.x+5,del.y+3,20,BLACK);
                    if(CheckCollisionPointRec(MousePosWorld, del)){
                        DrawRectangleRec(del, (Color){0,0,0,150});
                        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            ReadEnable = true;
                            data.options.erase(data.options.begin()+i);
                            CopyUiObjects();
                        }
                    }
                }
            }
        } else if (tag == "checkbox") {
            CheckBox(sheet, pos+1, name, data, true);
            Rectangle add = {x+sheet.width,y,25,25};
            DrawRectangleRec(add,GREEN);
            DrawText("+",add.x+5,add.y,20,BLACK);
            if(CheckCollisionPointRec(MousePosWorld, add)){
                DrawRectangleRec(add,(Color){0,0,0,100});
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    ReadEnable = true;
                    SheetUiData.at(pos).options.push_back( "option" );
                    CopyUiObjects();
                }
            }
            for(int i=1; i< (int)data.options.size(); i++){
                if(data.int_data[0]){
                    Rectangle del = {sheet.x+sheet.width-30, y+15+25*i, 25, 25};
                    DrawRectangleRec(del,red);
                    DrawText("-",del.x+5,del.y,20,BLACK);
                    if(CheckCollisionPointRec(MousePosWorld, del)){
                        DrawRectangleRec(del, (Color){0,0,0,150});
                        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                            ReadEnable = true;
                            data.options.erase(data.options.begin()+i);
                            CopyUiObjects();
                        }
                    }
                }
            }
        } else if (tag == "radio") {
            Radio(sheet, pos+1, name, data, true);
        } else if (tag == "toggle") {
            Toggle(sheet, pos+1, name, data, true);
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

// Add ui objects to the vector FileFormat screen 2

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

// show the template menu screen 1

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
            *screen = 4;
            camera.zoom = 1.0f;
            camera.target = Vector2{0,0};
            camera.offset = Vector2{(float)screenWidth/2,(float)screenHeight/2};
            GetSheetData(); // load the data from the exel file
        }
    }else if(CheckCollisionPointRec(mousePosWorld, edit)){ // EDIT
        DrawRectangleRec(edit, slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){  
            *screen = 2;
            camera.zoom = 1.0f;
            camera.target = Vector2{0,0};
            camera.offset = Vector2{(float)screenWidth/2,(float)screenHeight/2};
            ifstream Format((format_folder+Templates.at(*TempPos)+".txt").c_str());
            string s;
            FileFormat.clear();
            while(getline(Format, s)){
                FileFormat.push_back(s);
            }
            Format.close();
            ReadEnable = true; // enable reading from the file
            Name = Templates[*TempPos];
        }
    }else if(CheckCollisionPointRec(mousePosWorld, del)){ //DELETE
        DrawRectangleRec(del, slt);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            delperm = true; // set the delete permission to true
        }
    }//check if the menu button has not been clicked
    else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(mousePosWorld, (Rectangle){temp.x+90,temp.y-15,70,125})) { 
        TempMenu = false; 
    }
}

// Create a new blank template file screen 1

void CreateBlankTemp(){
    //check of the value
    string temp = TextFormat("Unames%i",(int)Templates.size());
    for(int i=0; i<(int)Templates.size(); i++){
        if(!Templates[i].compare(temp)){
            temp = TextFormat("Unames%i",(int)(Templates.size()+1));
        }
    }
    Templates.push_back(temp);
    
    fstream file; // Object of fstream class
    file.open(format_folder+temp+".txt", ios::out); // Open file "test.txt" in out(write) mode
    if (!file) { // If file is not created, return error
        std::cout << "---Error in file creation!" << format_folder+temp+".txt" << std::endl;
    } else {
        std::cout << "---File Creation successful." << format_folder+temp+".txt" << std::endl;
    }
    file.close();
    
}

// Close all the files

void CloseFormatFile(){
    std::cout << "Running CloseFormatFile()";
    std::fstream file;
    file.open(TextFormat("Templates/Formats/%s", (Templates.at(*TempPos)+".txt").c_str()), std::ios::out | std::ios::trunc); // Open file for writing and truncate if it exists

    if (file.is_open()) {
        for(int i=0; i<(int)FileFormat.size(); i++){
            file << FileFormat[i] << "\n";
        }
        file.close(); // Close the file after writing
    } else {
        std::cout << "-- Unable to open file FileFormat ";
    }
    SheetUiData.clear(); // clear the vector of objects
    FileFormat.clear(); // clear the vector of strings
    std::cout << "-- successful." << std::endl;
}

// Copy the ui objects from the vector SheetUiData to FileFormat

void CopyUiObjects(){
    if (SheetUiData.empty()) {
        std::cout << "---No UI objects to copy.\n";
        return;
    }
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

// static functions
static int GetTruePos(std::vector<int> &int_data){
    for(int i=1; i < (int)int_data.size(); i++){
        if(int_data[i] == 1){
            return i-1;
        }
    }
    return -1; // Default to the first option if none are selected
}