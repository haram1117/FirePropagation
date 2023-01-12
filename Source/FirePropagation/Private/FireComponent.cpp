// Fill out your copyright notice in the Description page of Project Settings.


#include "FireComponent.h"

#include "ParticleEmitterInstances.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleLODLevel.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/Orbit/ParticleModuleOrbit.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UFireComponent::UFireComponent()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
	if(ParticleAsset.Succeeded())
	{
		SetTemplate(ParticleAsset.Object);
		ParticleSystem = ParticleAsset.Object;	
	}
}


void UFireComponent::AddVertexToFireComponent(FVertexID VertexID, FVector newLocation)
{
	VertexIds.Add(VertexID);
	int num = VertexIds.Num(); 
	if(num != 1)
	{
		centerLocation = centerLocation * (num / (num + 1)) + newLocation / (num + 1);
		// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, newLocation);
	}
	else
	{
		// centerLocation = newLocation;
	}
}

bool UFireComponent::CheckInBoundary(FVertexID VertexID, FVector newLocation)
{
	float distance = FVector::Distance(centerLocation, newLocation);
	if(distance <= FireSimulation->boundary)
	{
		return true;
	}
	
	return false;
}

void UFireComponent::SetFireSimulation(UFireSimulation* __fireSim)
{
	FireSimulation = __fireSim;
}

void UFireComponent::SmokeStart()
{
	// fire 추가
	ToggleActive();
}

void UFireComponent::FireStart()
{
	EmitterInstances[0]->bEnabled = true;
	EmitterInstances[1]->bEnabled = true;
	EmitterInstances[3]->bEnabled = true;
	EmitterInstances[4]->bEnabled = true;
	EmitterInstances[5]->bEnabled = true;
}


// Called every frame
void UFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

