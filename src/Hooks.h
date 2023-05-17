#pragma once

namespace Hooks
{
	inline static REL::Relocation<std::uintptr_t> GetStaminaDamageHook(REL::RelocationID(25864, 26430));  //1.5,1.6
	inline static REL::Relocation<std::uintptr_t> GetStaminaBashHook(REL::RelocationID(25863,26429));    //1.5,1.6

	inline static REL::Relocation<std::uintptr_t> OnFrame_Update_Hook{ REL::RelocationID(35565,36564), REL::Relocate(0x1E,0x6E) };
	inline static REL::Relocation<std::uintptr_t> Scale_Patch_Hook{ REL::RelocationID(37013,38041),  REL::Relocate(0x1A,0x1F) };

	inline static REL::Relocation<std::uintptr_t> Block_GameSetting_Hook{ REL::RelocationID(42842,44014), REL::Relocate(0x452,0x438) };
	inline static REL::Relocation<std::uintptr_t> fBlock_GameSetting{ REL::RelocationID(505023,374158), 0x8 };

	inline static REL::Relocation<std::uintptr_t> SpellCap_Hook{ REL::RelocationID(37792,38741), REL::Relocate(0x53,0x55) };

	static REL::Relocation<std::uintptr_t> armorRating1(REL::RelocationID(42842, 44014));
	static REL::Relocation<std::uintptr_t> armorRating2(REL::RelocationID(37605, 38558)); 

	bool InstallHooks();
	bool InstallBashMultHook();
}
