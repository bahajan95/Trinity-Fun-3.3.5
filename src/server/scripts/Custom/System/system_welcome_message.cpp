#include "ScriptPCH.h"
#include "system_megapoint.h"
#include "system_votepoint.h"
#include "system_warlords.h"

#define sM  ACE_Singleton<MegaPoint, ACE_Null_Mutex>::instance()
#define sV  ACE_Singleton<VotePoint, ACE_Null_Mutex>::instance()
#define Warlords  ACE_Singleton<Warlords, ACE_Null_Mutex>::instance()

class welcome_message : public PlayerScript
{
public:
	welcome_message() : PlayerScript("welcome_message") {}
	uint8 type = 1;
	void OnLogin(Player* player)
	{
		if (player->GetTotalPlayedTime() > 10)
		{
			player->PlayerTalkClass->ClearMenus();
			player->ADD_GOSSIP_ITEM(0, "|CFF00A500|TInterface\\icons\\INV_Glyph_MajorDeathKnight:30:30:-18:0|t|r Latest News", GOSSIP_SENDER_MAIN, Warlords->Melt(LATEST_NEWS, 0));
			player->ADD_GOSSIP_ITEM(0, "|CFF00A500|TInterface\\icons\\INV_Glyph_MajorDruid:30:30:-18:0|t|r My Information", GOSSIP_SENDER_MAIN, Warlords->Melt(MY_INFO, 0));
			player->ADD_GOSSIP_ITEM(0, "|CFF00A500|TInterface\\icons\\INV_Glyph_MajorHunter:30:30:-18:0|t|r Server Information", GOSSIP_SENDER_MAIN, Warlords->Melt(SERVER_INFO, 0));
			std::stringstream welcome_msg;
			welcome_msg
				<< "#########################\n"
				<< "#|CFFFF0000" << player->GetName() << "\n"
				<< "|CFF000000######################### \n#"
				<< "\n" << "#|CFF007C17 Website: |CFF000000Www.Yekta-core.ir\n#"
				<< "\n" << "#|CFF007C17 Realm: |CFF00000010.201.59.38\n#"
				<< "\n" << "#|CFF007C17 Online Points: |CFF000000 " << player->GetOps() << "\n#"
				<< "\n" << "#|CFF007C17 Mega Points: |CFF000000 " << sM->GetPlayerMps(player) << "\n#"
				<< "\n" << "#|CFF007C17 Vote Points: |CFF000000 " << sV->GetPlayerVPs(player) << "\n#"
				<< "\n" << "#|CFF007C17 Total Vote Points: |CFF000000 " << sV->GetPlayerTotalVPs(player) << "\n#"
				<< "\n" << "#|CFF007C17 Level: |CFF000000 " << player->GetLevelFromDB(player->GetGUID()) << "\n"
				<< "#\n######################### \n";
			player->SEND_GOSSIP_MENU(welcome_msg.str(), player->GetGUID());
		}
		else
		{
			player->PlayerTalkClass->ClearMenus();
			player->ADD_GOSSIP_ITEM(0, "|CFF00A500|TInterface\\icons\\Achievement_Arena_2v2_1:30:30:-18:0|t|r PvP", GOSSIP_SENDER_MAIN, Warlords->Melt(ROLE_PVE, 0));
			std::stringstream msg;
			msg
				<< "#########################\n"
				<< "#|CFFFF0000 Yekta-Core\n"
				<< "|CFF000000######################### \n#"
				<< "\n" << "# Welcome To Yekta-Core\n"
				<< "#\n######################### \n";
			player->SEND_GOSSIP_MENU(msg.str(), player->GetGUID());
		}
	}

