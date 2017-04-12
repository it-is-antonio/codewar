#include <bits/stdc++.h>
using namespace std;

const int dx[9] = {-1, -1, -1,  0, 0,  1, 1, 1, 0};
const int dy[9] = {-1,  0,  1, -1, 1, -1, 0, 1, 0};
const string DIRECT_NAME[4] = { "LEFT", "RIGHT", "UP", "DOWN"};
const int dir_x[4] = { 0, 0, -1, 1};
const int dir_y[4] = {-1, 1,  0, 0};

class State
{
public:
	vector<string> table;
	vector<string> lastCellState;
	vector<pair<int, int> > playersPos;
	State()
	{
		this->table = this->lastCellState = vector<string>(20, "000000000000000000000000000000");
	}
	State(const vector<string>& _table, const vector<pair<int, int> >& _playersPos)
	{
		this->lastCellState = vector<string>(20, "000000000000000000000000000000");
		this->table = _table;
		this->playersPos = _playersPos;
	}
	void printState()
	{
		for(int i = 0; i < 20; i++) {
			for(int j = 0; j < 30; j++) {
				if( this->lastCellState[i][j] == '0')
					cout << ".";
				else
					cout << this->lastCellState[i][j];
			}
			cout << endl;
		}
		cout << "----------------------------------" << endl;
		for(int i = 0; i < 20; i++)
		{
			// cout << this->table[i] << endl;
			for(int j = 0; j < 30; j++)
			{
				if( this->table[i][j] == '0')
					cout << ".";
				else
					cout << this->table[i][j];
			}
			cout << endl;
		}
		for(int i = 0; i < this->playersPos.size(); i++)
		{
			cout << this->playersPos[i].first <<" "<<this->playersPos[i].second<<endl;
		}
	}
};

class Player
{
public:
	vector<State> states;
	int numberOfPlayer;
	int ownId;
	int turn;
//	Player(const Player&);
	/**
	* @param int _numberOfPlayer
	* @param int _ownId
	* @param const vector<string>& table
	* @param const vector<pair<int, int> >& palyersPos
	*/
	Player(int, int, const vector<string>&, const vector<pair<int, int> >&);
	~Player();
	string getDecision()
	{
		if( this->ownId == 1) return "RIGHT";
		else
			if(this->ownId == 2) return "LEFT";
		return DIRECT_NAME[this->ownId-1];
	}
	void updateState(const State&);
	bool isValidDecision(string);
	int getScore(int);
	int minDistanceToOtherUnstable();
	int minDistanceToStable();
	void printPos(int);
	int currentDirect(int);
};
int Player::currentDirect(int playerId)
{
    if( this->states.size() == 1) return -1;
    pair<int, int> lastPos = this->states[this->states.size()-1].playersPos[playerId-1];
    pair<int, int> prevPos = this->states[this->states.size()-2].playersPos[playerId-1];
    pair<int, int> delta = make_pair(lastPos.first - prevPos.first, lastPos.second - prevPos.second);
    for(int i = 0; i < 4; i++)
        if( delta.first == dir_x[i] && delta.second == dir_y[i] )
            return i;
    return 555;
}
Player::Player(int _numberOfPlayer,int _ownId,const vector<string> &_table, const vector<pair<int, int> > &_playersPos)
{
	this->turn = 0;
	this->ownId = _ownId;
	this->numberOfPlayer = _numberOfPlayer;
	this->states.push_back(State(_table, _playersPos));
}
void Player::updateState(const State& state)
{
	this->states.push_back(state);
	this->turn++;
}
int Player::getScore(int playerId)
{
	int score = 0;
	State state = this->states[this->states.size()-1];
	for(int i = 0; i < 20; i++) {
		for(int j = 0; j < 30; j++) {
			if( state.table[i][j] == playerId * 2 - 1 + '0') score++;
		}
	}
	return score;
}
void Player::printPos(int playerId)
{
	// cout << this->playersPos[playerId].first <<" "<<this->playersPos[playerId].second << endl;
}
Player::~Player()
{

}
class Splix
{
public:
	// players (AI)
	vector<Player> players;
	vector<State> states;
	int numberOfPlayer;
	int turn;
	Splix(int _numberOfPlayer);
	/**
	/* @param int _numberOfPlayer
	/* @param const vector<string>& table
	/* @param const vector<pair<int, int> >& palyersPos
	*/
	Splix(int, const vector<string>&, const vector<pair<int, int> >&);
	~Splix();
	/***
	/* @brief get each player decision alternatively
	/* @step
	/* - each players
	/*   + callPlayer
	/*   + check if decision is in law
	/* - find all loser if exist
	/*	 + updateTable
	*/
	void nextTurn();
	/***
	/* @brief get each player decision alternatively
	/* @step
	/* - each players
	/*   + callPlayer
	/*   + check if decision is in law
	/* - find all loser if exist
	/*	 + updateTable
	*/
    void nextTurn(const vector<string>&);
	/***
	/* @brief: update table
	/* @param: const vector<string>& decisions
	/* @step:
	/*  - remove losers
	/*  - update stable cell
    */
	void stablize(State&);
	void updateGame(const vector<string>& decisions);
	/***
	/* @param int playerId
	/* @param const State& state
	/* @param const vector<string>& decsions
	*/
	int getDirectID(int,const State&, const vector<string>&);
	bool decisionInLawAndChangePos(int playerId, State &state, const vector<string> &decisions);
	void updateStatePlayer();
	bool isOuter(const pair<int, int>&);
	void testConstructor()
	{
		cout << "Number of Player " << this->numberOfPlayer << endl;
		cout << "Turn " << this->turn << endl;
		this->states[0].printState();
	}
	void printPulse()
	{
		cout << "Number of Player " << this->numberOfPlayer << endl;
		cout << "Turn " << this->turn << endl;
		this->states[this->turn].printState();
		for(int i = 1; i <= this->numberOfPlayer; i++)
		{
			cout << "Score of Player " << i <<" is " << this->players[i-1].getScore(i) <<endl;
		}
	}
};
int main()
{
	Splix splix = Splix(3);
	// splix.testConstructor();
	for(int step = 0; step < 10; step++) {
		splix.updateStatePlayer();
		splix.printPulse();
		splix.nextTurn();
	}
	splix.updateStatePlayer();
	splix.printPulse();
	return 0;
}

