#include "Actors/CameraOperator.h"


ACameraOperator::ACameraOperator()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(Root);
	Camera->SetupAttachment(SpringArm);

	RootComponent = Root;
}

void ACameraOperator::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraOperator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
