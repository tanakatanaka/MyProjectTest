#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Roguelike/RoguePickup.h"
#include "RogueGameMode.generated.h"

UCLASS()
class MYPROJECTTEST_API ARogueGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ARogueGameMode();
	virtual void BeginPlay() override;
	void EnemyDefeated(bool bElite);
	void CollectPickup(ERoguePickupType Type);
	void TryAdvanceFloor();
	float ScaleProjectileDamage(float BaseDamage) const { return BaseDamage * DamageMultiplier; }
	int32 GetFloor() const { return Floor; }
	int32 GetEnemiesRemaining() const { return EnemiesRemaining; }
	float GetHealth() const { return PlayerHealth; }
	float GetDamageMultiplier() const { return DamageMultiplier; }
protected:
	int32 Floor = 1;
	int32 EnemiesRemaining = 0;
	float PlayerHealth = 100.f;
	float DamageMultiplier = 1.f;
	TArray<TWeakObjectPtr<AActor>> SpawnedActors;
	TWeakObjectPtr<ARoguePickup> Exit;
	void BuildFloor();
	void ClearFloor();
	void BindPlayer();
	void SpawnBlock(const FVector& Location, const FVector& Scale, bool bPillar = false);
	UFUNCTION() void OnPlayerDamaged(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser);
};
