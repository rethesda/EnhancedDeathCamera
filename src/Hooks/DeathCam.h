#pragma once

#include "Hooks.h"
#include "Settings.h"

namespace Hooks::DEATH
{
	template <std::size_t N>
	struct StartBleedoutMode
	{
		static void thunk(RE::PlayerCamera* a_camera)
		{
			if (!detail::SetCamera(a_camera, Settings::GetSingleton()->GetDeathCamera())) {
				return func(a_camera);
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};
	
	void Install();
}
