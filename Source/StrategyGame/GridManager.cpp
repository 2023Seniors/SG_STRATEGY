// Fill out your copyright notice in the Description page of Project Settings.
#include "GridManager.h"
#include "Public/PathFinding.h"

Cell::Cell() : mEntity(nullptr), mWalkable(true){}

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	//mGridMap[mMapSize.Y * 1 + 1].mWalkable = false;
	//
	//FindFullPath( { 0,0,0 }, {50,50, 0} );
}

FVector AGridManager::GetMapSize()
{
	return mMapSize;
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
int AGridManager::GetCellSize()
{
	return mCellSize.X;
}


// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AGridManager::SetEntityFromWorldPos(AActor* Entity, bool walkable)
{
	FVector cell = GetCellXYFromWorldPos(Entity->GetActorLocation());
	Cell* selectedCell = &mGridMap[mMapSize.Y * cell.X + cell.Y];

	selectedCell->mEntity = Entity;
	selectedCell->mWalkable = walkable;
}

AActor* AGridManager::GetEntityFromWorldPos(FVector WorldPos)
{
	FVector cell = GetCellXYFromWorldPos(WorldPos);

	return mGridMap[mMapSize.Y * cell.X + cell.Y].mEntity;
}

FVector AGridManager::GetCellXYFromWorldPos(FVector WorldPos)
{
	// Offset respect to grid
	FVector Delta = WorldPos - this->GetActorLocation();

	int x = (int)Delta.X / mCellSize.X;
	int y = (int)Delta.Y / mCellSize.Y;

	return FVector(x, y, 0);
}

FVector AGridManager::GetWorldPosFromCellXY(FVector cellXY)
{
	// Get the center of the cell, not bottom left
	float x = (cellXY.X + 0.5f) * mCellSize.X;
	float y = (cellXY.Y + 0.5f) * mCellSize.Y;

	FVector world = { x, y, 0 };

	// Offset respect to grid
	world += this->GetActorLocation();

	return world;
}

bool AGridManager::IsCellWalkableFromGridXY(int x, int y)
{
	return mGridMap[mMapSize.Y * x + y].mWalkable;
}
void AGridManager::SetCellWalkableFromGridXY(int x, int y, bool walkable)
{
	mGridMap[mMapSize.Y * x + y].mWalkable = walkable;
}
void AGridManager::SetCellModifierFromXY(int x, int y, GridCellModifier modifier)
{
	mGridMap[mMapSize.Y * x + y].mModifier = modifier;
}

TArray<FVector> AGridManager::FindFullPath(FVector start, FVector end)
{
	FVector gridPosStart = GetCellXYFromWorldPos(start);
	FVector gridPosEnd = GetCellXYFromWorldPos(end);

	TArray<FIntVector2> pathInt = PathFinding::PFFindFullPath({ (int)gridPosStart.X, (int)gridPosStart.Y }, { (int)gridPosEnd.X, (int)gridPosEnd.Y} , this);
	TArray<FVector> path;
	path.SetNum(pathInt.Num());

	for (int i = 0; i < path.Num(); i++) {
		path[i] = GetWorldPosFromCellXY( { (float)pathInt[i].X, (float)pathInt[i].Y, 0 } );
	}

	return path;
}