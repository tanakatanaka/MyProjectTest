#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueEnemy.generated.h"

UCLASS()
class MYPROJECTTEST_API ARogueEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ARogueEnemy();
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void Configure(int32 FloorNumber, bool bEliteEnemy);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Body;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPointLightComponent> Glow;

	float Health = 50.f;
	float MoveSpeed = 240.f;
	float ContactDamage = 12.f;
	float AttackCooldown = 0.f;
	bool bElite = false;
};
