#include "DeathCam.h"

namespace Hooks::DEATH
{
	struct UpdateWhenAIControlledOrDead
	{
		static void thunk(RE::PlayerCharacter* a_player, float a_delta)
		{
			if (a_player->GetLifeState() == Settings::GetSingleton()->GetDeadState()) {
				if (const auto killer = a_player->GetKiller(); killer && !killer->IsDead()) {
					if (const auto camera = RE::PlayerCamera::GetSingleton(); camera->cameraTarget != a_player->myKiller) {
						if (const auto tps = detail::GetThirdPersonState(camera)) {
							camera->cameraTarget = a_player->myKiller;
							tps->toggleAnimCam = true;
						} else {
							camera->ForceThirdPerson();
						}
					}
				}
			}

			func(a_player, a_delta);
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(39375, 40447), OFFSET(0xDC5, 0x1477) };  //PlayerCharacter::Update
			stl::write_thunk_call<UpdateWhenAIControlledOrDead>(target.address());
		}
	};

	namespace BleedoutMode
	{
		void Install()
		{
			REL::Relocation<std::uintptr_t> target_0{ RELOCATION_ID(36872, 37896), OFFSET(0x107E, 0x1122) };
			stl::write_thunk_call<StartBleedoutMode<0>>(target_0.address());


			REL::Relocation<std::uintptr_t> target_1{ RELOCATION_ID(36604, 37612), OFFSET(0x47F, 0x408) };
			stl::write_thunk_call<StartBleedoutMode<1>>(target_1.address());
		}
	}

	void Install()
	{
		BleedoutMode::Install();

		const auto settings = Settings::GetSingleton();
		if (!settings->UseAltThirdPersonCam() && settings->GetDeathCamera()->moveCamToKiller) {
			UpdateWhenAIControlledOrDead::Install();
		}
	}
}
