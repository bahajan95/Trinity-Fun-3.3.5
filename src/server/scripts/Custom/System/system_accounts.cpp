class Accounts : public PlayerScript
{
    public:
        Accounts() : PlayerScript("Accounts") { }

        void OnLogin(Player* player) OVERRIDE
        {
            QueryResult Result1 = LoginDatabase.PQuery("SELECT * FROM account_vip WHERE id = '%u' AND active = 1", player->GetSession()->GetAccountId());
            if (Result1)
                player->VIP_PLAYER_ACTIVE = true;

            QueryResult Result2 = CharacterDatabase.PQuery("SELECT * FROM duel_system WHERE guid = '%u'", player->GetGUID());
            if (!Result2)
                CharacterDatabase.PQuery("INSERT INTO duel_system (guid, win, lose, rating) VALUES (%u, 0, 0, 1000)", player->GetGUID());
			
			QueryResult Result3 = LoginDatabase.PQuery("SELECT * FROM account_megapoint WHERE `id` = '%u' AND `character_guid` = '%u'", player->GetSession()->GetAccountId(), player->GetGUID());
            if (!Result3)
                LoginDatabase.PQuery("insert INTO account_megapoint (id, character_guid, mps) VALUES (%u, %u, 0)", player->GetSession()->GetAccountId(), player->GetGUID());
        }
};

void AddSC_Accounts()
{
    new Accounts();
}