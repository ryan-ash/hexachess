#pragma once

#include <CoreMinimal.h>

#include "PointOfInterest.generated.h"


UENUM(BlueprintType)
enum class EPointOfInterestType : uint8
{
    Location UMETA(DisplayName = "Location"),
    Spline UMETA(DisplayName = "Spline"),
    FirstPerson UMETA(DisplayName = "First Person"),
    Spectator UMETA(DisplayName = "Spectator")
};


UENUM(BlueprintType)
enum class EPointOfInterestTransitionType : uint8
{
    PingPong,
    LerpToStart,
    FadeToStart,
    ChainToNext
};


USTRUCT(BlueprintType)
struct ONLOOKER_API FPointOfInterest
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker")
    EPointOfInterestType Type = EPointOfInterestType::Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker|Rotation", meta = (EditCondition = "Type == EPointOfInterestType::Location", EditConditionHides))
    bool RotationEnabled = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker|Rotation", meta = (EditCondition = "RotationEnabled", EditConditionHides))
    float TimeFor360 = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker|Spline", meta = (EditCondition = "Type == EPointOfInterestType::Spline", EditConditionHides))
    float WaitAtEnd = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker|Spline", meta = (EditCondition = "Type == EPointOfInterestType::Spline", EditConditionHides))
    EPointOfInterestTransitionType TransitionType = EPointOfInterestTransitionType::PingPong;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker|Spline", meta = (EditCondition = "(Type == EPointOfInterestType::Spline && TransitionType != EPointOfInterestTransitionType::ChainToNext) || Type == EPointOfInterestType::FirstPerson || Type == EPointOfInterestType::Spectator", EditConditionHides))
    float TransitionTime = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker|Spline", meta = (EditCondition = "Type == EPointOfInterestType::Spline && TransitionType != EPointOfInterestTransitionType::ChainToNext && TransitionType != EPointOfInterestTransitionType::FadeToStart", EditConditionHides))
    UCurveFloat* TransitionCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker|Spline", meta = (EditCondition = "Type == EPointOfInterestType::Spline && TransitionType != EPointOfInterestTransitionType::ChainToNext", EditConditionHides))
    float WaitAfterTransition = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Onlooker|Spline", meta = (EditCondition = "Type == EPointOfInterestType::Spline && TransitionType == EPointOfInterestTransitionType::ChainToNext", EditConditionHides))
    class ACameraOperator* NextPointOfInterest;
};
