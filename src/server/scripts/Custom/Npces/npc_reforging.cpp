#include "ScriptPCH.h"
#include "../System/system_warlords.h"

#define Warlords  ACE_Singleton<Warlords, ACE_Null_Mutex>::instance()

static const uint8 stat_type_max = sizeof(statTypes) / sizeof(*statTypes);

void RemoveReforge(Player* player, uint32 itemguid, bool update);

void RemoveReforge(Player* player, uint32 itemguid, bool update)
{
	uint32 lowguid = player->GetGUIDLow();
	if (!itemguid || player->reforgeMap.empty() ||
		player->reforgeMap.find(itemguid) == player->reforgeMap.end())
		return;

	Item* invItem = update ? player->GetItemByGuid(itemguid) : NULL;
	if (!invItem)
	{
		player->reforgeMap.erase(itemguid);
		return;
	}

	player->_ApplyItemMods(invItem, invItem->GetSlot(), false);
	player->reforgeMap.erase(itemguid);
	player->_ApplyItemMods(invItem, invItem->GetSlot(), true);
	Warlords->SendPacketItem(player, invItem->GetEntry());
}

class REFORGE_PLAYER : public PlayerScript
{
public:
	REFORGE_PLAYER() : PlayerScript("REFORGE_PLAYER")
	{
		CharacterDatabase.DirectExecute("DELETE FROM `custom_reforging` WHERE NOT EXISTS (SELECT 1 FROM `item_instance` WHERE `item_instance`.`guid` = `custom_reforging`.`GUID`)");
	}

	class SendRefPackLogin : public BasicEvent
	{
	public:
		SendRefPackLogin(Player* _player) : player(_player)
		{
			_player->m_Events.AddEvent(this, _player->m_Events.CalculateTime(1000));
		}

		bool Execute(uint64, uint32) override
		{
			Warlords->SendReforgePackets(player);
			return true;
		}
		Player* player;
	};

	void OnLogin(Player* player) override
	{
		uint32 playerGUID = player->GetGUIDLow();
		QueryResult result = CharacterDatabase.PQuery("SELECT `GUID`, `increase`, `decrease`, `stat_value` FROM `custom_reforging` WHERE `Owner` = %u", playerGUID);
		if (result)
		{
			do
			{
				uint32 lowGUID = (*result)[0].GetUInt32();
				Item* invItem = player->GetItemByGuid(lowGUID);
				if (invItem)
					player->_ApplyItemMods(invItem, invItem->GetSlot(), false);
				ReforgeData& data = player->reforgeMap[lowGUID];
				data.increase = (*result)[1].GetUInt32();
				data.decrease = (*result)[2].GetUInt32();
				data.stat_value = (*result)[3].GetInt32();
				if (invItem)
					player->_ApplyItemMods(invItem, invItem->GetSlot(), true);
				// SendReforgePacket(player, entry, lowGUID);
			} while (result->NextRow());

			// SendReforgePackets(player);
			new SendRefPackLogin(player);
		}
	}

	//void OnLogout(Player* player) override
	//{
	//    if (player->reforgeMap.empty())
	//        return;
	//    for (ReforgeMapType::const_iterator it = player->reforgeMap.begin(); it != player->reforgeMap.end();)
	//    {
	//        ReforgeMapType::const_iterator old_it = it++;
	//        RemoveReforge(player, old_it->first, false);
	//    }
	//}

	void OnSave(Player* player) override
	{
		uint32 lowguid = player->GetGUIDLow();
		SQLTransaction trans = CharacterDatabase.BeginTransaction();
		trans->PAppend("DELETE FROM `custom_reforging` WHERE `Owner` = %u", lowguid);
		if (!player->reforgeMap.empty())
		{
			// Only save items that are in inventory / bank / etc
			std::vector<Item*> items = Warlords->GetItemList(player);
			for (std::vector<Item*>::const_iterator it = items.begin(); it != items.end(); ++it)
			{
				ReforgeMapType::const_iterator it2 = player->reforgeMap.find((*it)->GetGUIDLow());
				if (it2 == player->reforgeMap.end())
					continue;

				const ReforgeData& data = it2->second;
				trans->PAppend("REPLACE INTO `custom_reforging` (`GUID`, `increase`, `decrease`, `stat_value`, `Owner`) VALUES (%u, %u, %u, %i, %u)", it2->first, data.increase, data.decrease, data.stat_value, lowguid);
			}
		}

		if (trans->GetSize()) // basically never false
			CharacterDatabase.CommitTransaction(trans);
	}
};

class REFORGER_NPC : public CreatureScript
{
public:
	REFORGER_NPC() : CreatureScript("REFORGER_NPC") { }

