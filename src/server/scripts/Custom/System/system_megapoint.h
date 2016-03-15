class Item;
class Player;
class WorldSession;
struct ItemTemplate;

class MegaPoint
{
public:
	//Boolean
	void AddAccountMPs(uint32 accountid, uint32 mps);
	void AddPlayerMPs(Player* player, uint32 mps);
	void RemoveAccountMPs(uint32 accountid, uint32 mps);
	void RemovePlayerMPs(Player* player, uint32 mps);

	//Integer
	uint32 GetPlayerMps(Player* player);
	uint32 GetAccountMps(uint32 accountid);
	float RandomFloat(float min, float max);
	uint32 GetCreatureMinMps(Creature* creature);
	uint32 GetCreatureMaxMps(Creature* creature);
};