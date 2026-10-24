//
// Created by Rev on 29/05/2021.
// Adapted for Oxide Survival Island - Offsets.h
//

#ifndef OFFSETS_H
#define OFFSETS_H

#include "BNM.hpp" // Или твой основной заголовок памяти

struct Offsets
{
    // --- Основные классы игры Oxide ---
    LoadClass PlayerManager = LoadClass(OBFUSCATE_BNM(""), OBFUSCATE_BNM("PlayerManager"));[cite: 4]
    LoadClass PlayerModelInfo = LoadClass(OBFUSCATE_BNM(""), OBFUSCATE_BNM("PlayerModelInfo"));[cite: 3]

    // --- Офсеты и смещения в памяти ---
    DWORD offset_head = 0x20;            // Смещение до Transform головы в PlayerModelInfo
    DWORD offset_body = 0x40;            // Смещение до Transform тела в PlayerModelInfo[cite: 3]
    DWORD offset_characterModel = 0x150; // Смещение до поля characterModel в PlayerManager
};

inline Offsets *offsets;

#endif