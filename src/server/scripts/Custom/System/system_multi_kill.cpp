class Multi_Kill : public PlayerScript
{
public:
	Multi_Kill() : PlayerScript("Multi_Kill") { }


	void OnPVPKill(Player* Killer, Player* Killed) OVERRIDE
	{
		SetKills(Killer, Killed);
		uint32 kills = CheckKillStreakData(Killer);

		if (kills > 2)
		{
			uint32 killer_guid = Killer->GetGUID();
			QueryResult result = WorldDatabase.PQuery("SELECT `kills`, `icon`, `name`, `type`, `value`, `aura`, `item_count`  FROM `custom_kill_system` WHERE `kills` = %u", kills);
			if (result)
			{
				Field* field = result->Fetch();
				uint32 kills = field[0].GetInt32();
				std::string icon = field[1].GetString();
				std::string name = field[2].GetString();
				uint32 type = field[3].GetInt16();
				uint32 value = field[4].GetInt32();
				uint32 aura = field[5].GetInt32();
				uint32 itemcount = field[6].GetInt32();

				if (type == 1)
				{
					int32 val = value;
					Killer->ModifyHonorPoints(val);
				}
				if (type == 2)
				{
					int32 val = value;
					Killer->ModifyArenaPoints(-val);
				}
				if (type == 3)
				{
					Killer->AddItem(value, itemcount);
				}
				if (type == 4)
				{
					int32 val = value * 10000;
					Killer->ModifyMoney(val, false);
				}

				if (aura > 0)
				{
					Killer->AddAura(aura, Killer);
				}

				SendMessage(Killer, Killed, name, icon);
			}
		}
	}

	void SetKills(Player* Killer, Player* Killed)
	{
		uint32 killer_guid = Killer->GetGUID();
		uint32 killed_guid = Killed->GetGUID();

		QueryResult result = CharacterDatabase.PQuery("SELECT * FROM `custom_kill_system` WHERE `guid` = %u", killer_guid);
		if (!result)
			CharacterDatabase.PExecute("INSERT INTO `custom_kill_system` (`guid`, `kill`, `total_kill`, `godlikes`) VALUES (%u, %u, %u, %u);", killer_guid, 1, 1, 0);
		else
			CharacterDatabase.PExecute("UPDATE `custom_kill_system` SET `kill` = `kill` + 1, `total_kill` = `total_kill` + 1  WHERE `guid` = %u", killer_guid);

		QueryResult result2 = CharacterDatabase.PQuery("SELECT * FROM `custom_kill_system` WHERE `guid` = %u", killed_guid);
		if (!result2)
			CharacterDatabase.PExecute("INSERT INTO `custom_kill_system` (`guid`, `kill`, `total_kill`, `godlikes`) VALUES (%u, %u, %u, %u);", killed_guid, 0, 0, 0);
		else
			CharacterDatabase.PExecute("UPDATE `custom_kill_system` SET `kill` = 0 WHERE `guid` = %u", killed_guid);
	}

	uint32 CheckKillStreakData(Player* Killer)
	{
		uint32 killer_guid = Killer->GetGUID();
		QueryResult result = CharacterDatabase.PQuery("SELECT `kill` FROM `custom_kill_system` WHERE `guid` = %u", killer_guid);

		if (result)
		{
			Field* field = result->Fetch();
			uint32 kill = field[0].GetInt32();
			if (kill == 9)
			{
				CharacterDatabase.PExecute("UPDATE `custom_kill_system` SET `godlikes` = `godlikes` + 1 WHERE `guid` = %u", killer_guid);
			}
			else if (kill > 9)
			{
				CharacterDatabase.PExecute("UPDATE `custom_kill_system` SET `kill` = 0 WHERE `guid` = %u", killer_guid);
			}

			return kill;
		}
		else
			return 0;
	}

	void SendMessage(Player* Killer, Player* Killed, std::string Name, std::string Icon)
	{
		std::string Message = "";
		Message += Icon;
		Message += " |cff00ccff";
		Message += Killer->GetName();
		Message += "|r Is On |cffff0000";
		Message += Name;
		Message += "|r";
		Message += " By Killing |cffff0000";
		Message += Killed->GetName();

		SessionMap Sessions = sWorld->GetAllSessions();
		for (SessionMap::iterator itr = Sessions.begin(); itr != Sessions.end(); ++itr)
		if (Player* plr = itr->second->GetPlayer())
		if (plr->GetReceiveMessage(CHECK_KILL_PLAYER_MESSAGE) == true)
		{
			WorldPacket data(SMSG_SERVER_MESSAGE, 50);
			data << uint32(3);
			data << Message;
			plr->GetSession()->SendPacket(&data);
		}
	}
};

void AddSC_Multi_Kill()
{
	new Multi_Kill();
}