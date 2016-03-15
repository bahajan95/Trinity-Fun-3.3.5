#include "ScriptPCH.h"
#include "Group.h"
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
};

class npc_group_summon : public CreatureScript
{
public:
	npc_group_summon() : CreatureScript("npc_group_summon") { }
	bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
	{
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(10, "|TInterface\\icons\\INV_Misc_GroupNeedMore:30:30:-18:0|t Group Summon", GOSSIP_SENDER_MAIN, Warlords->Melt(SHOW_AUCTIONEER, 0));
		player->SEND_GOSSIP_MENU("Welcome To Game Server Golden Game", creature->GetGUID());
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
		else if (menu == SHOW_AUCTIONEER)
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
		}
		return true;
	}
};


void AddSC_npc_group_summon()
{
	new npc_group_summon();
}