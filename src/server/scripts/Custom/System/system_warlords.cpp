#include "ScriptPCH.h"
#include "Config.h"
#include "Language.h"
#include "system_warlords.h"
#include "system_megapoint.h"
#include "system_votepoint.h"

#define MegaPoint  ACE_Singleton<MegaPoint, ACE_Null_Mutex>::instance()
#define VotePoint  ACE_Singleton<VotePoint, ACE_Null_Mutex>::instance()

std::string Warlords::GetItemIcon(uint32 entry, uint32 width, uint32 height, int x, int y)
{
	std::ostringstream ss;
	ss << "|TInterface";
	const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
	const ItemDisplayInfoEntry* dispInfo = NULL;
	if (temp)
	{
		dispInfo = sItemDisplayInfoStore.LookupEntry(temp->DisplayInfoID);
		if (dispInfo)
			ss << "/ICONS/" << dispInfo->inventoryIcon;
	}
	if (!temp && !dispInfo)
		ss << "/InventoryItems/WoWUnknownItem01";
	ss << ":" << width << ":" << height << ":" << x << ":" << y << "|t";
	return ss.str();
}

std::string Warlords::GetItemLink(Item* item, WorldSession* session)
{
	int loc_idx = session->GetSessionDbLocaleIndex();
	const ItemTemplate* temp = item->GetTemplate();
	std::string name = temp->Name1;
	if (ItemLocale const* il = sObjectMgr->GetItemLocale(temp->ItemId))
		ObjectMgr::GetLocaleString(il->Name, loc_idx, name);

	if (int32 itemRandPropId = item->GetItemRandomPropertyId())
	{
		char* const* suffix = NULL;
		if (itemRandPropId < 0)
		{
			const ItemRandomSuffixEntry* itemRandEntry = sItemRandomSuffixStore.LookupEntry(-item->GetItemRandomPropertyId());
			if (itemRandEntry)
				suffix = itemRandEntry->nameSuffix;
		}
		else
		{
			const ItemRandomPropertiesEntry* itemRandEntry = sItemRandomPropertiesStore.LookupEntry(item->GetItemRandomPropertyId());
			if (itemRandEntry)
				suffix = itemRandEntry->nameSuffix;
		}
		if (suffix)
		{
			std::string test(suffix[(name != temp->Name1) ? loc_idx : DEFAULT_LOCALE]);
			if (!test.empty())
			{
				name += ' ';
				name += test;
			}
		}
	}

	std::ostringstream oss;
	oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
		"|Hitem:" << temp->ItemId << ":" <<
		item->GetEnchantmentId(PERM_ENCHANTMENT_SLOT) << ":" <<
		item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT) << ":" <<
		item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_2) << ":" <<
		item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_3) << ":" <<
		item->GetEnchantmentId(BONUS_ENCHANTMENT_SLOT) << ":" <<
		item->GetItemRandomPropertyId() << ":" << item->GetItemSuffixFactor() << ":" <<
		(uint32)item->GetOwner()->getLevel() << "|h[" << name << "]|h|r";

	return oss.str();
}

std::string Warlords::GetItemLink(uint32 entry, WorldSession* session)
{
	const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
	int loc_idx = session->GetSessionDbLocaleIndex();
	std::string name = temp->Name1;
	if (ItemLocale const* il = sObjectMgr->GetItemLocale(entry))
		ObjectMgr::GetLocaleString(il->Name, loc_idx, name);

	std::ostringstream oss;
	oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
		"|Hitem:" << entry << ":0:0:0:0:0:0:0:0:0|h[" << name << "]|h|r";

	return oss.str();
}

char* Warlords::GetStatName(uint32 ItemStatType)
{
	switch (ItemStatType)
	{
	case ITEM_MOD_INTELLECT: return "Intellect"; break;
	case ITEM_MOD_STRENGTH: return "Strength"; break;
	case ITEM_MOD_STAMINA: return "Stamina"; break;
	case ITEM_MOD_AGILITY: return "Agility"; break;
	case ITEM_MOD_SPIRIT: return "Spirit"; break;
	case ITEM_MOD_ATTACK_POWER: return "Attack Power"; break;
	case ITEM_MOD_SPELL_POWER: return "Spell Power"; break;
	case ITEM_MOD_DODGE_RATING: return "Dodge rating"; break;
	case ITEM_MOD_PARRY_RATING: return "Parry rating"; break;
	case ITEM_MOD_HIT_RATING: return "Hit rating"; break;
	case ITEM_MOD_CRIT_RATING: return "Crit rating"; break;
	case ITEM_MOD_HASTE_RATING: return "Haste rating"; break;
	case ITEM_MOD_EXPERTISE_RATING: return "Expertise rating"; break;
	case ITEM_MOD_RESILIENCE_RATING: return "Resilience"; break;
	default: return NULL;
	}
}

char* Warlords::GetSlotName(uint8 slot, WorldSession* session)
{
	switch (slot)
	{
	case EQUIPMENT_SLOT_HEAD: return "Head";
	case EQUIPMENT_SLOT_NECK: return "Neck";
	case EQUIPMENT_SLOT_SHOULDERS: return "Shoulders";
	case EQUIPMENT_SLOT_BODY: return "Shirt";
	case EQUIPMENT_SLOT_CHEST: return "Chest";
	case EQUIPMENT_SLOT_WAIST: return "Waist";
	case EQUIPMENT_SLOT_LEGS: return "Legs";
	case EQUIPMENT_SLOT_FEET: return "Feet";
	case EQUIPMENT_SLOT_WRISTS: return "Wrists";
	case EQUIPMENT_SLOT_HANDS: return "Hands";
	case EQUIPMENT_SLOT_FINGER1: return "Right finger";
	case EQUIPMENT_SLOT_FINGER2: return "Left finger";
	case EQUIPMENT_SLOT_TRINKET1: return "Right trinket";
	case EQUIPMENT_SLOT_TRINKET2: return "Left trinket";
	case EQUIPMENT_SLOT_BACK: return "Back";
	case EQUIPMENT_SLOT_MAINHAND: return "Main hand";
	case EQUIPMENT_SLOT_OFFHAND: return "Off hand";
	case EQUIPMENT_SLOT_TABARD: return "Tabard";
	case EQUIPMENT_SLOT_RANGED: return "Ranged";
	default: return NULL;
	}
}

