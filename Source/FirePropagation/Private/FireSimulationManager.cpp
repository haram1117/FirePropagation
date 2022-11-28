// Fill out your copyright notice in the Description page of Project Settings.


#include "FireSimulationManager.h"

#include "Components/ShapeComponent.h"

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
	
	FVector size;
	GetStaticMeshSize(BuildingMesh, size);
	
	if (VolumeAmount == EHeatTransferVolumeAmount::Custom)
		VolumeRatio = VolumeNumber;
	VolumeRatio = static_cast<uint8>(VolumeAmount);
	UE_LOG(LogTemp, Log, TEXT("Size: %s"), *size.ToString());
	SpawnVolumes(size);
}

void AFireSimulationManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called every frame
void AFireSimulationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireSimulationManager::GetStaticMeshSize(UStaticMesh* Mesh, FVector& Size)
{
	if (!Mesh)
		Size = FVector::ZeroVector;
	
	Size = Mesh->GetBounds().GetBox().GetSize();
}

void AFireSimulationManager::SpawnVolumes(FVector size)
{
	// front Wall Spawn Volumes
	const FVector Scale_XZ = FVector(size.X * VolumeRatio * 0.01f * 0.5f, 20.0f, size.Z * VolumeRatio * 0.01f * 0.5f);
	for(int i = 0; i < VolumeRatio; i++)
	{
		for(int j = 0; j < VolumeRatio; j++)
		{
			const FVector Location = FVector(size.X * VolumeRatio * 0.01f * j + size.X * VolumeRatio * 0.01f * 0.5f, 0.0f, size.Z * VolumeRatio * 0.01f * i + size.Z * VolumeRatio * 0.01f * 0.5f);
			AHeatTransferVolume* VolumeActor = Cast<AHeatTransferVolume>(GetWorld()->SpawnActor(Volume, &Location));
			VolumeActor->GetCollisionComponent()->SetWorldScale3D(Scale_XZ);
			VolumeActor->SetID(HeatTransferVolumes.Num());
			HeatTransferVolumes.Add(VolumeActor);
		}
	}
	
	// right Wall Spawn Volumes
	const FVector Scale_YZ = FVector(20.0f, size.Y * VolumeRatio * 0.01f * 0.5f, size.Z * VolumeRatio * 0.01f * 0.5f);
	for(int i = 0; i < VolumeRatio; i++)
	{
		for(int j = 0; j < VolumeRatio; j++)
		{
			const FVector Location = FVector(size.X,  - size.Y * VolumeRatio * 0.01f * j - size.Y * VolumeRatio * 0.01f * 0.5f, size.Z * VolumeRatio * 0.01f * i + size.Z * VolumeRatio * 0.01f * 0.5f);
			AHeatTransferVolume* VolumeActor = Cast<AHeatTransferVolume>(GetWorld()->SpawnActor(Volume, &Location));
			VolumeActor->GetCollisionComponent()->SetWorldScale3D(Scale_YZ);
			VolumeActor->SetID(HeatTransferVolumes.Num());
			HeatTransferVolumes.Add(VolumeActor);
		}
	}

	// back Wall Spawn Volumes
	// scale: Use Scale_XZ
	for(int i = 0; i < VolumeRatio; i++)
	{
		for(int j = 0; j < VolumeRatio; j++)
		{
			const FVector Location = FVector(size.X * VolumeRatio * 0.01f * j + size.X * VolumeRatio * 0.01f * 0.5f,  - size.Y, size.Z * VolumeRatio * 0.01f * i + size.Z * VolumeRatio * 0.01f * 0.5f);
			AHeatTransferVolume* VolumeActor = Cast<AHeatTransferVolume>(GetWorld()->SpawnActor(Volume, &Location));
			VolumeActor->GetCollisionComponent()->SetWorldScale3D(Scale_XZ);
			VolumeActor->SetID(HeatTransferVolumes.Num());
			HeatTransferVolumes.Add(VolumeActor);
		}
	}
	// left Wall Spawn Volumes
	// scale: Use Scale_YZ
	for(int i = 0; i < VolumeRatio; i++)
	{
		for(int j = 0; j < VolumeRatio; j++)
		{
			const FVector Location = FVector(0.0f,  - size.Y * VolumeRatio * 0.01f * j - size.Y * VolumeRatio * 0.01f * 0.5f, size.Z * VolumeRatio * 0.01f * i + size.Z * VolumeRatio * 0.01f * 0.5f);
			AHeatTransferVolume* VolumeActor = Cast<AHeatTransferVolume>(GetWorld()->SpawnActor(Volume, &Location));
			VolumeActor->GetCollisionComponent()->SetWorldScale3D(Scale_YZ);
			VolumeActor->SetID(HeatTransferVolumes.Num());
			HeatTransferVolumes.Add(VolumeActor);
		}
	}

	// roof Wall Spawn Volumes
	const FVector Scale_XY = FVector(size.X * VolumeRatio * 0.01f * 0.5f, size.Y * VolumeRatio * 0.01f * 0.5f, 20.0f);
	for(int i = 0; i < VolumeRatio; i++)
	{
		for(int j = 0; j < VolumeRatio; j++)
		{
			const FVector Location = FVector(size.X * VolumeRatio * 0.01f * j + size.X * VolumeRatio * 0.01f * 0.5f,  - size.Y * VolumeRatio * 0.01f * i - size.Y * VolumeRatio * 0.01f * 0.5f, size.Z);
			AHeatTransferVolume* VolumeActor = Cast<AHeatTransferVolume>(GetWorld()->SpawnActor(Volume, &Location));
			VolumeActor->GetCollisionComponent()->SetWorldScale3D(Scale_XY);
			VolumeActor->SetID(HeatTransferVolumes.Num());
			HeatTransferVolumes.Add(VolumeActor);
		}
	}
	
}

void AFireSimulationManager::SetNeighborVolumes()
{
	for(int i = 0; i < HeatTransferVolumes.Num(); i++)
	{
		
	}
	
	// if(j != 0) // 한 line의 가장 첫 block이 아닐 때
	// 	{
	// 	VolumeActor->PutNextId(HeatTransferVolumes.Num() - 1);
	// 	}
	// if(j != VolumeRatio - 1) // 한 line의 가장 마지막 block이 아닐 때
	// 	{
	// 	VolumeActor->PutNextId(HeatTransferVolumes.Num() + 1);
	// 	}
	// if(i != 0) // 가장 밑 line이 아닐 때
	// 	{
	// 	VolumeActor->PutNextId(HeatTransferVolumes.Num() - VolumeRatio);
	// 	}
	// else
	// {
	// 			
	// }
}
