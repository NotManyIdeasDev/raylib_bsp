/*
* Code by NotManyIdeasDev (discord: NotManyIdeas#9972, github: https://github.com/NotManyIdeasDev)
* Copyright (c) 2021.
* You are free to use this code, as long as the right credits are given.
*/

#include "raylib.h"
#include "imgui.h"
#include <string>
#include <iostream>

#include "window.h"
#include "pixel_camera.h"
#include "graphics/sprite.h"
#include "core/random_gen.h"
#include "core/bsp_dungeon_gen.h"

int main()
{
    //********************SETUP********************//
    Window window = Window(1920, 1080, "raylib - pixel art test", true);
    Sprite::SetBaseFilePath("res/sprites/");

    const int virtualScreenWidth = 1920;
    const int virtualScreenHeight = 1080;
    const float virtualRatio = (float)window.width / (float)virtualScreenWidth;

    PixelCamera camera(window.width, window.height, virtualRatio);

    Rectangle dungeonArea = { virtualScreenWidth / 2 - 550, virtualScreenHeight / 2 - 400, 800, 800 };
    BSPDungeonGenerator dungeonGenerator(dungeonArea);

    int totalGeneration = 1;
    int iterations = 3;
    int maxRooms = 8;

    int minRoomX = 60;
    int minRoomY = 60;

    float minAmplitude = 0.45f;
    float maxAmplitude = 0.55f;

    float minPercentage = 0.3f;
    float maxPercentage = 0.7f;

    bool preferBiggerRooms = true;

    int thickness = 1;
    Color subpartsColor = GREEN;

    bool uiLocked = true;
    bool showBSPsubparts = true;

    bool showAdvancedInfo = false;

    dungeonGenerator.GenerateSubparts(iterations, minAmplitude, maxAmplitude);
    dungeonGenerator.GenerateRooms(minPercentage, maxPercentage);
    dungeonGenerator.CutRooms(maxRooms, preferBiggerRooms, minRoomX, minRoomY);

    while (!WindowShouldClose())
    {
        //********************UPDATE********************//

        if (IsKeyDown(KEY_A))
        {
            camera.position.x--;
        }

        if (IsKeyDown(KEY_S))
        {
            camera.position.y++;
        }

        if (IsKeyDown(KEY_D))
        {
            camera.position.x++;
        }

        if (IsKeyDown(KEY_W))
        {
            camera.position.y--;
        }

        camera.Update();

        //********************DRAW********************//

        BeginDrawing();
        ClearBackground({0xFF, 0x00, 0xFF, 0xFF});
        camera.BeginWorldMode();

        if (showBSPsubparts)
        {
            dungeonGenerator.DrawSubparts(thickness, subpartsColor);
        }

        dungeonGenerator.DrawRooms();

        camera.EndWorldMode();
        camera.DrawWorld();
        DrawFPS(10, 10);

        //********************IMGUI-DRAW********************//

        BeginRLImGui();     

        ImGui::PushFont(loadedImGuiFonts[24]);
        ImGui::Begin("Dungeon Generator v0.1", (bool*)0, ImGuiWindowFlags_NoTitleBar);

        if (uiLocked)
        {
            ImGui::SetWindowSize({ 500, 685 });
            ImGui::SetWindowPos({ (float)window.width - 515, 15 });
        }

        ImGui::TextColored({ (float)240 / 255, (float)180 / 255, (float)36 / 255, (float)255 / 255 }, "BSP Dungeon Generator");
        ImGui::SameLine(0, 0);
        ImGui::TextColored({ (float)92 / 255, (float)80 / 255, (float)100 / 255, (float)255 / 255 }, " - v0.3");

        ImGui::PopFont();
        
        ImGui::PushFont(loadedImGuiFonts[20]);
        ImGui::TextColored( { (float)92 / 255, (float)80 / 255,  (float)100 / 255, (float)255 / 255 }, "Debug Info");
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[18]);

        ImGui::TextColored({ (float)220 / 255, (float)220 / 255,  (float)220 / 255, (float)255 / 255 }, "Dungeon #");
        ImGui::SameLine(0, 0);
        ImGui::TextColored({ (float)150 / 255, (float)255 / 255,  (float)255 / 255, (float)255 / 255 }, "%d", totalGeneration);

        ImGui::TextColored({ (float)220/255, (float)220/255,  (float)220/255, (float)255/255 }, "Subparts: ");
        ImGui::SameLine(0, 2);
        ImGui::TextColored({ (float)150/255, (float)255/255,  (float)255/255, (float)255/255 }, "%d", dungeonGenerator.GetSubpartsCount());

        ImGui::TextColored({ (float)220 / 255, (float)220 / 255,  (float)220 / 255, (float)255 / 255 }, "Rooms: ");
        ImGui::SameLine(0, 2);
        ImGui::TextColored({ (float)150 / 255, (float)255 / 255,  (float)255 / 255, (float)255 / 255 }, "%d", dungeonGenerator.GetRoomsCount());

        ImGui::TextColored({ (float)220 / 255, (float)220 / 255,  (float)220 / 255, (float)255 / 255 }, "Discarded Rooms: ");
        ImGui::SameLine(0, 2);
        ImGui::TextColored({ (float)150 / 255, (float)255 / 255,  (float)255 / 255, (float)255 / 255 }, "%d", dungeonGenerator.GetDiscardedRoomsCount());

        if (ImGui::Button("Advanced Info", { 120, 30 }))
        {
            showAdvancedInfo = true;
        }

        if (showAdvancedInfo)
        {
            if (!ImGui::Begin("Advanced Debug Info", &showAdvancedInfo))
            {
                ImGui::End();
            }
            else
            {
                ImGui::PushFont(loadedImGuiFonts[22]);
                ImGui::TextColored({ (float)240 / 255, (float)180 / 255, (float)36 / 255, (float)255 / 255 }, "Rooms");
                ImGui::PopFont();

                for (int i = 0; i < dungeonGenerator.GetRoomsCount(); i++)
                {
                    ImGui::Text("Room [%d]", i);
                    ImGui::SameLine(0);
                    ImGui::TextColored({ (float)255 / 255, (float)40 / 255, (float)60 / 255, (float)255 / 255 }, "w: %.1f", dungeonGenerator.GetRooms()[i].width);
                    ImGui::SameLine(0);
                    ImGui::TextColored({ (float)40 / 255, (float)255 / 255, (float)60 / 255, (float)255 / 255 }, "h: %.1f", dungeonGenerator.GetRooms()[i].height);
                }

                ImGui::End();
            }
        }

        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[20]);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
        ImGui::TextColored({ (float)92 / 255, (float)80 / 255,  (float)100 / 255, (float)255 / 255 }, "Procgen Settings");
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[18]);
        if (ImGui::SliderInt("Iterations", &iterations, 1, 8))
        {
            maxRooms = pow(2, iterations);
        }

        ImGui::SliderInt("Max Rooms", &maxRooms, 1, pow(2, iterations));

        if (minAmplitude >= maxAmplitude)
        {
            maxAmplitude = minAmplitude + 0.01f;
        }

        if (minPercentage >= maxPercentage)
        {
            maxPercentage = minPercentage + 0.01f;
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        ImGui::SliderInt("Min. X Size", &minRoomX, 0, 200);
        ImGui::SliderInt("Min. Y Size", &minRoomY, 0, 200);

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        ImGui::SliderFloat("Min. Amplitude", &minAmplitude, 0.10f, 0.89f);
        ImGui::SliderFloat("Max. Amplitude", &maxAmplitude, 0.11f, 0.90f);

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        ImGui::SliderFloat("Min. Room Multiplier", &minPercentage, 0.10f, 0.70f);
        ImGui::SliderFloat("Max. Room Multiplier", &maxPercentage, 0.15f, 0.75f);

        ImGui::Checkbox("Prefer bigger rooms", &preferBiggerRooms);
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[20]);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
        ImGui::TextColored({ (float)92 / 255, (float)80 / 255,  (float)100 / 255, (float)255 / 255 }, "Debug Settings");
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[18]);
        ImGui::SliderInt("Line Thickness", &thickness, 1, 8);

        ImGui::Checkbox("Show BSP Subparts", &showBSPsubparts);

        if (showBSPsubparts)
        {
            int min = 0;
            int max = 255;
            ImGui::DragScalarN("Color", ImGuiDataType_U8, &subpartsColor.r, 4, 1.0f, &min, &max);
            ImGui::SameLine(0);
            ImGui::ColorButton("Edit Color", { (float)subpartsColor.r / 255, (float)subpartsColor.g / 255, (float)subpartsColor.b / 255, (float)subpartsColor.a / 255 });
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

        ImGui::Checkbox("Lock UI", &uiLocked);
        ImGui::PopFont();

        

        ImGui::PushFont(loadedImGuiFonts[22]);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
        if (ImGui::Button("Generate", { 120, 40 }))
        {
            dungeonGenerator.GenerateSubparts(iterations, minAmplitude, maxAmplitude);
            dungeonGenerator.GenerateRooms(minPercentage, maxPercentage);
            dungeonGenerator.CutRooms(maxRooms, preferBiggerRooms, minRoomX, minRoomY);
            totalGeneration++;
        }

        ImGui::SameLine(0);

        if (ImGui::Button("Reset", { 120, 40 }))
        {
            iterations = 3;
            minAmplitude = 0.45f;
            maxAmplitude = 0.55f;
            thickness = 1;
            subpartsColor = GREEN;
            uiLocked = true;
        }

        ImGui::PopFont();
        ImGui::End();


        ImGui::PushFont(loadedImGuiFonts[24]);
        ImGui::Begin("Camera Controls", (bool*)0, ImGuiWindowFlags_NoTitleBar);

        if (uiLocked)
        {
            ImGui::SetWindowSize({ 500, 350 });
            ImGui::SetWindowPos({ (float)window.width - 515, (float)window.height - 365 });
        }

        ImGui::TextColored({ (float)68 / 255, (float)209 / 255, (float)219 / 255, (float)255 / 255 }, "Camera Controls");
        ImGui::SameLine(0, 0);
        ImGui::TextColored({ (float)92 / 255, (float)80 / 255, (float)100 / 255, (float)255 / 255 }, " - Raylib");
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[18]);
        ImGui::DragFloat2("Position", &camera.position.x, 0.7f, -2000.0f, 2000.0f);
        ImGui::DragFloat("Rotation", &camera.rotation, 0.8f, -360.0f, 360.0f);
        ImGui::DragFloat("Zoom", &camera.zoom, 0.001f, 0.5f, 2.0f);
        ImGui::PopFont();


        ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 55);
        ImGui::PushFont(loadedImGuiFonts[22]);
        if (ImGui::Button("Reset", { 120, 40 }))
        {
            camera.position = { 0, 0 };
            camera.rotation = 0.0f;
            camera.zoom = 1.0f;
        }
        ImGui::PopFont();

        ImGui::End();
        //ImGui::ShowDemoWindow();
        EndRLImGui();
        
        EndDrawing();
    }

    window.Close();
    ShutdownRLImGui();

    return 0;
}