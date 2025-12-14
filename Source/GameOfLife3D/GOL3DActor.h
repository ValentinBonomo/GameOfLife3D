#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOL3DActor.generated.h"

UCLASS()
class GAMEOFLIFE3D_API AGOL3DActor : public AActor
{
	GENERATED_BODY()
    
public:    
	AGOL3DActor();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game of Life")
	int32 GridSize = 10;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game of Life")
	float CellSize = 100.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game of Life")
	float UpdateInterval = 1.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game of Life")
	UStaticMesh* CubeMesh;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game of Life")
	UMaterial* CubeMaterial;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game of Life")
	int32 RandomSeed = 0;

private:
	TArray<TArray<TArray<bool>>> Grid;
	TArray<TArray<TArray<bool>>> NewGrid;
	TArray<UStaticMeshComponent*> CubeComponents;
    
	FTimerHandle UpdateTimerHandle;
    
	void InitGrid();
	void NextGeneration();
	void UpdateVisuals();
	void OnTimerUpdate();
	int32 CountNeighbors(int32 x, int32 y, int32 z);
};