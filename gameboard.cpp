#include"gameboard.h"

#include <QPixmap>
#include <QPainter>
#include <QGraphicsDropShadowEffect>

Block::Block(int x,int y):x(x),y(y)
{
	setAcceptHoverEvents(true);
	
	this->setRect(this->toPPos().x(),this->toPPos().y(),BLOCKSIZE,BLOCKSIZE);
	this->setPen(QPen(Qt::black));
	this->setBrush(QBrush(Qt::white));
}

void Block::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit blockClicked(x,y);
}

QPoint Block::toPPos()
{
	return QPoint(x*BLOCKSIZE,y*BLOCKSIZE);
}

Piece::Piece(int col,int x,int y):col(col),x(x),y(y)
{
	setAcceptHoverEvents(true);
	this->highlighted=0;
	
	this->setRect(this->toPPos().x(),this->toPPos().y(),PIECESIZE,PIECESIZE);
	if(this->col!=0)
	{
		this->setVisible(true);
		this->setPen(QPen(Qt::black));
		this->setBrush(this->col==1?QBrush(Qt::black):QBrush(Qt::white));
	}else
	{
		this->setVisible(false);
	}
}

QPoint Piece::toPPos()
{
	return QPoint(x*BLOCKSIZE+(BLOCKSIZE-PIECESIZE)/2,
	y*BLOCKSIZE+(BLOCKSIZE-PIECESIZE)/2);
}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit pieceClicked(x,y);
}

void Piece::setColor(int col)
{
	this->col=col;
	if(!col) this->setVisible(false);
	else
	{
		this->setVisible(true);
		this->setPen(QPen(Qt::black));
		this->setBrush(this->col==1?QBrush(Qt::black):QBrush(Qt::white));
	}
}

void Piece::switchHighlight()
{
	if(!highlighted)
	{
		highlighted=true;
		this->setPen(QPen(Qt::magenta));
		this->setPos(this->pos()-QPoint(2,2));
	}else
	{
		highlighted=false;
		this->setPen(QPen(Qt::black));
		this->setPos(this->pos()+QPoint(2,2));
	}
}

bool Piece::isHighlighted()
{
	return this->highlighted;
}

ChessBoard::ChessBoard(QObject *parent)
	:
	QObject(parent),
	scene(new QGraphicsScene(this)),
	view(new QGraphicsView(scene))
{
	have_highlighted=false;
	view->setRenderHints(QPainter::Antialiasing);
	
	
	
	for(int i=0;i<7;i++)for(int j=0;j<7;j++)
	{
		blocks[i][j]=new Block(i,j);
		scene->addItem(blocks[i][j]);
		connect(blocks[i][j],SIGNAL(blockClicked(int,int)),
			this,SLOT(tileClicked(int,int)));
		
		pieces[i][j]=new Piece(0,i,j);
		scene->addItem(pieces[i][j]);
		connect(pieces[i][j],SIGNAL(pieceClicked(int,int)),
			this,SLOT(tileClicked(int,int)));
	}
}

void ChessBoard::tileClicked(int x,int y)
{
	emit Clicked(x,y);
}

QGraphicsView* ChessBoard::getview()
{
	return view;
}

void ChessBoard::setCol(int x,int y,int col)
{
	pieces[x][y]->setColor(col);
}

void ChessBoard::doHighlight(int x,int y,int sta)
{
	if(pieces[x][y]->isHighlighted())
	{
		
	}else
	{
		
	}
	pieces[x][y]->switchHighlight();
}
