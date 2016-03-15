#include "ScriptPCH.h"

class Test : public CreatureScript
{
	public:
		Test() : CreatureScript("Test") { }
		
	 bool OnGossipHello(Player *player, Creature *creature)
	{
		player->ADD_GOSSIP_ITEM(1, "Weapon", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(1, "Test", GOSSIP_SENDER_MAIN, 2);
		
		player->SEND_GOSSIP_MENU(69, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
    
			switch (action)
			{
				case 1:
					player->GetSession()->SendListInventory(creature->GetGUID(), 12785);
            	break;
				case 2:
					player->GetSession()->SendListInventory(creature->GetGUID(), 190001);
            	break;
			}
		return true;
	}
};

void AddSC_DK_FIX_npc()
{
	new Test();
}	