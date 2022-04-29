#include "game.h"
#include "bot.h"

#include <QtWidgets>

#include <string>
using std::string;
using std::pair;

Game::Game(QWidget *parent)
	:
	QWidget(parent),
	board(new ChessBoard(this))
{
	NowTurn=1,PlayerFlag=0,IsPlayer=0;
	have_highlighted=0,hx=0,hy=0;
	memset(GameMap,0,sizeof(GameMap));
	
	connect(this,SIGNAL(actFinished()),this,SLOT(toMove()));
	
	connect(board,SIGNAL(Clicked(int,int)),this,SLOT(blockClicked(int,int)));
	QVBoxLayout *layout=new QVBoxLayout;
	layout->addWidget(board->getview());
	setLayout(layout);
	
	resize(BOARDSIZE,BOARDSIZE);
}

void Game::newGame(ECPlayerType player[2])
{
	loadGame(player,0,
	0b1000001000000000000000000000000000000000001000001ll,
	0b1000000000000000000000000000000000000000000000001ll);
}

void Game::loadGame(ECPlayerType player[2],bool nowplayer,long long boardmap,long long piecemap)
{
	NowTurn=1,PlayerFlag=0,IsPlayer=0;
	have_highlighted=0,hx=0,hy=0;
	for(int i=0;i<7;i++)for(int j=0;j<7;j++)
	{
		if(boardmap&1ll) setPiece(i,j,(piecemap&1ll)?1:-1);
		else setPiece(i,j,0);
		boardmap>>=1,piecemap>>=1;
	}
	Player[0]=player[0],Player[1]=player[1];
	PlayerFlag=nowplayer,NowTurn=nowplayer?-1:1;
	emit actFinished();
}

QJsonObject Game::saveAsJson()
{
	QJsonObject json;
	
	long long iboard=0,ipieces=0;
	for(int i=0;i<7;i++)for(int j=0;j<7;j++)
	{
		if(GameMap[i][j])
		{
			iboard|=1ll<<(i*7+j);
			if(GameMap[i][j]==1)
				ipieces|=1ll<<(i*7+j);
		}
	}
	
	json["board"]=QString(to_string(iboard).c_str());
	json["pieces"]=QString(to_string(ipieces).c_str());
	json["nowplayer"]=PlayerFlag;
	QJsonArray playerarray;
	for(int i=0;i<2;i++) playerarray.append(Player[i]==ECPlayerType::BOT?1:0);
	json["player"]=playerarray;
	return json;
}

void Game::loadFromJson(QJsonObject json)
{
	long long iboard=stoll(json["board"].toString().toStdString());
	long long ipieces=stoll(json["pieces"].toString().toStdString());
	bool nowplayer=json["nowplayer"].toBool();
	QJsonArray playerarray=json["player"].toArray();
	ECPlayerType player[2];
	for(int i=0;i<2;i++) player[i]=(playerarray[i].toInt())?ECPlayerType::BOT:ECPlayerType::HUMAN;
	loadGame(player,nowplayer,iboard,ipieces);
}

void Game::setPiece(int x,int y,int col)
{
	GameMap[x][y]=col;
	board->setCol(x,y,col);
}

void Game::gameMove(int x0,int y0,int x,int y)
{
	setPiece(x,y,NowTurn);
	if(std::max(abs(x0-x),abs(y0-y))==2)
		setPiece(x0,y0,0);
	for(int d=0;d<8;d++)
	{
		int x1=x+MOVEC[d][0],y1=y+MOVEC[d][1];
		if(0<=x1&&x1<7&&0<=y1&&y1<7)
		if(GameMap[x1][y1]==-NowTurn)
			setPiece(x1,y1,NowTurn);
	}
	NowTurn=-NowTurn;
	PlayerFlag=!PlayerFlag;
	if(isOver())
		return (void)gameOver();
	emit actFinished();
}

bool Game::isOver()
{
	for(int i=0;i<7;i++)for(int j=0;j<7;j++)
	{
		if(GameMap[i][j]==NowTurn)
		{
			for(int dx=-2;dx<=2;dx++)for(int dy=-2;dy<=2;dy++)
			{
				int x1=i+dx,y1=j+dy;
				if(0<=x1&&x1<7&&0<=y1&&y1<7&&GameMap[x1][y1]==0) return false;
			}
		}
	}
	return true;
}

int Game::getWinner()
{
	int cnt=0;
	for(int i=0;i<7;i++)for(int j=0;j<7;j++)
	{
		cnt+=GameMap[i][j]?GameMap[i][j]:-NowTurn;
	}
	return cnt<0;
}

void Game::gameOver()
{
	static const QString sWinner[]={QStringLiteral("Black Win"),QStringLiteral("White Win")};
	QMessageBox mb;
	mb.setText(sWinner[getWinner()]);
	mb.exec();
}

void Game::blockClicked(int x,int y)
{
	if(!IsPlayer||isOver()) return;
	if(GameMap[x][y]==NowTurn)
	{
		if(have_highlighted)
		{
			board->doHighlight(hx,hy,0),
			have_highlighted=0;
			if(!(x==hx&&y==hy))
				have_highlighted=1,hx=x,hy=y,
				board->doHighlight(hx,hy,1);
		}else
		{
			have_highlighted=1,hx=x,hy=y,
			board->doHighlight(hx,hy,1);
		}
	}else if(have_highlighted&&GameMap[x][y]==0)
	{
		if(abs(x-hx)<=2&&abs(y-hy)<=2)
		{
			board->doHighlight(hx,hy,0),
			have_highlighted=0;
			gameMove(hx,hy,x,y);
		}
	}
}

void Game::toMove()
{
	IsPlayer=0;
	if(Player[PlayerFlag]==ECPlayerType::HUMAN)
	{
		IsPlayer=1;
	}else
	{
		long long iboard=0,ipieces=0;
		for(int i=0;i<7;i++)for(int j=0;j<7;j++)
		{
			if(GameMap[i][j])
			{
				iboard|=1ll<<(i*7+j);
				if(GameMap[i][j]==1)
					ipieces|=1ll<<(i*7+j);
			}
		}
		pair<pair<int,int>,pair<int,int> > mv=BOT::main(iboard,ipieces,PlayerFlag);
		gameMove(mv.first.first,mv.first.second,
		mv.second.first,mv.second.second);
	}
}