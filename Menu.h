//
// Created by Rev on 29/05/2021.
// Adapted for Oxide Survival Island
//

#ifndef MENU_H
#define MENU_H

#include "Drawing.h"
#include "Players.h"

namespace Menu
{
    int width;
    int height;

    void DrawMenu()
    {
        void *displays = Display_get_main();

        if (displays != nullptr)
        {
            width = Display_get_width(displays);
            height = Display_get_height(displays);
            Screen_SetResolution(width, height, true);

            ImGui::SetNextWindowSize(ImVec2(width * 0.9, height * 0.8));
            ImGui::Begin(OBFUSCATE("Oxide Survival Island - ESP By Rev"));

            // Получаем актуальный список игроков из PlayerManager (clientPlayerList)[cite: 4]
            monoList<void **> *playerList = offsets->PlayerManager.GetStaticField<monoList<void **> *>(OBFUSCATE_BNM("clientPlayerList"));

            if (isESP && playerList != nullptr)
            {
                for (int i = 0; i < playerList->getSize(); i++)
                {
                    void *player = playerList->getItems()[i];

                    if (player != nullptr)
                    {
                        // Получаем модель игрока / компонент через офсет персонажа
                        void *playerModelInfo = *(void **)((uint64_t)player + 0x150); // Поле characterModel[cite: 4]

                        if (playerModelInfo != nullptr)
                        {
                            // Используем найденные офсеты головы (0x20) и тела (0x40)[cite: 3]
                            void *headTransform = *(void **)((uint64_t)playerModelInfo + 0x20); 
                            void *bodyTransform = *(void **)((uint64_t)playerModelInfo + 0x40); 

                            if (headTransform != nullptr && bodyTransform != nullptr)
                            {
                                Vector3 headWorldPos = getPosition(headTransform);
                                Vector3 bodyWorldPos = getPosition(bodyTransform);

                                Vector3 headScreen = WorldToScreenPoint(get_main(), headWorldPos);
                                Vector3 bodyScreen = WorldToScreenPoint(get_main(), bodyWorldPos);

                                if (headScreen.Z > 0.0f)
                                {
                                    float heightFactor = abs(headScreen.Y - bodyScreen.Y) * 2.0f;
                                    float boxWidth = heightFactor / 2.0f;
                                    float boxHeight = heightFactor;

                                    Vector2 screenCoord = Vector2(headScreen.X, height - headScreen.Y);
                                    Rect playerRect(screenCoord.X - (boxWidth / 2.0f), screenCoord.Y, boxWidth, boxHeight);

                                    // 1. Линии до игроков
                                    if (isESPLine)
                                    {
                                        Drawing::DrawLine(ImVec2(width * 0.5f, height), ImVec2(screenCoord.X, screenCoord.Y + boxHeight), ImVec4(1, 0, 0, 1));
                                    }

                                    // 2. 2D Коробки (Боксы)
                                    if (isESPBox)
                                    {
                                        Drawing::DrawBox(playerRect, ImVec4(1, 1, 1, 1));
                                    }

                                    // 3. Дистанция над головой
                                    if (isESPDistance)
                                    {
                                        void *camTransform = get_transform(get_main());
                                        Vector3 camPos = getPosition(camTransform);
                                        float distance = Vector3::Distance(camPos, headWorldPos);

                                        char buffer[32];
                                        sprintf(buffer, "[%.0fm]", distance);
                                        Drawing::DrawText2(14.0f, ImVec2(screenCoord.X, screenCoord.Y - 15.0f), ImVec4(1, 1, 0, 1), buffer);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Интерфейс меню читов
            if (ImGui::BeginTabBar(OBFUSCATE("")))
            {
                if (ImGui::BeginTabItem(OBFUSCATE("ESP Settings")))
                {
                    ImGui::Checkbox(OBFUSCATE("Enable ESP"), &isESP);

                    if (isESP)
                    {
                        ImGui::Indent(25.0f);
                        ImGui::Checkbox(OBFUSCATE("Lines"), &isESPLine);
                        ImGui::Checkbox(OBFUSCATE("2D Boxes"), &isESPBox);
                        ImGui::Checkbox(OBFUSCATE("Distance"), &isESPDistance);
                        ImGui::Unindent(25.0f);
                    }

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }
    }
}

#endif