class Duel_End_Script : public PlayerScript
{
    public:
        Duel_End_Script() : PlayerScript("Duel_End_Script") { }

        void OnDuelStart(Player* Winner, Player* Loser) OVERRIDE
        {
            DuelInfo(Winner, Loser);
        }

        void OnDuelEnd(Player* Winner, Player* Loser, DuelCompleteType) OVERRIDE
        {
		   if (Loser->GetHealth() < 1000)
			{
            DuelExecute(Winner, Loser);
			Winner->RemoveAllSpellCooldown();
			Loser->RemoveAllSpellCooldown();
			}
            Winner->SetHealth(Winner->GetMaxHealth());
            Winner->SetPower(POWER_MANA, Winner->GetMaxPower(POWER_MANA));
            Loser->SetHealth(Loser->GetMaxHealth());
            Loser->SetPower(POWER_MANA, Loser->GetMaxPower(POWER_MANA));
        }

        void DuelExecute(Player* Winner, Player* Loser)
        {
			if (Winner->GetAreaId() == 4242)
            {
				if (Winner->getLevel() == 85)
				{
					if (Loser->getLevel() == 85)
					{
						CharacterDatabase.PExecute("UPDATE duel_system SET win = (win + 1), rating = (rating + 10) WHERE guid = '%u'", Winner->GetGUID());
                        CharacterDatabase.PExecute("UPDATE duel_system SET lose = (lose + 1), rating = (rating - 5) WHERE guid = '%u'", Loser->GetGUID());	
					}
					else
					{
						ChatHandler(Loser->GetSession()).PSendSysMessage("[|cdd00ff00Duel|r]: Bayad 2 Taraf Level 85 Bashad ke Duel_rating Daryaft konid.");
					}
				}
				else
				{
				ChatHandler(Winner->GetSession()).PSendSysMessage("[|cdd00ff00Duel|r]: Bayad 2 Taraf Level 85 Bashad ke Duel_rating Daryaft konid.");
				}
            }
			else
			{
				ChatHandler(Winner->GetSession()).PSendSysMessage("[|cdd00ff00Duel|r]: Bayad Shoma va [%u] Dar .Duel bashid ke Duel_rating daryaft konid.", Loser->GetName());
				ChatHandler(Loser->GetSession()).PSendSysMessage("[|cdd00ff00Duel|r]: Bayad Shoma va [%u] Dar .Duel bashid ke Duel_rating daryaft konid.", Winner->GetName());
			}
        }

        void DuelInfo(Player* Winner, Player* Loser)
        {
            QueryResult WinnerQuery = CharacterDatabase.PQuery("SELECT * FROM duel_system WHERE guid = '%u'", Winner->GetGUID());
            QueryResult LoserQuery = CharacterDatabase.PQuery("SELECT * FROM duel_system WHERE guid = '%u'", Loser->GetGUID());

            if (WinnerQuery && LoserQuery)
            {
                Field* WinnerFields = WinnerQuery->Fetch();
                Field* LoserFields = LoserQuery->Fetch();

                int16 WWin = WinnerFields[1].GetInt16();
                int16 WLose = WinnerFields[2].GetInt16();
                int16 WRating = WinnerFields[3].GetInt16();
                int16 LWin = LoserFields[1].GetInt16();
                int16 LLose = LoserFields[2].GetInt16();
                int16 LRating = LoserFields[3].GetInt16();

                ChatHandler(Winner->GetSession()).PSendSysMessage("|cffff0000[info-Duel]|r |cff00ccff%s|r Win: [|cff00ccff%u|r] Lose: [|cff00ccff%u|r] Rating: [|cff00ccff%u|r]", Winner->GetName().c_str(), WWin, WLose, WRating);
                ChatHandler(Winner->GetSession()).PSendSysMessage("|cffff0000[info-Duel]|r |cff00ccff%s|r Win: [|cff00ccff%u|r] Lose: [|cff00ccff%u|r] Rating: [|cff00ccff%u|r]", Loser->GetName().c_str(), LWin, LLose, LRating);
                ChatHandler(Loser->GetSession()).PSendSysMessage("|cffff0000[info-Duel]|r |cff00ccff%s|r Win: [|cff00ccff%u|r] Lose: [|cff00ccff%u|r] Rating: [|cff00ccff%u|r]", Loser->GetName().c_str(), LWin, LLose, LRating);
                ChatHandler(Loser->GetSession()).PSendSysMessage("|cffff0000[info-Duel]|r |cff00ccff%s|r Win: [|cff00ccff%u|r] Lose: [|cff00ccff%u|r] Rating: [|cff00ccff%u|r]", Winner->GetName().c_str(), WWin, WLose, WRating);
            }
        }
};

class Duel_Command : public CommandScript
{
    public:
        Duel_Command() : CommandScript("Duel_Command") { }

        ChatCommand* GetCommands() const OVERRIDE
        {
            static ChatCommand Command[] =
            {
                { "infoduel", rbac::RBAC_PERM_COMMAND_DUEL_COMMAND, true,  &DuelCommand, "", NULL },
                { NULL,   0,                                    false, NULL,         "", NULL }
            };

            return Command;
        }

        static bool DuelCommand(ChatHandler* handler, char const* args)
        {
            Player* player = handler->GetSession()->GetPlayer();
            Player* target;

            if (handler->extractPlayerTarget((char*)args, &target))
            {
                QueryResult Result = CharacterDatabase.PQuery("SELECT * FROM duel_system WHERE guid = '%u'", target->GetGUID());

                if (Result)
                {
                    Field* Fields = Result->Fetch();

                    int16 Win = Fields[1].GetInt16();
                    int16 Lose = Fields[2].GetInt16();
                    int16 Rating = Fields[3].GetInt16();

                    ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000[info-Duel]|r |cff00ccff%s|r Win: [|cff00ccff%u|r] Lose: [|cff00ccff%u|r] Rating: [|cff00ccff%u|r]", target->GetName().c_str(), Win, Lose, Rating);
                }
            }
            else
            {
                QueryResult Result = CharacterDatabase.PQuery("SELECT * FROM duel_system WHERE guid = '%u'", player->GetGUID());

                if (Result)
                {
                    Field* Fields = Result->Fetch();

                    int16 Win = Fields[1].GetInt16();
                    int16 Lose = Fields[2].GetInt16();
                    int16 Rating = Fields[3].GetInt16();

                    ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000[info-Duel]|r |cff00ccff%s|r Win: [|cff00ccff%u|r] Lose: [|cff00ccff%u|r] Rating: [|cff00ccff%u|r]", player->GetName().c_str(), Win, Lose, Rating);
                }
            }

            return true;
        }
};

void AddSC_Duel_Script()
{
    new Duel_End_Script();
    new Duel_Command();
}