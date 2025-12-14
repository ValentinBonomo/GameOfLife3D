#include "GOL3DActor.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

AGOL3DActor::AGOL3DActor()
{
    PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AGOL3DActor::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Warning, TEXT("BeginPlay called"));
    
    if (!CubeMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("CubeMesh is NULL!"));
        return;
    }
    
    if (RandomSeed != 0)
    {
        FMath::RandInit(RandomSeed);
    }
    
    InitGrid();
    UpdateVisuals();
    
    GetWorld()->GetTimerManager().SetTimer(
        UpdateTimerHandle,
        this,
        &AGOL3DActor::OnTimerUpdate,
        UpdateInterval,
        true
    );
}

void AGOL3DActor::OnTimerUpdate()
{
    NextGeneration();
    UpdateVisuals();
}

void AGOL3DActor::InitGrid()
{
    UE_LOG(LogTemp, Warning, TEXT("InitGrid called with GridSize: %d"), GridSize);
    
    Grid.SetNum(GridSize);
    NewGrid.SetNum(GridSize);
    
    int32 AliveCells = 0;
    
    for (int32 x = 0; x < GridSize; x++)
    {
        Grid[x].SetNum(GridSize);
        NewGrid[x].SetNum(GridSize);
        
        for (int32 y = 0; y < GridSize; y++)
        {
            Grid[x][y].SetNum(GridSize);
            NewGrid[x][y].SetNum(GridSize);
            
            for (int32 z = 0; z < GridSize; z++)
            {
                Grid[x][y][z] = (FMath::Rand() % 2 == 0);
                NewGrid[x][y][z] = false;
                
                if (Grid[x][y][z])
                    AliveCells++;
            }
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("InitGrid finished. Alive cells: %d"), AliveCells);
}

void AGOL3DActor::NextGeneration()
{
    for (int32 x = 0; x < GridSize; x++)
    {
        for (int32 y = 0; y < GridSize; y++)
        {
            for (int32 z = 0; z < GridSize; z++)
            {
                int32 Neighbors = CountNeighbors(x, y, z);
                
                if (Grid[x][y][z])
                {
                    NewGrid[x][y][z] = !(Neighbors < 5 || Neighbors == 8);
                }
                else
                {
                    NewGrid[x][y][z] = (Neighbors == 5);
                }
            }
        }
    }
    
    Grid = NewGrid;
}

int32 AGOL3DActor::CountNeighbors(int32 x, int32 y, int32 z)
{
    int32 Count = 0;
    
    for (int32 dx = -1; dx <= 1; dx++)
    {
        for (int32 dy = -1; dy <= 1; dy++)
        {
            for (int32 dz = -1; dz <= 1; dz++)
            {
                if (dx == 0 && dy == 0 && dz == 0)
                    continue;
                
                int32 nx = x + dx;
                int32 ny = y + dy;
                int32 nz = z + dz;
                
                if (nx >= 0 && nx < GridSize && 
                    ny >= 0 && ny < GridSize && 
                    nz >= 0 && nz < GridSize)
                {
                    if (Grid[nx][ny][nz])
                    {
                        Count++;
                    }
                }
            }
        }
    }
    
    return Count;
}

void AGOL3DActor::UpdateVisuals()
{
    for (UStaticMeshComponent* Comp : CubeComponents)
    {
        if (Comp)
        {
            Comp->DestroyComponent();
        }
    }
    CubeComponents.Empty();
    
    int32 InstanceCount = 0;
    
    for (int32 x = 0; x < GridSize; x++)
    {
        for (int32 y = 0; y < GridSize; y++)
        {
            for (int32 z = 0; z < GridSize; z++)
            {
                if (Grid[x][y][z])
                {
                    UStaticMeshComponent* CubeComp = NewObject<UStaticMeshComponent>(this);
                    CubeComp->SetupAttachment(RootComponent);
                    CubeComp->SetStaticMesh(CubeMesh);
                    if (CubeMaterial)
                    {
                        CubeComp->SetMaterial(0, CubeMaterial);
                    }
                    CubeComp->RegisterComponent();
                    
                    FVector Location(x * CellSize, y * CellSize, z * CellSize);
                    CubeComp->SetRelativeLocation(Location);
                    
                    CubeComponents.Add(CubeComp);
                    InstanceCount++;
                }
            }
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("UpdateVisuals: Added %d cubes"), InstanceCount);
}