std::string Warlords::GetSlotIcon(uint8 slot, uint32 width, uint32 height, int x, int y)
{
	std::ostringstream ss;
	ss << "|TInterface/PaperDoll/";
	switch (slot)
	{
	case EQUIPMENT_SLOT_HEAD: ss << "UI-PaperDoll-Slot-Head"; break;
	case EQUIPMENT_SLOT_NECK: ss << "UI-PaperDoll-Slot-Neck"; break;
	case EQUIPMENT_SLOT_SHOULDERS: ss << "UI-PaperDoll-Slot-Shoulder"; break;
	case EQUIPMENT_SLOT_BODY: ss << "UI-PaperDoll-Slot-Shirt"; break;
	case EQUIPMENT_SLOT_CHEST: ss << "UI-PaperDoll-Slot-Chest"; break;
	case EQUIPMENT_SLOT_WAIST: ss << "UI-PaperDoll-Slot-Waist"; break;
	case EQUIPMENT_SLOT_LEGS: ss << "UI-PaperDoll-Slot-Legs"; break;
	case EQUIPMENT_SLOT_FEET: ss << "UI-PaperDoll-Slot-Feet"; break;
	case EQUIPMENT_SLOT_WRISTS: ss << "UI-PaperDoll-Slot-Wrists"; break;
	case EQUIPMENT_SLOT_HANDS: ss << "UI-PaperDoll-Slot-Hands"; break;
	case EQUIPMENT_SLOT_FINGER1: ss << "UI-PaperDoll-Slot-RFinger"; break;
	case EQUIPMENT_SLOT_FINGER2: ss << "UI-PaperDoll-Slot-RFinger"; break;
	case EQUIPMENT_SLOT_TRINKET1: ss << "UI-PaperDoll-Slot-REar"; break;
	case EQUIPMENT_SLOT_TRINKET2: ss << "UI-PaperDoll-Slot-REar"; break;
	case EQUIPMENT_SLOT_BACK: ss << "UI-PaperDoll-Slot-Chest"; break;
	case EQUIPMENT_SLOT_MAINHAND: ss << "UI-PaperDoll-Slot-MainHand"; break;
	case EQUIPMENT_SLOT_OFFHAND: ss << "UI-PaperDoll-Slot-SecondaryHand"; break;
	case EQUIPMENT_SLOT_RANGED: ss << "UI-PaperDoll-Slot-Ranged"; break;
	case EQUIPMENT_SLOT_TABARD: ss << "UI-PaperDoll-Slot-Tabard"; break;
	default: ss << "UI-Backpack-EmptySlot";
	}
	ss << ":" << width << ":" << height << ":" << x << ":" << y << "|t";
	return ss.str();
}

uint32 Warlords::Melt(uint8 i, uint8 j)
{
	return (i << 8) + j;
}

void Warlords::Unmelt(uint32 melt, uint8& i, uint8& j)
{
	i = melt >> 8;
	j = melt & 0xFF;
}

std::vector<Item*> Warlords::GetItemList(const Player* player)
{
	std::vector<Item*> itemlist;

	// Copy paste from Player::GetItemByGuid(guid)

	for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i)
	if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
		itemlist.push_back(pItem);

	for (uint8 i = KEYRING_SLOT_START; i < CURRENCYTOKEN_SLOT_END; ++i)
	if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
		itemlist.push_back(pItem);

	for (int i = BANK_SLOT_ITEM_START; i < BANK_SLOT_BAG_END; ++i)
	if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
		itemlist.push_back(pItem);

	for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
	if (Bag* pBag = player->GetBagByPos(i))
	for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
	if (Item* pItem = pBag->GetItemByPos(j))
		itemlist.push_back(pItem);

	for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
	if (Bag* pBag = player->GetBagByPos(i))
	for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
	if (Item* pItem = pBag->GetItemByPos(j))
		itemlist.push_back(pItem);

	return itemlist;
}

Item* Warlords::GetEquippedItem(Player* player, uint32 guidlow)
{
	for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i)
	if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	if (pItem->GetGUIDLow() == guidlow)
		return pItem;
	return NULL;
}

