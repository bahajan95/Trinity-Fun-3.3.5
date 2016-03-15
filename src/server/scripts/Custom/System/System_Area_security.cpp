enum ForbiddenAreas
{
    AREA_TESTING         = 3817,
    AREA_GM_ISLAND           = 876
};
     
class Area_security : public PlayerScript
{
public:
    Area_security() : PlayerScript("Area_security") {}
     
    void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea)
    {
        switch (newArea)
        {
            case AREA_TESTING:
                if (player->GetSession()->GetSecurity() >= 1)
                    return;
                player->TeleportTo(1, 6566.661133f, -4047.084473f, 658.646118f, 2.083416f);
                break;
            case AREA_GM_ISLAND:
                if (player->GetSession()->GetSecurity() >= 1)
                    return;
                player->TeleportTo(1, 6566.661133f, -4047.084473f, 658.646118f, 2.083416f);
                break;
        }
    }
};
     
void AddSC_Area_security()
{
    new Area_security();
}  