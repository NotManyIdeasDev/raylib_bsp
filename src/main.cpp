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

    int iterations = 1;
    int thickness = 1;
    float minAmplitude = 0.45f;
    float maxAmplitude = 0.55f;
    bool uiLocked = true;

    dungeonGenerator.GenerateSubparts(iterations, minAmplitude, maxAmplitude);

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

        dungeonGenerator.DrawSubparts(thickness);

        camera.EndWorldMode();
        camera.DrawWorld();
        DrawFPS(10, 10);

        //********************IMGUI-DRAW********************//

        BeginRLImGui();     

        ImGui::PushFont(loadedImGuiFonts[24]);
        ImGui::Begin("Dungeon Generator v0.1", (bool*)0, ImGuiWindowFlags_NoTitleBar);

        if (uiLocked)
        {
            ImGui::SetWindowSize({ 400, 685 });
            ImGui::SetWindowPos({ (float)window.width - 415, 15 });
        }

        ImGui::TextColored({ (float)240 / 255, (float)180 / 255, (float)36 / 255, (float)255 / 255 }, "BSP Dungeon Generator");
        ImGui::SameLine(0, 0);
        ImGui::TextColored({ (float)92 / 255, (float)80 / 255, (float)100 / 255, (float)255 / 255 }, " - v0.1");

        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[20]);
        ImGui::TextColored( { (float)90 / 255, (float)90 / 255,  (float)90 / 255, (float)255 / 255 }, "Debug Info");
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[18]);
        ImGui::TextColored({ (float)220/255, (float)220/255,  (float)220/255, (float)255/255 }, "Room count:");
        ImGui::SameLine(0, 2);
        ImGui::TextColored({ (float)150/255, (float)255/255,  (float)255/255, (float)255/255 }, "%d", dungeonGenerator.GetSubpartsCount());
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[20]);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
        ImGui::TextColored({ (float)90 / 255, (float)90 / 255,  (float)90 / 255, (float)255 / 255 }, "Procgen Settings");
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[18]);
        ImGui::SliderInt("Iterations", &iterations, 1, 8);

        if (minAmplitude > maxAmplitude)
        {
            maxAmplitude = minAmplitude + 0.01f;
        }

        ImGui::SliderFloat("Min. Amplitude", &minAmplitude, 0.10f, 0.89f);
        ImGui::SliderFloat("Max. Amplitude", &maxAmplitude, 0.11f, 0.90f);
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[20]);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
        ImGui::TextColored({ (float)90 / 255, (float)90 / 255,  (float)90 / 255, (float)255 / 255 }, "Debug Settings");
        ImGui::PopFont();

        ImGui::PushFont(loadedImGuiFonts[18]);
        ImGui::SliderInt("Line Thickness", &thickness, 1, 8);
        ImGui::Checkbox("Lock UI", &uiLocked);
        ImGui::PopFont();

        

        ImGui::PushFont(loadedImGuiFonts[22]);
        ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 55);
        if (ImGui::Button("Generate", { 120, 40 }))
        {
            dungeonGenerator.GenerateSubparts(iterations, minAmplitude, maxAmplitude);
        }
        ImGui::PopFont();
        ImGui::End();


        ImGui::PushFont(loadedImGuiFonts[24]);
        ImGui::Begin("Camera Controls", (bool*)0, ImGuiWindowFlags_NoTitleBar);

        if (uiLocked)
        {
            ImGui::SetWindowSize({ 400, 350 });
            ImGui::SetWindowPos({ (float)window.width - 415, (float)window.height - 365 });
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