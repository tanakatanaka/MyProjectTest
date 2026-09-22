#include "Roguelike/RogueHUD.h"
#include "Roguelike/RogueGameMode.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

void ARogueHUD::DrawHUD()
{
	Super::DrawHUD();
	const ARogueGameMode* GM = GetWorld() ? GetWorld()->GetAuthGameMode<ARogueGameMode>() : nullptr;
	if (!GM || !Canvas) return;
	const float S = Canvas->SizeX / 1920.f;
	DrawRect(FLinearColor(.015f, .02f, .035f, .82f), 28*S, 28*S, 430*S, 142*S);
	DrawText(FString::Printf(TEXT("ABYSS // FLOOR %02d"), GM->GetFloor()), FLinearColor(.95f, .68f, .12f), 50*S, 45*S, GEngine->GetLargeFont(), 1.1f*S);
	DrawText(FString::Printf(TEXT("HP  %3d / 100"), FMath::CeilToInt(GM->GetHealth())), FLinearColor(.92f, .95f, 1.f), 50*S, 92*S, GEngine->GetMediumFont(), 1.f*S);
	DrawRect(FLinearColor(.12f, .13f, .18f, 1.f), 188*S, 101*S, 230*S, 15*S);
	DrawRect(FLinearColor(.1f, .85f, .38f, 1.f), 188*S, 101*S, 230*S * GM->GetHealth()/100.f, 15*S);
	DrawText(FString::Printf(TEXT("HOSTILES %02d     POWER x%.1f"), GM->GetEnemiesRemaining(), GM->GetDamageMultiplier()), FLinearColor(.55f, .72f, .9f), 50*S, 132*S, GEngine->GetSmallFont(), 1.1f*S);
	DrawText(TEXT("WASD  MOVE     MOUSE  AIM     LMB  FIRE"), FLinearColor(.65f, .68f, .75f), 50*S, Canvas->SizeY - 58*S, GEngine->GetSmallFont(), 1.05f*S);
	if (GM->GetEnemiesRemaining() == 0)
		DrawText(TEXT("EXIT UNSEALED"), FLinearColor(1.f, .58f, .08f), Canvas->SizeX*.5f - 105*S, 48*S, GEngine->GetLargeFont(), 1.15f*S);
}
