#include "ScriptPCH.h"

/*
Portal Master convert to C++ for Trinitycore
By Rochet2

Credits:
Rochet2
Eric
Lhorath

ScriptName: cloner_teleporter
*/

//      Link Types//
/*
{#,     #,      2,      "|cff4169E1 Non-Icon Links|r",  0,      0,      0,      0,      0,      0,      0,      0},
{#,     #,      2,      "|cff4169E1|TInterface\\icons\\Achievement_Leader_King_Varian_Wrynn:30|t Icon & Color Links|r", 0,      0,      0,      0,      0,      0,      0,      0},
{#,     #,      2,      "|TInterface\\icons\\Achievement_Leader_King_Varian_Wrynn:30|t Icon Links (NoColor)|t", 0,      0,      0,      0,      0,      0,      0,      0},
{#,     #,      2,      "Non-Formated Links Links",     0,      0,      0,      0,      0,      0,      0,      0},
*/

// Color List //
/*
ORANGE                  "|cffFFA500"
DARKORANGE              "|cffFF8C00"
RED                             "|cffFF0000"
LIGHTRED                "|cffD63931"
ROYALBLUE               "|cff4169E1"
LIGHTBLUE               "|cffADD8E6"
YELLOW                  "|cffFFFF00"
GREEN                   "|cff008000"
PURPLE                  "|cffDA70D6"
WHITE                   "|cffffffff"
SUBWHITE                "|cffbbbbbb"
*/

#include "ScriptPCH.h"

#define SERVER_WHISPER "Welcome To Golden Game"
#define ARE_YOU_SURE    "|cffffffffAre You Sure Teleport To|r "
#define ERROR_COMBAT    "|cffff0000You Are In Combat!|r"

// TextIDs from npc_text
enum npc_text_ids
{
    TEXT_MAIN_H             =       400000,                 //Horde main menu text
    TEXT_MAIN_A             =       400001,                 //Alliance main menu text
    TEXT_DUNGEON            =       400002,                 //Dungeon teleport menu texts
    TEXT_RAID               =       400003,                 //Raid teleport menu text
    TEXT_AREA               =       400004,                 //Area teleport location menu texts
};

// Teleport data

struct Rochet2
{
    uint32 menu_id; uint32 next_menu_id; uint8 icon; std::string name; uint32 cost; uint8 level; uint8 faction; // 0 Both, 1 Horde, 2 Ally
    uint32 map; float x; float y; float z; float o;
};

//  {menu_id, next_menu_id, icon, name, cost, level, faction, map, x, y, z, o}
struct Rochet2 TeLe [] = // key = i & Key
{
	{1,     0,      2,      "|TInterface\\icons\\INV_Misc_ShadowEgg:25|t Shop Mall",     0,      0,      0,      530,      -2268.540039,      5565.250000,      67.008598,      5.967160},
    {1,     0,      2,      "|TInterface\\icons\\TEMP:25|t Duel Zone",     0,      0,      0,      1,     8049.740234,      -3888.489990,      687.028015,      3.831340},
	{1,     0,      2,      "|TInterface\\icons\\Achievement_BG_KillXEnemies_GeneralsRoom:25|t PvP Zone",     100000,      0,      0,      0,      -13181.799805,      339.355988,      42.980488,      4.368848},
	{1,     5,      2,      "|TInterface\\icons\\Achievement_Dungeon_HEROIC_GloryoftheRaider:25|t Instances",     0,      0,      0,      0,      0,      0,      0,      0},
    {1,     2,      2,      "|cff4169E1|TInterface\\icons\\INV_BannerPVP_02:30|t Alliance Cities|r",    0,      0,      0,      0,      0,      0,      0,      0},
    {1,     3,      2,      "|cffFF0000|TInterface\\icons\\INV_BannerPVP_01:30|t Horde Cities|r",        0,      0,      0,      0,      0,      0,      0,      0},
    {1,     4,      2,      "|cffFFFF00|TInterface\\icons\\Achievement_Zone_Hinterlands_01:30|t Neutral Cities|r",     0,      0,      0,      0,      0,      0,      0,      0},

