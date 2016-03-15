#include "ScriptPCH.h"
#include "../System/system_warlords.h"

#define Warlords  ACE_Singleton<Warlords, ACE_Null_Mutex>::instance()

class npc_trainer : public CreatureScript
{
public:
	npc_trainer() : CreatureScript("npc_trainer") { }
	uint32 typeas;
	bool OnGossipHello(Player * player, Creature * creature)
	{
		uint32 classid;
		classid = player->getClass();
		std::string spell_requirements;
		spell_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_SPELL);
		player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_TRAINER, "|TInterface/ICONS/Spell_Holy_SurgeOfLight:30:30:-18:0|t Train me!", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_SPELL, classid), spell_requirements, 0, false);

		std::string talent_requirements;
		talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "|TInterface/ICONS/Ability_Marksmanship:30:30:-18:0|t Talent Template", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_TALENT, classid));
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 melt)
	{
		player->PlayerTalkClass->ClearMenus();

		uint8 menu, action;
		Warlords->Unmelt(melt, menu, action);

		std::string talent_requirements;
		talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);

		switch (menu)
		{
		case MAIN_MENU: OnGossipHello(player, creature); break;
		case LEARN_SPELL:
		{
			//action = id
			if (Warlords->CheckRequirement(player, REGUIREMENT_LEARN_SPELL))
			{
				uint32 id_class = action;
				int32 spell_id, required_level;
				QueryResult SpellQuery = WorldDatabase.PQuery("SELECT `spell`, `requiredlevel` FROM `custom_trainer` WHERE `class` = '%u' OR `class` = 0", id_class);
				do {
					Field *SpellFields = SpellQuery->Fetch();

					spell_id = SpellFields[0].GetInt32();
					required_level = SpellFields[1].GetInt32();
					if (!player->HasSpell(spell_id) && player->getLevel() >= required_level)
					{
						player->learnSpell(spell_id, false);
						ChatHandler(player->GetSession()).PSendSysMessage("|cff00FF00Spell Learned.|r");
					}
				} while (SpellQuery->NextRow());

				player->PlayerTalkClass->SendCloseGossip();
			}
		}
			break;
		case SHOW_TALENT:
			//action = id
			switch (action)
			{
			case CLASS_WARRIOR:
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Rogue_Eviscerate:30:30:-18:0|t Arms", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 1), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Warrior_InnerRage:30:30:-18:0|t Fury", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 2), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Warrior_DefensiveStance:30:30:-18:0|t Protection", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 3), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
					typeas = 3;
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case CLASS_PALADIN:
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_HolyBolt:30:30:-18:0|t Holy", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 4), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/SPELL_HOLY_DEVOTIONAURA:30:30:-18:0|t Protection", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 5), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_AuraOfLight:30:30:-18:0|t Retribution", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 6), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
					typeas = 3;
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case CLASS_ROGUE:
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Rogue_Eviscerate:30:30:-18:0|t Assassination", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 7), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_BackStab:30:30:-18:0|t Combat", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 8), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Stealth:30:30:-18:0|t Subtley", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 9), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
					typeas = 3;
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case CLASS_HUNTER:
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Hunter_BeastTaming:30:30:-18:0|t Beast Mastery", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 10), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Marksmanship:30:30:-18:0|t Marksmanship", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 11), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Hunter_SwiftStrike:30:30:-18:0|t Survival", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 12), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
					typeas = 3;
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case CLASS_MAGE:
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_MagicalSentry:30:30:-18:0|t Arcane", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 13), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Frost_FrostBolt02:30:30:-18:0|t Frost", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 14), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Fire_FlameBolt:30:30:-18:0|t Fire", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 15), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
					typeas = 3;
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case CLASS_WARLOCK:
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Shadow_DeathCoil:30:30:-18:0|t Afflication", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 16), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Shadow_Metamorphosis:30:30:-18:0|t Demonology", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 17), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Shadow_RainOfFire:30:30:-18:0|t Destruction", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 18), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
					typeas = 3;
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case CLASS_PRIEST:
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_WordFortitude:30:30:-18:0|t Descipline", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 19), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_HolyBolt:30:30:-18:0|t Holy", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 20), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Shadow_ShadowWordPain:30:30:-18:0|t Shadow Magic", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 21), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
					typeas = 3;
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case CLASS_SHAMAN:
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_Lightning:30:30:-18:0|t Elemental Combat", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 22), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_LightningShield:30:30:-18:0|t Enhancement", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 23), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_MagicImmunity:30:30:-18:0|t Restoration", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 24), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
					typeas = 3;
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case CLASS_DEATH_KNIGHT:
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Deathknight_BloodPresence:30:30:-18:0|t Blood", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 25), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Deathknight_FrostPresence:30:30:-18:0|t Frost", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 26), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Deathknight_UnholyPresence:30:30:-18:0|t Unholy", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 27), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
					typeas = 3;
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			case CLASS_DRUID:
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_StarFall:30:30:-18:0|t Balance", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 28), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Racial_BearForm:30:30:-18:0|t Feral Combat", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 29), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_HealingTouch:30:30:-18:0|t Restoration", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_TALENT, 30), talent_requirements, 0, false);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
					typeas = 3;
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			}
			break;
			case LEARN_TALENT:
			{
				if (Warlords->CheckRequirement(player, REGUIREMENT_LEARN_TALENT))
				{
					uint32 spec_id = action;
					int32 spec, talent, rank;
					QueryResult TalentQuery = WorldDatabase.PQuery("SELECT `spec`, `talent`, `rank` FROM `custom_talent_templates` WHERE `spec` = '%u' ORDER BY `ordination` ASC", spec_id);
					player->resetTalents();
					do {
						Field *TalentFields = TalentQuery->Fetch();

						spec = TalentFields[0].GetInt32();
						talent = TalentFields[1].GetInt32();
						rank = TalentFields[2].GetInt32();
						rank += 1;
						for (size_t i = 0; i < rank; i++)
						{
							player->LearnTalent(talent, i);
						}

					} while (TalentQuery->NextRow());

					player->PlayerTalkClass->SendCloseGossip();
				}
			}
			break;
		}
		return true;
	}
};

void AddSC_npc_trainer()
{
	new npc_trainer;
}