Splix::Splix(int _numberOfPlayer)
{
	this->numberOfPlayer = _numberOfPlayer;
	this->turn = 0;
	pair<int, int> pos[4] = {
		make_pair(5,5),
		make_pair(5, 24),
		make_pair(9, 10),
		make_pair(14, 24)
	};
	// pair<int, int> pos[4] = {
	// 	make_pair(5,5),
	// 	make_pair(5, 24),
	// 	make_pair(14, 5),
	// 	make_pair(14, 24)
	// };
	State state;
	for(int i = 0; i < 20; i++) {
		state.table[i] = "000000000000000000000000000000";
	}
	for(int i = 1; i <= _numberOfPlayer; i++) {
		for(int j = 0; j < 9; j++) {
			state.table[pos[i-1].first + dx[j]][pos[i-1].second + dy[j]] = i * 2 - 1 + '0';
		}
	}
	state.playersPos = vector<pair<int, int> >(pos, pos+_numberOfPlayer);
	this->states.push_back(state);
}
Splix::Splix(int _numberOfPlayer, const vector<string> &_table, const vector<pair<int, int> > &_playersPos)
{
	this->numberOfPlayer = _numberOfPlayer;
	this->turn = 0;
	State state;
	state.table = _table;
	state.playersPos = _playersPos;
	this->states.push_back(state);
}
Splix::~Splix()
{
}
void Splix::updateStatePlayer() 
{
	// start game
	if( this->turn == 0 )
	{
		for(int i = 1; i <= this->numberOfPlayer; i++)
		{
			Player player = Player
			(
				this->numberOfPlayer,
				i,
				this->states[this->turn].table,
				this->states[this->turn].playersPos
			);
			this->players.push_back(player);
		}
	}
	else // inGame
	{
		for(int i = 0; i < this->players.size(); i++)
		{
			players[i].updateState(this->states[this->turn]);
		}
	}
}
/***
/* @brief get each player decision alternatively
/* @step
/* - each players
/*   + getDecisions respectively
/*   + updateGame
*/
void Splix::nextTurn()
{
	vector<string> decision = vector<string>(this->numberOfPlayer);
	for(int i = 0; i < this->numberOfPlayer; i++)
	{
		decision[i] = this->players[i].getDecision();
	}
	cout << "Decisions: "<<endl;
	for(int i = 0; i < this->players.size(); i++) cout << decision[i]<<" "; cout << endl;
	this->updateGame(decision);
	this->turn++;
}
int Splix::getDirectID(int playerId,const State& state, const vector<string> &decisions)
{
	int directId = -1;
	for(int j = 0; j < 4; j++)
	{
		if( decisions[playerId-1] == DIRECT_NAME[j] )
		{
			directId = j;
			break;
		}
	}
	if( directId == -1) return -1;
	int previousDirect = this->players[playerId-1].currentDirect(playerId);
	if( previousDirect != -1 &&
        (
        (DIRECT_NAME[directId] == "UP" && DIRECT_NAME[previousDirect] == "DOWN") ||
		(DIRECT_NAME[directId] == "DOWN" && DIRECT_NAME[previousDirect] == "UP") ||
		(DIRECT_NAME[directId] == "RIGHT" && DIRECT_NAME[previousDirect] == "LEFT") ||
		(DIRECT_NAME[directId] == "LEFT" && DIRECT_NAME[previousDirect] == "RIGHT")
        )
	  )
		return -1;
	return directId;
}
bool Splix::isOuter(const pair<int, int>& pos)
{
	return (pos.first < 0 || pos.first >= 20 || pos.second < 0 || pos.second >= 30);
}
/**
/* @brief: check if playerId make a decision in law, if true update currentPos of playerId
/* @return: playerId make a decision in law

case1: Không đưa ra được nước đi nào (sau thời gian timeout là 1s)
case2: Đưa ra các output không đúng với format 4 hướng di chuyển
case3: Đi ra khỏi bàn đấu
case4: Đi ngược lại với hướng đi của lượt trước đó
case5: Đi vào ô unstable của chính mình
*/

