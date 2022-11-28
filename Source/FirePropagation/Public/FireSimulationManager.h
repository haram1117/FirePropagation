// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeatTransferVolume.h"
#include "GameFramework/Actor.h"
#include "FireSimulationManager.generated.h"

UENUM(BlueprintType)
enum class EHeatTransferVolumeAmount : uint8
{
	High = 5,
	Low = 10,
	Custom = 0
};

UCLASS()
class FIREPROPAGATION_API AFireSimulationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireSimulationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AHeatTransferVolume> Volume;
	UPROPERTY(EditAnywhere)
	UStaticMesh* BuildingMesh;
	virtual void OnConstruction(const FTransform& Transform) override;


	UPROPERTY(EditAnywhere, Category = "HeatTransferVolumeSettings")
	EHeatTransferVolumeAmount VolumeAmount = EHeatTransferVolumeAmount::Custom;
	
	UPROPERTY(EditAnywhere, Category = "HeatTransferVolumeSettings", meta = (EditCondition = "VolumeAmount == EHeatTransferVolumeAmount::Custom"))
	int VolumeNumber;
	int VolumeRatio;

	TArray<AHeatTransferVolume*> HeatTransferVolumes;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	static void GetStaticMeshSize(UStaticMesh* Mesh, FVector& Size);
	void SpawnVolumes(FVector size);
	void SetNeighborVolumes();

};

