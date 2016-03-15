#include "ScriptPCH.h"
#include "Group.h"
#include "system_megapoint.h"

#define sM  ACE_Singleton<MegaPoint, ACE_Null_Mutex>::instance()

class megapoint : public PlayerScript
{
public:
	megapoint() : PlayerScript("megapoint") {}
	void OnCreatureKill(Player* player, Creature* creature)
	{
		uint32 mps = sM->RandomFloat(sM->GetCreatureMinMps(creature), sM->GetCreatureMaxMps(creature));
		if (mps != 0)
		{
			Group* grp = player->GetGroup();
			if (!grp)
			{
				if (player)
				{
					sM->AddPlayerMPs(player, mps);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000MP System:|r You've got %u MPs.", mps);
				}
			}
			else
			{
				for (GroupReference* itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
				{
					Player* player = itr->GetSource();
					if (player)
					{
						if (!player || !player->GetSession())
							continue;

						sM->AddPlayerMPs(player, mps);
						ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000MP System:|r You've got %u MPs.", mps);
					}
				}
			}
		}
	}
};
void AddSC_megapoint()
{
	new megapoint;
}