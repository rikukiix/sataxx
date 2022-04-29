#ifndef GAME_H
#define GAME_H

#include "gameboard.h"

#include <QJsonObject>

#include <cstdio>
#include <algorithm>
using std::pair;
using std::make_pair;

enum class ECPlayerType
{
	HUMAN,
	BOT
};

class Game: public QWidget
{
	Q_OBJECT
public:
	Game(QWidget *parent);
	void newGame(ECPlayerType player[]);
	void loadGame(ECPlayerType player[],bool nowplayer,long long boardmap,long long piecemap);
	QJsonObject saveAsJson();
	void loadFromJson(QJsonObject json);
private:
	ChessBoard *board;
	int GameMap[7][7];
	bool PlayerFlag;
	ECPlayerType Player[2];
	bool have_highlighted;
	int hx,hy;
	int NowTurn;
	int IsPlayer;
	void setPiece(int x,int y,int col);
	bool isOver();
	int getWinner();
	void gameOver();
	void gameMove(int x0,int y0,int x,int y);
protected slots:
	void blockClicked(int x,int y);
	void toMove();
signals:
	void actFinished();
};

#endif // GAME_H