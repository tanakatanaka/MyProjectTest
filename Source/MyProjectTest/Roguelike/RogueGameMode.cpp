#include "Roguelike/RogueGameMode.h"
#include "Roguelike/RogueEnemy.h"
#include "Roguelike/RogueHUD.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

ARogueGameMode::ARogueGameMode()
{
	HUDClass = ARogueHUD::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PawnBP(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APlayerController> ControllerBP(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonPlayerController"));
	if (PawnBP.Succeeded()) DefaultPawnClass = PawnBP.Class;
	if (ControllerBP.Succeeded()) PlayerControllerClass = ControllerBP.Class;
}

void ARogueGameMode::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle StartTimer;
	GetWorldTimerManager().SetTimer(StartTimer, [this]() { BindPlayer(); BuildFloor(); }, .15f, false);
}

void ARogueGameMode::BindPlayer()
{
	if (APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		Player->Tags.AddUnique(TEXT("Player"));
		Player->OnTakeAnyDamage.AddDynamic(this, &ARogueGameMode::OnPlayerDamaged);
	}
}

void ARogueGameMode::ClearFloor()
{
	for (TWeakObjectPtr<AActor> Actor : SpawnedActors) if (Actor.IsValid()) Actor->Destroy();
	SpawnedActors.Empty();
	Exit.Reset();
}

void ARogueGameMode::SpawnBlock(const FVector& Location, const FVector& Scale, bool bPillar)
{
	AStaticMeshActor* Block = GetWorld()->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator);
	if (!Block) return;
	UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	Block->GetStaticMeshComponent()->SetStaticMesh(Cube);
	Block->SetActorScale3D(Scale);
	Block->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	Block->Tags.Add(TEXT("GeneratedDungeon"));
	SpawnedActors.Add(Block);
}

void ARogueGameMode::BuildFloor()
{
	ClearFloor();
	FRandomStream RNG(Floor * 7919 + FDateTime::Now().GetMillisecond());
	// The template map already provides the lighting and a ground reference at Z=0.
	// Build the generated dungeon around that reference so every spawned pawn lands
	// on the procedural floor instead of falling to the template level below it.
	const FVector Center(0.f, 0.f, 0.f);
	const float Half = 1900.f;
	SpawnBlock(Center + FVector(0,0,-105), FVector(40,40,1));
	SpawnBlock(Center + FVector(Half,0,190), FVector(1,40,5));
	SpawnBlock(Center + FVector(-Half,0,190), FVector(1,40,5));
	SpawnBlock(Center + FVector(0,Half,190), FVector(40,1,5));
	SpawnBlock(Center + FVector(0,-Half,190), FVector(40,1,5));
	for (int32 i=0; i<7; ++i)
	{
		const FVector P = Center + FVector(RNG.FRandRange(-1250,1250), RNG.FRandRange(-1250,1250), 130.f);
		SpawnBlock(P, FVector(RNG.FRandRange(1.1f,2.8f), RNG.FRandRange(.5f,1.3f), RNG.FRandRange(1.8f,3.8f)), true);
	}
	if (APawn* Player = UGameplayStatics::GetPlayerPawn(this,0))
	{
		Player->SetActorLocation(Center + FVector(-1450,-1450,80));
		if (AController* C = Player->GetController()) C->SetControlRotation(FRotator(-12,45,0));
	}
	EnemiesRemaining = FMath::Clamp(4 + Floor * 2, 6, 22);
	for (int32 i=0; i<EnemiesRemaining; ++i)
	{
		FVector P = Center + FVector(RNG.FRandRange(-1300,1300), RNG.FRandRange(-1300,1300), 70.f);
		if (FVector::Dist2D(P, Center + FVector(-1450,-1450,0)) < 650.f) P = Center + FVector(800,800,70);
		ARogueEnemy* Enemy = GetWorld()->SpawnActor<ARogueEnemy>(P, FRotator::ZeroRotator);
		if (Enemy) { Enemy->Configure(Floor, i == EnemiesRemaining-1 && Floor % 3 == 0); SpawnedActors.Add(Enemy); }
	}
	ARoguePickup* Heal = GetWorld()->SpawnActor<ARoguePickup>(Center + FVector(RNG.FRandRange(-900,900), RNG.FRandRange(-900,900), 100), FRotator::ZeroRotator);
	if (Heal) { Heal->Configure(ERoguePickupType::Heal); SpawnedActors.Add(Heal); }
	ARoguePickup* Gate = GetWorld()->SpawnActor<ARoguePickup>(Center + FVector(1450,1450,120), FRotator::ZeroRotator);
	if (Gate) { Gate->Configure(ERoguePickupType::Exit, false); Exit = Gate; SpawnedActors.Add(Gate); }
}

void ARogueGameMode::EnemyDefeated(bool bElite)
{
	EnemiesRemaining = FMath::Max(0, EnemiesRemaining - 1);
	if (bElite || FMath::FRand() < .18f)
	{
		if (ARoguePickup* Power = GetWorld()->SpawnActor<ARoguePickup>(UGameplayStatics::GetPlayerPawn(this,0)->GetActorLocation()+FVector(120,0,60), FRotator::ZeroRotator))
		{ Power->Configure(ERoguePickupType::Power); SpawnedActors.Add(Power); }
	}
	if (EnemiesRemaining == 0 && Exit.IsValid()) Exit->SetAvailable(true);
}

void ARogueGameMode::CollectPickup(ERoguePickupType Type)
{
	if (Type == ERoguePickupType::Heal) PlayerHealth = FMath::Min(100.f, PlayerHealth + 35.f);
	else if (Type == ERoguePickupType::Power) DamageMultiplier = FMath::Min(4.f, DamageMultiplier + .25f);
}

void ARogueGameMode::TryAdvanceFloor()
{
	if (EnemiesRemaining > 0) return;
	Floor++;
	PlayerHealth = FMath::Min(100.f, PlayerHealth + 20.f);
	BuildFloor();
}

void ARogueGameMode::OnPlayerDamaged(AActor*, float Damage, const UDamageType*, AController*, AActor*)
{
	PlayerHealth = FMath::Max(0.f, PlayerHealth - Damage);
	if (PlayerHealth <= 0.f) UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
