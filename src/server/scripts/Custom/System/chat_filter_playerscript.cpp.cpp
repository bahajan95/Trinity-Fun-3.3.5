/*
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptPCH.h"
#include "DatabaseEnv.h"

enum ChatFilterAuras
{
	SPELL_AURA_FREEZE = 9454,
	SPELL_AURA_STUN = 31539,
};

class chat_filter_playerscript : public PlayerScript
{
public:
	chat_filter_playerscript() : PlayerScript("chat_filter_playerscript") { }

	void OnLogin(Player* player)
	{
		if (player->HasAura(SPELL_AURA_FREEZE))
		{
			if (player->LoggedOutWhilePunished())
				player->GetSession()->SendNotification("Your freeze timer was set to the number before you logged out during your freeze.");

			if (QueryResult result = CharacterDatabase.PQuery("SELECT FreezeTimer FROM characters WHERE guid = %u", player->GetGUIDLow()))
			{
				player->SetFreezeStunTimer(true, (*result)[0].GetUInt32());
				CharacterDatabase.PExecute("UPDATE characters SET FreezeTimer = 0 WHERE guid = %u", player->GetGUIDLow());
			}
		}

		if (player->HasAura(SPELL_AURA_STUN))
		{
			if (player->LoggedOutWhilePunished())
				player->GetSession()->SendNotification("Your stun timer was set to the number before you logged out during your stun.");

			if (QueryResult result = CharacterDatabase.PQuery("SELECT StunTimer FROM characters WHERE guid = %u", player->GetGUIDLow()))
			{
				player->SetFreezeStunTimer(false, (*result)[0].GetUInt32());
				CharacterDatabase.PExecute("UPDATE characters SET StunTimer = 0 WHERE guid = %u", player->GetGUIDLow());
			}
		}

		player->SetLoggedOutWhilePunished(false);
	}

	void OnLogout(Player* player)
	{
		//! If player is frozen or stunned by the chat filter system because
		//! they used 'bad words' and logout during their punishment, the
		//! remaining time they should be frozen or stunned will be saved
		//! into a database column and pulled back in on logout.
		if (player->HasAura(SPELL_AURA_FREEZE))
		{
			player->SetLoggedOutWhilePunished(true);
			CharacterDatabase.PExecute("UPDATE characters SET FreezeTimer = %u WHERE guid = %u", player->GetFreezeStunTimer(true), player->GetGUIDLow());
		}

		if (player->HasAura(SPELL_AURA_STUN))
		{
			player->SetLoggedOutWhilePunished(true);
			CharacterDatabase.PExecute("UPDATE characters SET StunTimer = %u WHERE guid = %u", player->GetFreezeStunTimer(false), player->GetGUIDLow());
		}
	}
};

void AddSC_chat_filter_playerscript()
{
	new chat_filter_playerscript();
}