    {2,     0,      2,      "|cff4169E1|TInterface\\icons\\Spell_arcane_teleportstormwind:30|t Stormwind|r",    0,      0,      2,      0,      -8886.603516,      575.004028,      92.769081,      0.792816},
    {2,     0,      2,      "|CFF540081|TInterface\\icons\\Spell_arcane_teleportdarnassus:30|t Darnassus|r",    0,      0,      2,      1,      9869.91f,       2493.58f,       1315.88f,       2.78897f},
    {2,     0,      2,      "|cFF00FFFF|TInterface\\icons\\Spell_arcane_teleportironforge:30|t Ironforge|r",    0,      0,      2,      0,      -4900.47f,      -962.585f,      501.455f,       5.40538f},
    {2,     0,      2,      "|cffff9696|TInterface\\icons\\Spell_arcane_teleportexodar:30|t The Exodar|r",       0,      0,      2,      530,    -3864.92f,      -11643.7f,      -137.644f,      5.50862f},
    {2,     0,      9,      "|CFFFE8A0E|TInterface\\icons\\Achievement_leader_king_varian_wrynn:30|t Attack|r To |cff4169E1Stormwind|r", 0,      80,     1,      0,      -9154.49f,      363.471f,      90.0444f,       0.79674f},
    {2,     0,      9,      "|CFFFE8A0E|TInterface\\icons\\Achievement_leader_tyrande_whisperwind:30|t Attack|r To |CFF540081Darnassus|r", 0,      80,     1,      1,      9983.334f,       1948.610f,       1325.28f,       1.59779f},
    {2,     0,      9,      "|CFFFE8A0E|TInterface\\icons\\Achievement_leader_king_magni_bronzebeard:30|t Attack|r To |cFF00FFFFIronforge|r", 0,      80,     1,      0,      -5050.398f,      -800.744f,      495.127f,        0.94253f},
    {2,     0,      9,      "|CFFFE8A0E|TInterface\\icons\\Achievement_leader_prophet_velen:30|t Attack|r To |cffff9696Exodar|r",    0,      80,     1,      530,    1486.98,      -4416.04,      24.1094,      0.150753},
    {2,     1,      7,      "|cffffffffBack..|r",   0,      0,      0,      0,      0,      0,      0,      0},

    {3,     0,      2,      "|CFFFE8A0E|TInterface\\icons\\Spell_arcane_teleportorgrimmar:30|t Orgrimmar |r",    0,      0,      1,      1,      1601.08f,       -4378.69f,      9.9846f,        2.14362f},
    {3,     0,      2,      "|cFF00FFFF|TInterface\\icons\\Spell_arcane_teleportthunderbluff:30|t Thunder bluff |r",        0,      0,      1,      1,      -1274.45f,      71.8601f,       128.159f,       2.80623f},
    {3,     0,      2,      "|cFF00FF00|TInterface\\icons\\Spell_arcane_teleportundercity:30|t Undercity|r",    0,      0,      1,      0,      1633.75f,       240.167f,       -43.1034f,      6.26128f},
    {3,     0,      2,      "|cFFFF0303|TInterface\\icons\\Spell_arcane_teleportsilvermoon:30|t Silvermoon|r",      0,      0,      1,      530,    9738.28f,       -7454.19f,      13.5605f,       0.043914f},
    {3,     0,      9,      "|CFFFE8A0E|TInterface\\icons\\Achievement_character_orc_male:30|t Attack|r To |CFFFE8A0EOrgrimmar|r", 0,      80,     2,      1,      1324.28f,       -4380.951f,      26.2216f,       0.172750f},
    {3,     0,      9,      "|CFFFE8A0E|TInterface\\icons\\Achievement_character_tauren_male:30|t Attack|r To |cFF00FFFFThunder bluff|r",     0,      80,     2,      1,      -1304.11f,      205.72355f,      68.6815f,      5.06174f},
    {3,     0,      9,      "|CFFFE8A0E|TInterface\\icons\\Achievement_leader_sylvanas:30|t Attack|r To |cFF00FF00Undercity|r", 0,      80,     2,      0,      1892.555f,       237.426f,       56.4824f,       3.542357f},
    {3,     0,      9,      "|CFFFE8A0E|TInterface\\icons\\Achievement_character_bloodelf_male:30|t Attack|r To |CFFFF0303Silvermoon|r",        0,      80,     2,      530,    9375.61f,       -7279.03f,      14.23984f,       6.24227f},
    {3,     1,      7,      "|cffffffffBack..|r",   0,      0,      0,      0,      0,      0,      0,      0},

    {4,     0,      2,      "|CFFE55BB0|TInterface\\icons\\Spell_arcane_teleportdalaran:30|t Dalaran|r",      0,      67,     0,      571,    5809.55f,       503.975f,       657.526f,       2.38338f},
    {4,     0,      2,      "|CFF008000|TInterface\\icons\\Spell_arcane_teleportshattrath:30|t Shattrath|r",    0,      57,     0,      530,    -1887.62f,      5359.09f,       -12.4279f,      4.40435f},
    {4,     0,      2,      "|CFF00FFFF|TInterface\\icons\\Achievement_zone_howlingfjord_01:30|t Booty bay|r",    0,      30,     0,      0,      -14281.9f,      552.564f,       8.90422f,       0.860144f},
    {4,     1,      7,      "|cffffffffBack..|r",   0,      0,      0,      0,      0,      0,      0,      0},
	
