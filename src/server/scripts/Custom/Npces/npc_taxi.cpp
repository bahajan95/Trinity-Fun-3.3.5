#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "ScriptedGossip.h"
#include "Spell.h"
#include "ScriptedEscortAI.h"

enum Taxi
{
	npc_mount = 70012,
};

const float TaxiWP[5][3] =
{
	{ 5807.184f, 558.841f, 650.413f },
	{ 5783.023f, 533.770f, 641.559f },
	{ 5808.670f, 577.508f, 627.996f },
	{ 5837.940f, 611.126f, 620.651f },
	{ 5804.369f, 640.132f, 609.886f },
};

class npc_dalaran_taxi_mount : public CreatureScript
{
public:
	npc_dalaran_taxi_mount() : CreatureScript("npc_dalaran_taxi_mount") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_dalaran_taxi_mountAI(creature);
	}

	struct npc_dalaran_taxi_mountAI : public npc_escortAI
	{
		npc_dalaran_taxi_mountAI(Creature* creature) : npc_escortAI(creature)
		{
			//me->SetReactState(REACT_PASSIVE);
		}

		bool PlayerOn;

		void Reset()
		{
			PlayerOn = false;
		}

		void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply)
		{
			if (who->GetTypeId() == TYPEID_PLAYER)
			{
				PlayerOn = true;
				if (apply)
					Start(false, true, who->GetGUID());
			}
		}

		void FinishEscort()
		{
			me->DespawnOrUnsummon();
		}

		void WaypointReached(uint32 point)
		{
			Player* player = GetPlayerForEscort();

			if (point == 4)
			{
				player->ExitVehicle();
				me->DespawnOrUnsummon();
			}
		}

		void UpdateAI(uint32 diff)
		{
			npc_escortAI::UpdateAI(diff);
			Player* player = GetPlayerForEscort();

			if (PlayerOn)
			{
				player->SetClientControl(me, 0);
				PlayerOn = false;
			}
		}
	};
};

class npc_dalaran_taxi : public CreatureScript
{
public:
	npc_dalaran_taxi() : CreatureScript("npc_dalaran_taxi") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (player->IsInCombat())
			return true;

		float x, y;
		player->GetNearPoint2D(x, y, 2.0f, player->GetOrientation() + M_PI / 4);

		if (Creature* taxi = player->SummonCreature(npc_mount, x, y, player->GetPositionZ() + 1, player->GetOrientation()))
		if (npc_escortAI* escort = CAST_AI(npc_escortAI, taxi->AI()))
		{
			player->EnterVehicle(taxi, 0);

			for (int i = 0; i < 6; ++i)
				escort->AddWaypoint(i, TaxiWP[i][0], TaxiWP[i][1], TaxiWP[i][2]);
		}
		return true;
	}
};

void AddSC_npc_taxi2()
{
	new npc_dalaran_taxi_mount;
	new npc_dalaran_taxi;
}