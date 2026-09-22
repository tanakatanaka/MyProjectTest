#include "Roguelike/RoguePickup.h"
#include "Roguelike/RogueGameMode.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"

ARoguePickup::ARoguePickup()
{
	PrimaryActorTick.bCanEverTick = true;
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->InitSphereRadius(90.f);
	Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = Trigger;
	Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
	Visual->SetupAttachment(RootComponent);
	Visual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (Mesh.Succeeded()) Visual->SetStaticMesh(Mesh.Object);
	Visual->SetRelativeScale3D(FVector(.48f));
	Glow = CreateDefaultSubobject<UPointLightComponent>(TEXT("Glow"));
	Glow->SetupAttachment(Visual);
	Glow->SetIntensity(2600.f);
	Glow->SetAttenuationRadius(420.f);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ARoguePickup::OnOverlap);
}

void ARoguePickup::Configure(ERoguePickupType NewType, bool bAvailable)
{
	Type = NewType;
	if (Type == ERoguePickupType::Heal) Glow->SetLightColor(FLinearColor(.05f, 1.f, .25f));
	else if (Type == ERoguePickupType::Power) Glow->SetLightColor(FLinearColor(.05f, .45f, 1.f));
	else { Glow->SetLightColor(FLinearColor(1.f, .55f, .04f)); Visual->SetRelativeScale3D(FVector(1.2f)); }
	SetAvailable(bAvailable);
}

void ARoguePickup::SetAvailable(bool bAvailable)
{
	bIsAvailable = bAvailable;
	Glow->SetIntensity(bAvailable ? 3400.f : 180.f);
	Visual->SetVisibility(bAvailable);
}

void ARoguePickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Visual->AddLocalRotation(FRotator(20.f * DeltaSeconds, 75.f * DeltaSeconds, 12.f * DeltaSeconds));
}

void ARoguePickup::OnOverlap(UPrimitiveComponent*, AActor* Other, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	const APawn* OtherPawn = Cast<APawn>(Other);
	if (!bIsAvailable || !OtherPawn || !OtherPawn->IsPlayerControlled()) return;
	if (ARogueGameMode* GM = GetWorld()->GetAuthGameMode<ARogueGameMode>())
	{
		if (Type == ERoguePickupType::Exit) { GM->TryAdvanceFloor(); return; }
		GM->CollectPickup(Type);
		Destroy();
	}
}