	{5,     0,      2,      "|CFFE55BB0|TInterface\\icons\\Achievement_Zone_IceCrown_01:30|t icecrown Citadel|r",      0,      80,     0,      571,    5859.278320f,       2103.904297f,       635.932434f,       3.577433f},
    {5,     1,      7,      "|cffffffffBack..|r",   0,      0,      0,      0,      0,      0,      0,      0},
    //      {10,    0,      2,      "Teleport",     0,      0,      0,      map,    xf,     yf,     zf,     of},
    //      {10,    1,      7,      "Back..",       0,      0,      0,      0,      0,      0,      0,      0},
};

const uint32 size = sizeof TeLe/sizeof(*TeLe);

class cloner_teleporter : public CreatureScript
{
public:
    struct Range {
        Range(uint32 startValue) : start(startValue), stop(startValue) {}
        Range() : start(0), stop(0) {}
        uint32 start, stop;
    };
    typedef std::map<uint32, Range> menuRangeMap;
    menuRangeMap menuRanges;

    cloner_teleporter() : CreatureScript("Npce_teleporter")
    {
        if(size < 1)
            return;
        uint32 lastMenuId = TeLe[0].menu_id;
        Range* temp = new Range(0);
        for(uint32 i = 0; i <= sizeof TeLe/sizeof(*TeLe); ++i)
        {
            Rochet2* data = &TeLe[i];
            if(data->menu_id != lastMenuId)
            {
                temp->stop = i;
                menuRanges[lastMenuId] = *temp;
                lastMenuId = data->menu_id;
                temp = new Range(i);
                temp->start = i;
            }
        }
        temp->stop = size;
        menuRanges[lastMenuId] = *temp;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        //Teleports
        Custom_GetMenu(player, creature, 1); // 1 = main menu
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 menu_id, uint32 id) // ID means the key in the array
    {
        player->PlayerTalkClass->ClearMenus(); // clear the menu

        Rochet2* data = &TeLe[id];
        player->ModifyMoney(-1 * data->cost); // take cash

        if (data->next_menu_id)
        {
            switch(data->map) // MapID used to identify option used
            {
            case 1:
                player->GetSession()->SendAreaTriggerMessage(SERVER_WHISPER);
                ChatHandler(player->GetSession()).PSendSysMessage(SERVER_WHISPER);
                break;
            case 2: // Full Heal and Mana
            if(player->IsInCombat())
	        {
			ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000You are in Combat.|r");
            player->CLOSE_GOSSIP_MENU();
			return false;
            }
			if (player->InArena())
			{
			ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000You are in Combat.|r");
			return false;
			}
			else if(player->getPowerType() == POWER_MANA)
            player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));

			player->SetHealth(player->GetMaxHealth());
            player->CLOSE_GOSSIP_MENU();
                break;
            default:
                Custom_GetMenu(player, creature, data->next_menu_id);
                return true;
            }
            Custom_GetMenu(player, creature, 1);
        }
        else if (player->IsInCombat())
        {
            player->GetSession()->SendNotification(ERROR_COMBAT);
            Custom_GetMenu(player, creature, data->menu_id);
        }
        else
        {
            player->CLOSE_GOSSIP_MENU();
            player->TeleportTo(data->map, data->x, data->y, data->z, data->o);
        }
        return true;
    }

    static bool Custom_FactCheck(uint32 team, uint32 fact)
    {
        switch (fact)
        {
        case 0: return true;
        case 1: return (team == HORDE);
        case 2: return (team == ALLIANCE);
        }
        return false;
    }

    static uint32 Custom_GetText(uint32 menu, uint32 team)
    {
        switch (menu)
        {
        case 1:
            if (team == ALLIANCE)
                return TEXT_MAIN_A;
            else
                return TEXT_MAIN_H;
        case 2:
        case 3:
        case 4:
            return TEXT_DUNGEON;
        case 5:
            return TEXT_RAID;
        }
        return TEXT_AREA;
    }

    void Custom_GetMenu(Player* player, Creature* creature, uint32 menu_id)
    {
        if(!creature || !player)
            return;
        menuRangeMap::const_iterator it = menuRanges.find(menu_id);
        if(it == menuRanges.end()) // invalid menu_id
            return;
        for (uint32 i = it->second.start; i < it->second.stop; ++i)
        {
            Rochet2* data = &TeLe[i];
            if (player->getLevel() >= data->level && Custom_FactCheck(player->GetTeam(), data->faction))
            {
                if (data->next_menu_id)
                    player->ADD_GOSSIP_ITEM_EXTENDED(data->icon, data->name, menu_id, i, "", data->cost, false);
                else
                    player->ADD_GOSSIP_ITEM_EXTENDED(data->icon, data->name, menu_id, i, ARE_YOU_SURE+data->name, data->cost, false);
            }
        }
        player->SEND_GOSSIP_MENU(Custom_GetText(menu_id, player->GetTeam()), creature->GetGUID());
    }
};

void AddSC_npcs_telep()
{
    new cloner_teleporter();
}