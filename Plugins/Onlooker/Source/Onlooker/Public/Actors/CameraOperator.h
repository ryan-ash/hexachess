#pragma once

#include <Camera/CameraComponent.h>
#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include <GameFramework/SpringArmComponent.h>

#include "Types/PointOfInterest.h"

#include "CameraOperator.generated.h"


UCLASS(Blueprintable, BlueprintType)
class ONLOOKER_API ACameraOperator : public AActor
{
	GENERATED_BODY()
	
public:	
	ACameraOperator();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Onlooker")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Onlooker")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Onlooker")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker")
	bool EnabledInMenu = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker")
	FPointOfInterest Settings;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
