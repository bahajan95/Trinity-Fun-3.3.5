class npc_top_players : public CreatureScript
{
    public:
        npc_top_players() : CreatureScript("npc_top_players") { }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();   
            player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Achievement_BG_kill_flag_carrier:30:30:-18:0|t|r Top Killer", GOSSIP_SENDER_MAIN, 1);
            player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Ability_Rogue_WrongfullyAccused:30:30:-18:0|t|r Top Killer Today", GOSSIP_SENDER_MAIN, 7);
            player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Achievement_Boss_LordMarrowgar:30:30:-18:0|t|r Top Killer YesterDay", GOSSIP_SENDER_MAIN, 8);
            player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Achievement_Arena_2v2_7:30:30:-18:0|t|r Top Arena 2 Vs 2", GOSSIP_SENDER_MAIN, 2);
            player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Achievement_Arena_3v3_7:30:30:-18:0|t|r Top Arena 3 Vs 3", GOSSIP_SENDER_MAIN, 3);
            player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Achievement_Leader_Sylvanas:30:30:-18:0|t|r Top Class Killer", GOSSIP_SENDER_MAIN, 4);
            player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Achievement_BG_KillXEnemies_GeneralsRoom:30:30:-18:0|t|r Top Dueler", GOSSIP_SENDER_MAIN, 5);
            player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\Achievement_BG_grab_cap_flagunderXseconds:30:30:-18:0|t|r Top Play Time", GOSSIP_SENDER_MAIN, 6);
            player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
                case 1:
                {
                    QueryResult Result = CharacterDatabase.Query("SELECT name,totalKills FROM characters WHERE name != '' ORDER BY totalKills DESC LIMIT 10");
                    if (Result)
                    {
                        Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Killer]|r", GOSSIP_SENDER_MAIN, 0);
                        do
                        {
                            std::string Name = Fields[0].GetString();
                            uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
                        }
                        while(Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                   
                }
                break;
                case 2:
                {
                    QueryResult Result = CharacterDatabase.Query("SELECT name,rating,seasonGames,seasonWins FROM arena_team WHERE type = 2 ORDER BY rating DESC LIMIT 10");
                    if (Result)
                    {
                        Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Arena 2 Vs 2]|r", GOSSIP_SENDER_MAIN, 0);
                        do
                        {
                            std::string Name = Fields[0].GetString();
                            int16 Rating = Fields[1].GetInt16();
                            int16 Games = Fields[2].GetInt16();
                            int16 Wins = Fields[3].GetInt16();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Rating|r: |CFF006400" << Rating << "|r |cff000000Win|r: |CFF006400" << Wins << "|r |cff000000Lose|r: |CFF006400" << Games - Wins << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
                        }
                        while(Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 3:
                {
                    QueryResult Result = CharacterDatabase.Query("SELECT name,rating,seasonGames,seasonWins FROM arena_team WHERE type = 3 ORDER BY rating DESC LIMIT 10");
                    if (Result)
                    {
                        Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Arena 3 Vs 3]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							int16 Rating = Fields[1].GetInt16();
							int16 Games = Fields[2].GetInt16();
							int16 Wins = Fields[3].GetInt16();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Rating|r: |CFF006400" << Rating << "|r |cff000000Win|r: |CFF006400" << Wins << "|r |cff000000Lose|r: |CFF006400" << Games - Wins << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 4:
                {
                    player->PlayerTalkClass->ClearMenus();
                    player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\inv_sword_27:25|t|r Warrior", GOSSIP_SENDER_MAIN, 10);
                    player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\ability_thunderbolt:25|t|r Paladin", GOSSIP_SENDER_MAIN, 20);
                    player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\inv_weapon_bow_07:25|t|r Hunter", GOSSIP_SENDER_MAIN, 30);
                    player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\inv_throwingknife_04:25|t|r Rogue", GOSSIP_SENDER_MAIN, 40);
                    player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\inv_staff_30:25|t|r Priest", GOSSIP_SENDER_MAIN, 50);
                    player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\spell_deathknight_classicon:25|t|r Death Knight", GOSSIP_SENDER_MAIN, 60);
                    player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\inv_jewelry_talisman_04:25|t|r Shaman", GOSSIP_SENDER_MAIN, 70);
                    player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\inv_staff_13:25|t|r Mage", GOSSIP_SENDER_MAIN, 80);
                    player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\spell_nature_drowsy:25|t|r Warlock", GOSSIP_SENDER_MAIN, 90);
                    player->ADD_GOSSIP_ITEM(10, "|cff00ff00|TInterface\\icons\\inv_misc_monsterclaw_04:25|t|r Druid", GOSSIP_SENDER_MAIN, 110);
                    player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                }
                break;
                case 5:
                {
                    QueryResult Result = CharacterDatabase.Query("SELECT * FROM duel_system ORDER BY rating DESC LIMIT 10");
                    if (Result)
                    {
                        Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Dueler]|r", GOSSIP_SENDER_MAIN, 0);
                        do
                        {
                            uint32 Guid = Fields[0].GetUInt32();
                            int16 Win = Fields[1].GetInt16();
                            int16 Lose = Fields[2].GetInt16();
                            int16 Rating = Fields[3].GetInt16();
                            std::string Name = "Unknown";

                            QueryResult Result1 = CharacterDatabase.PQuery("SELECT name FROM characters WHERE guid = %u", Guid);
                            if (Result1)
                            {
                                Field* Fields1 = Result1->Fetch();
                                Name = Fields1[0].GetString();
                            }
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Rating|r: |CFF006400" << Rating << "|r |cff000000Win|r: |CFF006400" << Win << "|r |cff000000Lose|r: |CFF006400" << Lose << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
                        }
                        while(Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 6:
                {
                    QueryResult Result = CharacterDatabase.Query("SELECT name,totaltime FROM characters WHERE name != '' ORDER BY totaltime DESC LIMIT 10");
                    if (Result)
                    {
                        Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Play Time]|r", GOSSIP_SENDER_MAIN, 0);
                        do
                        {
                            std::string Name = Fields[0].GetString();
                            uint32 Time = Fields[1].GetUInt32();
                            // Played Time Math
                            uint32 Day = floor(Time / 86400);
                            Time = Time - (Day * 86400);
                            uint32 Hour = floor(Time / 3600);
                            Time = Time - (Hour * 3600);
                            uint32 Min = floor(Time / 60);
                            // Played Time Math
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Days|r: |CFF006400" << Day << "|r |cff000000Hours|r: |CFF006400" << Hour << "|r |cff000000Minutes|r: |CFF006400" << Min << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
                        }
                        while(Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 7:
                {
                    QueryResult Result = CharacterDatabase.Query("SELECT name,todayKills FROM characters WHERE name != '' ORDER BY todayKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Killer Today]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 8:
                {
                    QueryResult Result = CharacterDatabase.Query("SELECT name,yesterdayKills FROM characters WHERE name!= '' ORDER BY yesterdayKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Killer Yesterday]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 10:
                {
                    QueryResult Result = CharacterDatabase.PQuery("SELECT name,totalKills FROM characters WHERE class = 1 AND name != '' ORDER BY totalKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Warrior]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 20:
                {
                    QueryResult Result = CharacterDatabase.PQuery("SELECT name,totalKills FROM characters WHERE class = 2 AND name != '' ORDER BY totalKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Paladin]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 30:
                {
                    QueryResult Result = CharacterDatabase.PQuery("SELECT name,totalKills FROM characters WHERE class = 3 AND name != '' ORDER BY totalKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Hunter]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 40:
                {
                    QueryResult Result = CharacterDatabase.PQuery("SELECT name,totalKills FROM characters WHERE class = 4 AND name != '' ORDER BY totalKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Rogue]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 50:
                {
                    QueryResult Result = CharacterDatabase.PQuery("SELECT name,totalKills FROM characters WHERE class = 5 AND name != '' ORDER BY totalKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Priest]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 60:
                {
                    QueryResult Result = CharacterDatabase.PQuery("SELECT name,totalKills FROM characters WHERE class = 6 AND name != '' ORDER BY totalKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Death Knight]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 70:
                {
                    QueryResult Result = CharacterDatabase.PQuery("SELECT name,totalKills FROM characters WHERE class = 7 AND name != '' ORDER BY totalKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Shaman]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 80:
                {
                    QueryResult Result = CharacterDatabase.PQuery("SELECT name,totalKills FROM characters WHERE class = 8 AND name != '' ORDER BY totalKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Mage]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 90:
                {
                    QueryResult Result = CharacterDatabase.PQuery("SELECT name,totalKills FROM characters WHERE class = 9 AND name != '' ORDER BY totalKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Warlock]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
                case 110:
                {
                    QueryResult Result = CharacterDatabase.PQuery("SELECT name,totalKills FROM characters WHERE class = 11 AND name != '' ORDER BY totalKills DESC LIMIT 10");
                    if (Result)
                    {
						Field* Fields = Result->Fetch();
						player->ADD_GOSSIP_ITEM(10, "|cff000000[Top Druid]|r", GOSSIP_SENDER_MAIN, 0);
						do
						{
							std::string Name = Fields[0].GetString();
							uint32 Kills = Fields[1].GetUInt32();
							std::stringstream buffer;
							buffer << "|cff000000Name|r: |CFF006400" << Name.c_str() << "|r |cff000000Kills|r: |CFF640000" << Kills << "|r";
							player->ADD_GOSSIP_ITEM(10, buffer.str(), GOSSIP_SENDER_MAIN, 0);
						} while (Result->NextRow());
						player->PlayerTalkClass->SendGossipMenu(68, creature->GetGUID());
                    }
					else
					{
						ChatHandler(player->GetSession()).PSendSysMessage("There is no Item to display.");
						player->PlayerTalkClass->SendCloseGossip();
					}
                }
                break;
            }

            return true;
        }
};

void AddSC_npc_top_players()
{
   new npc_top_players();
}