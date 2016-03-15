class cheak_chat : public CommandScript
{
public:
	cheak_chat() : CommandScript("cheak_chat") { }

	ChatCommand* GetCommands() const OVERRIDE
	{
		static ChatCommand MSGRECIVE[] =
		{
			{ "chat", rbac::RBAC_PERM_COMMAND_CHEAK_GM_CHAT, true, &GmChatMessage, "", NULL },
			{ "global", rbac::RBAC_PERM_COMMAND_CHEAK_GLOBAL, true, &GlobalMessage, "", NULL },
			{ "kill", rbac::RBAC_PERM_COMMAND_CHEAK_KILL, true, &KillMessage, "", NULL },
			{ NULL, 0, false, NULL, "", NULL }
		};

		static ChatCommand Command[] =
		{
			{ "cheak", rbac::RBAC_PERM_COMMAND_CHEAK_CHEAK, true, NULL, "", MSGRECIVE },
			{ NULL, 0, false, NULL, "", NULL }
		};

		return Command;
	}

	static bool GmChatMessage(ChatHandler* handler, char const* args)
	{
		Player* player = handler->GetSession()->GetPlayer();

		std::string argstr = (char*)args;

		if (argstr == "off")
		{
			player->GM_CHECK_CHAT = false;
			ChatHandler(player->GetSession()).PSendSysMessage("Message Haye Player Ha Baraye Shoma Gheyre Fa'al Shodan");
			return true;
		}
		else if (argstr == "on")
		{
			player->GM_CHECK_CHAT = true;
			ChatHandler(player->GetSession()).PSendSysMessage("Message Haye Player Ha Baraye Shoma Fa'al Shodan");
			return true;
		}
		else
			ChatHandler(player->GetSession()).PSendSysMessage("Command Ra Eshteba Zadeid");

		return true;
	}

	static bool GlobalMessage(ChatHandler* handler, char const* args)
	{
		Player* player = handler->GetSession()->GetPlayer();

		std::string argstr = (char*)args;

		if (argstr == "off")
		{
			player->GLOBAL_MESSAGE_SEE = false;
			ChatHandler(player->GetSession()).PSendSysMessage("Message Haye Global Baraye Shoma Off Shod");
			return true;
		}
		else if (argstr == "on")
		{
			player->GLOBAL_MESSAGE_SEE = true;
			ChatHandler(player->GetSession()).PSendSysMessage("Message Haye Global Baraye Shoma On Shod");
			return true;
		}
		else
			ChatHandler(player->GetSession()).PSendSysMessage("Command Ra Eshteba Zadeid");

		return true;
	}

	static bool KillMessage(ChatHandler* handler, char const* args)
	{
		Player* player = handler->GetSession()->GetPlayer();

		std::string argstr = (char*)args;

		if (argstr == "off")
		{
			player->KILL_PLAYER_MESSAGE = false;
			ChatHandler(player->GetSession()).PSendSysMessage("Message Haye Kill Baraye Shoma Off Shod");
			return true;
		}
		else if (argstr == "on")
		{
			player->KILL_PLAYER_MESSAGE = true;
			ChatHandler(player->GetSession()).PSendSysMessage("Message Haye Kill Baraye Shoma On Shod");
			return true;
		}
		else
			ChatHandler(player->GetSession()).PSendSysMessage("Command Ra Eshteba Zadeid");

		return true;
	}
};

void AddSC_cheak_chat()
{
	new cheak_chat();
}