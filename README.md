# Roguelike 포트폴리오

포트폴리오 목적으로 제작한 로그라이크 프로토타입<br>

> 아래 이미지 클릭하면 유튜브로 이동합니다. 짧게 플레이 영상만 담았습니다. <br>

[![Image](https://user-images.githubusercontent.com/27758519/230600186-e4bb837b-e38f-4dfa-9073-b8daaf09665c.jpg)](https://youtu.be/VU1xKQi2RtI)



<br>
엔진 : UnrealEngine 5.0.3 - UnrealEngine 5.1.1 (반강제 업그레이드..) <br>
제작기간 : 약 1달 반 <br>
개발 규모 : 1인 개발 <br>

# 게임 설명
+ 생성되는 방의 갯수, 아이템 등장, 몬스터 속성 결정 등 이런 부분을 모두 운에 따라 플레이하도록 제작한 게임입니다.


# 구현 설명

### DFSAgrt 클래스
+ 맵은 그리드 형태. 방 생성을 위해 DFS 알고리즘 사용, DFS 특징을 활용하여 시작 방을 제외한 모든 방은 문이 최대 2개가 되도록 구현.
+ DFSAgrt 클래스는 UObject를 상속받을 필요가 없어서 스마트포인터로 관리.

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
+ 방 정보는 구조체로 정의하고 방 생성시 배열에 저장.
```c++
//RLGameInstance.h
UPROPERTY()
TArray<FCell> Board;
```
+ DFSAgrt 방 생성시에도 Board 형태로 초기화하여 임의의 시작점으로부터 길 찾기 시작하고 위, 아래, 오른쪽, 왼쪽을 FCell 구조체의 Visited 변수로 체크하며
길을 만들어 나가도록 구현.
+ 시작점은 최소 한 개의 갈래 길, 최대 3개의 갈래 길로 구성하도록 구현.
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
 + 방문한 셀 주변에 갈 수 있는 셀이 있는지 체크. (DFS 길찾기와 똑같음!)
 + while 문에서 빠져나가면 만들어야 하는 갈래 길 개수와 만들어진 갈래 길 개수를 비교하여 재귀 호출.
 ```c++
if (++CreatedPart != Parts)
{
    MazeGenerator();
}
  ```
  
### ListenerManager 클래스
+ 델리게이트 관리를 위한 클래스.
+ 발행-구독 패턴을 적용하여 C++로 구현한 위젯에서 게임 로직에 관여하지 않도록 설계하기 위해 구현.
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
+ 프로젝트 진행 중 방을 넘어갔을 때 프레임 드랍 현상이 종종 발생.
+ 몬스터 스폰이 문제라고 생각되어 객체 풀로 몬스터를 관리하도록 구현.
+ (몬스터 최대 7마리 스폰으로 많지 않음에도 객체 풀로 관리하니 현상이 일어나지 않음. -> 다른 액터 스폰은 그대로 유지)
+ 메인 게임 시작시 객체 풀 초기화.
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
+ 스폰 요청시, 몬스터 처치시 AIController, 스탯 등 초기화.

### LoadingScreenModule
+ MoviePlayer 모듈을 활용하여 객체 풀, 맵 초기화를 하면서 동시에 비동기로 로딩 스크린(위젯)이 화면에 보이도록 구현.
+ BlueprintLibrary에 static 함수로 구현.
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
+ BaseProjectile에서 MonsterProjectile, PlayerProjectile 로 나뉨.
+ MonsterProjectile에서 총 4개의 클래스로 나뉨.
+ 몬스터의 고유 공격마다 다른 구체를 적용하기 위함.
+ 예를 들어 폭탄을 던지는 공격은 BombProjectile을 스폰.
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
+ 여러 갈래로 구체를 발사하는 공격은 MonsterProjectile을 스폰.
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

### URoguelikeSaveGame 클래스
+ 데이터를 세이브, 로드하기 위한 클래스.
+ GameInstance에서 관리하던 데이터를 모두 저장해야 하기 때문에 구조체로(FInfoRecord) 정의하여 저장.
+ 구조체를 저장하기 위해 직렬화가 필요하고, 직렬화를 하기 위해 FArchive 클래스의 << 연산자 재정의.

```c++
USTRUCT(BlueprintType)
struct FInfoRecord
{
    GENERATED_BODY()
    //...
    UPROPERTY()
    TArray<FCell> Board;
    FVector2Int MapSize;
    //...

    friend FArchive& operator<<(FArchive& Ar, FInfoRecord& InfoRecord)
    {
        Ar << InfoRecord.FixMaxNum;
        Ar << InfoRecord.Board;
        Ar << InfoRecord.MapSize;
        //..

        return Ar;
    }
};
```

### 옵션 위젯
+ 옵션 위젯은 블루 프린트로만 구현.<br>
![OptionWidget](https://user-images.githubusercontent.com/27758519/229025324-d7b613c2-8b01-43be-9e78-367cc6325508.jpg)
+ 옵션 탭에서 옵션 선택시 그에 맞는 옵션 위젯 생성. 생성된 위젯에서 적용 버튼, 되돌리기 버튼 이벤트 디스패처 바인드. <br>
![EventBind](https://user-images.githubusercontent.com/27758519/229026328-9525c70b-0595-470c-85d7-4fd1f87f2572.jpg)
+ 적용, 되돌리기 함수는 인터페이스에 선언된 가상 함수. (옵션 위젯마다 재정의하여 구현)
+ 사운드를 제외한 나머지는 GameUserSettings 클래스를 이용하여 세이브 및 로드.
+ 사운드는 SaveGame 클래스를 이용하여 슬롯 저장. (옵션 저장과 인게임 내 데이터 저장은 분리)


### PlayersCamera 클래스
+ 우클릭으로 화면 이동을 위한 클래스.
+ 마우스 위치, 뷰포트 크기, 뷰포트와 스크린 비율을 구하고, 마우스 위치에 따른 Delta 값으로 카메라 위치 조정.
+ 쿼터뷰로 카메라는 캐릭터 위에 존재. -> 카메라와 캐릭터 거리 X, Y 만으로 계산 후 비교.
```c++
void APlayersCamera::MoveCam()
{
    FVector SubLoc = UKismetMathLibrary::Subtract_VectorVector(GetActorLocation(), PlayerCharacter->GetActorLocation()); //카메라와 캐릭터 거리
    const FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this); //마우스 위치(스크린 좌표)
    const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this); //뷰포트와 스크린 비율
    const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this); //뷰포트 크기(스크린 좌표)

    if (SubLoc.X <= 500.f)
    {
        float ViewportRange = UKismetMathLibrary::NormalizeToRange(MousePos.Y * ViewportScale, 0.f, ViewportSize.Y * 0.1f);
        ViewportRange = FMath::Clamp(ViewportRange, 0.f, 1.f);
        const FVector DeltaLoc{ CameraSpeed * (1.f - ViewportRange), 0.f , 0.f };
        RootComponent->AddRelativeLocation(DeltaLoc);
    }
//...
```
+ MoveCam 함수는 우클릭을 하면 Tick에서 호출하여 카메라가 이동.
+ 방 이동 시 SetViewTargetWithBlend 함수를 이용한 뷰 전환 효과.
```c++
SetViewTargetWithBlend(nullptr, 0.f);
	
if (CurrentPlayersCamera)
{
    SetViewTargetWithBlend(CurrentPlayersCamera, 0.7f);
}
```
### ManagerComponent
+ 플레이어, 몬스터 캐릭터 스탯을 관리하는 클래스.
+ 2스테이지부터 몬스터는 속성을 가짐. 대미지를 받으면 속성에 따라 속성 스택이 쌓이고 최대 스택이 됐을 때 상태 이상 부여, 지속 시간 관리.
+ 속성을 가지는 몬스터 스폰 시 맵 한 가운데 카운터 속성 원소 아이템 스폰, 상성에 따라 받는 대미지, 입히는 대미지가 달라짐.
+ 스탯은 구조체로 관리. 
```c++
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
FHealthManager HealthManager;
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
FCombatManager CombatManager;
```
+ 현재 캐릭터의 상태는 비트마스크로 관리.
```c++
//StatManage.h
UENUM(Meta = (Bitflags))
enum class EState : uint8
{
    Default			= 0,
    Burn			= 1 << 0,
    Frozen			= 1 << 1, 
    Poison			= 1 << 2,
    Fear			= 1 << 3, 
    Dead			= 1 << 4, 
    MAX
};
ENUM_CLASS_FLAGS(EState);
//ManagerComponent.h
UPROPERTY(Meta = (Bitmask, BitmaskEnum = EState))
uint8 CurrentState;
```

### ItemComponent
+ 플레이어만 가지는 아이템을 관리하는 클래스.
+ 플레이어는 스킬이 없고, 아이템을 장착하여 효과를 적용. (패시브 스킬 느낌)
![SelectItem](https://user-images.githubusercontent.com/27758519/229081399-f32bbd25-eb20-4780-9e1d-d43b7f515461.jpg)
+ 영구적으로 스탯을 증가시키는 아이템은 가중치 확률 적용하여 운에 따라 증가 수치 차이가 크도록 구현.
```c++
void UItemComponent::ApplyInfItem(EINFStackItem Item)
{
    URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
    if (GI && GI->GetListenerManager())
    {
        switch (Item)
        {
            case EINFStackItem::IncreaseAtk:
            {
                const TArray<float> Per = { 3.f, 6.f, 9.f, 10.f, 12.f, 30.f, 40.f };
                const TArray<int32> Value = { 9, 8, 7, 6, 5, 3, 2 };
                const float AddToValue = CalcStatValuePer(Per, Value);
                GI->GetListenerManager()->OnUpdateCurrentAtk(AddToValue);
            }
            break;
//...
float UItemComponent::CalcStatValuePer(const TArray<float>& Per, const TArray<int32>& Value)
{
    for (int32 i = 0; i < Per.Num() + 1; ++i)
    {
        const float RandValue = FMath::Rand() % 100 + 1;
        float AddValue = 0.f;
        for (int32 j = 0; j < Per.Num(); ++j)
        {
            AddValue += Per[j];
            if (RandValue <= AddValue)
            {
                return Value[j];
            }
        }
    }
    return 0.f;
}
```

### AI
+ 몬스터의 행동 특성에 따라 BehaviorTree를 다르게 6개로 나누어 구현
+ 기본적인 BehaviorTree <br>
![BT](https://user-images.githubusercontent.com/27758519/229112785-30361361-256c-4919-9d8e-cd41675ceb34.jpg) <br>
+ 한 가운데에서 일정하게 구체를 발사하는 터렛의 BehaviorTree <br>
![TurretBT](https://user-images.githubusercontent.com/27758519/229113254-17c62332-1a10-4c18-9812-6685de4b6095.jpg) <br>
+ 인바이런먼트 쿼리를 적용하여 움직일 위치를 구함. (최적의 위치 혹은 랜덤)
+ 긴 거리를 움직이지 않고, 짧은 거리를 짧은 시간의 텀으로 패트롤을 돌게 하기 위한 인바이런먼트 쿼리.
![FindPatrolLoc](https://user-images.githubusercontent.com/27758519/229114945-036fd137-b8e4-460d-9aed-c15e1b38ee3d.jpg)
