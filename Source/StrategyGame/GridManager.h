// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"

#include "GridManager.generated.h"

UENUM(BlueprintType)
enum class GridCellModifier : uint8
{
	EXIT	UMETA(DisplayName = "EXIT"),
	WALL	UMETA(DisplayName = "WALL"),
	VOID	UMETA(DisplayName = "VOID"),
	SPAWNER	UMETA(DisplayName = "SPAWNER"),
	START0	UMETA(DisplayName = "START0"),
	START1	UMETA(DisplayName = "START1"),
	START2	UMETA(DisplayName = "START2"),
	START3	UMETA(DisplayName = "START3")
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

	// Grid/Cell Size
	UFUNCTION(BlueprintCallable)
		void SetMapSize(int width, int height);
	UFUNCTION(BlueprintCallable)
		FVector GetMapSize();
	UFUNCTION(BlueprintCallable)
		void SetCellSize(int size);
	UFUNCTION(BlueprintCallable)
		int GetCellSize();
	// Position
	UFUNCTION(BlueprintCallable)
		FVector GetCellXYFromWorldPos(FVector WorldPos);
	UFUNCTION(BlueprintCallable)
		FVector GetWorldPosFromCellXY(FVector cellXY);
	// Cell
	UFUNCTION(BlueprintCallable)
		void SetEntityFromWorldPos(AActor* Entity, bool walkable = false);
	UFUNCTION(BlueprintCallable)
		AActor* GetEntityFromWorldPos(FVector WorldPos);
	UFUNCTION(BlueprintCallable)
		bool	IsCellWalkableFromGridXY(int x, int y);
	UFUNCTION(BlueprintCallable)
		void	SetCellWalkableFromGridXY(int x, int y, bool walkable);
	UFUNCTION(BlueprintCallable)
		void SetCellModifierFromXY(int x, int y, GridCellModifier modifier);

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
