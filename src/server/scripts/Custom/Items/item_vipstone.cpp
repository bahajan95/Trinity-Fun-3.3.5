#include "Group.h"
#include "../system/system_megapoint.h"
#include "../system/system_votepoint.h"
#include "../System/system_warlords.h"
#include <ctime>

#define sM  ACE_Singleton<MegaPoint, ACE_Null_Mutex>::instance()
#define sV  ACE_Singleton<VotePoint, ACE_Null_Mutex>::instance()
#define Warlords  ACE_Singleton<Warlords, ACE_Null_Mutex>::instance()

enum spells
{
	POWER_WORD_FORTITUDE = 48162,
	PRAYER_OF_SPRITE = 48074,
	SHADOW_BUFF = 48170,
	KINGS_BUFF = 43223,
	ARCANE_BUFF = 36880,
	MARK_OF_THE_WILD = 48469
};

class item_vipstone : public ItemScript	
{
public:
	item_vipstone() : ItemScript("item_vipstone") {}

	bool OnUse(Player* player, Item* item, SpellCastTargets const& targets)
	{
		std::string hs_requirements;
		hs_requirements = Warlords->GetRequirement(player, REQUIREMENT_HEARTHSTONE);
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM_EXTENDED(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportUnderCity:30:30:-18:0|t|r Teleports", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1, hs_requirements, 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(0, "|cff00ff00|TInterface\\icons\\INV_Misc_GroupNeedMore:30:30:-18:0|t|r Group Tools", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2, hs_requirements, 0, false);
		player->ADD_GOSSIP_ITEM_EXTENDED(0, "|cff00ff00|TInterface\\icons\\INV_Misc_GroupLooking:30:30:-18:0|t|r Player Tools", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3, hs_requirements, 0, false);
		player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t Update My Items Cache", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
		return false;
	}

	void OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		if (player->IsInCombat())
		{
			ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000You are in Combat.|r");
			return;
		}

		switch (action)
		{
		case 1:
			player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportUnderCity:30:30:-18:0|t|r Teleports", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
			player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\INV_Misc_GroupNeedMore:30:30:-18:0|t|r Group Tools", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
			player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\INV_Misc_GroupLooking:30:30:-18:0|t|r Player Tools", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
			player->ADD_GOSSIP_ITEM(0, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|t Update My Items Cache", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
			break;
		case 2:
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 1:
			if (!Warlords->CheckRequirement(player, REQUIREMENT_HEARTHSTONE))
				return;
			player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportDalaran:30:30:-18:0|t|r Dalaran|r", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 101);
			player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportShattrath:30:30:-18:0|t|r Shattath", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 102);
			if (player->GetTeam() == ALLIANCE)
			{
				player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportStormWind:30:30:-18:0|t|r Stormwind City", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 103);
				player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportIronForge:30:30:-18:0|t|r Ironforge", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 104);
				player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportDarnassus:30:30:-18:0|t|r Darnassus", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 105);
				player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportExodar:30:30:-18:0|t|r Exodar", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 106);
			}
			else
			{
				player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportOrgrimmar:30:30:-18:0|t|r Orgrimmar", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 107);
				player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportUnderCity:30:30:-18:0|t|r Undercity", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 108);
				player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportThunderBluff:30:30:-18:0|t|r Thunder Bluf", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 109);
				player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportSilvermoon:30:30:-18:0|t|r Silvermoon City", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 110);
			}
			player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Achievement_Arena_2v2_1:30:30:-18:0|t|r Duel Zone", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 111);
			player->ADD_GOSSIP_ITEM(0, "|cff00ff00|TInterface\\icons\\Achievement_BG_kill_flag_carrierWSG:30:30:-18:0|t|r Gurubashi Arena", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 112);
			player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, 1);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
			break;
		case GOSSIP_ACTION_INFO_DEF + 2:
			if (!Warlords->CheckRequirement(player, REQUIREMENT_HEARTHSTONE))
				return;
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\INV_Misc_GroupNeedMore:30:30:-18:0|t Group Summon", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 201);
			player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, 1);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
			break;
		case GOSSIP_ACTION_INFO_DEF + 3:
			player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Spell_Magic_GreaterBlessingofKings:30:30:-18:0|t|r Buff Me", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 301);
			player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Spell_Nature_UnyeildingStamina:30:30:-18:0|t|r Morphing", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 303);
			player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Ability_Creature_Cursed_02:30:30:-18:0|t|r Reset my Health & Mana", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 304);
			player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Ability_Repair:30:30:-18:0|t|r Repair all my of Items", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 305);
			player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\INV_Box_02:30:30:-18:0|t|r Show my Bank", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 306);
			player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\INV_Inscription_Papyrus:30:30:-18:0|t|r Show my Mailbox", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 307);
			player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Spell_Nature_TimeStop:30:30:-18:0|t|r Show my Online Points", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 309);
			player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Achievement_BG_tophealer_AB:30:30:-18:0|t|r Show my Vote Points", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 310);
			player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Ability_Paladin_BlessedHands:30:30:-18:0|t|r Show my Mega Points", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 308);
			player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, 1);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
			break;
		case GOSSIP_ACTION_INFO_DEF + 4:
		{
			QueryResult result = CharacterDatabase.PQuery("SELECT itemEntry FROM item_instance WHERE owner_guid = '%u'", player->GetGUIDLow());

			do
			{
				Field* field = result->Fetch();

				Warlords->SendPacketItem(player, field[0].GetInt32());
			} while (result->NextRow());
			ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000Your Cache has been updated.|r");
			player->CLOSE_GOSSIP_MENU();
		}
		break;

