/*
*******************************
*		Overhack_dev 	  	  *
*							  *
* fixed By overhack_Dev       *
*							  *
*YID : overhack_Dev@yahoo.com *
*							  *
*Tell : 0930 408 5657         *
*******************************
*/
class PvPTiTLeS : public PlayerScript
{
    public:
        PvPTiTLeS() : PlayerScript("PvPTiTLeS") { }

        void OnPVPKill(Player* Killer, Player* Killed) OVERRIDE
        {
			 switch (Killer->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS))
			 {
                case 100:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(1));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(15));
                break;
                case 250:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(2));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(16));
                break;
                case 500:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(3));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(17));
                break;
                case 750:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(4));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(18));
                break;
                case 1000:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(5));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(19));
                break;
                case 1500:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(6));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(20));
                break;
                case 2500:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(7));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(21));
                break;
                case 4000:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(8));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(22));
                break;
                case 5500:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(9));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(23));
                break;
                case 7500:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(10));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(24));
                break;
                case 9000:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(11));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(25));
                break;
                case 11000:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(12));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(26));
                break;
                case 13500:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(13));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(27));
                break;
                case 15000:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(14));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(28));
                break;
                case 20000:
                    if (Killer->GetTeam() == ALLIANCE)
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(126));
                    else
                        Killer->SetTitle(sCharTitlesStore.LookupEntry(127));
                break;
            }
        }
};

void AddSC_PvPTiTLeS()
{
     new PvPTiTLeS();
}