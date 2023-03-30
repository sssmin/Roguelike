# Roguelike 포트폴리오

포트폴리오 목적으로 제작한 로그라이크 프로토타입

> 아래 이미지 클릭하면 유튜브로 이동합니다. 짧게 플레이 영상만 담았습니다. <br>

<br>
엔진 : UnrealEngine 5.0.3 - UnrealEngine 5.1.1 <br>
제작기간 : 약 1달 반 <br>
개발 규모 : 1인 개발 

### DFSAgrt 클래스
+ 방 생성을 위해 DFS 알고리즘 사용, DFS 특징을 활용하여 시작 방을 제외한 모든 방은 문이 최대 2개가 되도록 구현
+ DFSAgrt 클래스는 UObject를 상속받을 필요가 없어서 스마트포인터로 관리

```c++
//RLGameInstance.h
TSharedPtr<DFSAgrt> DFS;

//RLGameInstance.cpp
void URLGameInstance::Init()
{
    Super::Init();
	
    DFS = MakeShared<DFSAgrt>();
  //....
```
+ 방 정보는 구조체로 정의하고 방 생성시 배열에 저장
```c++
//RLGameInstance.h
UPROPERTY()
TArray<FCell> Board;
```
+ DFSAgrt 방 생성시에도 Board 형태로 초기화하여 임의의 시작점으로부터 길 찾기 시작하고 위, 아래, 오른쪽, 왼쪽을 FCell 구조체의 Visited 변수로 체크하며
길을 만들어 나가도록 구현
+ 시작점은 최소 한 개의 갈래 길, 최대 3개의 갈래 길로 구성하도록 구현
```c++
void DFSAgrt::MazeGenerator()
{
    int32 CurrentCell = StartCell;

    TArray<int32> SideCells;
    int32 CreatedCellCount = 0;
    int32 BeforeCell = 0;

    while (true)
    {
        SideCells.Empty();

        Board[CurrentCell].Visited = true;
        Board[CurrentCell].CellState = ECellState::Normal;
        Board[CurrentCell].CellType = ECellType::Mobs;
        Board[CurrentCell].CellClass = FMath::RandRange(2, 4);
        for (int32 i = 0; i < 8; ++i)
        {
            int32 Rand = FMath::RandRange(1, 4);
            if (Rand == 1)
            {
                Board[CurrentCell].TempWall |= 1 << i;
            }
        }
        if (CreatedCellCount == CellCount) break;

        if (CurrentCell == Board.Num()) break;

        SideCells = CheckSideCell(CurrentCell);
        if (SideCells.Num() != 0)
        {
    //......
 ```
 + 방문한 셀 주변에 갈 수 있는 셀이 있는지 체크(DFS 길찾기와 똑같음!)
 + while 문에서 빠져나가면 만들어야 하는 갈래 길 개수와 만들어진 갈래 길 개수를 비교하여 재귀 호출
 ```c++
if (++CreatedPart != Parts)
{
    MazeGenerator();
}
  ```

### ListenerManager 클래스
+ 델리게이트 관리를 위한 클래스
+ 발행-구독 패턴을 적용하여 위젯에서는 게임 로직에 관여하지 않도록 설계하기 위해 구현
```c++
//RLListenerManager.h
//..
DECLARE_DELEGATE_TwoParams(FRequestItemSwap, const UItemInfo*, const UItemInfo*);
//..
FRequestItemSwap RequestItemSwapDelegate;
void RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem);

//RLListenerManager.cpp
void URLListenerManager::RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem)
{
    RequestItemSwapDelegate.ExecuteIfBound(OldItem, NewItem);
}

//SelectItemCellWidget.cpp
URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
if (GI && GI->GetListenerManager())
{
    GI->GetListenerManager()->RequestItemSwap(FirstItem, TempSelectItem);
//..
```

### MonsterObjectPool 클래스
+ 프로젝트 진행 중 방을 넘어갔을 때 프레임 드랍 현상이 종종 발생
+ 몬스터 스폰이 문제라고 생각되어 객체 풀로 몬스터를 관리하도록 구현
+ (몬스터 최대 7마리 스폰으로 많지 않음에도 객체 풀로 관리하니 현상이 일어나지 않음 -> 다른 액터 스폰은 그대로 유지)
+ 메인 게임 시작시 객체 풀 초기화
```c++
//MonsterCharacter.h
bool bActive;
void SetActive(bool Active);
void Deactivate();

//MonsterObjectPool.cpp
AMonsterCharacter* UMonsterObjectPool::GetDeactivateMonster(TArray<AMonsterCharacter*> Pool)
{
    for (AMonsterCharacter* Monster : Pool)
    {
        if (!Monster->IsActive()) return Monster;
    }
    return nullptr;
}
```
+ 스폰 요청시, 몬스터 처치시 AIController, 스탯 등 초기화

