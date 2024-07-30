// Copyright 2024 ChickenGames All rights reserved.


#include "SoulGameplayTags.h"

namespace SoulGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_IsDead, "Ability.ActivateFail.IsDead", "Ability failed to activate because its owner is dead.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cooldown, "Ability.ActivateFail.Cooldown", "Ability failed to activate because it is on cool down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cost, "Ability.ActivateFail.Cost", "Ability failed to activate because it did not pass the cost checks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsBlocked, "Ability.ActivateFail.TagsBlocked", "Ability failed to activate because tags are blocking it.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsMissing, "Ability.ActivateFail.TagsMissing", "Ability failed to activate because tags are missing.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Networking, "Ability.ActivateFail.Networking", "Ability failed to activate because it did not pass the network checks.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Attack_CriticalHit, "Ability.Attack.CriticalHit", "치명적 공격 어빌리티");

	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Damage, "SetByCaller.Damage");
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Damage_Stance, "SetByCaller.Damage.Stance");
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Damage_Poise, "SetByCaller.Damage.Poise");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Movement_Walking, "Status.Movement.Walking", "Movement Waking.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Movement_Sprinting, "Status.Movement.Sprinting", "Movement Sprinting.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Attack, "Status.Attack", "Attack state.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Attack_Charging, "Status.Attack.Charging", "Attack charging state.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Attack_Buffering, "Status.Attack.Buffering", "선입력 구간을 정의한다.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Attack_CriticalHit, "Status.Attack.CriticalHit", "치명적 공격 상태.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Guard, "Status.Guard", "방어중인 상태.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Dead, "Status.Dead", "죽은 상태");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Dying, "Status.Dying", "죽어가는 상태");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Stagger, "Status.Stagger", "맞아서 비틀거림.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_HyperArmor, "Status.HyperArmor", "HyperArmor 상태로 공격 순간에 강인도를 증가시킴.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Collapsing, "Status.Collapsing", "자세 붕괴. 치명타 공격 받을 수 있는 상태.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_GuardBreak, "Status.GuardBreak", "방어시 스테미너 부족으로 인한 자세 붕괴.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_BeCriticalHit, "Status.BeCriticalHit", "치명적인 공격을 맞고 있음.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Action_LockOn, "Status.Action.LockOn", "LockOn State..");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Action_Jumping, "Status.Action.Jumping", "Jumping State..");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Action_Rolling, "Status.Action.Rolling", "Rolling State.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Falling, "Status.Falling", "Falling State..");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Force_LockOnUpdate, "Status.Force.LockOnUpdate", "공격 등에서 현재 상태와 관련 없이 LockOnUpdate를 강행한다.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cancel_Attack, "Cancel.Attack", "공격이 취소됨.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cancel_Attack_WallImpact, "Cancel.Attack.WallImpact", "공격이 벽에 튕겨 취소됨.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cancel_Attack_Stagger, "Cancel.Attack.Stagger", "비틀거려 취소됨.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cancel_Attack_Collapsing, "Cancel.Attack.Collapsing", "자세 붕괴로 취소됨.");

	// UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_BufferAction, "Block.SoulAction", "이 Tag동안 SoulAction의 버퍼를 쌓아놓는다.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Block_SoulAction, "Block.SoulAction", "이 Tag동안 캐릭터는 다른 액션을 실행할 수 없음.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Dead, "Ability.Dead", "죽음 어빌리티 발동용 태그.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Consume_Potion, "Ability.Consume.Potion", "포션 먹는 어빌리티 이벤트.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Consume_LargePotion, "Ability.Consume.LargePotion", "대용량 포션 먹는 어빌리티 이벤트.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Damage, "GameplayCue.Damage", "데미지를 입었을 때의 Cue.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Damage_Guard, "GameplayCue.Damage.Guard", "가드 데미지를 입었을 때의 Cue.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_LockOnTargeted, "Event.LockOnTarget", "로컬에서 락온 받는 대상.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_ConsumeItem, "Event.ConsumeItem", "아이템 사용 이벤트");

	UE_DEFINE_GAMEPLAY_TAG(Input, "Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Right, "Input.Right", "오른손 키.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Right_Normal, "Input.Right.Normal", "오른손 약공격 키.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Right_Strong, "Input.Right.Strong", "오른손 강공격 키.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Left, "Input.Left", "왼손 키.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Left_Normal, "Input.Left.Normal", "왼손 약공격 키.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Left_Strong, "Input.Left.Strong", "왼손 강공격 키.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Combo, "Combo", "콤보 카운트");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Combo_Right_Normal, "Combo.Right.Normal", "콤보 오른손 약공격 키.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Combo_Right_Strong, "Combo.Right.Strong", "콤보 오른손 강공격 키.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Combo_Left_Normal, "Combo.Left.Normal", "콤보 왼손 약공격 키.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Combo_Left_Strong, "Combo.Left.Strong", "콤보 왼손 강공격 키.");
	
	// 공격 불가 상태 정의
	const FGameplayTagContainer InvincibleTags = FGameplayTagContainer::CreateFromArray(
		TArray<FGameplayTag>({Status_BeCriticalHit, Status_Attack_CriticalHit, Status_Action_Rolling}));

	const TMap<FGameplayTag, FGameplayTag> ComboToInputMap = {
		{Input_Right_Normal, Combo_Right_Normal},
		{Input_Right_Strong, Combo_Right_Strong},
		{Input_Left_Normal, Combo_Left_Normal},
		{Input_Left_Strong, Combo_Left_Strong}
	};
}