		case GOSSIP_ACTION_INFO_DEF + 101: // Dalaran
			if (player->GetTeam() == ALLIANCE)
			{
				player->TeleportTo(571, 5640.906f, 687.643f, 651.993f, 5.8623f);
			}
			else
			{
				player->TeleportTo(571, 5966.961f, 613.913f, 650.627f, 2.75112f);
			}
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 102: // Shattrath
			player->TeleportTo(530, -1838.16f, 5301.79f, -12.428f, 5.9517f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 103: // Stormwind
			player->TeleportTo(0, -9011.671f, 475.135f, 96.46f, 0.612963f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 104: // Ironforge
			player->TeleportTo(0, -5018.959f, -836.3f, 496.661f, 5.403440f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 105: // Darnassus
			player->TeleportTo(1, 9952.002f, 2280.157f, 1341.394f, 1.642987f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 106: // Exodar
			player->TeleportTo(530, -3864.154f, -11742.115f, -107.251f, 2.191258f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 107: // Orgrimmar
			player->TeleportTo(1, 1387.549f, -4368.863f, 27.242f, 0.190812f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 108: // Undercity
			player->TeleportTo(0, 1710.57f, 434.548f, 0.671f, 4.188679f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 109: // Thunderbluf
			player->TeleportTo(1, -1304.065f, 205.513f, 68.681f, 5.06959f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 110: // Silvermoon
			player->TeleportTo(530, 9393.694f, -7278.413f, 14.24f, 6.274735f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 111: // DuelZone
			player->TeleportTo(571, 5831.176f, 673.433f, 609.886f, 4.039404f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 112: // Gurubashi Arena
			player->TeleportTo(0, -13277.4f, 127.372f, 26.141f, 1.11878f);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 201: // Group Summon
		{
			Group* grp = player->GetGroup();
			if (!grp)
			{
				ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000Your aren't in a Party Group or a Raid Group.|r");
			}
			else
			{
				if (grp->IsLeader(player->GetGUID()))
				{
					for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
					{
						Player* _player = itr->GetSource();
						if (_player)
						{
							if (!_player || !_player->GetSession())
								continue;

							float x, y, z;
							player->GetSession()->GetPlayer()->GetClosePoint(x, y, z, _player->GetObjectSize());
							_player->TeleportTo(player->GetSession()->GetPlayer()->GetMapId(), x, y, z, player->GetSession()->GetPlayer()->GetOrientation());
							_player->SetPhaseMask(_player->GetSession()->GetPlayer()->GetPhaseMask(), true);
							ChatHandler(_player->GetSession()).PSendSysMessage("|cffFF0000You're Summoned by Leader of Group.|r.");
						}
					}
				}
				else
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000Your aren't leader of this group.|r");
				}
			}
			player->CLOSE_GOSSIP_MENU();
		}break;
		case GOSSIP_ACTION_INFO_DEF + 301:
		{
			if (!Warlords->CheckRequirement(player, REQUIREMENT_HEARTHSTONE))
				return;
			switch (player->getClass())
			{
				case CLASS_WARRIOR:
					player->CastSpell(player, POWER_WORD_FORTITUDE, true);
					player->CastSpell(player, KINGS_BUFF, true);
					player->CastSpell(player, MARK_OF_THE_WILD, true);
					player->CastSpell(player, PRAYER_OF_SPRITE, true);
					player->CastSpell(player, SHADOW_BUFF, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000You are now buffed!.|r");
					break;

				case CLASS_PALADIN:
					player->CastSpell(player, POWER_WORD_FORTITUDE, true);
					player->CastSpell(player, KINGS_BUFF, true);
					player->CastSpell(player, MARK_OF_THE_WILD, true);
					player->CastSpell(player, PRAYER_OF_SPRITE, true);
					player->CastSpell(player, ARCANE_BUFF, true);
					player->CastSpell(player, SHADOW_BUFF, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000You are now buffed!.|r");
					break;

				case CLASS_HUNTER:
					player->CastSpell(player, POWER_WORD_FORTITUDE, true);
					player->CastSpell(player, KINGS_BUFF, true);
					player->CastSpell(player, MARK_OF_THE_WILD, true);
					player->CastSpell(player, PRAYER_OF_SPRITE, true);
					player->CastSpell(player, ARCANE_BUFF, true);
					player->CastSpell(player, SHADOW_BUFF, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000You are now buffed!.|r");
					break;

				case CLASS_ROGUE:
					player->CastSpell(player, POWER_WORD_FORTITUDE, true);
					player->CastSpell(player, KINGS_BUFF, true);
					player->CastSpell(player, MARK_OF_THE_WILD, true);
					player->CastSpell(player, PRAYER_OF_SPRITE, true);
					player->CastSpell(player, SHADOW_BUFF, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000You are now buffed!.|r");
					break;

				case CLASS_PRIEST:
					player->CastSpell(player, POWER_WORD_FORTITUDE, true);
					player->CastSpell(player, KINGS_BUFF, true);
					player->CastSpell(player, MARK_OF_THE_WILD, true);
					player->CastSpell(player, PRAYER_OF_SPRITE, true);
					player->CastSpell(player, ARCANE_BUFF, true);
					player->CastSpell(player, SHADOW_BUFF, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000You are now buffed!.|r");
					break;

				case CLASS_DEATH_KNIGHT:
					player->CastSpell(player, POWER_WORD_FORTITUDE, true);
					player->CastSpell(player, KINGS_BUFF, true);
					player->CastSpell(player, MARK_OF_THE_WILD, true);
					player->CastSpell(player, PRAYER_OF_SPRITE, true);
					player->CastSpell(player, SHADOW_BUFF, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000You are now buffed!.|r");
					break;

				case CLASS_SHAMAN:
					player->CastSpell(player, POWER_WORD_FORTITUDE, true);
					player->CastSpell(player, KINGS_BUFF, true);
					player->CastSpell(player, MARK_OF_THE_WILD, true);
					player->CastSpell(player, PRAYER_OF_SPRITE, true);
					player->CastSpell(player, ARCANE_BUFF, true);
					player->CastSpell(player, SHADOW_BUFF, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000You are now buffed!.|r");
					break;

				case CLASS_MAGE:
					player->CastSpell(player, POWER_WORD_FORTITUDE, true);
					player->CastSpell(player, KINGS_BUFF, true);
					player->CastSpell(player, MARK_OF_THE_WILD, true);
					player->CastSpell(player, PRAYER_OF_SPRITE, true);
					player->CastSpell(player, ARCANE_BUFF, true);
					player->CastSpell(player, SHADOW_BUFF, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000You are now buffed!.|r");
					break;

				case CLASS_WARLOCK:
					player->CastSpell(player, POWER_WORD_FORTITUDE, true);
					player->CastSpell(player, KINGS_BUFF, true);
					player->CastSpell(player, MARK_OF_THE_WILD, true);
					player->CastSpell(player, PRAYER_OF_SPRITE, true);
					player->CastSpell(player, ARCANE_BUFF, true);
					player->CastSpell(player, SHADOW_BUFF, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000You are now buffed!.|r");
					break;

				case CLASS_DRUID:
					player->CastSpell(player, POWER_WORD_FORTITUDE, true);
					player->CastSpell(player, KINGS_BUFF, true);
					player->CastSpell(player, MARK_OF_THE_WILD, true);
					player->CastSpell(player, PRAYER_OF_SPRITE, true);
					player->CastSpell(player, ARCANE_BUFF, true);
					player->CastSpell(player, SHADOW_BUFF, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000You are now buffed!.|r");
					break;
			}
			player->CLOSE_GOSSIP_MENU();
		} break;
		case GOSSIP_ACTION_INFO_DEF + 303:
			if (!Warlords->CheckRequirement(player, REQUIREMENT_HEARTHSTONE))
				return;
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Boss_YoggSaron_01:30:30:-18:0|t Broken", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 311);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Boss_KelidanTheBreaker:30:30:-18:0|t Fel Orc", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 312);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Bloodelf_Male:30:30:-18:0|t Blood Elf(Male)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 313);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Bloodelf_Female:30:30:-18:0|t Blood Elf(Female)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 314);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Draenei_Female:30:30:-18:0|t Draeni", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 315);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Dwarf_Male:30:30:-18:0|t Dwarf", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 316);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Gnome_Male:30:30:-18:0|t Gnome(Male)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 317);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Gnome_Female:30:30:-18:0|t Gnome(Female)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 318);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_BG_captureflag_WSG:30:30:-18:0|t Goblin(Male)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 319);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_BG_captureflag_EOS:30:30:-18:0|t Goblin(Female)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 320);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Human_Male:30:30:-18:0|t Human(Male)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 321);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Human_Female:30:30:-18:0|t Human(Female)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 322);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Nightelf_Male:30:30:-18:0|t Night Elf", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 323);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Orc_Female:30:30:-18:0|t Orc", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 324);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Tauren_Male:30:30:-18:0|t Tauren(Male)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 325);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Tauren_Female:30:30:-18:0|t Tauren(Female)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 327);
			player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\Achievement_Character_Troll_Male:30:30:-18:0|t Troll", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 326);
			player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
			break;
		case GOSSIP_ACTION_INFO_DEF + 311: // Broken
			player->SetDisplayId(21105);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 312: // Fel Orc
			player->SetDisplayId(21267);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 313: // Blood Elf (Male)
			player->SetDisplayId(20578);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 314: // Blood Elf (Female)
			player->SetDisplayId(20579);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 315: // Draenie
			player->SetDisplayId(20323);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 316: // Dwarf
			player->SetDisplayId(20317);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 317: // Gnome (Male)
			player->SetDisplayId(20580);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 318: // Gnome (Female)
			player->SetDisplayId(20581);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 319: // Goblin (Male)
			player->SetDisplayId(20582);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 320: // Goblin (Female)
			player->SetDisplayId(20583);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 321: // Human (Male)
			player->SetDisplayId(19723);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 322: // Human (Female)
			player->SetDisplayId(19724);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 323: // Night Elf
			player->SetDisplayId(20318);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 324: // Orc
			player->SetDisplayId(20316);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 325: // Tauren (Male)
			player->SetDisplayId(20584);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 326: // Tauren (Female)
			player->SetDisplayId(20585);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 327: // Troll
			player->SetDisplayId(20321);
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 304:
			if (!Warlords->CheckRequirement(player, REQUIREMENT_HEARTHSTONE))
				return;
			player->SetHealth(player->GetMaxHealth());
			player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
			ChatHandler(player->GetSession()).PSendSysMessage("|cff00FF00Your HP & Mana has been reseted.|r");
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 305:
			if (!Warlords->CheckRequirement(player, REQUIREMENT_HEARTHSTONE))
				return;
			player->DurabilityRepairAll(false, false, false);
			ChatHandler(player->GetSession()).PSendSysMessage("|cff00FF00Your Items has been repaired.|r");
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 306:
			player->GetSession()->SendShowBank(player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 307:
			player->GetSession()->SendShowMailBox(player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			break;
		case GOSSIP_ACTION_INFO_DEF + 309:
		{
			std::stringstream buffer;
			buffer << "|cff00ff00|TInterface\\icons\\Spell_Nature_TimeStop:30:30:-18:0|t|r Your Online Points: " << player->GetOps();
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, buffer.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 309);
			player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
		} break;
		case GOSSIP_ACTION_INFO_DEF + 310:
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
				votes << "|cff00ff00|TInterface\\icons\\Achievement_BG_tophealer_AB:30:30:-18:0|t|r Your Vote Points: " << sV->GetPlayerVPs(player);
				std::stringstream total_votes;
				total_votes << "|cff00ff00|TInterface\\icons\\Achievement_BG_tophealer_AB:30:30:-18:0|t|r Your Total Vote Points: " << sV->GetPlayerTotalVPs(player);
				std::stringstream lvp;
				lvp << "Last Vote |CFFFF0000->|r ";
				if (Day == 0 && Hour == 0 && Min == 0)
				{
					lvp << "|cffff0000Now !";
				}
				else
				{
					if (Day != 0)
						lvp << "|cff00ccff" << Day << "|rd ";
					if (Hour != 0)
						lvp << "|cff00ccff" << Hour << "|rh ";
					if (Min != 0)
						lvp << "|cff00ccff" << Min << "|rm ";
					lvp << "Ago";
				}

				std::stringstream nextvp;
				nextvp << "Next vote |CFFFF0000->|r ";

				uint32 nextvote = last_vote + votetime + 1;

				if (unix > nextvote)
				{
					nextvp << " |cffff0000Now !";
					nextvp << unix;
				}
				else
				{
					if (Day_Next != 0)
						nextvp << "|cff00ccff" << Day_Next << "|rd ";
					if (Hour_Next != 0)
						nextvp << "|cff00ccff" << Hour_Next << "|rh ";
					if (Min_Next != 0)
						nextvp << "|cff00ccff" << Min_Next << "|rm ";
					nextvp << " Later";
				}

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, votes.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 310);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, total_votes.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 310);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, lvp.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 310);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, nextvp.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 310);
			}
			else
			{
				player->ADD_GOSSIP_ITEM(10, "There is no Items to display.", GOSSIP_SENDER_MAIN, 2);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
			}
			player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
		} break;
		case GOSSIP_ACTION_INFO_DEF + 308:
		{
			std::stringstream buffer;
			buffer << "|cff00ff00|TInterface\\icons\\Ability_Paladin_BlessedHands:30:30:-18:0|t|r Your Mega Points: " << sM->GetPlayerMps(player);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, buffer.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 308);
			player->ADD_GOSSIP_ITEM(10, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);


			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
		} break;
		}
	};
};
void AddSC_item_vipstone()
{
	new item_vipstone();
}