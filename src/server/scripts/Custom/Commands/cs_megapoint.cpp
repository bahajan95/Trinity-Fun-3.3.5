#include "../system/system_megapoint.h"
#include "Language.h"

#define sM  ACE_Singleton<MegaPoint, ACE_Null_Mutex>::instance()

class Megapoint_Command : public CommandScript
{
public:
	Megapoint_Command() : CommandScript("Megapoint_Command") { }

	ChatCommand* GetCommands() const OVERRIDE
	{

		static ChatCommand megapointCommandTable[] =
		{
			{ "add", rbac::RBAC_PERM_COMMAND_MEGAPOINT_ADD_COMMAND, false, &HandleMegapointAddCommand, "", NULL },
			{ "remove", rbac::RBAC_PERM_COMMAND_MEGAPOINT_REMOVE_COMMAND, false, &HandleMegapointRemoveCommand, "", NULL },
			{ NULL, 0, false, NULL, "", NULL }
		};
		static ChatCommand megapointCommand[] =
		{
			{ "megapoint", rbac::RBAC_PERM_COMMAND_MEGAPOINT_COMMAND, false, NULL, "", megapointCommandTable },
			{ NULL, 0, false, NULL, "", NULL }
		};

		return megapointCommand;
	}

	static bool HandleMegapointAddCommand(ChatHandler* handler, char const* args)
	{
		if (!*args)
			return false;

		int32 value = atoi((char*)args);

		if (value < 1)
		{
			handler->SendSysMessage(LANG_BAD_VALUE);
			handler->SetSentErrorMessage(true);
			return false;
		}

		Player* target = handler->getSelectedPlayer();
		if (!target)
		{
			handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
			handler->SetSentErrorMessage(true);
			return false;
		}

		sM->AddPlayerMPs(target, value);
		handler->PSendSysMessage("%u MPs be Player Dade Shod.", value);
		return true;
	}

	static bool HandleMegapointRemoveCommand(ChatHandler* handler, char const* args)
	{
		if (!*args)
			return false;

		int32 value = atoi((char*)args);

		if (value < 1)
		{
			handler->SendSysMessage(LANG_BAD_VALUE);
			handler->SetSentErrorMessage(true);
			return false;
		}

		Player* target = handler->getSelectedPlayer();
		if (!target)
		{
			handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
			handler->SetSentErrorMessage(true);
			return false;
		}

		sM->RemovePlayerMPs(target, value);
		handler->PSendSysMessage("%u MPs az Player Kam Shod.", value);
		return true;
	}
};

void AddSC_Megapoint_Command()
{
	new Megapoint_Command();
}