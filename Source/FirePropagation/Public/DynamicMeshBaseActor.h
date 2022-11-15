// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicMesh3.h"
#include "MeshDescriptionBuilder.h"
#include "DynamicMeshBaseActor.generated.h"

UENUM()
enum class FloorType
{
	E_Rectangular UMETA(DisplayName = "Rectangular"), 
	E_Circular UMETA(DisplayName = "Circular")
};

UCLASS()
class FIREPROPAGATION_API ADynamicMeshBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicMeshBaseActor();
	UPROPERTY(EditAnywhere)
	FloorType m_Floor = FloorType::E_Rectangular;

	UPROPERTY(EditAnywhere)
	int width = 3;

	UPROPERTY(EditAnywhere)
	int length = 1;

	UPROPERTY(EditAnywhere)
	int height = 3;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FDynamicMesh3 SourceMesh;
	UStaticMeshComponent* MeshComponent;
	void SetDescription();
	void UpdateMesh();
	void MakeFloor();
	void MakeFrontWall();
	void MakeMesh();
	FMeshDescriptionBuilder meshDescBuilder;
	FMeshDescription meshDesc;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
