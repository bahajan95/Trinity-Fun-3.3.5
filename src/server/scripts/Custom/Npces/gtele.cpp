#include "ScriptPCH.h"
#include "Pet.h"
class GTele : public CreatureScript
{
	public:
		GTele() : CreatureScript("GTele") {}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(2, "Open Malls", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(2, "Duel Zone", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(2, "Training Dummy Area", GOSSIP_SENDER_MAIN, 3);

		player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 Sender, uint32 Actions)
	{
		player->PlayerTalkClass->ClearMenus();

		switch(Actions)
		{
			case 1:
				if (player->GetTeam() == ALLIANCE)
				{
				    player->TeleportTo(0, -8857.639648f, 658.614014f, 96.625504f, 5.385040f);  
				    player->PlayerTalkClass->SendCloseGossip();
				} 
                else
				{
				    player->TeleportTo(1, 1579.109985f, -4453.709961f, 6.326848f, 2.225780f);
				    player->PlayerTalkClass->SendCloseGossip();
				}
			break;
			
			case 2:
				player->TeleportTo(1, 6402.46f, -4047.29f, 658.283f, 0.022242f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			
			case 3:
					player->TeleportTo(571, 6423.918945f, -4471.852539f, 451.075653f, 6.161902f);
					player->PlayerTalkClass->SendCloseGossip();
			break;
			case 4:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(530, -3000.320068f, 791.362000f, -8.054930f, 5.684930f);
				player->SetPhaseMask(2,1);
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 5:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(530, -3000.320068f, 791.362000f, -8.054930f, 5.684930f);
				player->SetPhaseMask(4,1);
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 6:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(571, 5870.605957f, 2109.123779f, 636.033813f, 3.573560f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 100:
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Achievement_Boss_LordMarrowgar:30|t|r .::Icecrown Citadel::.", GOSSIP_SENDER_MAIN, 6);
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\INV_Misc_Head_ClockworkGnome_01:30|t|r .::Vauly of Archavon::.", GOSSIP_SENDER_MAIN, 9);

				player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
			break;
			case 7:
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportStormWind:25|t|r Stormwind", GOSSIP_SENDER_MAIN, 70);
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportIronForge:25|t|r Ironforge", GOSSIP_SENDER_MAIN, 71);
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportDarnassus:25|t|r Darnassus", GOSSIP_SENDER_MAIN, 72);
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportExodar:25|t|r The Exodar", GOSSIP_SENDER_MAIN, 73);
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportOrgrimmar:25|t|r Orgrimmar", GOSSIP_SENDER_MAIN, 74);
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportThunderBluff:25|t|r Thunder Bluff", GOSSIP_SENDER_MAIN, 75);
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportUnderCity:25|t|r Undercity", GOSSIP_SENDER_MAIN, 76);
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportSilvermoon:25|t|r Silvermoon", GOSSIP_SENDER_MAIN, 77);
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportDalaran:25|t|r Dalaran", GOSSIP_SENDER_MAIN, 78);
				player->ADD_GOSSIP_ITEM(2, "|cff00ff00|TInterface\\icons\\Spell_Arcane_TeleportShattrath:25|t|r Shattrath", GOSSIP_SENDER_MAIN, 79);

				player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
			break;
			case 8:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(0, -13321.400391f, 75.832497f, 22.221901f, 4.003460f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 9:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(571, 5466.548340f, 2839.251465f, 418.675659f, 0.025898f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 70:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(0, -9129.269531f, 388.208740f, 91.057014f, 0.637305f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 71:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(0, -5000.476074f, -859.561523f, 497.045746f, 5.405354f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 72:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(1, 9954.119141f, 2167.963867f, 1327.462891f, 1.536971f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 73:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(530, -4001.156982f, -11873.359375f, -0.527358f, 1.064644f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 74:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(1, 1346.678955f, -4373.142090f, 26.181185f, 0.070212f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 75:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(1, -1267.347534f, 221.531189f, 59.157173f, 4.593608f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 76:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(0, 1887.412964f, 237.202225f, 57.703121f, 3.034173f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 77:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(530, 9370.547852f, -7277.637207f, 14.240896f, 0.007574f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 78:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(571, 5804.149902f, 624.770996f, 647.767029f, 1.640000f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
			case 79:
				player->CastSpell(player, 35517, true);
				player->TeleportTo(530, -1721.781128f, 5382.058105f, 1.545847f, 2.754345f);
				
				player->PlayerTalkClass->SendCloseGossip();
			break;
		}
		return true;
	}
};

void AddSC_GTele_npc()
{
	new GTele();
}