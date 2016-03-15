#include "ScriptPCH.h"
#include "player.h"  
class System_event : public PlayerScript
{
public:
    System_event() : PlayerScript("System_event") { }
	void OnPVPKill(Player *pKiller, Player *pVictim){
	int evenhonor = 10; //meghdare rewarde honor dar event
    int eventarena = 1; //meghdare rewarde arena dar event
	switch (pVictim ->GetAreaId())
	{
	    case 2177:
		    if (pVictim->GetTeam() == ALLIANCE){
	     	pVictim->ResurrectPlayer(0.5f);
	        pVictim->TeleportTo(0, -13276.126953f, 262.6968338f, 46.615314f, 0.232508f);
			pVictim->SetPower(POWER_MANA, pVictim->GetMaxPower(POWER_MANA));
			pVictim->RemoveAllSpellCooldown();
			pVictim->DurabilityRepairAll(false, 0, false);
			pVictim->SetHealth(pVictim->GetMaxHealth());
			}
			else if (pVictim->GetTeam() == HORDE){
		 	pVictim->ResurrectPlayer(0.5f);
	        pVictim->TeleportTo(0, -13141.816406f, 233.534470f, 47.018986f, 2.723450f);
			pVictim->SetPower(POWER_MANA, pVictim->GetMaxPower(POWER_MANA));
			pVictim->RemoveAllSpellCooldown();
			pVictim->DurabilityRepairAll(false, 0, false);
			pVictim->SetHealth(pVictim->GetMaxHealth());
			}
			pKiller->AddItem(37829, 1);
			break;
		case 3217:
		   if (pVictim->GetTeam() == ALLIANCE){
		   pVictim->ResurrectPlayer(0.5f);
		   pVictim->TeleportTo(0, -8908.96f, 556.3f, 93.5345f, 0.73f);
		   std::ostringstream eventLog;
		   eventLog << "|cffFFFF00|TInterface\\icons\\Achievement_bg_killingblow_berserker:30|t |cffff0000Event Killers :|cFFFFFF00 Player |cff0000ff|r"<< pKiller->GetName() << " |r|cFFFFFF00Killed  |cff0000ff"<< pVictim->GetName() <<"|r";
           sWorld->SendServerMessage(SERVER_MSG_STRING, eventLog.str().c_str());
		   }
		   else if (pVictim->GetTeam() == HORDE){
		   pVictim->ResurrectPlayer(0.5f);
		   pVictim->TeleportTo(1, 1677.91f, -4314.79f, 62.35f, 2.66969f);
		   std::ostringstream eventLog;
		   eventLog << "|cffFFFF00|TInterface\\icons\\Achievement_bg_killingblow_berserker:30|t |cffff0000Event Killers :|cFFFFFF00 Player |cff0000ff|r"<< pKiller->GetName() << " |r|cFFFFFF00Killed  |cff0000ff"<< pVictim->GetName() <<"|r";
           sWorld->SendServerMessage(SERVER_MSG_STRING, eventLog.str().c_str());
		   }
		   int honor = pKiller->GetHonorPoints();
           pKiller->ModifyHonorPoints(honor + evenhonor);
		   int arena = pKiller->GetArenaPoints();
           pKiller->ModifyArenaPoints(arena + eventarena);
		   pKiller->AddItem(38186, 10);
 		   break;
		   }
	}	   
};
void AddSC_System_event ()
{
    new System_event();
}
