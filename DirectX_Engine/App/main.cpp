#include <Windows.h>

#include <EnginePlatform/EngineWindow.h>

#pragma comment (lib, "EngineBase.lib")
#pragma comment (lib, "EnginePlatform.lib")

class EngineCore
{
public:
	void Update()
	{

	}

	void End()
	{

	}
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) 
{
	UEngineWindow NewWindow;
	NewWindow.Open();

	EngineCore Core;
	UEngineWindow::WindowMessageLoop(
		std::bind(&EngineCore::Update, &Core),
		std::bind(&EngineCore::End, &Core)
	);
}