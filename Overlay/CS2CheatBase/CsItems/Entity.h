#pragma once
#include "utils.h"
#include <vector>


struct weapon {

	weapon(uintptr_t handle) {
		uintptr_t weapon_base = g_memory::mem.Read<uintptr_t>(handle + offsets::m_pClippingWeapon);
		uintptr_t p_entity = g_memory::mem.Read<uintptr_t>(weapon_base + offsets::m_pEntity);
		uintptr_t weapon_name = g_memory::mem.Read<uintptr_t>(p_entity + offsets::m_designerName);
		name = g_memory::mem.ReadString(weapon_name, 256);
	}

	std::string name;
};

struct Local_Player {

	Local_Player() {
		player = g_memory::mem.Read<uintptr_t>(dlls::client+offsets::dwLocalPlayerController);
		playerpawn = g_memory::mem.Read<uintptr_t>(player + offsets::m_hPlayerPawn);
		uintptr_t local_list_entry2 = g_memory::mem.Read<uintptr_t>(interfaces::entity_list + 0x8 * ((playerpawn & 0x7FFF) >> 9) + 16);
		pCSPlayerPawn = g_memory::mem.Read<uintptr_t>(local_list_entry2 + 120 * (playerpawn & 0x1FF));
		Pawn = g_memory::mem.Read<uintptr_t>(dlls::client + offsets::dwLocalPlayerPawn);
	}

	int Team() {
		return g_memory::mem.Read<int>(player + offsets::m_iTeamNum);
	}

	int Health() {
		return g_memory::mem.Read<int>(pCSPlayerPawn + offsets::m_iHealth);
	}

	Vector3 Origin() {
		return g_memory::mem.Read<Vector3>(pCSPlayerPawn + offsets::m_vOldOrigin);
	}

	Vector3 Cam_pos() {
		return g_memory::mem.Read<Vector3>(pCSPlayerPawn + offsets::m_vecLastClipCameraPos);
	}

	bool Is_Scoped() {
		return g_memory::mem.Read<bool>(pCSPlayerPawn + offsets::m_bIsScoped);
	}

	bool Old_Scoped() {
		return g_memory::mem.Read<bool>(pCSPlayerPawn + offsets::m_bOldIsScoped);
	}

	bool Is_valid() {
		return player != 0;
	}

	bool Is_Connected() {
		return g_memory::mem.Read<bool>(pCSPlayerPawn + offsets::m_iConnected);
	}

	std::string GetWeapon() {
		return weapon(pCSPlayerPawn).name;
	}

	std::string WeaponType() {
		std::string wpn = weapon(pCSPlayerPawn).name;
		if (wpn == "weapon_smokegrenade" || wpn == "weapon_hegrenade" || wpn == "weapon_flashbang" || wpn == "weapon_decoy" || wpn == "weapon_fire_grenade" || wpn == "weapon_firebomb") {
			return "grenade";
		}
		else if (wpn == "weapon_healthshot") {
			return "healthshot";
		}
		else if (wpn == "weapon_c4") {
			return "C4";
		}
		else if (wpn == "weapon_base") {
			return "base";
		}
		else if (wpn == "weapon_knife") {
			return "knife";
		}
		
		return "aimbot_enabled";
	}

	uintptr_t player, playerpawn, pCSPlayerPawn,Pawn;
};
struct CBase_Player {
	uintptr_t Player,hPawn,pCSPlayerPawn;
	CBase_Player(int i) {
		uintptr_t list_entry = g_memory::mem.Read<uintptr_t>(interfaces::entity_list + (8 * (i & 0x7FFF) >> 9) + 16);
		Player = g_memory::mem.Read<uintptr_t>(list_entry + 120 * (i & 0x1FF));
		hPawn = g_memory::mem.Read<uintptr_t>(Player + offsets::m_hPlayerPawn);
		uintptr_t list_entry2 = g_memory::mem.Read<uintptr_t>(interfaces::entity_list + (0x8 * (i & 0x7FFF) >> 9) + 16);
		pCSPlayerPawn = g_memory::mem.Read<uintptr_t>(list_entry2 + 120 * (hPawn & 0x1FF));
	}

	bool Is_valid() {
		return Player != 0 && hPawn != 0 && pCSPlayerPawn != 0;
	}

	int Team() {
		return g_memory::mem.Read<int>(Player + offsets::m_iTeamNum);
	}

	bool Is_lp() {
		return Player == Local_Player().player;
	}

	int Health() {
		return g_memory::mem.Read<int>(pCSPlayerPawn + offsets::m_iHealth);
	}

	bool Is_Enemy() {
		return Team() != Local_Player().Team();
	}

	Vector3 Origin() {
		return g_memory::mem.Read<Vector3>(pCSPlayerPawn + offsets::m_vOldOrigin);
	}

	Vector3 Bone_Origin(int index) {
		uintptr_t gamescene = g_memory::mem.Read<uintptr_t>(pCSPlayerPawn + offsets::m_pGameSceneNode);
		uintptr_t bonearray = g_memory::mem.Read<uintptr_t>(gamescene + offsets::m_modelState + offsets::m_boneArray);
		Vector3 position = g_memory::mem.Read<Vector3>(bonearray + index * 32);
		return position;
	}

	bool Visible() {
		uintptr_t state = g_memory::mem.Read<uintptr_t>(pCSPlayerPawn + offsets::m_entitySpottedState + offsets::m_bSpottedByMask);
		return state & (1 << Local_Player().player);	
	}

	Vector3 Cam_pos() {
		return g_memory::mem.Read<Vector3>(pCSPlayerPawn + offsets::m_vecLastClipCameraPos);
	}

	char* GetName() {
		char buff[256];
		uintptr_t name_addr = g_memory::mem.Read<uintptr_t>(Player + offsets::m_sSanitizedPlayerName);
		g_memory::mem.ReadRaw(name_addr, buff, sizeof(buff));
		return buff;
	}
};
