#include "PreCompile.h"
#include "Player.h"
#include <EngineCore/Renderer.h>

APlayer::APlayer()
{
	Renderer = CreateDefaultSubObject<URenderer>("Renderer");

	Renderer->SetMesh("Rect");
	Renderer->SetMaterial("2DImage");
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

}