### LoadingScreenModule
+ MoviePlayer 모듈을 활용하여 객체 풀, 맵 초기화를 하면서 동시에 비동기로 로딩 스크린(위젯)이 화면에 보이도록 구현
+ BlueprintLibrary에 static 함수로 구현
```c++
//RoguelikeLoadingScreen.cpp
virtual void StartInGameLoadingScreen(UUserWidget* Widget, bool bPlayUntilStopped, float PlayTime) override
{
    FLoadingScreenAttributes LoadingScreen;
    LoadingScreen.bAutoCompleteWhenLoadingCompletes = !bPlayUntilStopped;
    LoadingScreen.bWaitForManualStop = bPlayUntilStopped;
    LoadingScreen.bAllowEngineTick = bPlayUntilStopped;
    LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
    LoadingScreen.WidgetLoadingScreen = Widget->TakeWidget();
    GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}
  
//CustomBPFL.cpp
void UCustomBPFL::PlayLoadingScreen(UUserWidget* Widget, bool PlayUntilStopped, float PlayTime)
{
    IRoguelikeLoadingScreenModule& LoadingScreenModule = IRoguelikeLoadingScreenModule::Get();
    LoadingScreenModule.StartInGameLoadingScreen(Widget, PlayUntilStopped, PlayTime);
}
```

### Projectile 클래스 구조
+ BaseProjectile에서 MonsterProjectile, PlayerProjectile 로 나뉨
+ MonsterProjectile에서 총 4개의 클래스로 나뉨
+ 몬스터의 고유 공격마다 다른 구체를 적용하기 위함
+ 예를 들어 폭탄을 던지는 공격은 BombProjectile을 스폰
```c++
void UMonsterCombatComponent::ThrowBomb(AActor* Target, TSubclassOf<UDamageType> DamageType)
{
    PlayFireMontage();
		
    if (GetCombatManager.IsBound())
    {
        FCombatManager CombatManager = GetCombatManager.Execute();
        FActorSpawnParameters Params;
        Params.Owner = GetOwner();
        if (BombProjectileClass && GetWorld())
        {
            ABombProjectile* SpawnedProjectile =
                GetWorld()->SpawnActor<ABombProjectile>(
                    BombProjectileClass,
                    GetOwner()->GetActorLocation(),
                    GetOwner()->GetActorRotation(),
                    Params);
            if (SpawnedProjectile)
            {
                FVector ThrowVector = CalcThrowVector(Target);
                SpawnedProjectile->SetVelocity(ThrowVector);
                SpawnedProjectile->SetCombatManage(CombatManager);
                SpawnedProjectile->SetDamageType(DamageType);
            }
        }
    }
}
```
+ 여러 갈래로 구체를 발사하는 공격은 MonsterProjectile을 스폰
```c++
void UMonsterCombatComponent::FireInParts(int32 Parts, float StartDegree, float DeltaDegree, TSubclassOf<UDamageType> DamageType)
{
    PlayFireMontage();
    if (GetCombatManager.IsBound())
    {
        FCombatManager CombatManager = GetCombatManager.Execute();
        FActorSpawnParameters Params;
        Params.Owner = GetOwner();

        if (ProjectileClass && GetWorld())
        {
            float Degree = StartDegree + DeltaDegree;
            for (int32 i = 0; i < Parts; ++i)
            {
                AMonsterProjectile* SpawnedProjectile =
                    GetWorld()->SpawnActor<AMonsterProjectile>(
                        ProjectileClass,
                        GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f,
                        FRotator(0.f, 0.f, 0.f),
                        Params);
                FVector Dir = UKismetMathLibrary::CreateVectorFromYawPitch(GetOwner()->GetActorRotation().Yaw + (Degree -= DeltaDegree), 0.f);
                if (SpawnedProjectile)
                {
                    SpawnedProjectile->SetVelocity(Dir);
                    SpawnedProjectile->SetCombatManage(CombatManager);
                    SpawnedProjectile->SetDamageType(DamageType);
                }
            }
        }
    }
}
```