	bool OnGossipHello(Player* player, Creature* creature) override
	{
		for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
		{
			if (Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
			if (Warlords->IsReforgable(invItem, player))
			if (const char* slotname = Warlords->GetSlotName(slot, player->GetSession()))
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, Warlords->GetSlotIcon(slot, 30, 30, -18, 0) + slotname, 0, Warlords->Melt(SELECT_STAT_REDUCE, slot));
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|TInterface/ICONS/INV_Enchant_Disenchant:30:30:-18:0|tRemove reforges", 0, Warlords->Melt(SELECT_RESTORE, 0));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:30:30:-18:0|tUpdate menu", 0, Warlords->Melt(MAIN_MENU, 0));
		player->SEND_GOSSIP_MENU("|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|tSelect slot of the item to reforge:", creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 melt) override
	{
		player->PlayerTalkClass->ClearMenus();

		uint8 menu, action;
		Warlords->Unmelt(melt, menu, action);

		switch (menu)
		{
		case MAIN_MENU: OnGossipHello(player, creature); break;
		case SELECT_STAT_REDUCE:
			// action = slot
			if (Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, action))
			{
				if (Warlords->IsReforgable(invItem, player))
				{
					uint32 guidlow = invItem->GetGUIDLow();
					const ItemTemplate* pProto = invItem->GetTemplate();
					for (uint32 i = 0; i < pProto->StatsCount; ++i)
					{
						int32 stat_diff = ((int32)floorf((float)pProto->ItemStat[i].ItemStatValue * 0.4f));
						if (stat_diff > 1)
						if (const char* stat_name = Warlords->GetStatName(pProto->ItemStat[i].ItemStatType))
						{
							player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:30:30:-18:0|tContinune", guidlow, Warlords->Melt(SELECT_STAT_INCREASE, i));
							break;
						}
					}
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				}
				else
				{
					player->GetSession()->SendNotification("Invalid item selected");
					OnGossipHello(player, creature);
				}
			}
			else
			{
				player->GetSession()->SendNotification("Invalid item selected");
				OnGossipHello(player, creature);
			}
			break;
		case SELECT_STAT_INCREASE:
			// sender = item guidlow
			// action = StatsCount id
		{
									 Item* invItem = Warlords->GetEquippedItem(player, sender);
									 if (invItem)
									 {
										 const ItemTemplate* pProto = invItem->GetTemplate();
										 int32 stat_diff = ((int32)floorf((float)pProto->ItemStat[action].ItemStatValue * 0.4f));

										 for (uint8 i = 0; i < stat_type_max; ++i)
										 {
											 bool cont = false;
											 for (uint32 j = 0; j < pProto->StatsCount; ++j)
											 {
												 if (statTypes[i] == pProto->ItemStat[j].ItemStatType) // skip existing stats on item
												 {
													 cont = true;
													 break;
												 }
											 }
											 if (cont)
												 continue;
											 if (const char* stat_name = Warlords->GetStatName(statTypes[i]))
											 {
												 std::ostringstream oss;
												 oss << stat_name << " |cFF3ECB3C+" << stat_diff << "|r";
												 player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, oss.str(), sender, Warlords->Melt(i, (uint8)pProto->ItemStat[action].ItemStatType), "Are you sure you want to reforge\n\n" + pProto->Name1, (pProto->SellPrice < (10 * GOLD) ? (10 * GOLD) : pProto->SellPrice), false);
											 }
										 }
										 player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", 0, Warlords->Melt(MAIN_MENU, invItem->GetSlot()));
										 player->SEND_GOSSIP_MENU("|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|tStat to increase:", creature->GetGUID());
									 }
									 else
									 {
										 player->GetSession()->SendNotification("Invalid item selected");
										 OnGossipHello(player, creature);
									 }
		}
			break;
		case SELECT_RESTORE:
		{
							   if (!player->reforgeMap.empty())
							   {
								   for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
								   {
									   if (Item* invItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
									   if (player->reforgeMap.find(invItem->GetGUIDLow()) != player->reforgeMap.end())
									   if (const char* slotname = Warlords->GetSlotName(slot, player->GetSession()))
										   player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_INTERACT_1, Warlords->GetSlotIcon(slot, 30, 30, -18, 0) + slotname, invItem->GetGUIDLow(), Warlords->Melt(RESTORE, 0), "Remove reforge from\n\n" + invItem->GetTemplate()->Name1, 0, false);
								   }
							   }
							   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:30:30:-18:0|tUpdate menu", sender, melt);
							   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "|TInterface/Glues/Common/Glue-LeftArrow-Button-Up:30:30:-18:0|tBack..", 0, Warlords->Melt(MAIN_MENU, 0));
							   player->SEND_GOSSIP_MENU("|TInterface/ICONS/INV_Misc_Book_11:30:30:-18:0|tSelect slot to remove reforge from:", creature->GetGUID());
		}
			break;
		case RESTORE:
			// sender = item guidlow
		{
						if (Item* invItem = player->GetItemByGuid(sender))
						{
							if (!player->reforgeMap.empty() && player->reforgeMap.find(sender) != player->reforgeMap.end())
								RemoveReforge(player, sender, true);
						}
						OnGossipHello(player, creature);
						RemoveReforge(player, sender, true);
		}
			break;
		default: // Reforge
			// sender = item guidlow
			// menu = stat type to increase index to statTypes[]
			// action = stat type to decrease
		{
					 if (menu < stat_type_max)
					 {
						 Item* invItem = Warlords->GetEquippedItem(player, sender);
						 if (invItem && Warlords->IsReforgable(invItem, player))
						 {
							 if (player->HasEnoughMoney(invItem->GetTemplate()->SellPrice < (10 * GOLD) ? (10 * GOLD) : invItem->GetTemplate()->SellPrice))
							 {
								 // int32 stat_diff = ((int32)floorf((float)invItem->GetTemplate()->ItemStat[action].ItemStatValue * 0.4f));
								 Warlords->UpdatePlayerReforgeStats(invItem, player, action, statTypes[menu]); // rewrite this function
							 }
							 else
								 player->GetSession()->SendNotification("Not enough money");
						 }
						 else
							 player->GetSession()->SendNotification("Invalid item selected");
					 }
					 OnGossipHello(player, creature);
		}
		}
		return true;
	}

	enum Menus
	{
		MAIN_MENU = 200, // stat_type_max
		SELECT_ITEM,
		SELECT_STAT_REDUCE,
		SELECT_STAT_INCREASE,
		SELECT_RESTORE,
		RESTORE,
		REFORGE,
	};
};

void AddSC_REFORGER_NPC()
{
	new REFORGER_NPC;
	new REFORGE_PLAYER;
}

#undef FOR_REFORGE_ITEMS