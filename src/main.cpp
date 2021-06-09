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
#include "core/room.h"
#include "core/random_gen.h"
#include "core/bsp_dungeon_gen.h"
#include "core/core_utils.h"

int main()
{
    //********************SETUP********************//
    Window window = Window(1920, 1080, "C++ BSP Dungeon Generator - v0.3.3", true);
    Sprite::SetBaseFilePath("res/sprites/");

    const int virtualScreenWidth = 1920;
    const int virtualScreenHeight = 1080;
    const float virtualRatio = (float)window.width / (float)virtualScreenWidth;

    PixelCamera camera(window.width, window.height, virtualRatio);

    Rectangle dungeonArea = { virtualScreenWidth / 2 - 550, virtualScreenHeight / 2 - 400, 800, 800 };
    BSPDungeonGenerator dungeonGenerator(dungeonArea);

    int totalGeneration = 1;

    Vector4Int startDims = { dungeonArea.x, dungeonArea.y, dungeonArea.width, dungeonArea.height };

    int iterations = 3;
    int maxRooms = 8;

    int unitSize = 16;

    int minRoomX = 60;
    int minRoomY = 60;

    float minAmplitude = 0.45f;
    float maxAmplitude = 0.55f;

    float minPercentage = 0.3f;
    float maxPercentage = 0.7f;

    bool preferBiggerRooms = true;

    int thickness = 2;
    Color subpartsColor = GREEN;

    bool uiLocked = true;
    bool showBSPsubparts = true;

    bool showAdvancedInfo = false;

    dungeonGenerator.GenerateSubparts(iterations, minAmplitude, maxAmplitude);
    dungeonGenerator.GenerateRooms(minPercentage, maxPercentage, unitSize);
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
        ImGui::Begin("Dungeon Generator v0.3.1", (bool*)0, ImGuiWindowFlags_NoTitleBar);

        if (uiLocked)
        {
            ImGui::SetWindowSize({ 500, 685 });
            ImGui::SetWindowPos({ (float)window.width - 515, 15 });
        }

        ImGui::TextColored(CoreUtils::GetRainbowColor() , "BSP Dungeon Generator");
        ImGui::SameLine(0, 0);
        ImGui::TextColored(CoreUtils::HexToRGB("#5c5064"), " - v0.3.3");

        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[20]);
        ImGui::TextColored(CoreUtils::HexToRGB("#5c5064"), "Procgen Settings");
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[18]);

        ImGui::InputInt4("Dungeon dimensions", &startDims.x);

        if (ImGui::SliderInt("Iterations", &iterations, 1, 7))
        {
            maxRooms = pow(2, iterations);
        }

        ImGui::SliderInt("Max Rooms", &maxRooms, 1, pow(2, iterations));

        if (maxRooms < pow(2, iterations))
        {
            ImGui::Checkbox("Prefer bigger rooms", &preferBiggerRooms);
        }

        if (minAmplitude >= maxAmplitude)
        {
            maxAmplitude = minAmplitude + 0.01f;
        }

        if (minPercentage >= maxPercentage)
        {
            maxPercentage = minPercentage + 0.01f;
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        ImGui::SliderInt("Unit Size", &unitSize, 0, 128);

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        ImGui::SliderInt("Min. X Size", &minRoomX, 0, 200);
        ImGui::SliderInt("Min. Y Size", &minRoomY, 0, 200);

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        ImGui::SliderFloat("Min. Amplitude", &minAmplitude, 0.10f, 0.89f);
        ImGui::SliderFloat("Max. Amplitude", &maxAmplitude, 0.11f, 0.90f);

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        ImGui::SliderFloat("Min. Room Multiplier", &minPercentage, 0.10f, 0.70f);
        ImGui::SliderFloat("Max. Room Multiplier", &maxPercentage, 0.15f, 0.75f);

        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[20]);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
        ImGui::TextColored(CoreUtils::HexToRGB("#5c5064"), "Debug Settings");
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[18]);


        ImGui::Checkbox("Show BSP Subparts", &showBSPsubparts);

        if (showBSPsubparts)
        {
            ImGui::SliderInt("Line Thickness", &thickness, 1, 8);

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

        ImGui::PushStyleColor(ImGuiCol_Text, CoreUtils::GetRainbowColor());
        if (ImGui::Button("Generate", { 120, 40 }))
        {
            dungeonGenerator.SetStartArea(startDims);
            dungeonGenerator.GenerateSubparts(iterations, minAmplitude, maxAmplitude);
            dungeonGenerator.GenerateRooms(minPercentage, maxPercentage, unitSize);
            dungeonGenerator.CutRooms(maxRooms, preferBiggerRooms, minRoomX, minRoomY);
            dungeonGenerator.ConnectRooms();
            totalGeneration++;
        }
        ImGui::PopStyleColor();

        ImGui::SameLine(0);

        if (ImGui::Button("Reset", { 120, 40 }))
        {
            iterations = 3;
            minAmplitude = 0.45f;
            maxAmplitude = 0.55f;
            thickness = 1;
            subpartsColor = GREEN;
            uiLocked = true;
            preferBiggerRooms = true;
            minRoomX = 60;
            minRoomY = 60;
            unitSize = 8;
            startDims = { 410, 100, 800, 800 };
            minPercentage = 0.3f;
            maxPercentage = 0.7f;
            maxRooms = 8;
        }

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

        ImGui::PushFont(loadedImGuiFonts[20]);
        ImGui::TextColored(CoreUtils::HexToRGB("#5c5064"), "Debug Info");
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[18]);

        ImGui::TextColored(CoreUtils::HexToRGB("#dcdcdc"), "Dungeon #");
        ImGui::SameLine(0, 0);
        ImGui::TextColored(CoreUtils::HexToRGB("#96ffff"), "%d", totalGeneration);

        ImGui::TextColored(CoreUtils::HexToRGB("#dcdcdc"), "Subparts: ");
        ImGui::SameLine(0, 2);
        ImGui::TextColored(CoreUtils::HexToRGB("#96ffff"), "%d", dungeonGenerator.GetSubpartsCount());

        ImGui::TextColored(CoreUtils::HexToRGB("#dcdcdc"), "Rooms: ");
        ImGui::SameLine(0, 2);
        ImGui::TextColored(CoreUtils::HexToRGB("#96ffff"), "%d", dungeonGenerator.GetRoomsCount());

        ImGui::TextColored(CoreUtils::HexToRGB("#dcdcdc"), "Discarded Rooms: ");
        ImGui::SameLine(0, 2);
        ImGui::TextColored(CoreUtils::HexToRGB("#96ffff"), "%d", dungeonGenerator.GetDiscardedRoomsCount());

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

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
                ImGui::TextColored(CoreUtils::HexToRGB("#f0b424"), "Rooms");
                ImGui::PopFont();

                for (int i = 0; i < dungeonGenerator.GetRoomsCount(); i++)
                {
                    ImGui::Text("Room [%d]", i);
                    ImGui::SameLine(0);
                    ImGui::TextColored(CoreUtils::HexToRGB("#ff283c"), "w: %.1f", dungeonGenerator.GetRooms()[i].transform.width);
                    ImGui::SameLine(0);
                    ImGui::TextColored(CoreUtils::HexToRGB("#28ff3c"), "h: %.1f", dungeonGenerator.GetRooms()[i].transform.height);
                }

                ImGui::End();
            }
        }

        if (dungeonGenerator.GetRoomsCount() == 0)
        {
            ImGui::Begin("Error");
            ImGui::SetWindowSize({460, 150});

            ImGui::PushFont(loadedImGuiFonts[30]);
            ImGui::TextColored(CoreUtils::HexToRGB("#db0f3f"), "Error!");
            ImGui::PopFont();

            ImGui::PushFont(loadedImGuiFonts[22]);
            ImGui::TextColored(CoreUtils::HexToRGB("#ffe6eb"), "No rooms were generated, try different settings.");
            ImGui::PopFont();

            ImGui::End();
        }

        ImGui::PopFont();


        ImGui::PopFont();
        ImGui::End();


        ImGui::PushFont(loadedImGuiFonts[24]);
        ImGui::Begin("Camera Controls", (bool*)0, ImGuiWindowFlags_NoTitleBar);

        if (uiLocked)
        {
            ImGui::SetWindowSize({ 500, 350 });
            ImGui::SetWindowPos({ (float)window.width - 515, (float)window.height - 365 });
        }

        ImGui::TextColored(CoreUtils::HexToRGB("#44d1db"), "Camera Controls");
        ImGui::SameLine(0, 0);
        ImGui::TextColored(CoreUtils::HexToRGB("#5c5064"), " - Raylib");
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