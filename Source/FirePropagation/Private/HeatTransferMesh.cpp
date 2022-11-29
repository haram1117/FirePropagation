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
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestMesh"), false);
	
}

// Called when the game starts or when spawned
void AHeatTransferMesh::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh = MeshComponent->GetStaticMesh();
	MeshDescription = StaticMesh->GetMeshDescription(0);
	
	MeshComponent->SetLODDataCount(1, MeshComponent->LODData.Num());
	LODInfo = &MeshComponent->LODData[0];

	LODInfo->PaintedVertices.Empty();
	LODInfo->OverrideVertexColors = new FColorVertexBuffer();

	FStaticMeshLODResources& LODResources = StaticMesh->RenderData->LODResources[0];
	UE_LOG(LogTemp, Log, TEXT("LODResources: %d"), LODResources.GetNumVertices());
	UE_LOG(LogTemp, Log, TEXT("MeshDescription instances: %d"), MeshDescription->VertexInstances().Num());
	UE_LOG(LogTemp, Log, TEXT("MEshDescription Vertex: %d"), MeshDescription->Vertices().Num());
	// for(const auto& elem : MeshDescription->VertexInstances().GetElementIDs())
	// {
	// 	MeshDescription->VertexInstanceAttributes().SetAttribute(elem, MeshAttribute::VertexInstance::Color, 0, FColor::Blue);
	// }
	// StaticMesh->BuildFromMeshDescription(*MeshDescription, LODResources);
	// LODResources.VertexBuffers.
	// nextVertices.Append(MeshDescription->GetVertexVertexInstances(FVertexID(0)));
	// for(int j = 0; j < LODResources.GetNumVertices(); j++)
	// {
	// 	ColorArray.Add(FColor::Blue);
	// }
	// ColorVertexInstances(FVertexID(0));
	// TArray<FVertexID> nextId = FindAdjacentVertices(FVertexID(0));
	// for(int i = 0; i < nextId.Num(); i++)
	// {
	// 	ColorVertexInstances(nextId[i]);
	// }
	// FTimerHandle WaitHandle;
	// GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	// {
	// 	nowVertices.Empty();
	// 	nowVertices.Append(nextVertices);
	// 	nextVertices.Empty();
	// 	UE_LOG(LogTemp, Log, TEXT("nextVertices: %d"), nowVertices.Num());
	// 	for(int i = 0; i < nowVertices.Num(); i++)
	// 		ColorVertexInstances(nowVertices[i]);
	// 	
	// 	
	// }), 10.0f, false);
}

void AHeatTransferMesh::ColorVertexInstances(FVertexID id)
{
	TArray<FVertexInstanceID> VertexInstanceIds = MeshDescription->GetVertexVertexInstances(id);
	for(int j = 0; j < VertexInstanceIds.Num(); j++)
	{
		// if(MeshDescription->VertexInstanceAttributes().GetAttribute<FColor>(VertexInstanceIds[j], MeshAttribute::VertexInstance::Color) == FColor::Blue)
		// 	MeshDescription->VertexInstanceAttributes().SetAttribute(VertexInstanceIds[j], MeshAttribute::VertexInstance::Color, 0, FColor::Red);
		
		// if(ColorArray[VertexInstanceIds[j].GetValue()] != FColor::Red)
		// {
		// 	// ColorArray[VertexInstanceIds[j].GetValue()] = FColor::Red;
		// 	// MeshDescription->VertexAttributes().SetAttribute(id, )
		// }
	}
	// LODInfo->OverrideVertexColors->InitFromColorArray(ColorArray);

	// BeginInitResource(LODInfo->OverrideVertexColors);
	// MeshComponent->MarkRenderStateDirty();
	// StaticMesh->BuildFromMeshDescription(*MeshDescription, StaticMesh->RenderData->LODResources[0]);
	FindAdjacentVertices(id);
}

TArray<FVertexID> AHeatTransferMesh::FindAdjacentVertices(FVertexID id)
{
	TArray<FVertexID> AdjacentVertices = MeshDescription->GetVertexAdjacentVertices(id);
	// for(int i = 0; i < AdjacentVertices.Num(); i++)
	// {
	// 	FVertexInstanceID temp = MeshDescription->GetVertexVertexInstances(AdjacentVertices[i])[0];
	// 	if(ColorArray[temp.GetValue()] != FColor::Red)
	// 	{
	// 		nextVertices.Add(AdjacentVertices[i]);
	// 	}
	// }
	return AdjacentVertices;
}

// Called every frame
void AHeatTransferMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


