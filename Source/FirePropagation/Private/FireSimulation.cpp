// Fill out your copyright notice in the Description page of Project Settings.


#include "FireSimulation.h"
#include "FireComponent.h"
#include "ParticleEmitterInstances.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFireSimulation::UFireSimulation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UFireSimulation::SpawnSmoke(FVector fireLocation, FVertexID VertexID)
{
	UE_LOG(LogTemp, Log, TEXT("FireComponents 개수: %d"), FireComponents.Num());
	if(FireComponents.Num() == 0)
	{
		int num1 = FireComponents.Num() + 1;
		UFireComponent* Component = NewObject<UFireComponent>(this, UFireComponent::StaticClass(), FName(TEXT("Fire%d"), num1));
		Component->RegisterComponent();
		Component->SetFireSimulation(this);
		Component->SetRelativeLocation(fireLocation);
		UE_LOG(LogTemp, Log, TEXT("%s"), *Component->GetComponentLocation().ToString());
		// Component->VertexIds.Add(VertexID);
		Component->AddVertexToFireComponent(VertexID, fireLocation);
		
		Component->EmitterInstances[0]->bEnabled = false;
		Component->EmitterInstances[1]->bEnabled = false;
		Component->EmitterInstances[3]->bEnabled = false;
		Component->EmitterInstances[4]->bEnabled = false;
		Component->EmitterInstances[5]->bEnabled = false;
		FireComponents.Add(Component);
	}
	else
	{
		for (auto __fireComponent : FireComponents)
		{
			if(__fireComponent->CheckInBoundary(VertexID, fireLocation))
			{
				// 추가만
				
				__fireComponent->AddVertexToFireComponent(VertexID, fireLocation);
				return;
			}
		}
		
		//새로 정의
		int num1 = FireComponents.Num() + 1;
		UFireComponent* Component = NewObject<UFireComponent>(this, UFireComponent::StaticClass(), FName(TEXT("Fire%d"), num1));
		Component->RegisterComponent();
		Component->SetFireSimulation(this);
		Component->SetRelativeLocation(fireLocation);
		// Component->VertexIds.Add(VertexID);
		Component->AddVertexToFireComponent(VertexID, fireLocation);
		
		Component->EmitterInstances[0]->bEnabled = false;
		Component->EmitterInstances[1]->bEnabled = false;
		Component->EmitterInstances[3]->bEnabled = false;
		Component->EmitterInstances[4]->bEnabled = false;
		Component->EmitterInstances[5]->bEnabled = false;
		FireComponents.Add(Component);
	}
}

void UFireSimulation::FindNearestFire(FVector fireLocation, FVertexID VertexID)
{
	for (auto __fireComponent : FireComponents)
	{
		if(__fireComponent->VertexIds.Contains(VertexID))
		{
			// 추가만
			// __fireComponent->AddVertexToFireComponent(VertexID, fireLocation);
			__fireComponent->FireStart();
			// __fireComponent->AddVertexToFireComponent(VertexID, fireLocation);
			return;
		}
	}
}


// Called when the game starts
void UFireSimulation::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFireSimulation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

