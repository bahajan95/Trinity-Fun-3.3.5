#include "ScriptPCH.h"
#include "Map.h"

/*Buffs - Start*/
#define BUFF_MARK_OF_THE_WILD           48469
#define BUFF_BLESSING_OF_WISDOM         48938
#define BUFF_BLESSING_OF_KINGS          25898
#define BUFF_BLESSING_OF_SANCTUARY      25899
#define BUFF_BLESSING_OF_MIGHT          48932
#define BUFF_POWER_WORD_FORTITUDE       48161
/*Buff - End*/
class Reset_OnDuelEnd : public PlayerScript
{
public:
	Reset_OnDuelEnd() : PlayerScript("Reset_OnDuelEnd") {}

	void OnDuelStart(Player * pobeditel, Player * izgubi)
	{
		if (pobeditel->getPowerType() == POWER_ENERGY)
			pobeditel->SetPower(POWER_ENERGY, pobeditel->GetMaxPower(POWER_ENERGY));
		if (pobeditel->getPowerType() == POWER_RAGE)
			pobeditel->SetPower(POWER_RAGE, 0);
	}

	void OnDuelEnd(Player *pobeditel, Player *izgubi/*, Player *pet* SymbolixDEV */, DuelCompleteType type)
	{
		//Set Max Healt [START] //
		pobeditel->SetHealth(pobeditel->GetMaxHealth());
		izgubi->SetHealth(izgubi->GetMaxHealth());
		if (pobeditel->getPowerType() == POWER_MANA)
			pobeditel->SetPower(POWER_MANA, pobeditel->GetMaxPower(POWER_MANA));
		if (izgubi->getPowerType() == POWER_MANA)
			izgubi->SetPower(POWER_MANA, izgubi->GetMaxPower(POWER_MANA));
		//Set Max Healt [END]
		// Cast And Attack Stop [Start] //
		pobeditel->CastStop();
		izgubi->CastStop();
		pobeditel->AttackStop();
		izgubi->AttackStop();
		// Cast And Attack Stop [END]
		//Combat Stop [START]
		pobeditel->CombatStop();
		izgubi->CombatStop();
		pobeditel->RemoveAura(15007);
		izgubi->RemoveAura(15007);
		pobeditel->CastSpell(pobeditel, BUFF_POWER_WORD_FORTITUDE, false);
		izgubi->CastSpell(pobeditel, BUFF_POWER_WORD_FORTITUDE, false);
		pobeditel->CastSpell(pobeditel, BUFF_BLESSING_OF_KINGS, false);
		izgubi->CastSpell(pobeditel, BUFF_BLESSING_OF_KINGS, false);
		izgubi->ModifyMoney(-0);
		pobeditel->ModifyMoney(+0);
		pobeditel->CastSpell(pobeditel, 11543, 1);
		pobeditel->PlayDirectSound(11530, pobeditel);
		izgubi->DurabilityRepairAll(false, 0, false);
		pobeditel->DurabilityRepairAll(false, 0, false);
		pobeditel->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
		izgubi->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE);
		// Debuffs
	}
};

void AddSC_Reset()
{
	new Reset_OnDuelEnd;
}
