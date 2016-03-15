#include "ScriptPCH.h"
#include "../system/system_megapoint.h"
#include "../system/system_votepoint.h"
#include "../System/system_warlords.h"
#include <ctime>

#define sM  ACE_Singleton<MegaPoint, ACE_Null_Mutex>::instance()
#define sV  ACE_Singleton<VotePoint, ACE_Null_Mutex>::instance()
#define Warlords  ACE_Singleton<Warlords, ACE_Null_Mutex>::instance()

enum Melt {
	NONE,
	SHOW_BUFF,
	APPLY_BUFF,
	CLEAR_CACHE,
	SHOW_MORPH,
	APPLY_MORPH,
	RESET_HP_MANA,
	REPAIR_ITEM,
	SHOW_BANK,
	SHOW_MAILBOX,
	SHOW_AUCTIONEER,
	SHOW_OPS,
	SHOW_VPS,
	SHOW_MPS,
	SHOW_MAXSKILL_WEAPON,
	SHOW_REMOVE_COOLDOWN,
	SHOW_REMOVE_INCTANCE,
};

class npc_tools : public CreatureScript
{
public:
	npc_tools() : CreatureScript("npc_tools") { }
	bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Magic_GreaterBlessingofKings:30:30:-18:0|t Buff Me", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_BUFF, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t Update My Items Cache", GOSSIP_SENDER_MAIN, Warlords->Melt(CLEAR_CACHE, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Nature_UnyeildingStamina:30:30:-18:0|t Morphing", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_MORPH, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Ability_Creature_Cursed_02:30:30:-18:0|t Reset my Health & Mana", GOSSIP_SENDER_MAIN, Warlords->Melt(RESET_HP_MANA, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Ability_Repair:30:30:-18:0|t Repair all my of Items", GOSSIP_SENDER_MAIN, Warlords->Melt(REPAIR_ITEM, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Ability_Warrior_SecondWind:30:30:-18:0|t Maxskill Weapon", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_MAXSKILL_WEAPON, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Ability_Hunter_Readiness:30:30:-18:0|t Remove Cooldown", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_REMOVE_COOLDOWN, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/INV_Box_02:30:30:-18:0|t Show my Bank", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_BANK, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/INV_Inscription_Papyrus:30:30:-18:0|t Show my Mailbox", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_MAILBOX, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Trade_BlackSmithing:30:30:-18:0|t Show Auctioneer", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_AUCTIONEER, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Spell_Nature_TimeStop:30:30:-18:0|t Show my Online Points", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_OPS, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Achievement_BG_tophealer_AB:30:30:-18:0|t Show my Vote Points", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_VPS, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Ability_Paladin_BlessedHands:30:30:-18:0|t Show my Mega Points", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_MPS, 0));
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/Achievement_Dungeon_UlduarRaid_Misc_06:30:30:-18:0|t Remove All Instance Save", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_REMOVE_INCTANCE, 0));
		player->SEND_GOSSIP_MENU("|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|tTools:", creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 melt) OVERRIDE
	{
		player->PlayerTalkClass->ClearMenus();
		if (player->IsInCombat())
		{
			ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000You are in Combat.|r");
			return true;
		}

		uint8 menu, action;
		Warlords->Unmelt(melt, menu, action);

		if (menu == MAIN_MENU)
			OnGossipHello(player, creature);
		else if (menu == SHOW_BUFF)
		{
			QueryResult Result = WorldDatabase.Query("SELECT `id`, `spell`, `castspell`, `require_id` FROM `custom_buff`");
			if (!Result)
			{
				ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000There is no buff to apply.|r");
			}
			else if (Result)
			{
				uint32 id, spell, castspell, require_id;
				std::string name, require_text;
				Field* Fields = Result->Fetch();
				do
				{
					id = Fields[0].GetInt32();
					spell = Fields[1].GetInt32();
					castspell = Fields[2].GetInt32();
					require_id = Fields[3].GetInt16();

					SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell);
					int locale = player->GetSession()->GetSessionDbcLocale();
					name = spellInfo->SpellName[locale];
					require_text = Warlords->GetRequirement(player, require_id);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, name.c_str(), GOSSIP_SENDER_MAIN, Warlords->Melt(APPLY_BUFF, id));
				} while (Result->NextRow());
				player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
			}
		}
		else if (menu == APPLY_BUFF)
		{
			uint32 id_spell = action, id, spell, castspell, require_id;
			QueryResult TextQuery = WorldDatabase.PQuery("SELECT `id`, `spell`, `castspell`, `require_id` FROM `custom_buff` WHERE `id` = '%u'", id_spell);
			Field *TextFields = TextQuery->Fetch();
			id = TextFields[0].GetInt32();
			spell = TextFields[1].GetInt32();
			castspell = TextFields[2].GetInt32();
			require_id = TextFields[3].GetUInt32();
			if (!require_id)
			{
				player->CastSpell(player, castspell, false);
				player->AddAura(spell, player);
				ChatHandler(player->GetSession()).PSendSysMessage("the buff has been applied");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				if (Warlords->CheckRequirement(player, require_id))
				{
					player->CastSpell(player, castspell, false);
					player->AddAura(spell, player);
					ChatHandler(player->GetSession()).PSendSysMessage("the buff has been applied");
					player->CLOSE_GOSSIP_MENU();
				}
			}
		}
		else if (menu == CLEAR_CACHE)
		{
			QueryResult result = CharacterDatabase.PQuery("SELECT itemEntry FROM item_instance WHERE owner_guid = '%u'", player->GetGUIDLow());
			do
			{
				Field* field = result->Fetch();
				Warlords->SendPacketItem(player, field[0].GetInt32());
			} while (result->NextRow());
			ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000Your Cache has been updated.|r");
		}
		else if (menu == SHOW_MORPH)
		{
			QueryResult Result = WorldDatabase.Query("SELECT `id`, `name`, `scale`, `displayid`, `require_id` FROM `custom_morph`");
			if (!Result)
			{
				ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000There is no morph to apply.|r");
			}
			else if (Result)
			{
				uint32 id, scale, displayid, require_id;
				std::string name;
				Field* Fields = Result->Fetch();
				do
				{
					id = Fields[0].GetInt32();
					name = Fields[1].GetString();
					scale = Fields[2].GetInt32();
					displayid = Fields[3].GetInt32();
					require_id = Fields[4].GetInt16();

					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, name.c_str(), GOSSIP_SENDER_MAIN, Warlords->Melt(APPLY_MORPH, id));
					player->CLOSE_GOSSIP_MENU();
				} while (Result->NextRow());
			}

			player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		}
		else if (menu == APPLY_MORPH)
		{
			uint32 id_morph = action, id, scale, displayid, require_id;
			std::string name;
			QueryResult TextQuery = WorldDatabase.PQuery("SELECT `id`, `name`, `scale`, `displayid`, `require_id` FROM `custom_morph` WHERE `id` = '%u'", id_morph);
			Field *TextFields = TextQuery->Fetch();
			id = TextFields[0].GetInt32();
			name = TextFields[1].GetString();
			scale = TextFields[2].GetInt32();
			displayid = TextFields[3].GetInt32();
			require_id = TextFields[4].GetUInt32();
			if (!require_id)
			{
				player->SetDisplayId(displayid);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, scale);
				ChatHandler(player->GetSession()).PSendSysMessage("Your display has been changed.");
				player->CLOSE_GOSSIP_MENU();
			}
			else
			{
				if (Warlords->CheckRequirement(player, require_id))
				{
					player->SetDisplayId(displayid);
					player->SetFloatValue(OBJECT_FIELD_SCALE_X, scale);
					ChatHandler(player->GetSession()).PSendSysMessage("Your display has been changed.");
					player->CLOSE_GOSSIP_MENU();
				}
			}
		}
		else if (menu == RESET_HP_MANA)
		{
			player->SetHealth(player->GetMaxHealth());
			player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
			ChatHandler(player->GetSession()).PSendSysMessage("|cff00FF00Your Health & Mana has been reset.|r");
			player->CLOSE_GOSSIP_MENU();
		}
		else if (menu == REPAIR_ITEM)
		{
			player->DurabilityRepairAll(false, false, false);
			ChatHandler(player->GetSession()).PSendSysMessage("|cff00FF00Your items has been repaired.|r");
			player->CLOSE_GOSSIP_MENU();
		}
		else if (menu == SHOW_BANK)
		{
			player->GetSession()->SendShowBank(player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
		}
		else if (menu == SHOW_MAILBOX)
		{
			player->GetSession()->SendShowMailBox(player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
		}
		else if (menu == SHOW_AUCTIONEER)
		{
			player->GetSession()->SendAuctionHello(player->GetGUID(), creature);
			player->CLOSE_GOSSIP_MENU();
		}
		else if (menu == SHOW_OPS)
		{
			std::stringstream buffer;
			buffer << "|cff00ff00|TInterface/ICONS/Spell_Nature_TimeStop:30:30:-18:0|t|r Your Online Points: " << player->GetOps();
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, buffer.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
			player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		}
		else if (menu == SHOW_VPS)
		{
			uint32 accountid;
			accountid = player->GetSession()->GetAccountId();

			QueryResult Result = LoginDatabase.PQuery("SELECT last_vote FROM account_votepoint WHERE id = %u", accountid);
			if (Result)
			{
				Field* Fields = Result->Fetch();

				uint32 last_vote = Fields[0].GetUInt32();

				std::time_t unix = std::time(nullptr);
				uint32 Time = unix - last_vote;

				uint32 votetime = 86400;
				uint32 nextvote_Time = votetime - Time;

				uint32 Day = floor(Time / 86400);
				Time = Time - (Day * 86400);
				uint32 Hour = floor(Time / 3600);
				Time = Time - (Hour * 3600);
				uint32 Min = floor(Time / 60);

				uint32 Day_Next = floor(nextvote_Time / 86400);
				nextvote_Time = nextvote_Time - (Day_Next * 86400);
				uint32 Hour_Next = floor(nextvote_Time / 3600);
				nextvote_Time = nextvote_Time - (Hour_Next * 3600);
				uint32 Min_Next = floor(nextvote_Time / 60);

				std::stringstream votes;
				votes << "|cff00ff00|TInterface/ICONS/Achievement_BG_tophealer_AB:30:30:-18:0|t|r Your Vote Points: " << sV->GetPlayerVPs(player);
				std::stringstream total_votes;
				total_votes << "|cff00ff00|TInterface/ICONS/Achievement_BG_tophealer_AB:30:30:-18:0|t|r Your Total Vote Points: " << sV->GetPlayerTotalVPs(player);
				std::stringstream lvp;
				lvp << "Last Vote |CFFFF0000->|r ";
				if (Day != 0)
					lvp << "Days: |cff00ccff" << Day << "|r ";
				if (Hour != 0)
					lvp << "Hours: |cff00ccff" << Hour << "|r ";
				if (Min != 0)
					lvp << "Minutes: |cff00ccff" << Min << "|r ";
				lvp << "Ago";

				std::stringstream nextvp;
				nextvp << "To Next Vote |CFFFF0000->|r ";
				if (Day_Next != 0)
					nextvp << "Days: |cff00ccff" << Day_Next << "|r ";
				if (Hour_Next != 0)
					nextvp << "Hours: |cff00ccff" << Hour_Next << "|r ";
				if (Min_Next != 0)
					nextvp << "Minutes: |cff00ccff" << Min_Next << "|r ";
				nextvp << " Remain";

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, votes.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, total_votes.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, lvp.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, nextvp.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
			}
			else
			{
				player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface/ICONS/Achievement_BG_tophealer_AB:30:30:-18:0|t|r Your Vote Points: 0", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
			}
			player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		}
		else if (menu == SHOW_MPS)
		{
			std::stringstream buffer;
			buffer << "|cff00ff00|TInterface/ICONS/Ability_Paladin_BlessedHands:30:30:-18:0|t|r Your Mega Points: " << sM->GetPlayerMps(player);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, buffer.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
			player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		}
		else if (menu == SHOW_MAXSKILL_WEAPON)
		{
			player->UpdateSkillsToMaxSkillsForLevel();
			player->PlayerTalkClass->SendCloseGossip();
			ChatHandler(player->GetSession()).PSendSysMessage("|cddff0000[GoldenGame]:|r Maxskill Weapon Shoma Anjam Shod", player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
		}
		else if (menu == SHOW_REMOVE_COOLDOWN)
		{
			player->RemoveAurasDueToSpell(15007);
			player->PlayerTalkClass->SendCloseGossip();
			ChatHandler(player->GetSession()).PSendSysMessage("|cddff0000[GoldenGame]:|r Remove Cooldown Shoma Anjam Shod", player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
		}
		return true;
	}
};


void AddSC_npc_tools()
{
	new npc_tools();
}