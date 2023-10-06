// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"

#include "GridManager.generated.h"


enum GridCellModifier
{
	EXIT = 0,
	WALL,
	VOID,
	SPAWNER,
	START0,
	START1,
	START2,
	START3
};

struct Cell
{
	Cell();
	AActor* mEntity;
	GridCellModifier mModifier;
	bool mWalkable;
};

UCLASS()
class STRATEGYGAME_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UFUNCTION(BlueprintCallable)
		void SetMapSize(int width, int height);
	UFUNCTION(BlueprintCallable)
		FVector GetMapSize();
	UFUNCTION(BlueprintCallable)
		void SetCellSize(int size);
	UFUNCTION(BlueprintCallable)
		int GetCellSize();
	UFUNCTION(BlueprintCallable)
		void SetEntityFromWorldPos(AActor* Entity);
	UFUNCTION(BlueprintCallable)
		AActor* GetEntityFromWorldPos(FVector WorldPos);
	UFUNCTION(BlueprintCallable)
		FVector GetCellXYFromWorldPos(FVector WorldPos);
	UFUNCTION(BlueprintCallable)
		FVector GetWorldPosFromCellXY(FVector cellXY);
	UFUNCTION(BlueprintCallable)
		bool	IsCellWalkableFromGridXY(int x, int y);

	// Pathfinding 
	UFUNCTION(BlueprintCallable)
		TArray<FVector> FindFullPath(FVector start, FVector end);


protected:
	// Size of the Grid Map
	FVector mMapSize;
	FUintVector2 mCellSize;

	// Grid container
	TArray<Cell> mGridMap;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