bool Splix::decisionInLawAndChangePos(int playerId, State &state, const vector<string> &decisions)
{
	if( state.playersPos[playerId-1] == make_pair(-1, -1) ) return true;
	int directId = this->getDirectID(playerId, state, decisions);
	cout << "directId " << playerId <<" "<<directId << endl;
	bool goInOwnUnstable;
	if( directId == -1) return false;
	pair<int, int> newPos;
	newPos.first = state.playersPos[playerId-1].first + dir_x[directId];
	newPos.second = state.playersPos[playerId-1].second + dir_y[directId];
	cout << "newPos " << newPos.first <<" "<<newPos.second << endl;
	bool isOuter = this->isOuter(newPos);
	if( isOuter ) return false;
	goInOwnUnstable = state.table[newPos.first][newPos.second] == playerId * 2;
	if( goInOwnUnstable ) return false;
	state.playersPos[playerId-1] = newPos;
	return true;
}
/***
/* @brief: update Game(last state)
/* @step:
/*  - check if players take a decision in law
/*  - find loser who was hit by another
/*  - update cell
*/
void Splix::updateGame(const vector<string> &decisions)
{
	State lastState = this->states[this->turn];
	vector<bool> illegalDecision = vector<bool>(this->numberOfPlayer, false);
	for(int i = 0; i < this->numberOfPlayer; i++)
	{
		illegalDecision[i] = !this->decisionInLawAndChangePos(i+1, lastState, decisions);
	}
	for(int i = 0; i < illegalDecision.size(); i++)	cout << illegalDecision[i] << " ";
	cout << endl;
	vector<bool> isHit = vector<bool>(this->numberOfPlayer, false);
	for(int i = 0; i < this->numberOfPlayer; i++) if(lastState.playersPos[i] != make_pair(-1, -1))
	{
		int cell = lastState.table[lastState.playersPos[i].first][lastState.playersPos[i].second] - '0';
		if( cell > 0 && cell % 2 == 0 ) isHit[cell/2-1] = true;
	}
	for(int i = 0; i < this->numberOfPlayer; i++) if (lastState.playersPos[i] != make_pair(-1, -1))
		for(int j = i+1; j < this->numberOfPlayer; j++) if (lastState.playersPos[j] != make_pair(-1, -1))
			if( lastState.playersPos[i] == lastState.playersPos[j] )
			{
				isHit[i] = isHit[j] = true;
			}
	for(int i = 0; i < isHit.size(); i++) cout << isHit[i] <<" ";
	cout << endl;
	// remove player who make a wrong decision or is hit by another player
	// remove unstable cell
	for(int id = 1; id <= this->numberOfPlayer; id++)
	{
		if( isHit[id-1] || illegalDecision[id-1] )
		{
			lastState.playersPos[id-1] = make_pair(-1, -1);
			for(int i = 0; i < 20; i++) {
				for(int j = 0; j < 30; j++) {
					if( lastState.table[i][j] == id * 2 + '0') {
						lastState.table[i][j] = lastState.lastCellState[i][j];
						lastState.lastCellState[i][j] = '0';
					}
					if( (lastState.lastCellState[i][j] - '0' + 1) / 2 == id )
						lastState.lastCellState[i][j] = '0';
				}
			}
		}
	}
	// remove stable cell
	for(int id = 1; id <= this->numberOfPlayer; id++)
	{
		if( isHit[id-1] || illegalDecision[id-1] )
		{
			for(int i = 0; i < 20; i++) {
				for(int j = 0; j < 30; j++) {
					if( lastState.table[i][j] == id * 2 - 1 + '0') {
						lastState.table[i][j] = lastState.lastCellState[i][j];
						lastState.lastCellState[i][j] = '0';
					}
				}
			}
		}
		else if(lastState.playersPos[id-1] != make_pair(-1, -1))
		{
			pair<int, int> pos = lastState.playersPos[id-1];
			if( lastState.table[pos.first][pos.second] != id * 2 - 1 + '0') 
			{
				// cout <<" position " <<" "<<id*2-1<<" "<<pos.first <<" "<<pos.second << endl;
				lastState.lastCellState[pos.first][pos.second] = lastState.table[pos.first][pos.second];
				lastState.table[pos.first][pos.second] = id * 2 + '0';
			}
		}
	}
	this->stablize(lastState);
	this->states.push_back(lastState);
}
void Splix::stablize(State &state) 
{

}
