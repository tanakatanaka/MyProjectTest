#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RogueHUD.generated.h"

UCLASS()
class MYPROJECTTEST_API ARogueHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;
};
