// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "BD_PlayerState.generated.h"

class UBD_AbilitySystemComponent;
class UAbilitySystemComponent;
class ABD_PlayerController;
class UBD_PlayerClassData;

/**
 * - `void AddExperience(float ExpAmount)`
- `void LevelUp()`

| **멤버 함수** | **역할 및 설명** |
| --- | --- |
| `AddExperience(float)` | 경험치 획득. LevelUpXPCurve를 참조하여 임계치 초과 시 LevelUp() 호출. UI update |
| `LevelUp()` | PlayerLevel 증가, 스탯 재조정, 스킬 포인트(CurrentSP) 지급 |
- PlayerLevel 관련
    - int32 PlayerLevel
    - float CurrentXP
- USkillTreeComponent
    - `int32` CurrentSP (보유 스킬 포인트)
    - `bool LearnSkill(FGameplayTag)` (특정 레벨 달성 시 스킬 자동 습득) ⇒ GiveAbility로 skill부여하기
    - `bool UpgradeSkill(FGameplayTag)` (해당되는 스킬 레벨 증가)
    - `void ResetSkillTree()` (모든 Skill들을 초기화, 일정 레벨까지는 무제한 초기화 가능)
    
    | **멤버 함수** | **역할 및 설명** |
    | --- | --- |
    | `LearnSkill(SkillTag)` | 특정 레벨 도달 시 조건 검사 후 ASC의 GiveAbility를 통해 1레벨 스킬 부여 |
    | `UpgradeSkill(SkillTag)` | UI에서 Button Event와 연결하여 호출. CurrentSP 차감 후, ASC에 등록된 해당 스킬의 Spec Level 증가 |
    | `ResetSkillTree()` | 모든 스킬 레벨 1으로 초기화(ASC) 및 소모된 SP 전액 반환  |
- UQuestManagerComponent
    - `FQuestProgress` Progress
        - TMap<FGameplayTag, int32>
    - `void AcceptQuest(QuestData)`
    - `void UpdateObjective(TargetTag)`
    - `private: bool CheckQuestCompletion(QuestData)`
    - `private: void CompleteQuest(QuestData)`
    
    | **멤버 함수** | **역할 및 설명** |
    | --- | --- |
    | `AcceptQuest(QuestData)` | 신규 퀘스트를 Progress Map에 등록하고 UI 갱신 발송 |
    | `UpdateObjective(TargetTag)` | 몬스터 킬/아이템 획득 이벤트(예>Event.Quest.MonsterKilled) 수신 시, 진행 중인 퀘스트의 해당 태그 Count 증가 |
    | `CheckQuestCompletion(QuestData)` | 목표(RequiredCount) 모두 달성 시 CompleteQuest 호출 |
    | `CompleteQuest(QuestData)` | 보상 지급(경험치, 돈, 아이템) 및 NextLinkedQuest가 있다면 연계 처리 |
 */
UCLASS()
class PROJECT_BLACKDESERT_API ABD_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ABD_PlayerState();

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	UBD_AbilitySystemComponent* GetBDAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    template <class T>
    const T* GetPlayerData() const { return Cast<T>(PlayerData); }
    void SetPlayerData(const UBD_PlayerClassData* InPlayerData);

    UFUNCTION(BlueprintCallable, Category = "PlayerState")
    ABD_PlayerController* GetBDPlayerController() const;
	
    virtual void PreInitializeComponents() override;
    virtual void PostInitializeComponents() override;


private:
    // The ability system component is owned by Playerstate
	UPROPERTY(VisibleAnywhere, Category = "PlayerState")
	TObjectPtr<UBD_AbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, Category = "PlayerState")
    TObjectPtr<const class UBD_HealthAttributeSet> HealthSet;

    UPROPERTY(VisibleAnywhere, Category = "PlayerState")
    TObjectPtr<const class UBD_CombatAttributeSet> CombatSet;

    UPROPERTY()
    TObjectPtr<const UBD_PlayerClassData> PlayerData;
	 
};
