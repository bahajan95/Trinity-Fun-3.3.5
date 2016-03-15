class Item;
class Player;
class WorldSession;
struct ItemTemplate;

class VotePoint
{
public:
	//Void
	void AddPlayerVPs(Player* player, uint32 mps);
	void RemovePlayerVPs(Player* player, uint32 mps);

	//Integer
	uint32 GetPlayerVPs(Player* player);
	uint32 GetPlayerTotalVPs(Player* player);
};