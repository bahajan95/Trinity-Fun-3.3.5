#include "ScriptPCH.h"
class Boss_Announcer : public PlayerScript
{
public:
	Boss_Announcer() : PlayerScript("Boss_Announcer") {}
	void OnCreatureKill(Player* player, Creature* boss)
	{
		if (player->GetSession()->GetSecurity() <= SEC_PLAYER)
		{
			if (boss->isWorldBoss())
			{
				std::string plrname = player->GetName();
				std::string bossname = boss->GetName();
				char msg[250];
				snprintf(msg, 250, "|CFF7BBEF7[Boss Announcer]|r:|cffff0000 %s |r Va Gorohanash Tavanestand Boss |CFF18BE00[%s]|r Ra Bokoshand |r !!!", plrname.c_str(), bossname.c_str());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
			}
		}
	}
};
void AddSC_Boss_Announcer()
{
	new Boss_Announcer;
}