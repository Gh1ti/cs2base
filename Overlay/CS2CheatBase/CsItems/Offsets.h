#pragma once
namespace offsets {
    constexpr std::ptrdiff_t dwEntityList = 0x17BB820;
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x180ACA0;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x16C2D98;
    constexpr std::ptrdiff_t dwViewMatrix = 0x1819FC0;
    constexpr std::ptrdiff_t m_hPlayerPawn = 0x7EC;
    constexpr std::ptrdiff_t m_iHealth = 0x32C;
    constexpr std::ptrdiff_t m_iTeamNum = 0x3BF;
    constexpr std::ptrdiff_t m_vOldOrigin = 0x1224;
    constexpr std::ptrdiff_t m_pGameSceneNode = 0x310;
    constexpr std::ptrdiff_t m_modelState = 0x160;
    constexpr std::ptrdiff_t m_boneArray = 0x80;
    constexpr std::ptrdiff_t m_vecOrigin = 0x80;
    constexpr std::ptrdiff_t m_bSpotted = 0x8; // bool
    constexpr std::ptrdiff_t m_bSpottedByMask = 0xC; // uint32_t[2]
    constexpr std::ptrdiff_t m_entitySpottedState = 0x1630;
    constexpr std::ptrdiff_t m_vecLastClipCameraPos = 0x128C; // Vector
    constexpr std::ptrdiff_t m_bIsScoped = 0x13A0;
    constexpr std::ptrdiff_t m_bOldIsScoped = 0x14E4; // bool
    constexpr std::ptrdiff_t m_sSanitizedPlayerName = 0x750;
    constexpr std::ptrdiff_t m_pEntity = 0x10; // CEntityIdentity*
    constexpr std::ptrdiff_t m_designerName = 0x20; // CUtlSymbolLarge
    constexpr std::ptrdiff_t m_pClippingWeapon = 0x12A8;
    constexpr std::ptrdiff_t m_iConnected = 0x63C;
    constexpr std::ptrdiff_t dwNetworkGameClient_maxClients = 0x250;
}