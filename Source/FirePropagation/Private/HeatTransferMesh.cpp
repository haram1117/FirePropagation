// Fill out your copyright notice in the Description page of Project Settings.


#include "HeatTransferMesh.h"
#include "MeshDescription.h"
#include "StaticMeshAttributes.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AHeatTransferMesh::AHeatTransferMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TestMesh"), false);
}

// Called when the game starts or when spawned
void AHeatTransferMesh::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh = Mesh->GetStaticMesh();
	MeshDescription = StaticMesh->GetMeshDescription(0);
	
	Mesh->SetLODDataCount(1, Mesh->LODData.Num());
	TAttributesSet<FVertexInstanceID>& AttributesSet = MeshDescription->VertexInstanceAttributes();
	
	for(const FVertexInstanceID& VertexInstanceID : MeshDescription->VertexInstances().GetElementIDs())
	{
		AttributesSet.SetAttribute(VertexInstanceID, MeshAttribute::VertexInstance::Color, 0, FVector4(FColor::Red));
	}
	StaticMesh->Build();
	// GetTriangle(FVertexID(0));
	// GetTriangle(FVertexID(1));
	// GetTriangle(FVertexID(2));
}

void AHeatTransferMesh::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Mesh->ClearInstances();
	Mesh->AddInstance(FTransform(FVector(0, 0, 0)));
}

void AHeatTransferMesh::GetTriangle(FVertexID id)
{
	// TAttributesSet<FVertexInstanceID>& AttributesSet = MeshDescription->VertexInstanceAttributes();
	TArray<FVertexInstanceID> VertexInstanceIds = MeshDescription->GetVertexVertexInstances(id);
	for(const FVertexInstanceID& VertexInstanceID : MeshDescription->VertexInstances().GetElementIDs())
	{
		MeshDescription->VertexInstanceAttributes().SetAttribute(VertexInstanceID, MeshAttribute::VertexInstance::Color, 0, FVector4(FColor::Red));
	}
	StaticMesh->Build();
}

// Called every frame
void AHeatTransferMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


