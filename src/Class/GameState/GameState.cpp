#include "GameState.hpp"
#include "../Deck/Deck.cpp" // temporary solution

// === CONSTRUCTOR DESTRUCTOR ==================================

GameState::GameState()
{
    this->game = 0;
    this->round = 0;
    this->prize = 0;
    this->target = 0;
    this->ongoing = false;
    this->players = new PlayerList();
    this->playingDeck = new Deck<Card>;
    this->abilityDeck = new Deck<Ability *>;
    this->table = new Table();
}

GameState::~GameState()
{
    delete this->players;
    delete this->playingDeck;
    delete this->table;
}

// === GETTER SETTER ===========================================

void GameState::setPrize(long long int newPrize)
{
    this->prize = newPrize;
}

long long int GameState::getPrize()
{
    return this->prize;
}

bool GameState::isOngoing()
{
    return this->ongoing;
}

// === DISPLAY =================================================

void GameState::displaySplash()
{
    system("clear");
    std::cout << "\n##=======## CANDY KINGDOM CARD GAME ##=======##\n"
              << std::endl;
    std::cout << "           Created by Kebo Ngibrit            " << std::endl;
    std::cout
        << "\n#-----=====#*#========#@#========#*#=====-----#\n"
        << std::endl;
    std::cout << "            Press enter to start...           " << std::endl;
    std::cin.get();
}

void GameState::displayGameState()
{
    // system("clear");
    std::cout << "\n##=======## CANDY KINGDOM CARD GAME ##=======##\n"
              << std::endl;
    std::cout << "Game:\t\t" << this->game << std::endl;
    std::cout << "Round:\t\t" << this->round << std::endl;
    std::cout << "Table prize:\t" << this->prize << " points" << std::endl;
    std::cout
        << "\n#-----=====#*#========#@#========#*#=====-----#\n"
        << std::endl;
}

void GameState::displayTable()
{
    std::cout << "\n#-----============== TABLE ==============-----#\n"
              << std::endl;
    this->table->display(this->round);
    std::cout << "\n#-----=====#*#========#@#========#*#=====-----#\n"
              << std::endl;
}

void GameState::displayWinner()
{
    std::cout << "\n#-----======= CONGRATULATION !!! ========-----#\n"
              << std::endl;
    std::cout << this->winner.getName() << " wins the game with " << this->winner.getPoints() << " points!" << std::endl;
    std::cout << "\n#-----=====#*#========#@#========#*#=====-----#\n"
              << std::endl;
}

// === INITIALIZER =============================================

void GameState::initializePlayer()
{
    system("clear");
    std::cout << "\n##=========## WAITING FOR PLAYERS ##=========##\n"
              << std::endl;
    for (int i = 0; i < 7; i++)
    {
        Player *player = new Player();
        player->inputName();
        players->push(*player);
    }
}

void GameState::initializePlayingDeck()
{
    bool isMenuValid = false;
    std::string menu;
    std::string errMsg;

    while (!isMenuValid)
    {
        system("clear");
        std::cout << "\n#-----======= PLAYING DECK OPTION =======-----#\n"
                  << std::endl;
        std::cout << "                1. Generate random" << std::endl;
        std::cout << "             2. Import from text file" << std::endl;
        std::cout << "\n#-----=====#*#========#@#========#*#=====-----#\n"
                  << std::endl;

        std::cout << errMsg;

        try
        {
            std::cout << "Select menu: ";
            std::cin >> menu;

            if (menu != "1" && menu != "2")
            {
                throw "Menu not available\n";
            }

            isMenuValid = true;
        }
        catch (const char *err)
        {
            errMsg = err;
        }
        catch (...)
        {
            errMsg = "Menu invalid\n";
        }
    }

    if (menu == "1")
    {
        this->randomizeDeck();
    }
}

void GameState::randomizeDeck()
{
    for (int i = 1; i < 14; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Card *newCard = new Card(i, j);
            this->playingDeck->push(*newCard);
        }
    }

    this->playingDeck->shuffle();

    for (int i = 0; i < 5; i++)
    {
        this->table->push(this->playingDeck->getElmt(i));
    }
}

void GameState::rollPlayingCard()
{
    std::vector<Card> temp;
    for (int j = 0; j < 2; j++)
    {
        std::vector<Card> rolledCards;
        for (int i = 0; i < 7; i++)
        {
            rolledCards.push_back(this->playingDeck->roll());
            rolledCards.push_back(this->playingDeck->roll());
            players->setPlayerCards(i, rolledCards);
            rolledCards.clear();
        }
    }

    for (int i = 0; i < 5; i++)
    {
        this->table->push(this->playingDeck->getElmt(0));
        this->playingDeck->pop();
    }
}

