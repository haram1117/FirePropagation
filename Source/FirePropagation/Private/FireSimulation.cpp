// Fill out your copyright notice in the Description page of Project Settings.


#include "FireSimulation.h"
#include "VertexColorSpreadMesh.h"
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

void UFireSimulation::SpawnSmoke(FVector fireLocation, VertexStruct* VertexStruct)
{
	// UE_LOG(LogTemp, Log, TEXT("FireComponents 개수: %d"), FireComponents.Num());
	// UE_LOG(LogTemp, Log, TEXT("normal: %s"), *normal.ToString())
	if(FireComponents.Num() == 0)
	{
		int num1 = FireComponents.Num() + 1;
		UFireComponent* Component = NewObject<UFireComponent>(this, UFireComponent::StaticClass(), FName(TEXT("Fire%d"), num1));
		Component->RegisterComponent();
		Component->SetFireSimulation(this);

		Component->SetWorldLocation(fireLocation);
		// Component->SetWorldRotation(normal.Rotation());
		Component->position = fireLocation;
		Component->id = num1;
		VertexStruct->setFireID(num1);
		// UE_LOG(LogTemp, Log, TEXT("%s"), *Component->GetComponentLocation().ToString());
		// Component->VertexIds.Add(VertexID);
		Component->AddVertexToFireComponent(FVertexID(VertexStruct->GetVertexID()), fireLocation);
		
		Component->EmitterInstances[0]->bEnabled = false;
		Component->EmitterInstances[1]->bEnabled = false;
		Component->EmitterInstances[3]->bEnabled = false;
		Component->EmitterInstances[4]->bEnabled = false;
		Component->EmitterInstances[5]->bEnabled = false;
		FireComponents.Add(num1, Component);
	}
	else
	{
		// VertexStruct* vertexStruct = vertexColorSpreadMesh->startedVerticesMap.Find(VertexID.GetValue());
		if(VertexStruct)
		{
			int id = VertexStruct->getFireID();
			if(id != 0)
			{
				// UFireComponent* fireComponent = *FireComponents.Find(VertexStruct->getFireID());
				UFireComponent* fireComponent = FireComponents.Find(id) ? *FireComponents.Find(id) : nullptr;
				if(fireComponent)
					fireComponent->AddVertexToFireComponent(FVertexID(VertexStruct->GetVertexID()), fireLocation);
				return;
			}

			for (auto elem : FireComponents)
			{
				if(elem.Value->CheckInBoundary(FVertexID(VertexStruct->GetVertexID()), fireLocation))
				{
					elem.Value->AddVertexToFireComponent(FVertexID(VertexStruct->GetVertexID()), fireLocation);
					VertexStruct->setFireID(elem.Value->id);
					return;
				}
			}
		}
		//새로 정의
		int num1 = FireComponents.Num() + 1;
		UFireComponent* Component = NewObject<UFireComponent>(this, UFireComponent::StaticClass(), FName(TEXT("Fire%d"), num1));
		Component->RegisterComponent();
		Component->SetFireSimulation(this);
		Component->SetWorldLocation(fireLocation);
		
		// UE_LOG(LogTemp, Log, TEXT("rotation: %s"), *normal.Rotation().ToString());
		// Component->SetWorldRotation(normal.Rotation());
		Component->position = fireLocation;
		Component->id = num1;
		VertexStruct->setFireID(num1);
		// Component->VertexIds.Add(VertexID);
		Component->AddVertexToFireComponent(FVertexID(VertexStruct->GetVertexID()), fireLocation);
		
		Component->EmitterInstances[0]->bEnabled = false;
		Component->EmitterInstances[1]->bEnabled = false;
		Component->EmitterInstances[3]->bEnabled = false;
		Component->EmitterInstances[4]->bEnabled = false;
		Component->EmitterInstances[5]->bEnabled = false;
		FireComponents.Add(num1, Component);
	}
}

void UFireSimulation::FindNearestFire(FVector fireLocation, VertexStruct* vertexStruct)
{
	// VertexStruct* vertexStruct = vertexColorSpreadMesh->startedVerticesMap.Find(VertexStruct..GetValue());
	if(vertexStruct)
	{
		int id = vertexStruct->getFireID();
		UE_LOG(LogTemp, Log, TEXT("id: %d"), id);
		UFireComponent* fireComponent = FireComponents.Find(id) ? *FireComponents.Find(id) : nullptr;
		if(fireComponent)
			fireComponent->FireStart();
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

