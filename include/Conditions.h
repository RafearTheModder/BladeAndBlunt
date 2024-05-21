#pragma once
#include "Cache.h"
#include "Settings.h"

//Originally intended to just implement some condition functions but iv been placing extensions/utility here as well
namespace Conditions
{ 
	static bool IsAttacking(RE::Actor* actor)
	{
		using func_t = decltype(&Conditions::IsAttacking);
		REL::Relocation<func_t> func{ Cache::IsAttackingAddress };
		return func(actor);
	}

	inline static REL::Relocation<decltype(IsAttacking)> _IsAttacking;

	static bool IsBlocking(RE::Actor* actor)
	{
		using func_t = decltype(&Conditions::IsBlocking);
		REL::Relocation<func_t> func{ Cache::IsBlockingAddress };
		return func(actor);
	}

	inline static REL::Relocation<decltype(IsBlocking)> _IsBlocking;

	static bool HasSpell(RE::Actor* actor, RE::SpellItem* spell)
	{	
		using func_t = decltype(&Conditions::HasSpell);

		REL::Relocation<func_t> func{ Cache::HasSpellAddress };

		return func(actor, spell);
	}
	inline static REL::Relocation<decltype(HasSpell)> _HasSpell;


	static bool IsMoving(RE::PlayerCharacter* player)
	{
		auto playerState = player->AsActorState();
		return (static_cast<bool>(playerState->actorState1.movingForward) || static_cast<bool>(playerState->actorState1.movingBack) || static_cast<bool>(playerState->actorState1.movingLeft) || static_cast<bool>(playerState->actorState1.movingRight));
	}

	static RE::TESObjectWEAP* GetUnarmedWeapon()
	{
		auto** singleton{ reinterpret_cast<RE::TESObjectWEAP**>(Cache::getUnarmedWeaponAddress) };
		return *singleton;
	}

	static inline bool PlayerHasActiveMagicEffect(RE::EffectSetting* a_effect)
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		if (!REL::Module::IsVR()) {
			auto activeEffects = player->AsMagicTarget()->GetActiveEffectList();
			RE::EffectSetting* setting = nullptr;
			for (auto& effect : *activeEffects) {
				setting = effect ? effect->GetBaseObject() : nullptr;
				if (setting) {
					if (setting == a_effect) {
						return true;
					}
				}
			}
		}
		else
		{
			bool matchFound = false;
			player->AsMagicTarget()->VisitActiveEffects([&](RE::ActiveEffect* effect) -> RE::BSContainer::ForEachResult {
				RE::EffectSetting* setting = nullptr;
				if (effect){
					setting = effect ? effect->GetBaseObject() : nullptr;
					if (setting) {
						if (setting == a_effect) {
							matchFound = true;
						}
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
			return matchFound;
		}
		
		return false;
	}

	static bool IsSurvivalEnabled() {
		auto settings = Settings::GetSingleton();
		if (!settings || !settings->Survival_ModeEnabled) {
			return false;
		} 

		if (settings->Survival_ModeEnabled->value == 1.0) {
			return true;
		} else {
			return false;
		}
	}

    static inline bool IsPowerAttacking(RE::Actor* actor) {
        if (auto high = actor->GetHighProcess()) {
            if (const auto attackData = high->attackData) {
                auto flags = attackData->data.flags;

                if (flags && flags.any(RE::AttackData::AttackFlag::kPowerAttack)) {
                    return true;
                }
            }
        }
        return false;
    }

	static inline float GetMaxHealth()
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		return player->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, RE::ActorValue::kHealth) +
		       player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kHealth);
	}

	//Credit: KernalsEgg for ApplySpell and IsPermanent
	//extensions
	static bool IsPermanent(RE::MagicItem* item)
	{
		switch (item->GetSpellType()) {
		case RE::MagicSystem::SpellType::kDisease:
		case RE::MagicSystem::SpellType::kAbility:
		case RE::MagicSystem::SpellType::kAddiction:
			{
				return true;
			}
		default:
			{
				return false;
			}
		}
	}
	
	static inline void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell)
	{
		if (IsPermanent(spell)) {
			target->AddSpell(spell);
		} else {
			caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
		}
	}
};