void GameState::initializeAbilityDeck()
{
    PlayerManipulation *abiliyless = new PlayerManipulation(3);
    this->abilityDeck->push(abiliyless);

    Multiplier *quadruple = new Multiplier(1);
    this->abilityDeck->push(quadruple);

    Multiplier *quarter = new Multiplier(2);
    this->abilityDeck->push(quarter);
}

void GameState::rollAbility()
{
    this->playersAbility[this->players->getElmt(0).getName()] = this->abilityDeck->getElmt(0);
    this->playersAbility[this->players->getElmt(1).getName()] = this->abilityDeck->getElmt(1);
    this->playersAbility[this->players->getElmt(2).getName()] = this->abilityDeck->getElmt(2);
    this->playersAbility[this->players->getElmt(3).getName()] = this->abilityDeck->getElmt(2);
    this->playersAbility[this->players->getElmt(4).getName()] = this->abilityDeck->getElmt(2);
    this->playersAbility[this->players->getElmt(5).getName()] = this->abilityDeck->getElmt(2);
    this->playersAbility[this->players->getElmt(6).getName()] = this->abilityDeck->getElmt(2);
}

void GameState::evaluateGameWinner()
{
    for (int i = 0; i < 7; i++)
    {
        if (this->players->getElmt(i).getPoints() >= this->target)
        {
            this->ongoing = false;
            this->winner = this->players->getElmt(i);
        }
    }
}

// === GAME CONTROL ============================================

void GameState::newGame()
{
    // 1. Initialize game status
    this->game = 1;
    this->round = 1;
    this->prize = 64;
    this->target = 4294967296;
    this->ongoing = true;

    // 2. Initialize players
    initializePlayer();

    // 3. Initialize deck
    initializePlayingDeck();

    // 4. Roll playing cards
    rollPlayingCard();

    // 5. Initialize ability
    initializeAbilityDeck();

    // 6. Roll ability
    rollAbility();
}

void GameState::nextTurn()
{
    this->players->roundRobin();
}

void GameState::nextRound()
{
    this->round++;
}

void GameState::nextGame()
{
    this->evaluateGameWinner();
    this->game++;
    this->round = 1;
    this->prize = 64;
}

void GameState::playerAction()
{
    std::string action;
    std::string errMsg;
    bool isValid = false;

    while (!isValid)
    {
        this->displayGameState();
        this->displayTable();
        std::cout << "\n#-----=========== PLAYER TURN ===========-----#\n"
                  << std::endl;
        std::cout << "Name:\t" << this->players->getElmt(0).getName() << std::endl;
        std::cout << "Points:\t" << this->players->getElmt(0).getPoints() << std::endl;
        std::cout << "Hands:\t";
        this->players->getElmt(0).displayHands();
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "Action menu:" << std::endl;
        std::cout << "1. Double" << std::endl;
        std::cout << "2. Next" << std::endl;
        std::cout << "3. Half" << std::endl;
        std::cout << "4. ";
        std::cout << this->playersAbility[this->players->getElmt(0).getName()]->getName();
        // this->players->getElmt(0).setIsDisable();
        std::cout << " [Ability]" << this->players->getElmt(0).getIsDisable();
        if (this->players->getElmt(0).getIsDisable())
        {
            std::cout << " [Not available]";
        }
        std::cout << std::endl;
        std::cout << std::endl;

        try
        {
            std::cerr << errMsg;
            std::cout << "Action: ";
            std::cin >> action;
            if (action == "1")
            {
                this->prize *= 2;
            }
            else if (action == "2")
            {
                // do nothing
            }
            else if (action == "3")
            {
                if (this->prize != 1)
                {
                    this->prize /= 2;
                }
            }
            else if (action == "4")
            {
                if (this->players->getElmt(0).getIsDisable())
                {
                    throw "Ability disabled\n";
                }
                this->setPrize(this->playersAbility[this->players->getElmt(0).getName()]->use(this->prize));
                this->playersAbility[this->players->getElmt(0).getName()]->use(*this->players, *this->playingDeck);
            }
            else
            {
                throw "Action is not available\n";
            }
            isValid = true;
        }
        catch (const char *err)
        {
            errMsg = err;
        }
    }
}