void Warlords::SendPacketItem(Player* player, uint32 entry, uint32 lowguid, const ReforgeData* reforge)
{
	ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(entry);
	if (!pProto)
		return;

	if (lowguid)
	{
		if (!player->reforgeMap.empty() && player->reforgeMap.find(lowguid) != player->reforgeMap.end())
			reforge = &player->reforgeMap[lowguid];
		else
			RemoveReforge(player, lowguid, true);
	}

	// Update player cache (self only) pure visual.
	// HandleItemQuerySingleOpcode copy paste
	std::string Name = pProto->Name1;
	std::string Description = pProto->Description;

	//Script For Cost Item
	//                                                       0
	QueryResult CostQuery = WorldDatabase.PQuery("SELECT `require_id` FROM `custom_item_costs` WHERE `entry` = '%u'", pProto->ItemId);
	if (CostQuery)
	{
		int32 require_id;

		Field* Fields = CostQuery->Fetch();

		require_id = Fields[0].GetInt32();
		Description = GetRequirement(player, require_id);
	}

	if (Description == "")
		Description += "Yekta-Core";
	else
		Description += "\"\n\"Yekta-Core";

	int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
	if (loc_idx >= 0)
	{
		if (ItemLocale const* il = sObjectMgr->GetItemLocale(pProto->ItemId))
		{
			ObjectMgr::GetLocaleString(il->Name, loc_idx, Name);
			ObjectMgr::GetLocaleString(il->Description, loc_idx, Description);
		}
	}
	WorldPacket data(SMSG_ITEM_QUERY_SINGLE_RESPONSE, 600);
	data << pProto->ItemId;
	data << pProto->Class;
	data << pProto->SubClass;
	data << pProto->SoundOverrideSubclass;
	data << Name;
	data << uint8(0x00);                                //pProto->Name2; // blizz not send name there, just uint8(0x00); <-- \0 = empty string = empty name...
	data << uint8(0x00);                                //pProto->Name3; // blizz not send name there, just uint8(0x00);
	data << uint8(0x00);                                //pProto->Name4; // blizz not send name there, just uint8(0x00);
	data << pProto->DisplayInfoID;
	data << pProto->Quality;
	data << pProto->Flags;
	data << pProto->Flags2;
	data << pProto->BuyPrice;
	data << pProto->SellPrice;
	data << pProto->InventoryType;
	data << pProto->AllowableClass;
	data << pProto->AllowableRace;
	data << pProto->ItemLevel;
	data << pProto->RequiredLevel;
	data << pProto->RequiredSkill;
	data << pProto->RequiredSkillRank;
	data << pProto->RequiredSpell;
	data << pProto->RequiredHonorRank;
	data << pProto->RequiredCityRank;
	data << pProto->RequiredReputationFaction;
	data << pProto->RequiredReputationRank;
	data << int32(pProto->MaxCount);
	data << int32(pProto->Stackable);
	data << pProto->ContainerSlots;
	data << pProto->StatsCount + (reforge ? 1 : 0); // increase stat count by 1
	bool decreased = false;
	for (uint32 i = 0; i < pProto->StatsCount; ++i)
	{
		data << pProto->ItemStat[i].ItemStatType;
		data << pProto->ItemStat[i].ItemStatValue;
	}
	if (reforge)
	{
		data << reforge->increase;      // set new stat type
		data << reforge->stat_value;    // and value
	}
	data << pProto->ScalingStatDistribution;            // scaling stats distribution
	data << pProto->ScalingStatValue;                   // some kind of flags used to determine stat values column
	for (int i = 0; i < MAX_ITEM_PROTO_DAMAGES; ++i)
	{
		data << pProto->Damage[i].DamageMin;
		data << pProto->Damage[i].DamageMax;
		data << pProto->Damage[i].DamageType;
	}

	// resistances (7)
	data << pProto->Armor;
	data << pProto->HolyRes;
	data << pProto->FireRes;
	data << pProto->NatureRes;
	data << pProto->FrostRes;
	data << pProto->ShadowRes;
	data << pProto->ArcaneRes;

	data << pProto->Delay;
	data << pProto->AmmoType;
	data << pProto->RangedModRange;

	for (int s = 0; s < MAX_ITEM_PROTO_SPELLS; ++s)
	{
		// send DBC data for cooldowns in same way as it used in Spell::SendSpellCooldown
		// use `item_template` or if not set then only use spell cooldowns
		SpellInfo const* spell = sSpellMgr->GetSpellInfo(pProto->Spells[s].SpellId);
		if (spell)
		{
			bool db_data = pProto->Spells[s].SpellCooldown >= 0 || pProto->Spells[s].SpellCategoryCooldown >= 0;

			data << pProto->Spells[s].SpellId;
			data << pProto->Spells[s].SpellTrigger;
			data << uint32(-abs(pProto->Spells[s].SpellCharges));

			if (db_data)
			{
				data << uint32(pProto->Spells[s].SpellCooldown);
				data << uint32(pProto->Spells[s].SpellCategory);
				data << uint32(pProto->Spells[s].SpellCategoryCooldown);
			}
			else
			{
				data << uint32(spell->RecoveryTime);
				data << uint32(spell->GetCategory());
				data << uint32(spell->CategoryRecoveryTime);
			}
		}
		else
		{
			data << uint32(0);
			data << uint32(0);
			data << uint32(0);
			data << uint32(-1);
			data << uint32(0);
			data << uint32(-1);
		}
	}
	data << pProto->Bonding;
	data << Description;
	data << pProto->PageText;
	data << pProto->LanguageID;
	data << pProto->PageMaterial;
	data << pProto->StartQuest;
	data << pProto->LockID;
	data << int32(pProto->Material);
	data << pProto->Sheath;
	data << pProto->RandomProperty;
	data << pProto->RandomSuffix;
	data << pProto->Block;
	data << pProto->ItemSet;
	data << pProto->MaxDurability;
	data << pProto->Area;
	data << pProto->Map;                                // Added in 1.12.x & 2.0.1 client branch
	data << pProto->BagFamily;
	data << pProto->TotemCategory;
	for (int s = 0; s < MAX_ITEM_PROTO_SOCKETS; ++s)
	{
		data << pProto->Socket[s].Color;
		data << pProto->Socket[s].Content;
	}
	data << pProto->socketBonus;
	data << pProto->GemProperties;
	data << pProto->RequiredDisenchantSkill;
	data << pProto->ArmorDamageModifier;
	data << pProto->Duration;                           // added in 2.4.2.8209, duration (seconds)
	data << pProto->ItemLimitCategory;                  // WotLK, ItemLimitCategory
	data << pProto->HolidayId;                          // Holiday.dbc?
	player->GetSession()->SendPacket(&data);
}


void Warlords::SendReforgePackets(Player* player)
{
	if (!send_cache_packets)
		return;

	std::vector<Item*> items = GetItemList(player);
	for (std::vector<Item*>::const_iterator it = items.begin(); it != items.end(); ++it)
		SendPacketItem(player, (*it)->GetEntry(), (*it)->GetGUIDLow());
}

void Warlords::RemoveReforge(Player* player, uint32 itemguid, bool update)
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
	SendPacketItem(player, invItem->GetEntry());
}

bool Warlords::IsReforgable(Item* invItem, Player* player)
{
	//if (!invItem->IsEquipped())
	//    return false;
	if (invItem->GetOwnerGUID() != player->GetGUID())
		return false;
	const ItemTemplate* pProto = invItem->GetTemplate();
	//if (pProto->ItemLevel < 200)
	//    return false;
	//if (pProto->Quality == ITEM_QUALITY_HEIRLOOM) // block heirlooms necessary?
	//    return false;
	if (!pProto->StatsCount || pProto->StatsCount >= MAX_ITEM_PROTO_STATS) // Mandatory! Do NOT remove or edit
		return false;
	if (!player->reforgeMap.empty() && player->reforgeMap.find(invItem->GetGUIDLow()) != player->reforgeMap.end()) // Mandatory! Do NOT remove or edit
		return false;
	for (uint32 i = 0; i < pProto->StatsCount; ++i)
	{
		if (!GetStatName(pProto->ItemStat[i].ItemStatType))
			continue;
		if (((int32)floorf((float)pProto->ItemStat[i].ItemStatValue * 0.4f)) > 1)
			return true;
	}
	return false;
}

