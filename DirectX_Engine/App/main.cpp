#include <Windows.h>

#include <EnginePlatform/EngineWindow.h>

// �������� lib �߰� ���� ���
#pragma comment (lib, "EngineBase.lib")
#pragma comment (lib, "EnginePlatform.lib")

class UEngineCore
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

	UEngineCore Core;
	UEngineWindow::WindowMessageLoop(
		std::bind(&UEngineCore::Update, &Core),
		std::bind(&UEngineCore::End, &Core)
	);
}