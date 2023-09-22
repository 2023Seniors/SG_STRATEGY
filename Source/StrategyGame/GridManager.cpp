// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"

Cell::Cell() : mEntity(nullptr) {}

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the grid container
	SetMapSize(100, 100);
	// Initialize the cell size
	SetCellSize(10);
}

void AGridManager::SetMapSize(int width, int height)
{
	mMapSize.X = width;
	mMapSize.Y = height;

	// Clear the array vectors
	mGridMap.Empty();
	
	// Resize to the new size
	mGridMap.SetNum(width * height);
}

void AGridManager::SetCellSize(int size)
{
	mCellSize.X = size;
	mCellSize.Y = size;
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AGridManager::SetEntityFromWorldPos(AActor* Entity)
{
	FVector Zero = this->GetActorLocation();
	FVector Delta = Entity->GetActorLocation() - Zero;

	int x = (int)Delta.X % mCellSize.X;
	int y = (int)Delta.Y % mCellSize.Y;

	mGridMap[mMapSize.Y * x + y].mEntity = Entity;
}

AActor* AGridManager::GetEntityFromWorldPos(FVector WorldPos)
{
	FVector Zero = this->GetActorLocation();
	FVector Delta = WorldPos - Zero;

	int x = (int)Delta.X % mCellSize.X;
	int y = (int)Delta.Y % mCellSize.Y;

	return mGridMap[mMapSize.Y * x + y].mEntity;
}

