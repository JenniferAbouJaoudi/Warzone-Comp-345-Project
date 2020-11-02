#include "GameEngine.h"
#include "Player.h"
#include "Cards.h"
#include "Utils.h"

namespace WZ
{
	bool GameManager::isNegotiating(const Player* p1, const Player* p2) { return GetManager().isNegotiatingImpl(p1, p2); }

	void GameManager::addNegotiatingPlayers(const Player* p1, const Player* p2) { GetManager().addNegotiatingPlayersImpl(p1, p2); }
	
	void GameManager::drawCard(Player* p) { GetManager().drawCardImpl(p); }

	Player* GameManager::getNeutralPlayer() { return GetManager().getNeutralPlayerImpl(); }

	void GameManager::startupPhase(const Player* p, const Territory* t) { GetManager().startupPhaseImpl(p, t); }

	GameManager::GameManager() : m_neutralPlayer(new Player("Neutral")), m_deck(new Deck()),
		map(nullptr), currentphase(GamePhase::Reinforcement)
	{
		Random::Init();
	}
	
	GameManager::~GameManager()
	{
		delete m_neutralPlayer;
		delete m_deck;
		delete map;
	}

	
	GameManager& GameManager::GetManager()
	{
		//since the manager variable is static it will be kept in 
		//static memory allowing use to reuse it everytime we call the GetManager function
		static GameManager manager;
		return manager;
	}

	bool GameManager::isNegotiatingImpl(const Player* p1, const Player* p2)
	{
		for (std::pair<const Player*, const Player*> pair : m_negotiatingPlayers)
		{
			if ((pair.first == p1 && pair.second == p2) || (pair.first == p2 && pair.second == p1))
			{
				return true;
			}
		}
		return false;
	}

	void GameManager::addNegotiatingPlayersImpl(const Player* p1, const Player* p2)
	{
		m_negotiatingPlayers.push_back({ p1, p2 });
	}

	void GameManager::drawCardImpl(Player* p)
	{
		if (p->hasDrawnCard)
		{
			std::cout << p->getPlayerName() << " tries to draws a card but they already drew this turn\n";
			return;
		}

		std::cout << p->getPlayerName() << " draws a card\n";
		p->hasDrawnCard = true;

		Card* c = m_deck->draw();
		if (c == nullptr)
		{
			return;
		}
		p->getHand()->addCardToHand(c);
	}

	Player* GameManager::getNeutralPlayerImpl()
	{
		return m_neutralPlayer;
	}

	void GameManager::resetPlayerDrawCard()
	{
		for (Player* p : m_activePlayers)
		{
			p->hasDrawnCard = false;
		}
	}

	//to be implemented
	void GameManager::startupPhaseImpl(const Player* p, const Territory* t) {

	}

	GamePhase GameManager::getCurrentPhase() {
		return GetManager().currentphase;
	}

	const Player* GameManager::getCurrentPlayer() {
		return GetManager().m_activePlayers[GetManager().CurrentPlayerIndex];
	}

	void GameManager::getUserMap()
	{
		MapLoader loader;
		delete map;
		//will automatically ask the user to select a map file and generate the map from that file if it is valid
		map = loader.mapGenerator(); 
	}
	
	int GameManager::getUserNumPlayers()
	{
		while(true)
		{
			std::cout << "Player with how many players?\n";
			int num = Clamp(2, 5, AskInt());

			std::cout << "Play with " << num << " players?\n";
			if (AskYN())
			{
				return num;
			}
		}
	}

}