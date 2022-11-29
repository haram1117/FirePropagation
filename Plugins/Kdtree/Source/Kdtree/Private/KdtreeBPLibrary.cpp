// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "KdtreeBPLibrary.h"
#include "Kdtree.h"
#include "./KdtreeInternal.h"


UKdtreeBPLibrary::UKdtreeBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UKdtreeBPLibrary::BuildKdtree(FKdtree& Tree, const TArray<FVector>& Data)
{
    KdtreeInternal::BuildKdtree(&Tree.Internal, Data);
}

void UKdtreeBPLibrary::ClearKdtree(FKdtree& Tree)
{
    KdtreeInternal::ClearKdtree(&Tree.Internal);
}

void UKdtreeBPLibrary::CollectFromKdtree(const FKdtree& Tree, const FVector Center, float Radius,
                                         TArray<int>& Indices, TArray<FVector>& Data)
{
    KdtreeInternal::CollectFromKdtree(Tree.Internal, Center, Radius, &Indices);
    for (int Index = 0; Index < Indices.Num(); ++Index)
    {
        Data.Add(Tree.Internal.Data[Indices[Index]]);
    }
}

void UKdtreeBPLibrary::ValidateKdtree(const FKdtree& Tree)
{
    KdtreeInternal::ValidateKdtree(Tree.Internal);
}

void UKdtreeBPLibrary::DumpKdtreeToConsole(const FKdtree& Tree)
{
    KdtreeInternal::DumpKdTree(Tree.Internal);
}
