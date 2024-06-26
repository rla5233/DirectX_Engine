#pragma once
#include <EngineBase/Transform.h>

#include <vector>
#include "TickObject.h"
#include "WorldObject.h"
#include "Level.h"


// 설명 : 액터가 트랜스폼을 가지는게 아니라
class ULevel;
class UActorComponent;
class USceneComponent;
class AActor : public UTickObject, public UWorldObject
{
	friend ULevel;
	GENERATED_BODY(UTickObject)

public:
	// constrcuter destructer
	AActor();
	~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	// 오로지 생성자에서만 호출될수 있게 만들어 놓는다.
	// 언리얼 따라할려고 그냥 함.
	template<typename ComponentType>
	ComponentType* CreateDefaultSubObject(std::string_view _Name)
	{
		if (false == ULevel::IsActorConstructer)
		{
			MsgBoxAssert("언리얼에서는 생성자에서밖에 컴포넌트를 생성할수 없습니다.");
		}
		
		std::shared_ptr<UActorComponent> NewComponent = std::make_shared<ComponentType>();

		PushComponent(NewComponent, _Name);

		return dynamic_cast<ComponentType*>(NewComponent.get());
	}

	FTransform& GetActorTransform();

	FVector GetActorLocation();
	FVector GetActorForwardVector();
	FVector GetActorRightVector();
	FVector GetActorUpVector();


	void SetActorLocation(FVector _Value);
	void SetActorScale3D(FVector _Value);

	void AddActorLocation(FVector _Value);
	void AddActorScale3D(FVector _Value);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	// 언리얼이 왜 액터가 트랜스폼을 가지지 못하게 했을까?
	// GameMode 기능적인 면에서 트랜스폼이 필요 없는 애들도 많아요.
	// 액터란 이 어떠한 레벨에서 어떠한 역할을 해주시면 하면 된다.
	// 거기에 필요한 기능만 만들면 되기 때문에 
	// 컴포넌트 구조라는 것.
	// 뭘만들든 동적으로 만들수 있는 구조를 만드는것.
	USceneComponent* RootComponent = nullptr;
	std::vector<std::shared_ptr<UActorComponent>> Components;

	void PushComponent(std::shared_ptr<UActorComponent> _Component, std::string_view _Name);
	void RootCheck();

};

