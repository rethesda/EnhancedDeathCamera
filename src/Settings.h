#pragma once

class Camera
{
public:
	enum class TYPE : std::uint32_t
	{
		kRagdoll = 0,
		kDeath = 1
	};

	enum class CAM : std::uint32_t
	{
		kThird,
		kUFO
	};

	enum class TPS : std::uint32_t
	{
		kFreeRotation,
		kAnimatorCam,
		kLocked
	};

	explicit Camera(TYPE a_type, std::string_view a_section, CAM a_camType, TPS a_tpsType) :
		type(a_type),
		typeStr(a_section),
		enableCam(a_section, "bEnable", true),
		camType(a_section, "iCameraType", std::to_underlying(a_camType)),
		hideUI(a_section, "bHideUI", true),
		thirdPersonStateType(a_section, "iThirdPersonRotationType", std::to_underlying(a_tpsType)),
		timeMult(a_section, "fTimeSpeedMult", 0.8f),
		timeMultPC(a_section, "fTimeSpeedPlayerMult", 0.8f)
	{}

	virtual ~Camera() = default;

	[[nodiscard]] CAM GetCamType() const { return static_cast<CAM>(camType.GetValue()); }
	[[nodiscard]] TPS GetTPSType() const { return static_cast<TPS>(thirdPersonStateType.GetValue()); }

	// members
	TYPE             type;
	std::string_view typeStr;

	REX::TIniSetting<bool>          enableCam;
	REX::TIniSetting<std::uint32_t> camType;  // 0 - third, 1 - fly
	REX::TIniSetting<bool>          hideUI;
	REX::TIniSetting<std::uint32_t> thirdPersonStateType;  // 0 - free rotation, 1 - animator cam, 2 - locked
	REX::TIniSetting<float>         timeMult;
	REX::TIniSetting<float>         timeMultPC;

	bool improvedCamCompability{ false };
};

class RagdollCamera final : public Camera
{
public:
	explicit RagdollCamera(CAM a_camType, TPS a_tpsType) :
		Camera(TYPE::kRagdoll, "Ragdoll Camera"sv, a_camType, a_tpsType)
	{}
};

class DeathCamera final : public Camera
{
public:
	explicit DeathCamera(CAM a_camType, TPS a_tpsType) :
		Camera(TYPE::kDeath, "Death Camera"sv, a_camType, a_tpsType),
		moveCamToKiller("Death Camera"sv, "bSnapCameraToKiller", false),
		setWhenDead("Death Camera"sv, "bSnapCameraWhenDead", true),
		camDuration("Death Camera"sv, "iCameraDuration", 5)
	{}

	// members
	REX::TIniSetting<bool>          moveCamToKiller;
	REX::TIniSetting<bool>          setWhenDead;
	REX::TIniSetting<std::uint32_t> camDuration;
};

class Settings : public REX::TSingleton<Settings>
{
public:
	void Load();

	DeathCamera*   GetDeathCamera() { return &deathCam; }
	RagdollCamera* GetRagdollCamera() { return &ragdollCam; }

	[[nodiscard]] RE::ACTOR_LIFE_STATE GetDeadState() const { return deadState; }
	[[nodiscard]] bool                 GetUseImprovedCam() const { return improvedCamInstalled; }
	[[nodiscard]] bool                 UseAltThirdPersonCam() const { return altTPSMode; }

private:
	void CheckImprovedCamera();
	void CheckSmoothCam();

	static constexpr auto path = R"(Data\SKSE\Plugins\po3_EnhancedDeathCamera.ini)"sv;

	// members
	DeathCamera          deathCam{ Camera::CAM::kUFO, Camera::TPS::kAnimatorCam };
	RagdollCamera        ragdollCam{ Camera::CAM::kThird, Camera::TPS::kLocked };
	RE::ACTOR_LIFE_STATE deadState{ RE::ACTOR_LIFE_STATE::kDead };
	bool                 altTPSMode{ false };
	bool                 improvedCamInstalled{ false };
};
