#include "Settings.h"

#include "SimpleINI.h"
#undef ERROR

void Settings::Load()
{
	const auto store = REX::FIniSettingStore::GetSingleton();
	store->Init(path.data(), "");

	store->Load();
	store->Save();

	deadState = deathCam.setWhenDead ? RE::ACTOR_LIFE_STATE::kDead : RE::ACTOR_LIFE_STATE::kDying;

	CheckImprovedCamera();
	CheckSmoothCam();
}

void Settings::CheckImprovedCamera()
{
	improvedCamInstalled = REX::W32::GetModuleHandleA("ImprovedCameraSE") != nullptr;

	if (!improvedCamInstalled) {
		REX::ERROR("Improved Camera SE is not installed");
		return;
	}

	CSimpleIniA improvedCameraSE;
	improvedCameraSE.SetUnicode();

	if (improvedCameraSE.LoadFile(R"(Data\SKSE\Plugins\ImprovedCameraSE\ImprovedCameraSE.ini)") < 0) {
		REX::ERROR("couldn't read Improved Camera SE config");
		return;
	}

	const auto profileName = improvedCameraSE.GetValue("MODULE DATA", "ProfileName", "");
	if (!profileName || !*profileName) {
		return;
	}

	const auto profilePath = std::format(R"(Data\SKSE\Plugins\ImprovedCameraSE\Profiles\{})", profileName);

	CSimpleIniA profile;
	profile.SetUnicode();

	if (profile.LoadFile(profilePath.c_str()) < 0) {
		REX::ERROR("couldn't read Improved Camera profile");
		return;
	}

	deathCam.improvedCamCompability = profile.GetBoolValue("EVENTS", "bDeath", false);
	ragdollCam.improvedCamCompability = profile.GetBoolValue("EVENTS", "bRagdoll", false);

	REX::INFO("Improved Camera - EventDeath {}", deathCam.improvedCamCompability ? "enabled" : "disabled");
	REX::INFO("Improved Camera - EventRagdoll {}", ragdollCam.improvedCamCompability ? "enabled" : "disabled");
}

void Settings::CheckSmoothCam()
{
	altTPSMode = REX::W32::GetModuleHandleA("SmoothCam") != nullptr;
	REX::INFO("Smooth Camera {}", altTPSMode ? "found" : "not found");
}