std::string Warlords::GetRaceIcon(uint8 id, uint8 gender)
{
	if (gender = GENDER_MALE)
		switch (id)
	{
		case RACE_HUMAN:
			return "|TInterface/ICONS/Achievement_Character_Human_Male:15:15:0:-7|t Human ";
		case RACE_DWARF:
			return "|TInterface/ICONS/Achievement_Character_Dwarf_Male:15:15:0:-7|t Dwarf ";
		case RACE_GNOME:
			return "|TInterface/ICONS/Achievement_Character_Gnome_Male:15:15:0:-7|t Gnome ";
		case RACE_NIGHTELF:
			return "|TInterface/ICONS/Achievement_Character_Nightelf_Male:15:15:0:-7|t Night Elf ";
		case RACE_DRAENEI:
			return "|TInterface/ICONS/Achievement_Character_Draenei_Male:15:15:0:-7|t Draenei ";

		case RACE_ORC:
			return "|TInterface/ICONS/Achievement_Character_Orc_Male:15:15:0:-7|t Orc ";
		case RACE_TROLL:
			return "|TInterface/ICONS/Achievement_Character_Troll_Male:15:15:0:-7|t Troll ";
		case RACE_TAUREN:
			return "|TInterface/ICONS/Achievement_Character_Tauren_Male:15:15:0:-7|t Tauren ";
		case RACE_UNDEAD_PLAYER:
			return "|TInterface/ICONS/Achievement_Character_Undead_Male:15:15:0:-7|t Undead ";
		case RACE_BLOODELF:
			return "|TInterface/ICONS/Achievement_Character_Bloodelf_Male:15:15:0:-7|t Blood Elf ";
		default:
			return "|TInterface/ICONS/INV_Misc_QuestionMark:15:15:0:-7|t Unkown ";
			break;
	}
	else
		switch (id)
	{
		case RACE_HUMAN:
			return "|TInterface/ICONS/Achievement_Character_Human_Female:15:15:0:-7|t Human ";
		case RACE_DWARF:
			return "|TInterface/ICONS/Achievement_Character_Dwarf_Female:15:15:0:-7|t Dwarf ";
		case RACE_GNOME:
			return "|TInterface/ICONS/Achievement_Character_Gnome_Female:15:15:0:-7|t Gnome ";
		case RACE_NIGHTELF:
			return "|TInterface/ICONS/Achievement_Character_Nightelf_Female:15:15:0:-7|t Night Elf ";
		case RACE_DRAENEI:
			return "|TInterface/ICONS/Achievement_Character_Draenei_Female:15:15:0:-7|t Draenei ";

		case RACE_ORC:
			return "|TInterface/ICONS/Achievement_Character_Orc_Female:15:15:0:-7|t Orc ";
		case RACE_TROLL:
			return "|TInterface/ICONS/Achievement_Character_Troll_Female:15:15:0:-7|t Troll ";
		case RACE_TAUREN:
			return "|TInterface/ICONS/Achievement_Character_Tauren_Female:15:15:0:-7|t Tauren ";
		case RACE_UNDEAD_PLAYER:
			return "|TInterface/ICONS/Achievement_Character_Undead_Female:15:15:0:-7|t Undead ";
		case RACE_BLOODELF:
			return "|TInterface/ICONS/Achievement_Character_Bloodelf_Female:15:15:0:-7|t Blood Elf ";
		default:
			return "|TInterface/ICONS/INV_Misc_QuestionMark:15:15:0:-7|t Unkown ";
			break;
	}
}

std::string Warlords::GetClassIcon(uint8 id)
{
	switch (id)
	{
	case CLASS_WARRIOR:
		return "|TInterface/ICONS/inv_sword_27:15:15:0:-7|t Warrior ";
	case CLASS_PALADIN:
		return "|TInterface/ICONS/ability_thunderbolt:15:15:0:-7|t Paladin ";
	case CLASS_HUNTER:
		return "|TInterface/ICONS/inv_weapon_bow_07:15:15:0:-7|t Hunter ";
	case CLASS_ROGUE:
		return "|TInterface/ICONS/inv_throwingknife_04:15:15:0:-7|t Rogue ";
	case CLASS_PRIEST:
		return "|TInterface/ICONS/inv_staff_30:15:15:0:-7|t Priest ";
	case CLASS_DEATH_KNIGHT:
		return "|TInterface/ICONS/spell_deathknight_classicon:15:15:0:-7|t Death Knight ";
	case CLASS_SHAMAN:
		return "|TInterface/ICONS/inv_jewelry_talisman_04:15:15:0:-7|t Shaman ";
	case CLASS_MAGE:
		return "|TInterface/ICONS/inv_staff_13:15:15:0:-7|t Mage ";
	case CLASS_WARLOCK:
		return "|TInterface/ICONS/spell_nature_drowsy:15:15:0:-7|t Warlock ";
	case CLASS_DRUID:
		return "|TInterface/ICONS/inv_misc_monsterclaw_04:15:15:0:-7|t Druid ";
	default:
		return "|TInterface/ICONS/INV_Misc_QuestionMark:15:15:0:-7|t Unkown ";
		break;
	}
}

void Warlords::UpdatePlayerReforgeStats(Item* invItem, Player* player, uint32 decrease, uint32 increase) // stat types
{
	const ItemTemplate* pProto = invItem->GetTemplate();

	int32 stat_diff = 0;
	for (uint32 i = 0; i < pProto->StatsCount; ++i)
	{
		if (pProto->ItemStat[i].ItemStatType == increase)
			return; // Should not have the increased stat already
		if (pProto->ItemStat[i].ItemStatType == decrease)
			stat_diff = (int32)floorf((float)pProto->ItemStat[i].ItemStatValue * 0.4f);
	}
	if (stat_diff <= 0)
		return; // Should have some kind of diff

	// Update player stats
	player->_ApplyItemMods(invItem, invItem->GetSlot(), false);
	uint32 guidlow = invItem->GetGUIDLow();
	ReforgeData& data = player->reforgeMap[guidlow];
	data.increase = increase;
	data.decrease = 0;
	data.stat_value = stat_diff;
	player->_ApplyItemMods(invItem, invItem->GetSlot(), true);
	// CharacterDatabase.PExecute("REPLACE INTO `custom_reforging` (`GUID`, `increase`, `decrease`, `stat_value`) VALUES (%u, %u, %u, %i)", guidlow, increase, decrease, stat_diff);
	player->ModifyMoney(pProto->SellPrice < (10 * GOLD) ? (-10 * GOLD) : -(int32)pProto->SellPrice);
	SendPacketItem(player, invItem->GetEntry(), 0, &data);
	// player->SaveToDB();
}

