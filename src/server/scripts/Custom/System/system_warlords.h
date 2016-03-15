class Item;
class Player;
class WorldSession;
struct ItemTemplate;

// Remember to add to GetStatName too
static const ItemModType statTypes[] =
{
	ITEM_MOD_INTELLECT,
	ITEM_MOD_STRENGTH,
	ITEM_MOD_STAMINA,
	ITEM_MOD_AGILITY,
	ITEM_MOD_SPIRIT,
	ITEM_MOD_ATTACK_POWER,
	ITEM_MOD_SPELL_POWER,
	ITEM_MOD_DODGE_RATING,
	ITEM_MOD_PARRY_RATING,
	ITEM_MOD_HIT_RATING,
	ITEM_MOD_CRIT_RATING,
	ITEM_MOD_HASTE_RATING,
	ITEM_MOD_EXPERTISE_RATING,
	ITEM_MOD_RESILIENCE_RATING
};

enum Requirements {
	REGUIREMENT_LEARN_SPELL = 1,
	REGUIREMENT_LEARN_TALENT = 2,
	REQUIREMENT_HEARTHSTONE = 3,
};

enum Menus
{
	MAIN_MENU,
	LEARN_SPELL,
	SHOW_TALENT,
	LEARN_TALENT,
	LATEST_NEWS,
	MY_INFO,
	SERVER_INFO,
	SHOW_NEWS,
	ROLE_PVP,
	ROLE_PVE,
	LEARN_START,
};

static bool send_cache_packets = true;

class Warlords
{
public:

	std::string GetItemIcon(uint32 entry, uint32 width, uint32 height, int x, int y);
	std::string GetItemLink(Item* item, WorldSession* session);
	std::string GetItemLink(uint32 entry, WorldSession* session);
	std::string GetSlotIcon(uint8 slot, uint32 width, uint32 height, int x, int y);
	std::string GetRequirement(Player* player, uint32 requirementid);
	std::string GetRaceIcon(uint8 id, uint8 gender);
	std::string GetClassIcon(uint8 id);
	std::string GetReward(Player* player, uint32 rewardid);

	char* GetStatName(uint32 ItemStatType);
	char* GetSlotName(uint8 slot, WorldSession* session);
	
	uint32 GetItemCount(uint32 item, bool inBankAlso, Item* skipItem) const;
	uint32 Melt(uint8 i, uint8 j);
	uint32 GetDuelRating(Player* player);
	uint32 GetArena2v2Rating(Player* player);
	uint32 GetArena3v3Rating(Player* player);
	uint32 GetArena5v5Rating(Player* player);
	uint32 GetPlayerTalent(Player* player);
	
	std::vector<Item*> GetItemList(const Player* player);
	Item* GetEquippedItem(Player* player, uint32 guidlow);

	void Unmelt(uint32 melt, uint8& i, uint8& j);
	void SendPacketItem(Player* player, uint32 entry, uint32 lowguid = 0, const ReforgeData* reforge = NULL);
	void RemoveReforge(Player* player, uint32 itemguid, bool update);
	void SendReforgePackets(Player* player);
	void UpdatePlayerReforgeStats(Item* invItem, Player* player, uint32 decrease, uint32 increase);
	void SendReward(Player* player, uint32 rewardid);
	void LearnSpells(Player* player, uint32 class_id);
	void LearnTalents(Player* player, uint32 spec_id);
	void AddGroupItems(Player* player, uint32 group_id);
	void RemoveGroupItems(Player* player, uint32 group_id);

	bool IsReforgable(Item* invItem, Player* player);
	bool CheckRequirement(Player* player, uint32 requirementid);
};