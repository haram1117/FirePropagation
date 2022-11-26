// Fill out your copyright notice in the Description page of Project Settings.


#include "FireSimulationManager.h"

// Sets default values
AFireSimulationManager::AFireSimulationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFireSimulationManager::BeginPlay()
{
	Super::BeginPlay();
	FVector size = GetStaticMeshSize(BuildingMesh);
	
	UE_LOG(LogTemp, Log, TEXT("size: %s"), *size.ToString());
}

// Called every frame
void AFireSimulationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AFireSimulationManager::GetStaticMeshSize(UStaticMesh* Mesh)
{
	if (!Mesh)
		return FVector::ZeroVector;

	return Mesh->GetBounds().GetBox().GetSize();
}

void AFireSimulationManager::SpawnVolumes(FVector size)
{
	
	GetWorld()->SpawnActor(Volume);
}