uint32 Warlords::GetDuelRating(Player* player)
{
	QueryResult result = CharacterDatabase.PQuery("SELECT `rating` FROM `duel_system` WHERE guid = %u", player->GetGUIDLow());
	if (result)
	{
		Field* fields = result->Fetch();
		uint16 rating = fields[0].GetInt16();
		return rating;
	}
	return 0;
}

uint32 Warlords::GetArena2v2Rating(Player* player)
{
	QueryResult result = CharacterDatabase.PQuery("SELECT arena_team_member.personalRating FROM `arena_team_member` INNER JOIN `arena_team` ON arena_team.arenaTeamId = arena_team_member.arenaTeamId WHERE arena_team.type = 2 AND arena_team_member.guid = %u", player->GetGUIDLow());
	if (result)
	{
		Field* fields = result->Fetch();
		uint16 rating = fields[0].GetInt16();
		return rating;
	}
	return 0;
}

uint32 Warlords::GetArena3v3Rating(Player* player)
{
	QueryResult result = CharacterDatabase.PQuery("SELECT arena_team_member.personalRating FROM `arena_team_member` INNER JOIN `arena_team` ON arena_team.arenaTeamId = arena_team_member.arenaTeamId WHERE arena_team.type = 3 AND arena_team_member.guid = %u", player->GetGUIDLow());
	if (result)
	{
		Field* fields = result->Fetch();
		uint16 rating = fields[0].GetInt16();
		return rating;
	}
	return 0;
}

uint32 Warlords::GetArena5v5Rating(Player* player)
{
	QueryResult result = CharacterDatabase.PQuery("SELECT arena_team_member.personalRating FROM `arena_team_member` INNER JOIN `arena_team` ON arena_team.arenaTeamId = arena_team_member.arenaTeamId WHERE arena_team.type = 5 AND arena_team_member.guid = %u", player->GetGUIDLow());
	if (result)
	{
		Field* fields = result->Fetch();
		uint16 rating = fields[0].GetInt16();
		return rating;
	}
	return 0;
}