	void OnGossipSelect(Player* player, uint32 menu_id, uint32 sender, uint32 melt)
	{
		player->PlayerTalkClass->ClearMenus();
		uint8 menu, action;
		Warlords->Unmelt(melt, menu, action);

		if (menu == MAIN_MENU)
			OnLogin(player);
		else if (menu == LATEST_NEWS)
		{
			QueryResult NewsQuery = WorldDatabase.PQuery("SELECT `id`, `title`, `date` FROM `custom_news`");
			int32 news_count = NewsQuery->GetRowCount();

			if (NewsQuery)
			{
				int32 id;
				std::string title, date;
				do {
					Field *NewsFields = NewsQuery->Fetch();

					id = NewsFields[0].GetInt32();
					title = NewsFields[1].GetString();
					date = NewsFields[2].GetString();

					std::stringstream buffer;
					buffer << title << ". [" << date << "] ";
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, buffer.str(), GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_NEWS, id));
				} while (NewsQuery->NextRow());
				player->ADD_GOSSIP_ITEM(0, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
				std::stringstream news_msg;
				news_msg
					<< "#########################\n"
					<< "#|CFFFF0000 Latest News\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF007C17 News count: |CFF000000" << news_count << "\n"
					<< "#\n######################### \n";
				player->SEND_GOSSIP_MENU(news_msg.str(), player->GetGUID());
			}
			else
			{
				player->ADD_GOSSIP_ITEM(0, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
				std::stringstream nnews_msg;
				nnews_msg
					<< "#########################\n"
					<< "#|CFFFF0000 Latest Server News\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFFFF0000 There is no news !\n"
					<< "#\n######################### \n";
				player->SEND_GOSSIP_MENU(nnews_msg.str(), player->GetGUID());
			}

		}
		else if (menu == SHOW_NEWS)
		{
			uint32 news_id = action;
			std::string text;
			QueryResult TextQuery = WorldDatabase.PQuery("SELECT `text` FROM `custom_news` WHERE `ID` = '%u'", news_id);
			Field *TextFields = TextQuery->Fetch();
			text = TextFields[0].GetString();
			player->SEND_GOSSIP_MENU(text, player->GetGUID());
		}
		else if (menu == MY_INFO)
		{
			player->ADD_GOSSIP_ITEM(0, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
			std::string msg;
			if (player->GetReceiveMessage(CHECK_VIP_PLAYER_ACTIVE) == true)
				msg = "|CFF00FF24You're VIP|CFF000000.";
			else
				msg = "|CFFFF0000Your aren't VIP|CFF000000.";
			std::stringstream pplinfo_msg;
			pplinfo_msg
				<< "#########################\n"
				<< "#|CFFFF0000 My Information \n"
				<< "|CFF000000######################### \n#"
				<< "\n" << "#|CFF007C17 Played Time: |CFF000000" << player->GetTotalPlayedTime() << "\n#"
				<< "\n" << "#|CFF007C17 Vip: |CFF000000" << msg << "\n#"
				<< "\n" << "#|CFF007C17 Arena Points: |CFF000000" << player->GetArenaPoints() << "\n#"
				<< "\n" << "#|CFF007C17 Honor Points: |CFF000000" << player->GetHonorPoints() << "\n#"
				<< "\n######################### \n";
			player->SEND_GOSSIP_MENU(pplinfo_msg.str(), player->GetGUID());
		}
		else if (menu == SERVER_INFO)
		{
			player->ADD_GOSSIP_ITEM(0, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
			std::stringstream srv_info;
			srv_info
				<< "#########################\n"
				<< "#|CFFFF0000 Server Information\n"
				<< "|CFF000000######################### \n#"
				<< "\n" << "#|CFF007C17 R.XP.Quest: |CFF000000" << sWorld->getRate(RATE_XP_QUEST) << "\n#"
				<< "\n" << "#|CFF007C17 R.XP.Kill: |CFF000000" << sWorld->getRate(RATE_XP_KILL) << "\n#"
				<< "\n" << "#|CFF007C17 R.Honor: |CFF000000" << sWorld->getRate(RATE_HONOR) << "\n#"
				<< "\n" << "#|CFF007C17 R.XP.Exp: |CFF000000 " << sWorld->getRate(RATE_XP_EXPLORE) << "\n#"
				<< "\n" << "#|CFF007C17 R.Drop: |CFF000000 " << sWorld->getRate(RATE_DROP_ITEM_EPIC) << "\n#"
				<< "\n" << "#|CFF007C17 R.Money: |CFF000000 " << sWorld->getRate(RATE_DROP_MONEY) << "\n"
				<< "|CFF000000######################### \n#"
				<< "#|CFFFF0000 Information: Account Vip\n"
				<< "|CFF000000######################### \n#"
				<< "\n" << "#|CFF007C17 R.XP.Quest: |CFF000000" << sWorld->getRate(VIP_RATE_QUEST) << "\n#"
				<< "\n" << "#|CFF007C17 R.XP.Kill: |CFF000000" << sWorld->getRate(VIP_RATE_KILL_EXP) << "\n#"
				<< "\n" << "#|CFF007C17 R.Honor: |CFF000000" << sWorld->getRate(VIP_RATE_HONOR) << "\n#"
				<< "\n" << "#|CFF007C17 R.XP.Exp: |CFF000000 " << sWorld->getRate(RATE_XP_EXPLORE) << "\n#"
				<< "\n" << "#|CFF007C17 R.Drop: |CFF000000 " << sWorld->getIntConfig(VIP_TOKEN_DROP_COUNT) << "\n#"
				<< "\n" << "#|CFF007C17 R.Money: |CFF000000 " << sWorld->getRate(RATE_DROP_MONEY) << "\n#"
				<< "\n" << "#|CFF007C17 R.Gathering: |CFF000000 " << sWorld->getIntConfig(VIP_RATE_GATHERING) << "\n#"
				<< "\n" << "#|CFF007C17 R.Crafting: |CFF000000 " << sWorld->getIntConfig(VIP_RATE_CRAFTING) << "\n#"
				<< "\n" << "#|CFF007C17 R.Rep: |CFF000000 " << sWorld->getRate(VIP_RATE_REP) << "\n#"
				<< "\n" << "#|CFF007C17 R.Weapon: |CFF000000 " << sWorld->getIntConfig(VIP_RATE_WEAPON) << "\n#"
				<< "\n" << "#|CFF007C17 R.Defense: |CFF000000 " << sWorld->getIntConfig(VIP_RATE_DEFENSE) << "\n"
				<< "#\n######################### \n";
			player->SEND_GOSSIP_MENU(srv_info.str(), player->GetGUID());
		}
		else if (menu == ROLE_PVP)
		{
			std::stringstream msg;
			if (player->getClass() == CLASS_WARRIOR)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 2 options In PvP of Warrior,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Rogue_Eviscerate:30:30:-18:0|t Arms", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 1), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Warrior_InnerRage:30:30:-18:0|t Fury", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 2), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_PALADIN)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 1 options In PvP of Paladin,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_AuraOfLight:30:30:-18:0|t Retribution", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 6), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_HUNTER)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 3 options In PvP of Hunter,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Hunter_BeastTaming:30:30:-18:0|t Beast Mastery", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 10), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Marksmanship:30:30:-18:0|t Marksmanship", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 11), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_ROGUE)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 3 options In PvP of Rogue,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Rogue_Eviscerate:30:30:-18:0|t Assassination", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 7), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_BackStab:30:30:-18:0|t Combat", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 8), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Stealth:30:30:-18:0|t Subtley", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 9), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_MAGE)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 3 options In PvP of Mage,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_MagicalSentry:30:30:-18:0|t Arcane", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 13), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Frost_FrostBolt02:30:30:-18:0|t Frost", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 14), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Fire_FlameBolt:30:30:-18:0|t Fire", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 15), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_WARLOCK)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 3 options In PvP of Warlock,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Shadow_DeathCoil:30:30:-18:0|t Afflication", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 16), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Shadow_RainOfFire:30:30:-18:0|t Destruction", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 18), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_PRIEST)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 2 options In PvP of Priest,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_WordFortitude:30:30:-18:0|t Descipline", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 19), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Shadow_ShadowWordPain:30:30:-18:0|t Shadow Magic", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 21), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_SHAMAN)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 2 options In PvP of Shaman,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_Lightning:30:30:-18:0|t Elemental Combat", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 22), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_LightningShield:30:30:-18:0|t Enhancement", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 23), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 2 options In PvP of Death Knight,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Deathknight_BloodPresence:30:3:-18:0|t Blood", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 25), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Deathknight_UnholyPresence:30:30:-18:0|t Unholy", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 27), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_DRUID)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 2 options In PvP of Druid,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_StarFall:30:30:-18:0|t Balance", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 28), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Racial_BearForm:30:30:-18:0|t Feral Combat", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 29), talent_requirements, 0, false);
			}

			player->ADD_GOSSIP_ITEM(0, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
			player->SEND_GOSSIP_MENU(msg.str(), player->GetGUID());
			type = 4;
		}
		else if (menu == ROLE_PVE)
		{
			std::stringstream msg;
			if (player->getClass() == CLASS_WARRIOR)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 3 options in PvP of Warrior,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Rogue_Eviscerate:30:30:-18:0|t Arms", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 1), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Warrior_InnerRage:30:30:-18:0|t Fury", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 2), talent_requirements, 0, false);
				//player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Warrior_DefensiveStance:30:30:-18:0|t Protection", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 3), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_PALADIN)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 3 options in PvP of Paladin,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_HolyBolt:30:30:-18:0|t Holy", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 4), talent_requirements, 0, false);
				//player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/SPELL_HOLY_DEVOTIONAURA:30:30:-18:0|t Protection", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 5), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_AuraOfLight:30:30:-18:0|t Retribution", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 6), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_HUNTER)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 2 options in PvP of Hunter,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Hunter_BeastTaming:30:30:-18:0|t Beast Mastery", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 10), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Marksmanship:30:30:-18:0|t Marksmanship", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 11), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_ROGUE)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 3 options in PvP of Rogue,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Rogue_Eviscerate:30:30:-18:0|t Assassination", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 7), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_BackStab:30:30:-18:0|t Combat", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 8), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Stealth:30:30:-18:0|t Subtley", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 9), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_MAGE)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 3 options in PvP of Mage,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_MagicalSentry:30:30:-18:0|t Arcane", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 13), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Frost_FrostBolt02:30:30:-18:0|t Frost", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 14), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Fire_FlameBolt:30:30:-18:0|t Fire", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 15), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_WARLOCK)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 2 options in PvP of Warlock,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Shadow_DeathCoil:30:30:-18:0|t Afflication", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 16), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Shadow_RainOfFire:30:30:-18:0|t Destruction", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 18), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_PRIEST)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 2 options in PvP of Priest,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_WordFortitude:30:30:-18:0|t Descipline", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 19), talent_requirements, 0, false);
				//player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Holy_HolyBolt:30:30:-18:0|t Holy", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 20), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Shadow_ShadowWordPain:30:30:-18:0|t Shadow Magic", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 21), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_SHAMAN)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 3 options in PvP of Shaman,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_Lightning:30:30:-18:0|t Elemental Combat", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 22), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_LightningShield:30:30:-18:0|t Enhancement", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 23), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_MagicImmunity:30:30:-18:0|t Restoration", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 24), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_DEATH_KNIGHT)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 3 options in PvP of Death Knight,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Deathknight_BloodPresence:30:30:-18:0|t Blood", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 25), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Deathknight_FrostPresence:30:30:-18:0|t Frost", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 26), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Deathknight_UnholyPresence:30:30:-18:0|t Unholy", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 27), talent_requirements, 0, false);
			}
			else if (player->getClass() == CLASS_DRUID)
			{
				msg
					<< "#########################\n"
					<< "#|CFFFF0000 Selecting Talent\n"
					<< "|CFF000000######################### \n#"
					<< "\n" << "#|CFF000000 Choose a Talent to gets related items.\n#"
					<< "\n" << "#|CFF000000 You've only 3 options in PvP of Druid,\n#"
					<< "\n" << "#|CFF000000 Please select a Talent.\n"
					<< "#\n######################### \n";

				std::string talent_requirements;
				talent_requirements = Warlords->GetRequirement(player, REGUIREMENT_LEARN_TALENT);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_StarFall:30:30:-18:0|t Balance", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 28), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Ability_Racial_BearForm:30:30:-18:0|t Feral Combat", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 29), talent_requirements, 0, false);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_DOT, "|TInterface/ICONS/Spell_Nature_HealingTouch:30:30:-18:0|t Restoration", GOSSIP_SENDER_MAIN, Warlords->Melt(LEARN_START, 30), talent_requirements, 0, false);
			}

			player->ADD_GOSSIP_ITEM(0, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", GOSSIP_SENDER_MAIN, Warlords->Melt(MAIN_MENU, 0));
			player->SEND_GOSSIP_MENU(msg.str(), player->GetGUID());
			type = 8;

		}
		else if (menu == LEARN_START)
		{
			Warlords->LearnSpells(player, player->getClass());
			Warlords->LearnTalents(player, action);
			uint32 groupid;
			QueryResult Query = WorldDatabase.PQuery("SELECT `groupid` FROM `custom_start_items` WHERE `class` = '%u' AND `spec` = '%u' AND `type` = '%u' ORDER BY `id` ASC", player->getClass(), action, type);
			
			if (Query)
			{
				Field *Fields = Query->Fetch();

				groupid = Fields[0].GetInt32();
				Warlords->AddGroupItems(player, groupid);
			}
			player->CLOSE_GOSSIP_MENU();
		}
	}
};

void AddSC_welcome_message()
{
	new welcome_message();
}
