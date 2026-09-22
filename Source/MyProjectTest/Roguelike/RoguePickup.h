#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoguePickup.generated.h"

UENUM()
enum class ERoguePickupType : uint8 { Heal, Power, Exit };

UCLASS()
class MYPROJECTTEST_API ARoguePickup : public AActor
{
	GENERATED_BODY()
public:
	ARoguePickup();
	virtual void Tick(float DeltaSeconds) override;
	void Configure(ERoguePickupType NewType, bool bAvailable = true);
	void SetAvailable(bool bAvailable);
protected:
	UPROPERTY(VisibleAnywhere) TObjectPtr<class USphereComponent> Trigger;
	UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> Visual;
	UPROPERTY(VisibleAnywhere) TObjectPtr<class UPointLightComponent> Glow;
	ERoguePickupType Type = ERoguePickupType::Heal;
	bool bIsAvailable = true;
	UFUNCTION() void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