bool Warlords::CheckRequirement(Player* player, uint32 requirementid)
{
	const CustomRequirementsContainer* require = sObjectMgr->GetCustomRequirementsMap();
	CustomRequirementsContainer::const_iterator cr = require->find(requirementid);

	if (cr != require->end())
	{
		uint32 requiredlevel, megapoints, viplevel, honorpoints, arenapoints, votepoints, onlinepoints, raceMask,
			classMask, duelrating, arena2v2rating, arena3v3rating, arena5v5rating, needitem[9], countitem[9], rewardid,
			pduelrating, parena2v2rating, parena3v3rating, parena5v5rating, plevel, prace, pclass, pgender;

		bool req = true, r = false, c = false;

		requiredlevel = cr->second.requiredlevel;
		megapoints = cr->second.megapoints;
		viplevel = cr->second.viplevel;
		honorpoints = cr->second.honorpoints;
		arenapoints = cr->second.arenapoints;
		votepoints = cr->second.votepoints;
		onlinepoints = cr->second.onlinepoints;
		raceMask = cr->second.raceMask;
		classMask = cr->second.classMask;
		duelrating = cr->second.duelrating;
		arena2v2rating = cr->second.arena2v2rating;
		arena3v3rating = cr->second.arena3v3rating;
		arena5v5rating = cr->second.arena5v5rating;
		for (size_t i = 0; i < 9; i++)
			needitem[i] = cr->second.needitem[i];
		for (size_t i = 0; i < 9; i++)
			countitem[i] = cr->second.itemcount[i];
		rewardid = cr->second.rewardid;

		prace = player->getRace();
		pclass = player->getClass();
		pgender = player->getGender();
		pduelrating = GetDuelRating(player);
		parena2v2rating = GetArena2v2Rating(player);
		parena3v3rating = GetArena3v3Rating(player);
		parena5v5rating = GetArena5v5Rating(player);
		plevel = player->GetLevelFromDB(player->GetGUID());

		std::stringstream all;
		player->PlayerTalkClass->ClearMenus();
		if (requiredlevel)
		{
			std::string color;
			if (requiredlevel > plevel)
			{
				req = false;
				color = "|CFF640000";
			}
			else
				color = "|CFF006400";
			all << color << "|TInterface/ICONS/Achievement_Level_80:20:20:0:-5|tLevel: " << player->GetLevelFromDB(player->GetGUID()) << "/" << requiredlevel << "|r\n*****************\n";
		}

		if (megapoints)
		{
			std::string color;
			if (megapoints > MegaPoint->GetPlayerMps(player))
			{
				req = false;
				color = "|CFF640000";
			}
			else
				color = "|CFF006400";
			all << color << "|TInterface/ICONS/Ability_Paladin_BlessedHands:20:20:0:-5|tMega Points: " << MegaPoint->GetPlayerMps(player) << "/" << megapoints << "|r\n*****************\n";
		}

		if (viplevel)
		{
			std::string color;
			if (viplevel > player->GetSession()->GetVipLevel())
			{
				req = false;
				color = "|CFF640000";
			}
			else
				color = "|CFF006400";
			all << color << "|TInterface/ICONS/Achievement_Quests_Completed_08:20:20:0:-5|tVIP Level: " << viplevel << "|r\n*****************\n";
		}

		if (honorpoints)
		{
			std::string color;
			if (honorpoints > player->GetHonorPoints())
			{
				req = false;
				color = "|CFF640000";
			}
			else
				color = "|CFF006400";
			all << color << "|TInterface/ICONS/Achievement_PVP_A_06:20:20:0:-5|tHonor Points: " << player->GetHonorPoints() << "/" << honorpoints << "|r\n*****************\n";
		}

		if (arenapoints)
		{
			std::string color;
			if (arenapoints > player->GetArenaPoints())
			{
				req = false;
				color = "|CFF640000";
			}
			else
				color = "|CFF006400";
			all << color << "|TInterface/ICONS/Achievement_PVP_A_15:20:20:0:-5|tArena Points: " << player->GetArenaPoints() << "/" << arenapoints << "|r\n*****************\n";
		}

		if (votepoints)
		{
			std::string color;
			if (votepoints > VotePoint->GetPlayerVPs(player))
			{
				req = false;
				color = "|CFF640000";
			}
			else
				color = "|CFF006400";
			all << color << "|TInterface/ICONS/Achievement_BG_tophealer_AB:20:20:0:-5|tVote Points: " << VotePoint->GetPlayerVPs(player) << "/" << votepoints << "|r\n*****************\n";
		}

		if (onlinepoints)
		{
			std::string color;
			if (onlinepoints > player->GetOps())
			{
				req = false;
				color = "|CFF640000";
			}
			else
				color = "|CFF006400";
			all << color << "|TInterface/ICONS/Spell_Nature_TimeStop:20:20:0:-5|tOnline Points: " << player->GetOps() << "/" << onlinepoints << "|r\n*****************\n";

		}

		if (raceMask)
		{
			std::string color;
			all << "|TInterface/ICONS/Temp:20:20:0:-5|tRaces:\n";
			for (size_t i = 0; i < 12; i++)
			if ((1 << (i - 1)) & raceMask)
			{
				if (i == prace)
				{
					color = "|CFF006400";
					r = true;
				}
				else
					color = "|CFF640000";

				std::string icon = GetRaceIcon(i, 1);
				all << color << icon << "|r\n";

			}
			all << "\n*****************\n";
			if (!r)
				req = false;
		}

		if (classMask)
		{
			std::string color;
			all << "|TInterface/ICONS/Temp:20:20:0:-5|tClasses:\n";
			for (size_t i = 0; i < 11; i++)
			if ((1 << (i - 1)) & classMask)
			{
				if (i == pclass)
				{
					color = "|CFF006400";
					c = true;
				}
				else
					color = "|CFF640000";

				std::string icon = GetClassIcon(i);
				all << color << icon << "|r\n";
			}
			all << "\n*****************\n";
			if (!c)
				req = false;
		}

		if (duelrating)
		{
			std::string color;
			if (duelrating > pduelrating)
			{
				req = false;
				color = "|CFF640000";
			}
			else
				color = "|CFF006400";
			all << color << "|TInterface/ICONS/Achievement_Arena_2v2_1:20:20:0:-5|tDuel Rating: " << pduelrating << "/" << duelrating << "|r\n*****************\n";
		}

		if (arena2v2rating)
		{
			std::string color;
			if (arena2v2rating > parena2v2rating)
			{
				req = false;
				color = "|CFF640000";
			}
			else
				color = "|CFF006400";
			all << color << "|TInterface/ICONS/Achievement_Arena_2v2_7:20:20:0:-5|tArena2v2 Rating: " << parena2v2rating << "/" << arena2v2rating << "|r\n*****************\n";
		}

		if (arena3v3rating)
		{
			std::string color;
			if (arena2v2rating > parena3v3rating)
			{
				req = false;
				color = "|CFF640000";
			}
			else
				color = "|CFF006400";
			all << color << "|TInterface/ICONS/Achievement_Arena_3v3_7:20:20:0:-5|tArena3v3 Rating: " << parena3v3rating << "/" << arena3v3rating << "|r\n*****************\n";
		}

		if (arena5v5rating)
		{
			std::string color;
			if (arena5v5rating > parena5v5rating)
			{
				req = false;
				color = "|CFF640000";
			}
			else
				color = "|CFF006400";
			all << color << "|TInterface/ICONS/Achievement_Arena_5v5_7:20:20:0:-5|tArena5v5 Rating: " << parena5v5rating << "/" << arena5v5rating << "|r\n*****************\n";
		}

		for (size_t i = 0; i < 9; i++)
		{
			uint32
				itemid = needitem[i],
				count = countitem[i];
			std::string color, itemicon, itemlink;
			if (itemid)
			{
				if (count > player->GetItemCount(itemid))
				{
					req = false;
					color = "|CFF640000";
				}
				else
					color = "|CFF006400";
				itemicon = GetItemIcon(itemid, 20, 20, 0, -5);
				itemlink = GetItemLink(itemid, player->GetSession());
				all << color << itemicon << itemlink << " " << player->GetItemCount(itemid) << "/" << count << "|r\n*****************\n";
			}
		}

		player->SEND_GOSSIP_MENU("|TInterface/ICONS/INV_Misc_Book_11:20:20:0:-5|t ***** Requirements *****|TInterface/ICONS/INV_Misc_Book_11:20:20:0:-5|t\n\n |CFF0000FFNotice : |rRequired items are colored with |CFF640000Dark Red|r.\n\n" + all.str(), player->GetGUID());
		if (!req)
		{
			return false;
		}
		else if (req)
		{
			if (megapoints)
				MegaPoint->RemovePlayerMPs(player, megapoints);
			if (honorpoints)
			{
				int32 val = honorpoints;
				player->ModifyHonorPoints(-val);
			}

			if (arenapoints)
			{
				int32 val = arenapoints;
				player->ModifyArenaPoints(-val);
			}
			if (votepoints)
				VotePoint->RemovePlayerVPs(player, votepoints);

			for (size_t i = 0; i < 9; i++)
			{
				uint32
					itemid = needitem[i],
					count = countitem[i];
				if (itemid)
				{
					int32 val = count;
					player->DestroyItemCount(itemid, count, true);
				}
			}

			if (req)
				SendReward(player, rewardid);

			player->CLOSE_GOSSIP_MENU();
			return true;
		}
		return req;
	}
	return true;
}

