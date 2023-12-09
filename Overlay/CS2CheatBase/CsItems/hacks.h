#pragma once
#include "Entity.h"

namespace hacks {
	void DoEsp(ImFont* font, ImDrawList* g_render) {
		Local_Player lp = Local_Player();
		// (lp.Health() > 0) {
			for (int i = 1; i < 64; i++) {
				CBase_Player player = CBase_Player(i);
				if (!player.Player) continue;
				if (!player.Is_Enemy()) continue;
				if (!player.Health() > 0) continue;
				Vector3 origin = player.Origin();
				Vector3 head_origin = player.Bone_Origin(6);
				head_origin.z += 5;
				Vector3 OriginW2S = origin.W2S(interfaces::vm);
				Vector3 HeadW2S = head_origin.W2S(interfaces::vm);
				head_origin.z -= 5;
				float height = HeadW2S.y - OriginW2S.y;
				float width = height *0.5f;
				float offset = height * 0.55f;
				float num_off = height * 0.3f;
				if (OriginW2S.z < 0.01f) continue;
				if (Visuals::box) {
					ImU32 col = FloatColorToU32(Visuals::esp_col[0], Visuals::esp_col[1], Visuals::esp_col[2], 1);
					g_render->AddRect(ImVec2(OriginW2S.x - width / 2, OriginW2S.y), ImVec2(OriginW2S.x + width / 2, OriginW2S.y + height), col);
				}
				if(Visuals::head_dot){
					HeadW2S = head_origin.W2S(interfaces::vm);
					ImU32 hcol = FloatColorToU32(Visuals::head_col[0], Visuals::head_col[1], Visuals::head_col[2], 1);
					g_render->AddCircleFilled({ HeadW2S.x,HeadW2S.y }, 1.5f, hcol, 360);
				}
				if (Visuals::ppdot) {
					Vector3 ppw2s = player.Bone_Origin(0).W2S(interfaces::vm);
					ImU32 pcol = FloatColorToU32(Visuals::pp_col[0], Visuals::pp_col[1], Visuals::pp_col[2], 1);
					g_render->AddCircleFilled({ ppw2s.x,ppw2s.y }, 1.5f, pcol, 360);
				}
				if (Visuals::name) {
					ImU32 ncol = FloatColorToU32(Visuals::name_col[0], Visuals::name_col[1], Visuals::name_col[2], 1);
					char* name = player.GetName();
					ImVec2 size = font->CalcTextSizeA(12, FLT_MAX,0.0f, name);
					g_render->AddText(font, 12, ImVec2(OriginW2S.x - size.x / 2, OriginW2S.y + size.y/2), ncol, name);
					if (player.Visible()) {
						g_render->AddText(font, 12, ImVec2(OriginW2S.x - size.x / 2, OriginW2S.y + size.y / 2+15), ncol, "Visible");
					}
					else
					{
						g_render->AddText(font, 12, ImVec2(OriginW2S.x - size.x / 2, OriginW2S.y + size.y / 2+15), ncol, "Hidden");
					}
				}
				if (Visuals::hp_bar) {
					ImU32 bcol = FloatColorToU32(Visuals::bar_col[0], Visuals::bar_col[1], Visuals::bar_col[2], 1);
					float size = (width / 15);
					float hp = (float)player.Health();

					g_render->AddLine(ImVec2(OriginW2S.x + (offset / 2), OriginW2S.y), ImVec2(OriginW2S.x + (offset / 2), OriginW2S.y + height), ImColor(0, 0, 0, 255),2);

					g_render->AddLine(ImVec2(OriginW2S.x + (offset / 2), OriginW2S.y), ImVec2(OriginW2S.x + (offset / 2), OriginW2S.y + ((hp / 100) * height)), bcol, 2);
				}
				if (Visuals::hp_num) {
					ImU32 hcol = FloatColorToU32(Visuals::num_col[0], Visuals::num_col[1], Visuals::num_col[2], 1);
					g_render->AddText(font, 12, ImVec2(OriginW2S.x - num_off, OriginW2S.y+height), hcol, std::to_string(player.Health()).c_str());
				}
			}
		//}
	}

	void Aimbot(float fov, ImVec2 screen) {
		for (int i = 1; i < 64; i++) {
			ImDrawList* g_render = ImGui::GetBackgroundDrawList();
			view_matrix_t vm = interfaces::vm;
			CBase_Player player = CBase_Player(i);
			Local_Player Local_player = Local_Player();
			if (player.Health() <= 0) continue;
			if (player.Is_lp()) continue;
			if (!player.Is_Enemy()) continue;
			if (!player.Visible()) continue;
			if (Local_player.WeaponType() != "aimbot_enabled") continue;
			Vector3 Head = player.Bone_Origin(6);
			Vector3 Head_W2S = Head.W2S(vm);
			if (Aimbot::enabled) {
				if (IsInFov({ Head_W2S.x,Head_W2S.y },screen,fov ) && GetAsyncKeyState(VK_LMENU)) {
					float smoothness = Aimbot::smoothness / 5;
					mouse_event(MOUSEEVENTF_MOVE, (Head_W2S.x - screen.x), (Head_W2S.y - screen.y), 0, 0);
				}
			}
			
		}
	}

}

