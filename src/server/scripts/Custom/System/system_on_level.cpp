class OnLevelChange : public PlayerScript
{
public:
	OnLevelChange() : PlayerScript("OnLevelChange") { }

	void OnLevelChanged(Player* player, uint8 /* Level */) OVERRIDE
	{
		ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000Congratulations, you've got a level: %u", player->getLevel());
		player->SetHealth(player->GetMaxHealth());
		player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));

		if (player->getLevel() == sWorld->getIntConfig(PLAYER_LEVEL_EVENT))
		{
			player->GiveLevel(80);
			player->InitTalentForLevel();
			player->ModifyMoney(50000000);
			player->GetTeam() == HORDE ? player->learnSpell(23251, false) : player->learnSpell(23228, false);
			player->TeleportTo(571, 5972.441895f, 662.624939f, 641.373657f, 2.845418f);
		}
	}
};

void AddSC_OnLevelChange()
{
	new OnLevelChange();
}