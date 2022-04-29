#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPicture>


const int BOARDSIZE=600;
const int CHESSRADIUS=35;
const int BLOCKSIZE=80;
const int PIECESIZE=60;

const int MOVEC[8][2]=
	{{0,1},{1,1},{1,0},{1,-1},
	{0,-1},{-1,-1},{-1,0},{-1,1}};
const int MOVEJ[16][2]=
	{{0,2},{1,2},{2,2},{2,1},
	{2,0},{2,-1},{2,-2},{1,-2},
	{0,-2},{-1,-2},{-2,-2},{-2,-1},
	{-2,0},{-2,1},{-2,2},{-1,2}};

class Block : public QObject,public QGraphicsRectItem
{
	Q_OBJECT
private:
	int x,y;
	QPoint toPPos();
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
public:
	Block(int x=0,int y=0);
signals:
	void blockClicked(int x,int y);
};

class Piece : public QObject,public QGraphicsEllipseItem
{
	Q_OBJECT
private:
	int x,y,col;
	bool highlighted;
	QPen pen;
	QBrush brush;
	QPoint toPPos();
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
public:
	Piece(int col=0,int x=0,int y=0);
	void setColor(int col);
	void switchHighlight();
	bool isHighlighted();
signals:
	void pieceClicked(int x,int y);
};

class ChessBoard : public QObject
{
	Q_OBJECT
private:
	QGraphicsScene *scene;
	QGraphicsView *view;
	Block *blocks[7][7];
	Piece *pieces[7][7];
	bool have_highlighted;
	int hx,hy;
protected slots:
	void tileClicked(int x,int y);
public:
	ChessBoard(QObject *parent = nullptr);
	QGraphicsView* getview();
	void setCol(int x,int y,int col);
	void doHighlight(int x,int y,int sta);
signals:
	void Clicked(int x,int y);
};

#endif // GAMEBOARD_H