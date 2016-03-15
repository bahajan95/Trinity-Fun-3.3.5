#include "ScriptPCH.h"
#include "Config.h"
#include "Language.h"
#include "system_megapoint.h"

void MegaPoint::AddAccountMPs(uint32 accountid, uint32 mps)
{
	QueryResult result = LoginDatabase.PQuery("SELECT * FROM `account_megapoint` WHERE `id` = '%u' AND character_guid = 0", accountid);

	if (!result)
	{
		LoginDatabase.PExecute("INSERT INTO `account_megapoint` (`id`, `character_guid`, `mps`) VALUES (%u, 0, %u);", accountid, mps);
	}
	else
	{
		LoginDatabase.PExecute("UPDATE `account_onlinepoint` SET `ops` = `ops` + %u WHERE `id` = %u", mps, accountid);
	}
}

void MegaPoint::AddPlayerMPs(Player* player, uint32 mps)
{
	uint32 accountid = player->GetSession()->GetAccountId();
	uint32 character_guid = player->GetGUID();

	QueryResult result = LoginDatabase.PQuery("SELECT * FROM `account_megapoint` WHERE `id` = %u AND character_guid = %u", accountid, character_guid);
	ChatHandler(player->GetSession()).PSendSysMessage("Shoma %u Mega Points Daryaft Kardid.|r", mps);
	if (!result)
	{
		LoginDatabase.PExecute("INSERT INTO `account_megapoint` (`id`, `character_guid`, `mps`) VALUES (%u, %u, %u);", accountid, character_guid, mps);
	}
	else
	{
		LoginDatabase.PExecute("UPDATE `account_megapoint` SET `mps` = `mps` + %u WHERE `id` = %u AND `character_guid` = %u", mps, accountid, character_guid);
	}
}

void MegaPoint::RemoveAccountMPs(uint32 accountid, uint32 mps)
{
	QueryResult result = LoginDatabase.PQuery("SELECT * FROM `account_megapoint` WHERE `id` = '%u' AND character_guid = 0", accountid);

	if (!result)
	{
		LoginDatabase.PExecute("INSERT INTO `account_megapoint` (`id`, `character_guid`, `mps`) VALUES (%u, 0, 0);", accountid);
	}
	else
	{
		Field *field = result->Fetch();
		int32 _mps = field->GetInt32();
		LoginDatabase.PExecute("UPDATE `account_onlinepoint` SET `ops` = `ops` - %u WHERE `id` = %u", mps, accountid);
	}
}

void MegaPoint::RemovePlayerMPs(Player* player, uint32 mps)
{
	uint32 accountid = player->GetSession()->GetAccountId();
	uint32 character_guid = player->GetGUID();

	QueryResult result = LoginDatabase.PQuery("SELECT * FROM `account_megapoint` WHERE `id` = %u AND character_guid = %u", accountid, character_guid);
	ChatHandler(player->GetSession()).PSendSysMessage("|CFFFF0000Shoma %u Mega Points Az Dast Dadid.|r", mps);
	if (!result)
	{
		LoginDatabase.PExecute("INSERT INTO `account_megapoint` (`id`, `character_guid`, `mps`) VALUES (%u, %u, 0);", accountid, character_guid);
	}
	else
	{
		Field *field = result->Fetch();
		int32 _mps = field->GetInt32();
		LoginDatabase.PExecute("UPDATE `account_megapoint` SET `mps` = `mps` - %u WHERE `id` = %u AND `character_guid` = %u", mps, accountid, character_guid);
	}
}

uint32 MegaPoint::GetPlayerMps(Player* player)
{
	uint32 accountid = player->GetSession()->GetAccountId();
	uint32 character_guid = player->GetGUID();

	QueryResult result = LoginDatabase.PQuery("SELECT `mps` FROM `account_megapoint` WHERE `id` = '%u' AND `character_guid` = %u", accountid, character_guid);
	if (result)
	{
		Field *field = result->Fetch();
		int32 mps = field->GetInt32();
		return mps;
	}
	else
		return 0;
}

uint32 MegaPoint::GetAccountMps(uint32 accountid)
{
	QueryResult result = LoginDatabase.PQuery("SELECT `mps` FROM `account_megapoint` WHERE `id` = '%u' AND `character_guid` = 0", accountid);
	if (result)
	{
		Field *field = result->Fetch();
		int32 mps = field->GetInt32();
		return mps;
	}
	else
		return 0;
}

float MegaPoint::RandomFloat(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

uint32 MegaPoint::GetCreatureMinMps(Creature* creature)
{
	uint32 id = creature->GetEntry();

	QueryResult result = WorldDatabase.PQuery("SELECT `mp_min` FROM `custom_creature_mp_loot` WHERE `entry` = '%u'", id);
	if (result)
	{
		Field *field = result->Fetch();
		int32 mps = field->GetInt32();
		return mps;
	}
	return 0;
}

uint32 MegaPoint::GetCreatureMaxMps(Creature* creature)
{
	uint32 id = creature->GetEntry();

	QueryResult result = WorldDatabase.PQuery("SELECT `mp_max` FROM `custom_creature_mp_loot` WHERE `entry` = '%u'", id);
	if (result)
	{
		Field *field = result->Fetch();
		int32 mps = field->GetInt32();
		return mps;
	}
	return 0;
}