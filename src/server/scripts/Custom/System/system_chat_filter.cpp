class Chat_Filter : public PlayerScript
{
public:
	Chat_Filter() : PlayerScript("Chat_Filter") {}

	void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg)
	{
		CheckMessage(player, msg, lang, NULL, NULL, NULL, NULL);
	}

	void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Player* receiver)
	{
		CheckMessage(player, msg, lang, receiver, NULL, NULL, NULL);
	}

	void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Group* group)
	{
		CheckMessage(player, msg, lang, NULL, group, NULL, NULL);
	}

	void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Guild* guild)
	{
		CheckMessage(player, msg, lang, NULL, NULL, guild, NULL);
	}

	void OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Channel* channel)
	{
		CheckMessage(player, msg, lang, NULL, NULL, NULL, channel);
	}



	void CheckMessage(Player* player, std::string& msg, uint32 lang, Player* /*receiver*/, Group* /*group*/, Guild* /*guild*/, Channel* channel)
	{
		std::string lower = msg;
		std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

		QueryResult Result = WorldDatabase.Query("SELECT `Id`, `Fosh`, `massage`, `Active` FROM `Chat_Filter` WHERE `Active` = 1 ORDER BY `id` ASC");

		if (!Result)
			return;

		do
		{
			uint32 Id, Active;
			std::string Fosh;
			std::string massage;
			Field* Fields = Result->Fetch();
			Id = Fields[0].GetInt32();
			Fosh = Fields[1].GetString();
			massage = Fields[2].GetString();
			Active = Fields[3].GetInt32();

			if (lower.find(Fosh) != std::string::npos)
			{
				sWorld->SendGMText(202541, player->GetName().c_str(), msg.c_str());
				msg = (massage);
				ChatHandler(player->GetSession()).PSendSysMessage("|cddff0000Tabligh Va Fosh Dar Server Gheyr Mojaz Mibashad.|r");
				return;
			}
		} while (Result->NextRow());
	}

	void CheckMessagee(Player* player, std::string& msg, uint32 lang, Player* /*receiver*/, Group* /*group*/, Guild* /*guild*/, Channel* channel)
	{
		// VIPs can use the chat too, also lang addon
		if (player->GetSession()->GetSecurity() >= 1 || lang == LANG_ADDON)
			return;

		uint32 reqPlayedTime = 30 * MINUTE; // 30 minutes

		if (player->GetTotalPlayedTime() <= reqPlayedTime)
		{
			uint32 remainingTime = ((30 * MINUTE) - player->GetTotalPlayedTime()) / MINUTE;
			uint32 currentTime = 30 - remainingTime;

			msg = "";

			ChatHandler(player->GetSession()).PSendSysMessage("|cffF00000You need at least 30 minutes playtime to use global channels or whispers. Note that you can still whisper to a GMs.");
			ChatHandler(player->GetSession()).PSendSysMessage("|cffF00000Your current played time: %u minutes.", currentTime);
			return;
		}
	}
};

void AddSC_Chat_Filter()
{
	new Chat_Filter();
}