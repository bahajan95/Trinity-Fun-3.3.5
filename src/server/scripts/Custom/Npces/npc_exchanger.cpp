#include "ScriptPCH.h"

class Exchanger_NPCs : public CreatureScript
{
	public:
		Exchanger_NPCs() : CreatureScript("Exchanger_NPCs") {}
		
	bool OnGossipHello(Player *player, Creature *_creature)
	{
		player->ADD_GOSSIP_ITEM(10, "50000 Honor point-> 2x Mega Points", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(10, "100 Arena Points-> 2x Mega Points" , GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(10, "300 Arena Points->30000 Honor Points" , GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(10, "50 PvP Token-> 2x Mega Points" , GOSSIP_SENDER_MAIN, 2);
		
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
        return true;
	}
	
	bool OnGossipSelect(Player *player, Creature *_creature, uint32 sender, uint32 uiAction)
	{
		if (sender == GOSSIP_SENDER_MAIN)
		{
			player->PlayerTalkClass->ClearMenus();
			switch(uiAction)
			{
				case 1:
					if (player->GetHonorPoints () >= 50000)
					{
						player->CLOSE_GOSSIP_MENU();
						LoginDatabase.PExecute("UPDATE `account_megapoint` SET `mps` = `mps` + 2 WHERE `id` = %u AND `character_guid` = %u", player->GetSession()->GetAccountId(), player->GetGUID());
						player->ModifyHonorPoints(-50000);
						player->GetSession()->SendNotification("|cffffea00Be Shoma |cdd00ff002x|r Mega Points Dade shod", player->GetGUID());
					}
					else
					{
						player->CLOSE_GOSSIP_MENU();
						player->GetSession()->SendNotification("|cffffea00Be Andaze Kafi Honor Points Nadarid", player->GetGUID());
						return false;
					}
					break;

				case 2:
					if (player->HasItemCount( 29434, 50, false ))
					{
						player->CLOSE_GOSSIP_MENU();
						LoginDatabase.PExecute("UPDATE `account_megapoint` SET `mps` = `mps` + 2 WHERE `id` = %u AND `character_guid` = %u", player->GetSession()->GetAccountId(), player->GetGUID());
						player->DestroyItemCount(29434, 50, true, false);
						player->GetSession()->SendNotification("|cffffea00Be Shoma |cdd00ff002x|r Mega Points Dade shod", player->GetGUID());
					}
					else
					{
						player->CLOSE_GOSSIP_MENU();
						player->GetSession()->SendNotification("|cffffea00Be Andaze kafi PVP Token Nadarid", player->GetGUID());
						return false;
					}
					break;
					
				case 3:
					if (player->GetArenaPoints () >= 100)
					{
						player->CLOSE_GOSSIP_MENU();
						LoginDatabase.PExecute("UPDATE `account_megapoint` SET `mps` = `mps` + 2 WHERE `id` = %u AND `character_guid` = %u", player->GetSession()->GetAccountId(), player->GetGUID());
						player->ModifyArenaPoints(-100);
						player->GetSession()->SendNotification("|cffffea00Be Shoma |cdd00ff002x|r Mega Points Dade shod", player->GetGUID());
					}
					else
					{
						player->CLOSE_GOSSIP_MENU();
						player->GetSession()->SendNotification("|cffffea00Be Andaze kafi Arena Points Nadarid", player->GetGUID());
						return false;
					}
					break;
				case 4:
					if (player->GetArenaPoints () >= 300)
					{
						player->CLOSE_GOSSIP_MENU();
						player->ModifyHonorPoints(30000);
						player->ModifyArenaPoints(-300);
						player->GetSession()->SendNotification("|cffffea00Be Shoma |cdd00ff0030000x|r Honor Points Dade shod", player->GetGUID());
					}
					else
					{
						player->CLOSE_GOSSIP_MENU();
						player->GetSession()->SendNotification("|cffffea00Be Andaze kafi Arena Points Nadarid", player->GetGUID());
						return false;
					}
					break;
			}
		}	
	}
};

void AddSC_Exchanger_NPC()
{
    new Exchanger_NPCs();
}		