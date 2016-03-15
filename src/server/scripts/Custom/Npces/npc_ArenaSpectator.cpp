#include "ScriptPCH.h"
#include "Chat.h"
#include "ArenaTeamMgr.h"
#include "BattlegroundMgr.h"
#include "WorldSession.h"
#include "Player.h"
#include "ArenaTeam.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "CreatureTextMgr.h"
#include "Config.h"

int8 UsingGossip;

class arena_spectator_commands : public CommandScript
{
public:
    arena_spectator_commands() : CommandScript("arena_spectator_commands") { }

    static bool HandleSpectateCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        uint64 target_guid;
        std::string target_name;
        if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
            return false;

        Player* player = handler->GetSession()->GetPlayer();
        if (target == player || target_guid == player->GetGUID())
        {
            handler->PSendSysMessage("You can't spectate yourself.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->IsInCombat())
        {
            handler->PSendSysMessage("You are in combat.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!target)
        {
            handler->PSendSysMessage("Target is not online or does not exist.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->GetPet())
        {
            handler->PSendSysMessage("You must hide your pet.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->GetMap()->IsBattlegroundOrArena() && !player->isSpectator())
        {
            handler->PSendSysMessage("You are already in a battleground or arena.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Map* cMap = target->GetMap();
        if (!cMap->IsBattleArena())
        {
            handler->PSendSysMessage("Player is not in an Arena match.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->GetMap()->IsBattleground())
        {
            handler->PSendSysMessage("You can't do that while in a battleground.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->HasAura(32728) || target->HasAura(32727))
        {
            handler->PSendSysMessage("You can't do that. The Arena match didn't start yet.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->isSpectator())
        {
            handler->PSendSysMessage("You can't do that. Your target is a spectator.");
            handler->SetSentErrorMessage(true);
            return false;
        }
        
        if (player->IsMounted())
        {
            handler->PSendSysMessage("Cannot Spectate while mounted.");
            handler->SetSentErrorMessage(true);
            return false;
        }


        // all's well, set bg id
        // when porting out from the bg, it will be reset to 0
        player->SetBattlegroundId(target->GetBattlegroundId(), target->GetBattlegroundTypeId());
        // remember current position as entry point for return at bg end teleportation
        if (!player->GetMap()->IsBattlegroundOrArena())
            player->SetBattlegroundEntryPoint();

        // stop flight if need
        if (player->IsInFlight())
        {
            player->GetMotionMaster()->MovementExpired();
            player->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            player->SaveRecallPosition();

        // search for two teams
        Battleground *bGround = target->GetBattleground();
        if (bGround->isRated())
        {
            uint32 slot = bGround->GetArenaType() - 2;
            if (bGround->GetArenaType() > 3)
                slot = 2;
            uint32 firstTeamID = target->GetArenaTeamId(slot);
            uint32 secondTeamID = 0;
            Player *firstTeamMember  = target;
            Player *secondTeamMember = NULL;
            for (Battleground::BattlegroundPlayerMap::const_iterator itr = bGround->GetPlayers().begin(); itr != bGround->GetPlayers().end(); ++itr)
                if (Player* tmpPlayer = ObjectAccessor::FindPlayer(itr->first))
                {
                    if (tmpPlayer->isSpectator())
                        continue;

                    uint32 tmpID = tmpPlayer->GetArenaTeamId(slot);
                    if (tmpID != firstTeamID && tmpID > 0)
                    {
                        secondTeamID = tmpID;
                        secondTeamMember = tmpPlayer;
                        break;
                    }
                }

                if (firstTeamID > 0 && secondTeamID > 0 && secondTeamMember)
                {
                    ArenaTeam *firstTeam  = sArenaTeamMgr->GetArenaTeamById(firstTeamID);
                    ArenaTeam *secondTeam = sArenaTeamMgr->GetArenaTeamById(secondTeamID);
                    if (firstTeam && secondTeam)
                    {
                        handler->PSendSysMessage("You entered a Rated Arena.");
                        handler->PSendSysMessage("Teams:");
                        handler->PSendSysMessage("|cFFffffff%s|r vs |cFFffffff%s|r", firstTeam->GetName().c_str(), secondTeam->GetName().c_str());
                        handler->PSendSysMessage("|cFFffffff%u(%u)|r -- |cFFffffff%u(%u)|r", firstTeam->GetRating(), firstTeam->GetAverageMMR(firstTeamMember->GetGroup()),
                            secondTeam->GetRating(), secondTeam->GetAverageMMR(secondTeamMember->GetGroup()));
                    }
                }
        }

        // to point to see at target with same orientation
        float x, y, z;
        target->GetContactPoint(player, x, y, z);

        player->TeleportTo(target->GetMapId(), x, y, z, player->GetAngle(target), TELE_TO_GM_MODE);
        player->SetPhaseMask(target->GetPhaseMask(), true);
        player->SetSpectate(true);
        target->GetBattleground()->AddSpectator(player->GetGUID());

        return true;
    }

    static bool HandleSpectateCancelCommand(ChatHandler* handler, const char* /*args*/)
    {
        Player* player =  handler->GetSession()->GetPlayer();

        if (!player->isSpectator() || player->isSpectateCanceled())
        {
            handler->PSendSysMessage("You are not a spectator.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        // RemoveSpectator shouldn't be needed to be done explicitly as it'll be done in the CancelSpectate func
        //player->GetBattleground()->RemoveSpectator(player->GetGUID());
        player->CancelSpectate();
        player->TeleportToBGEntryPoint();

        return true;
    }

    static bool HandleSpectateFromCommand(ChatHandler* handler, const char *args)
    {
        Player* target;
        uint64 target_guid;
        std::string target_name;
        if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
            return false;

        Player* player = handler->GetSession()->GetPlayer();

        if (!target)
        {
            handler->PSendSysMessage("Player is not online or does not exist.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!player->isSpectator())
        {
            handler->PSendSysMessage("You are not a spectator, spectate someone first.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->isSpectator() && target != player)
        {
            handler->PSendSysMessage("You can't do that. Your target is a spectator.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (player->GetMap() != target->GetMap())
        {
            handler->PSendSysMessage("You can't do that. Your target might be in a different arena match.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target && target->HasAuraType(SPELL_AURA_MOD_STEALTH))
        {
            handler->PSendSysMessage("You can't target stealthed players.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check for arena preperation
        // if exists than battle didn`t begin
        if (target && target->HasAura(32728) || target->HasAura(32727))
        {
            handler->PSendSysMessage("You can't do that. The Arena match didn't start yet.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target == player && player->getSpectateFrom())
            player->SetViewpoint(player->getSpectateFrom(), false);
        else
            player->SetViewpoint(target, true);
        return true;
    }

    // addon data
    static bool HandleSpectateResetCommand(ChatHandler* handler, const char *args)
    {
        Player* player = handler->GetSession()->GetPlayer();

        if (!player->isSpectator())
        {
            handler->PSendSysMessage("You are not a spectator.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Battleground *bGround = player->GetBattleground();
        if (!bGround)
            return false;

        if (bGround->GetStatus() != STATUS_IN_PROGRESS)
            return true;

        for (Battleground::BattlegroundPlayerMap::const_iterator itr = bGround->GetPlayers().begin(); itr != bGround->GetPlayers().end(); ++itr)
            if (Player* tmpPlayer = ObjectAccessor::FindPlayer(itr->first))
            {
                if (tmpPlayer->isSpectator())
                    continue;

                uint32 tmpID = bGround->GetPlayerTeam(tmpPlayer->GetGUID());

                // generate addon massage
                std::string pName = tmpPlayer->GetName();
                std::string tName = "";

                if (Player *target = tmpPlayer->GetSelectedPlayer())
                    tName = target->GetName();

                SpectatorAddonMsg msg;
                msg.SetPlayer(pName);
                if (tName != "")
                    msg.SetTarget(tName);
                msg.SetStatus(tmpPlayer->IsAlive());
                msg.SetClass(tmpPlayer->getClass());
                msg.SetCurrentHP(tmpPlayer->GetHealth());
                msg.SetMaxHP(tmpPlayer->GetMaxHealth());
                Powers powerType = tmpPlayer->getPowerType();
                msg.SetMaxPower(tmpPlayer->GetMaxPower(powerType));
                msg.SetCurrentPower(tmpPlayer->GetPower(powerType));
                msg.SetPowerType(powerType);
                msg.SetTeam(tmpID);
                msg.SendPacket(player->GetGUID());
            }

            return true;
    }

    ChatCommand* GetCommands() const OVERRIDE
    {
        static ChatCommand spectateCommandTable[] =
        {
            { "player",          rbac::RBAC_PERM_COMMAND_PLAYER_COMMAND,      true,  &HandleSpectateCommand,        "", NULL },
            { "view",            rbac::RBAC_PERM_COMMAND_VIEW_COMMAND,      true,  &HandleSpectateFromCommand,    "", NULL },
            { "reset",           rbac::RBAC_PERM_COMMAND_RESET_COMMAND,      true,  &HandleSpectateResetCommand,   "", NULL },
            { "leave",           rbac::RBAC_PERM_COMMAND_LEAVE_COMMAND,      true,  &HandleSpectateCancelCommand,  "", NULL },
            { NULL,             0,               false, NULL,                          "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "spectate",       rbac::RBAC_PERM_COMMAND_SPECTATE_COMMAND, false,  NULL, "", spectateCommandTable },
            { NULL,             0,          false,  NULL, "", NULL }
        };
        return commandTable;
    }
};

enum NpcSpectatorAtions {
    // will be used for scrolling
    NPC_SPECTATOR_ACTION_2V2_GAMES          = 1000,
    NPC_SPECTATOR_ACTION_3V3_GAMES          = 2000,
    NPC_SPECTATOR_ACTION_SPECIFIC           = 500,

    // NPC_SPECTATOR_ACTION_SELECTED_PLAYER + player.Guid()
    NPC_SPECTATOR_ACTION_SELECTED_PLAYER    = 3000
};

const uint8  GamesOnPage    = 15;

class npc_arena_spectator : public CreatureScript
{
public:
    npc_arena_spectator() : CreatureScript("npc_arena_spectator") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\icons\\Achievement_Arena_2v2_7:30|t Games: 2v2", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_2V2_GAMES);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\icons\\Achievement_Arena_3v3_7:30|t Games: 3v3", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_3V3_GAMES);
        pPlayer->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "|TInterface\\icons\\Spell_Holy_DevineAegis:30|t Spectate Specific Player.", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SPECIFIC, "", 0, true);
        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == NPC_SPECTATOR_ACTION_SPECIFIC)
        {

        }
        if (action = NPC_SPECTATOR_ACTION_2V2_GAMES)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Refresh", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_2V2_GAMES);
            ShowPage(player, action - NPC_SPECTATOR_ACTION_2V2_GAMES, false);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        }
        else if (action = NPC_SPECTATOR_ACTION_3V3_GAMES)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Refresh", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_3V3_GAMES);
            ShowPage(player, action - NPC_SPECTATOR_ACTION_3V3_GAMES, true);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        }
        else
        {

            uint64 guid = action - NPC_SPECTATOR_ACTION_SELECTED_PLAYER;
            if (Player* target = ObjectAccessor::FindPlayer(guid))
            {
                ChatHandler handler(player->GetSession());
                char const* pTarget = target->GetName().c_str();
                arena_spectator_commands::HandleSpectateCommand(&handler, pTarget);
            }
        }
        return true;
    }

    std::string GetClassNameById(uint8 id)
    {
        std::string sClass = "";
        switch (id)
        {
        case CLASS_WARRIOR:         sClass = "Warrior ";           break;
        case CLASS_PALADIN:         sClass = "Paladin ";           break;
        case CLASS_HUNTER:          sClass = "Hunter  ";           break;
        case CLASS_ROGUE:           sClass = "Rogue   ";           break;
        case CLASS_PRIEST:          sClass = "Priest  ";           break;
        case CLASS_DEATH_KNIGHT:    sClass = "DKnight ";           break;
        case CLASS_SHAMAN:          sClass = "Shaman  ";           break;
        case CLASS_MAGE:            sClass = "Mage    ";           break;
        case CLASS_WARLOCK:         sClass = "Warlock ";           break;
        case CLASS_DRUID:           sClass = "Druid   ";           break;
        }
        return sClass;
    }

    std::string GetGamesStringData(Battleground* team, uint16 mmr, uint16 mmrTwo)
    {
        std::string teamsMember[BG_TEAMS_COUNT];
        uint32 firstTeamId = 0;
        for (Battleground::BattlegroundPlayerMap::const_iterator itr = team->GetPlayers().begin(); itr != team->GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                if (player->isSpectator())
                    continue;

                if (player->IsGameMaster())
                    continue; 

                uint32 team = itr->second.Team;
                if (!firstTeamId)
                    firstTeamId = team;

                teamsMember[firstTeamId == team] += GetClassNameById(player->getClass());
            }

            std::string data = teamsMember[0] + "(";
            std::stringstream sstwo;
            std::stringstream ss;
            ss << mmr;
            sstwo << mmrTwo;
            data += ss.str();
            data += ") - ";
            data += teamsMember[1] + "(" + sstwo.str();
            data += ")";
            return data;
    }

    uint64 GetFirstPlayerGuid(Battleground* team)
    {
        for (Battleground::BattlegroundPlayerMap::const_iterator itr = team->GetPlayers().begin(); itr != team->GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                return itr->first;
        return 0;
    }

    void ShowPage(Player* player, uint16 page, bool IsTop)
    {
        uint32 firstTeamId = 0;
        uint16 TypeTwo = 0;
        uint16 TypeThree = 0;
        uint16 mmr = 0;
        uint16 mmrTwo = 0;
        bool haveNextPage = false;
        for (uint8 i = 0; i <= MAX_BATTLEGROUND_TYPE_ID; ++i)
        {
            if (!sBattlegroundMgr->IsArenaType(BattlegroundTypeId(i)))
                continue;

            BattlegroundData* arenas = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BattlegroundTypeId(i));

            if (!arenas || arenas->m_Battlegrounds.empty())
                continue;

            for (BattlegroundContainer::const_iterator itr = arenas->m_Battlegrounds.begin(); itr != arenas->m_Battlegrounds.end(); ++itr)
            {
                Battleground* arena = itr->second;
                Player* target = ObjectAccessor::FindPlayer(GetFirstPlayerGuid(arena));
                if (!target)
                    continue;

                if (target->HasAura(32728) || target->HasAura(32727))
                    continue;

                if (!arena->GetPlayersSize())
                    continue;

                if (!arena->isRated())
                    continue;

                if (arena->GetArenaType() == ARENA_TYPE_2v2)
                {
                    mmr = arena->GetArenaMatchmakerRatingByIndex(0);
                    firstTeamId = target->GetArenaTeamId(0);
                    Battleground::BattlegroundPlayerMap::const_iterator citr = arena->GetPlayers().begin();
                    for (; citr != arena->GetPlayers().end(); ++citr)
                        if (Player* plrs = sObjectAccessor->FindPlayer(citr->first))
                            if (plrs->GetArenaTeamId(0) != firstTeamId)
                                mmrTwo = arena->GetArenaMatchmakerRating(citr->second.Team);
                }
                else
                {
                    mmr = arena->GetArenaMatchmakerRatingByIndex(1);
                    firstTeamId = target->GetArenaTeamId(1);
                    Battleground::BattlegroundPlayerMap::const_iterator citr = arena->GetPlayers().begin();
                    for (; citr != arena->GetPlayers().end(); ++citr)
                        if (Player* plrs = sObjectAccessor->FindPlayer(citr->first))
                            if (plrs->GetArenaTeamId(1) != firstTeamId)
                                mmrTwo = arena->GetArenaMatchmakerRating(citr->second.Team);
                }

                if (IsTop && arena->GetArenaType() == ARENA_TYPE_3v3)
                {
                    TypeThree++;
                    if (TypeThree > (page + 1) * GamesOnPage)
                    {
                        haveNextPage = true;
                        break;
                    }

                    if (TypeThree >= page * GamesOnPage)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmr, mmrTwo), GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + GetFirstPlayerGuid(arena));
                }
                else if (!IsTop && arena->GetArenaType() == ARENA_TYPE_2v2)
                {
                    TypeTwo++;
                    if (TypeTwo > (page + 1) * GamesOnPage)
                    {
                        haveNextPage = true;
                        break;
                    }

                    if (TypeTwo >= page * GamesOnPage)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmr, mmrTwo), GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + GetFirstPlayerGuid(arena));
                }
            }
        }

        if (page > 0)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Prev..", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_2V2_GAMES + page - 1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Prev..", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_3V3_GAMES + page - 1);
        }

        if (haveNextPage)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Next..", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_2V2_GAMES + page + 1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "Next..", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_3V3_GAMES + page + 1);
        }
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code)
    {
        if (!player)
            return true;

        player->PlayerTalkClass->ClearMenus();
        player->CLOSE_GOSSIP_MENU();
        if (sender == GOSSIP_SENDER_MAIN)
        {
            switch (action)
            {
            case NPC_SPECTATOR_ACTION_SPECIFIC: // choosing a player

                const char* plrName = code;

                char playerName[50];
                strcpy(playerName, plrName);

                for (int i = 0; i < 13; i++)
                {
                    if (playerName[i] == NULL)
                        break;
                    if (i == 0 && playerName[i] > 96)
                        playerName[0] -= 32;
                    else if (playerName[i] < 97)
                        playerName[i] += 32;
                }

                if (Player* target = sObjectAccessor->FindPlayerByName(playerName))
                {
                    ChatHandler handler(player->GetSession());
                    char const* pTarget = target->GetName().c_str();
                    arena_spectator_commands::HandleSpectateCommand(&handler, pTarget);
                }
                ChatHandler(player->GetSession()).PSendSysMessage("Player is not online or does not exist.");
                return true;
            }
        }

        return false;
    }
};



void AddSC_arena_spectator_script()
{
    new arena_spectator_commands();
    new npc_arena_spectator();
}