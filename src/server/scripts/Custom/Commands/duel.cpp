#include "ScriptPCH.h"
#include "Chat.h"
#define error_command "|cffFFFF00Zone tanzim nashode hast.|r"
#define error_commbat "|cffFFFF00Shoma To Commbat Hastid! |r"



class cs_duel : public CommandScript
{
public:
    cs_duel() : CommandScript("cs_duel") { }

	ChatCommand* GetCommands() const
    {
        static ChatCommand IngameCommandTable[] =
        {
		{ "tg",           rbac::RBAC_PERM_COMMAND_duels_COMMAND,  	true,  &Handlecs_duel,                "", NULL },
		{ "pvp",            rbac::RBAC_PERM_COMMAND_pvps_COMMAND,  	    true,  &Handlecs_pvp,                 "", NULL },
        { NULL,             0,                  false, NULL,                              "", NULL }
        };
		 return IngameCommandTable;
    }

	static bool Handlecs_duel(ChatHandler * handler, const char * args)
    {
		
        Player * pl = handler->GetSession()->GetPlayer();
		
		QueryResult result = WorldDatabase.Query("SELECT `position_x`, `position_y`, `position_z`, `orientation`, `map` , `resplayer`, `dismount` , `combatcheck`FROM `home_command` WHERE ishome = '1' ORDER BY `combatcheck` DESC LIMIT 10");
		                        if (!result){
									ChatHandler(pl->GetSession()).PSendSysMessage(error_command, pl->GetGUID());
									return true;}
		Field * fields = NULL;
		                            fields = result->Fetch();
		float position_x = fields[0].GetFloat();
		float position_y = fields[1].GetFloat();
		float position_z = fields[2].GetFloat();
		float orientation = fields[3].GetFloat();
		uint16 map = fields[4].GetUInt16();
		uint16 resplayer = fields[5].GetUInt16();
		uint16 combatcheck = fields[6].GetUInt16();
		uint16 dismount = fields[7].GetUInt16();
		
		
		if(combatcheck>0){
		if(pl->IsInCombat()){//Combat Check
			ChatHandler(pl->GetSession()).PSendSysMessage(error_commbat, pl->GetGUID());
return true;
		}}
		if(dismount >0){// Dismount kardan Ghabl az Summon
			pl->RemoveAurasByType(SPELL_AURA_MOUNTED);}
	
		if(resplayer>0){//baraye Res kardan Player
			pl->ResurrectPlayer(1.0f,false);}
		pl->TeleportTo(map,position_x,position_y,position_z,orientation,0);
		return true;


    }
	
	static bool Handlecs_pvp(ChatHandler * handler, const char * args)
    {
		        Player * pl = handler->GetSession()->GetPlayer();
		
		QueryResult result = WorldDatabase.Query("SELECT `position_x`, `position_y`, `position_z`, `orientation`, `map` , `resplayer`, `dismount` , `combatcheck`FROM `Ghost Wolf`.`pvp_command` WHERE ispvp = '1' ORDER BY `combatcheck` DESC LIMIT 10");
		                        if (!result){
									ChatHandler(pl->GetSession()).PSendSysMessage(error_command, pl->GetGUID());
									return true;}
		Field * fields = NULL;
		                            fields = result->Fetch();
		float position_x = fields[0].GetFloat();
		float position_y = fields[1].GetFloat();
		float position_z = fields[2].GetFloat();
		float orientation = fields[3].GetFloat();
		uint16 map = fields[4].GetUInt16();
		uint16 resplayer = fields[5].GetUInt16();
		uint16 combatcheck = fields[6].GetUInt16();
		uint16 dismount = fields[7].GetUInt16();
		
		
		if(combatcheck>0){
		if(pl->IsInCombat()){//Combat Check
			ChatHandler(pl->GetSession()).PSendSysMessage(error_commbat, pl->GetGUID());
return true;
		}}
		if(dismount >0){// Dismount kardan Ghabl az Summon
			pl->RemoveAurasByType(SPELL_AURA_MOUNTED);}
	
		if(resplayer>0){//baraye Res kardan Player
			pl->ResurrectPlayer(1.0f,false);}
		pl->TeleportTo(map,position_x,position_y,position_z,orientation,0);
		return true;
	}
};

void AddSC_cs_duel()
{
    new cs_duel();
}