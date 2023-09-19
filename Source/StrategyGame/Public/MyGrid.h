// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <vector>

#include "MyGrid.generated.h"

enum class MyGridModifiers
{
	SLOW = 0,
	FIRE,
	POISON
};

UCLASS()
class STRATEGYGAME_API AMyGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGrid();

	UFUNCTION(BlueprintCallable)
		void SetObject(UObject* object);
	UFUNCTION(BlueprintCallable)
		bool Empty();

protected:

	// Object inside the grid
	UObject* mObject;

	// Possible gameplay modifiers in the grid
	std::vector<MyGridModifiers> mModifiers;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};