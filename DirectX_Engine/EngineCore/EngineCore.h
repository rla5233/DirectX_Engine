#pragma once

#include <map>
#include <string>
#include <string_view>

#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/EngineTime.h>

#include "EngineGraphicDevice.h"
#include "EngineOption.h"

#pragma comment (lib, "EngineBase.lib")
#pragma comment (lib, "EnginePlatform.lib")

class UserCore
{
public:
	virtual void Initialize() = 0;
};

// 설명 :
class AActor;
class ULevel;
class UEngineCore
{
public:
	// constrcuter destructer
	UEngineCore();
	~UEngineCore();

	// delete Function
	UEngineCore(const UEngineCore& _Other) = delete;
	UEngineCore(UEngineCore&& _Other) noexcept = delete;
	UEngineCore& operator=(const UEngineCore& _Other) = delete;
	UEngineCore& operator=(UEngineCore&& _Other) noexcept = delete;

	template<typename UserCoreType>
	static void Start(HINSTANCE _Inst)
	{
		UEngineCore Core;
		UserCoreType UserCore;
		Core.UserCorePtr = &UserCore;
		Core.EngineStart(_Inst);
	}

	void SetWindowScale(FVector _Scale)
	{
		EngineWindow.SetWindowScale(_Scale);
	}

	FVector GetWindowScale()
	{
		return EngineWindow.GetWindowScale();
	}


	UEngineGraphicDevice& GetEngineDevice()
	{
		return EngineDevice;
	}

	struct ID3D11Device* GetDirectXDevice()
	{
		return EngineDevice.GetDevice();
	}

	struct ID3D11DeviceContext* GetDirectXContext()
	{
		return EngineDevice.GetContext();
	}
	
	void ChangeLevel(std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);

		if (true != Levels.contains(UpperName))
		{
			MsgBoxAssert("존재하지 않는 레벨을 지정하려고 했습니다.");
			return;
		}
		NextLevel = Levels[UpperName];
	}

	template<typename GameModeType>
	void CreateLevel(std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);

		if (true == Levels.contains(UpperName))
		{
			MsgBoxAssert("이미 존재하는 레벨을 또 만들려고 했습니다.");
			return;
		}

		std::shared_ptr<GameModeType> NewGameMode = std::make_shared<GameModeType>();

		NewGameMode->SetOrder(INT_MIN);

		std::shared_ptr<ULevel> Level = NewLevelCreate(UpperName, NewGameMode);
	}

protected:

private:
	FEngineOption EngineOption;

	UEngineTime MainTimer;

	UEngineWindow EngineWindow;

	UEngineGraphicDevice EngineDevice;

	UserCore* UserCorePtr = nullptr;

	std::map <std::string, std::shared_ptr<ULevel>> Levels;
	std::shared_ptr<ULevel> NextLevel = nullptr;
	std::shared_ptr<ULevel> CurLevel = nullptr;

	void EngineOptionInit();

	void EngineStart(HINSTANCE _Inst);

	void EngineFrameUpdate();
	void EngineEnd();

	std::shared_ptr<ULevel> NewLevelCreate(std::string& _Name, std::shared_ptr<AActor> _GameMode);
};

extern UEngineCore* GEngine;