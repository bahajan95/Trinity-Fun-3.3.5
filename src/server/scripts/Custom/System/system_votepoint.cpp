#include "ScriptPCH.h"
#include "Config.h"
#include "Language.h"
#include "system_votepoint.h"

void VotePoint::AddPlayerVPs(Player* player, uint32 vps)
{
	uint32 accountid = player->GetSession()->GetAccountId();

	QueryResult result = LoginDatabase.PQuery("SELECT * FROM `account_votepoint` WHERE `id` = %u", accountid);

	if (!result)
	{
		LoginDatabase.PExecute("INSERT INTO `account_votepoint` (`id`, `votes`, `last_vote`, `total_votes`) VALUES (%u, %u, UNIX_TIMESTAMP(), %u);", accountid, vps, vps);
	}
	else
	{
		LoginDatabase.PExecute("UPDATE `account_votepoint` SET `votes` = `votes` + %u, `total_votes` = `total_votes` + %u  WHERE `id` = %u", vps, vps, accountid);
	}
}

void VotePoint::RemovePlayerVPs(Player* player, uint32 vps)
{
	uint32 accountid = player->GetSession()->GetAccountId();

	QueryResult result = LoginDatabase.PQuery("SELECT * FROM `account_votepoint` WHERE `id` = %u", accountid);

	if (!result)
	{
		LoginDatabase.PExecute("INSERT INTO `account_votepoint` (`id`, `votes`, `last_vote`, `total_votes`) VALUES (%u, 0, UNIX_TIMESTAMP(), 0);", accountid);
	}
	else
	{
		LoginDatabase.PExecute("UPDATE `account_votepoint` SET `votes` = `votes` - %u WHERE `id` = %u", vps, accountid);
	}
}

uint32 VotePoint::GetPlayerVPs(Player* player)
{
	uint32 accountid = player->GetSession()->GetAccountId();

	QueryResult result = LoginDatabase.PQuery("SELECT `votes` FROM `account_votepoint` WHERE `id` = '%u'", accountid);
	if (result)
	{
		Field *field = result->Fetch();
		int32 vps = field->GetInt32();
		return vps;
	}
	else
		return 0;
}

uint32 VotePoint::GetPlayerTotalVPs(Player* player)
{
	uint32 accountid = player->GetSession()->GetAccountId();

	QueryResult result = LoginDatabase.PQuery("SELECT `total_votes` FROM `account_votepoint` WHERE `id` = '%u'", accountid);
	if (result)
	{
		Field *field = result->Fetch();
		int32 vps = field->GetInt32();
		return vps;
	}
	else
		return 0;
}