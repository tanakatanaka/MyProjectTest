#include "Roguelike/RogueEnemy.h"
#include "Roguelike/RogueGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ARogueEnemy::ARogueEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(36.f, 48.f);
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded()) Body->SetStaticMesh(CubeAsset.Object);
	Body->SetRelativeLocation(FVector(0, 0, -4));
	Body->SetRelativeRotation(FRotator(0, 45, 0));
	Body->SetRelativeScale3D(FVector(.58f, .58f, .9f));

	Glow = CreateDefaultSubobject<UPointLightComponent>(TEXT("Glow"));
	Glow->SetupAttachment(Body);
	Glow->SetRelativeLocation(FVector(0, 0, 55));
	Glow->SetIntensity(1100.f);
	Glow->SetAttenuationRadius(260.f);
	Glow->SetLightColor(FLinearColor(1.f, .08f, .02f));
	Tags.Add(TEXT("RogueEnemy"));
}

void ARogueEnemy::Configure(int32 FloorNumber, bool bEliteEnemy)
{
	bElite = bEliteEnemy;
	Health = (45.f + FloorNumber * 10.f) * (bElite ? 2.4f : 1.f);
	MoveSpeed = FMath::Min(390.f, 215.f + FloorNumber * 10.f + (bElite ? 35.f : 0.f));
	ContactDamage = 8.f + FloorNumber * 2.f + (bElite ? 9.f : 0.f);
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	Body->SetRelativeScale3D(bElite ? FVector(.9f, .9f, 1.3f) : FVector(.58f, .58f, .9f));
	Glow->SetLightColor(bElite ? FLinearColor(1.f, .55f, .02f) : FLinearColor(1.f, .08f, .02f));
}

void ARogueEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AttackCooldown = FMath::Max(0.f, AttackCooldown - DeltaSeconds);
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return;
	FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
	ToPlayer.Z = 0.f;
	const float Distance = ToPlayer.Size();
	if (Distance > 105.f)
	{
		AddMovementInput(ToPlayer.GetSafeNormal(), 1.f);
		SetActorRotation(ToPlayer.Rotation());
	}
	else if (AttackCooldown <= 0.f)
	{
		UGameplayStatics::ApplyDamage(Player, ContactDamage, nullptr, this, UDamageType::StaticClass());
		AttackCooldown = bElite ? .7f : 1.05f;
	}
}

float ARogueEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Actual = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Health -= Damage;
	Glow->SetIntensity(2600.f);
	FTimerHandle FlashTimer;
	GetWorldTimerManager().SetTimer(FlashTimer, [this]() { if (IsValid(this)) Glow->SetIntensity(1100.f); }, .08f, false);
	if (Health <= 0.f)
	{
		if (ARogueGameMode* GM = GetWorld()->GetAuthGameMode<ARogueGameMode>()) GM->EnemyDefeated(bElite);
		Destroy();
	}
	return Actual > 0.f ? Actual : Damage;
}