std::string Warlords::GetRequirement(Player* player, uint32 requirementid)
{
	const CustomRequirementsContainer* require = sObjectMgr->GetCustomRequirementsMap();
	CustomRequirementsContainer::const_iterator cr = require->find(requirementid);
	if (cr != require->end())
	{
		uint32 requiredlevel, megapoints, viplevel, honorpoints, arenapoints, votepoints, onlinepoints, raceMask,
			classMask, duelrating, arena2v2rating, arena3v3rating, arena5v5rating, needitem[9], countitem[9], rewardid;

		requiredlevel = cr->second.requiredlevel;
		megapoints = cr->second.megapoints;
		viplevel = cr->second.viplevel;
		honorpoints = cr->second.honorpoints;
		arenapoints = cr->second.arenapoints;
		votepoints = cr->second.votepoints;
		onlinepoints = cr->second.onlinepoints;
		raceMask = cr->second.raceMask;
		classMask = cr->second.classMask;
		duelrating = cr->second.duelrating;
		arena2v2rating = cr->second.arena2v2rating;
		arena3v3rating = cr->second.arena3v3rating;
		arena5v5rating = cr->second.arena5v5rating;
		for (size_t i = 0; i < 9; i++)
		{
			needitem[i] = cr->second.needitem[i];
		}
		for (size_t i = 0; i < 9; i++)
		{
			countitem[i] = cr->second.itemcount[i];
		}
		rewardid = cr->second.rewardid;

		std::stringstream buffer;
		buffer << "|cffff0000|TInterface/ICONS/INV_Misc_Book_11:30|tRequirements|r:\n";
		if (requiredlevel)
			buffer << "|TInterface/ICONS/Achievement_Level_80:20:20:0:-5|tLevel |cff00ff00x" << requiredlevel << "|r\n";

		if (megapoints)
			buffer << "|TInterface/ICONS/Ability_Paladin_BlessedHands:20:20:0:-5|tMega Points |cff00ff00x" << megapoints << "|r\n";

		if (viplevel)
			buffer << "|TInterface/ICONS/Achievement_Quests_Completed_08:20:20:0:-5|tVIP Level |cff00ff00x" << viplevel << " |r\n";

		if (honorpoints)
			buffer << "|TInterface/ICONS/Achievement_PVP_A_06:20:20:0:-5|tHonor Points |cff00ff00x" << honorpoints << "|r\n";

		if (arenapoints)
			buffer << "|TInterface/ICONS/Achievement_PVP_A_15:20:20:0:-5|tArena Points |cff00ff00x" << arenapoints << "|r\n";

		if (votepoints)
			buffer << "|TInterface/ICONS/Achievement_BG_tophealer_AB:20:20:0:-5|tVote Points |cff00ff00x" << votepoints << "|r\n";

		if (onlinepoints)
			buffer << "|TInterface/ICONS/Spell_Nature_TimeStop:20:20:0:-5|tOnline Points |cff00ff00x" << onlinepoints << "|r\n";

		if (raceMask)
		{
			buffer << "|TInterface/ICONS/Temp:20:20:0:-5|tRequired Races:\n";
			for (size_t i = 0; i < 12; i++)
			if ((1 << (i - 1)) & raceMask)
			{
				std::string icon = "|cff00ff00" + GetRaceIcon(i, 0);
				buffer << icon << "|r\n";
			}
			buffer << "\n";
		}

		if (classMask)
		{
			buffer << "|TInterface/ICONS/Temp:20:20:0:-5|tRequired Classes: \n";
			for (size_t i = 0; i < 11; i++)
			if ((1 << (i - 1)) & classMask)
			{
				std::string icon = "|cff00ff00" + GetClassIcon(i);
				buffer << icon << "|r\n";
			}
			buffer << "\n";
		}

		if (duelrating)
			buffer << "|TInterface/ICONS/Achievement_Arena_2v2_1:20:20:0:-5|tDuel Rating |cff00ff00x" << duelrating << "|r\n";

		if (arena2v2rating)
			buffer << "|TInterface/ICONS/Achievement_Arena_2v2_7:20:20:0:-5|tArena2v2 Rating |cff00ff00x" << arena2v2rating << "|r\n";

		if (arena3v3rating)
			buffer << "|TInterface/ICONS/Achievement_Arena_3v3_7:20:20:0:-5|tArena3v3 Rating |cff00ff00x" << arena3v3rating << "|r\n";

		if (arena5v5rating)
			buffer << "|TInterface/ICONS/Achievement_Arena_5v5_7:20:20:0:-5|tArena5v5 Rating |cff00ff00x" << arena5v5rating << "|r\n";

		for (size_t i = 0; i < 9; i++)
		{
			uint32
				itemid = needitem[i],
				count = countitem[i];
			std::string itemicon, itemlink;
			if (itemid)
			{
				itemicon = GetItemIcon(itemid, 20, 20, 0, -5);
				itemlink = GetItemLink(itemid, player->GetSession());
				buffer << itemicon << itemlink << "|cff00ff00x" << count << "|r\n";
			}
		}

		if (rewardid)
			buffer << GetReward(player, rewardid);
		return buffer.str();
	}
	return "|CFF006400Doesn't have any Requirement(s).";
};

void Warlords::SendReward(Player* player, uint32 rewardid)
{
	const CustomRewardContainer* require = sObjectMgr->GetCustomRewardMap();
	CustomRewardContainer::const_iterator cr = require->find(rewardid);
	if (cr != require->end())
	{
		uint32 rlevel, rmegapoints, rhonorpoints, rarenapoints, rvotepoints, ronlinepoints, rneeditem[9], rcountitem[9];

		rlevel = cr->second.level;
		rmegapoints = cr->second.megapoints;
		rhonorpoints = cr->second.honorpoints;
		rarenapoints = cr->second.arenapoints;
		rvotepoints = cr->second.votepoints;
		ronlinepoints = cr->second.onlinepoints;
		for (size_t i = 0; i < 9; i++)
			rneeditem[i] = cr->second.needitem[i];
		for (size_t i = 0; i < 9; i++)
			rcountitem[i] = cr->second.itemcount[i];

		if (rlevel)
			player->SetLevel(rlevel);

		if (rmegapoints)
			MegaPoint->AddPlayerMPs(player, rmegapoints);

		if (rhonorpoints)
		{
			int32 val = rhonorpoints;
			player->ModifyHonorPoints(val);
		}

		if (rarenapoints)
		{
			int32 val = rarenapoints;
			player->ModifyArenaPoints(val);
		}
		if (rvotepoints)
			VotePoint->AddPlayerVPs(player, rvotepoints);

		if (ronlinepoints)
			player->AddOps(ronlinepoints);

		for (size_t i = 0; i < 9; i++)
		{
			uint32
				ritemid = rneeditem[i],
				rcount = rcountitem[i];
			if (ritemid)
				player->AddItem(ritemid, rcount);
		}
	}
};

void Warlords::LearnSpells(Player* player, uint32 class_id)
{
	int32 spell_id, required_level;
	QueryResult SpellQuery = WorldDatabase.PQuery("SELECT `spell`, `requiredlevel` FROM `custom_trainer` WHERE `class` = '%u' OR `class` = 0", class_id);
	do {
		Field *SpellFields = SpellQuery->Fetch();

		spell_id = SpellFields[0].GetInt32();
		required_level = SpellFields[1].GetInt32();
		if (!player->HasSpell(spell_id) && player->getLevel() >= required_level)
		{
			player->learnSpell(spell_id, false);
			ChatHandler(player->GetSession()).PSendSysMessage("|cff00FF00Spell Learned.|r");
		}
	} while (SpellQuery->NextRow());
};

void Warlords::LearnTalents(Player* player, uint32 spec_id)
{
	int32 spec, talent, rank;
	QueryResult TalentQuery = WorldDatabase.PQuery("SELECT `spec`, `talent`, `rank` FROM `custom_talent_templates` WHERE `spec` = '%u' ORDER BY `ordination` ASC", spec_id);
	player->resetTalents();
	do {
		Field *TalentFields = TalentQuery->Fetch();

		spec = TalentFields[0].GetInt32();
		talent = TalentFields[1].GetInt32();
		rank = TalentFields[2].GetInt32();
		rank += 1;
		for (size_t i = 0; i < rank; i++)
		{
			player->LearnTalent(talent, i);
		}

	} while (TalentQuery->NextRow());
};

void Warlords::AddGroupItems(Player* player, uint32 group_id)
{
	int32 itemid, amount;
	QueryResult GroupQuery = WorldDatabase.PQuery("SELECT `itemid`, `amount` FROM `custom_group_items` WHERE `groupid` = '%u' ORDER BY `id` ASC", group_id);

	do {
		Field *GroupFields = GroupQuery->Fetch();

		itemid = GroupFields[0].GetInt32();
		amount = GroupFields[1].GetInt32();
		
		player->StoreNewItemInBestSlots(itemid, amount);

	} while (GroupQuery->NextRow());
};

void Warlords::RemoveGroupItems(Player* player, uint32 group_id)
{
	int32 itemid, amount;
	QueryResult GroupQuery = WorldDatabase.PQuery("SELECT `itemid`, `amount` FROM `custom_group_items` WHERE `groupid` = '%u' ORDER BY `id` ASC", group_id);

	do {
		Field *GroupFields = GroupQuery->Fetch();

		itemid = GroupFields[0].GetInt32();
		amount = GroupFields[1].GetInt32();

		player->AddItem(itemid, -amount);

	} while (GroupQuery->NextRow());
};

std::string Warlords::GetReward(Player* player, uint32 rewardid)
{
	if (rewardid)
	{
		const CustomRewardContainer* require = sObjectMgr->GetCustomRewardMap();
		CustomRewardContainer::const_iterator cr = require->find(rewardid);
		if (cr != require->end())
		{
			;
			uint32 rlevel, rmegapoints, rhonorpoints, rarenapoints, rvotepoints, ronlinepoints, rneeditem[9], rcountitem[9];

			rlevel = cr->second.level;
			rmegapoints = cr->second.megapoints;
			rhonorpoints = cr->second.honorpoints;
			rarenapoints = cr->second.arenapoints;
			rvotepoints = cr->second.votepoints;
			ronlinepoints = cr->second.onlinepoints;
			for (size_t i = 0; i < 9; i++)
			{
				rneeditem[i] = cr->second.needitem[i];
			}
			for (size_t i = 0; i < 9; i++)
			{
				rcountitem[i] = cr->second.itemcount[i];
			}
			std::stringstream buffer;
			buffer << "|cff00ff00|TInterface/ICONS/INV_Misc_Book_11:30|tReward|r:\n";
			if (rlevel)
				buffer << "|TInterface/ICONS/Achievement_Level_80:20:20:0:-5|tLevel |cff00ff00x" << rlevel << "|r\n";

			if (rmegapoints)
				buffer << "|TInterface/ICONS/Ability_Paladin_BlessedHands:20:20:0:-5|tMPs |cff00ff00x" << rmegapoints << "|r\n";

			if (rhonorpoints)
				buffer << "|TInterface/ICONS/Achievement_PVP_A_06:20:20:0:-5|tHonor Points |cff00ff00x" << rhonorpoints << "|r\n";

			if (rarenapoints)
				buffer << "|TInterface/ICONS/Achievement_PVP_A_15:20:20:0:-5|tArena Points |cff00ff00x" << rarenapoints << "|r\n";

			if (rvotepoints)
				buffer << "|TInterface/ICONS/Achievement_BG_tophealer_AB:20:20:0:-5|tVPs |cff00ff00x" << rvotepoints << "|r\n";

			if (ronlinepoints)
				buffer << "|TInterface/ICONS/Spell_Nature_TimeStop:20:20:0:-5|tVPs |cff00ff00x" << ronlinepoints << "|r\n";

			for (size_t i = 0; i < 9; i++)
			{
				uint32
					ritemid = rneeditem[i],
					rcount = rcountitem[i];
				std::string ritemicon, ritemlink;
				if (ritemid)
				{
					ritemicon = GetItemIcon(ritemid, 20, 20, 0, -5);
					ritemlink = GetItemLink(ritemid, player->GetSession());
					buffer << ritemicon << ritemlink << "|cff00ff00x" << rcount << "|r\n";
				}
			}
			return buffer.str();
		}
	}
	return "";
};

uint32 Warlords::GetPlayerTalent(Player* player)
{
	QueryResult Query = CharacterDatabase.PQuery("SELECT `spec_talent` FROM `characters` WHERE `guid` = '%u'", player->GetGUIDLow());
	uint32 spec;
	if (Query)
	{
		Field *Fields = Query->Fetch();

		spec = Fields[0].GetInt32();

		return spec;
	}
};