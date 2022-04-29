#include "mainwindow.h"
#include <QFile>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent)
    :
	QMainWindow(parent),
	game(new Game(this)),
	modenow(0),
	BtnModeSelect(new QPushButton(this)),
	BtnStart(new QPushButton(this)),
	BtnSave(new QPushButton(this)),
	BtnLoad(new QPushButton(this))
{
	setFixedSize(820,720);
	
	// move gamewrapper
	game->move(160,60);
	
	// initialize BtnModeSelect
	BtnModeSelect->setGeometry(10,10,80,40);
	BtnModeSelect->setText("HUMANxHUMAN");
	connect(BtnModeSelect,SIGNAL(clicked()),this,SLOT(changeMode()));
	
	// initialize BtnStart
	BtnStart->setGeometry(10,60,80,40);
	BtnStart->setText("NewGame");
	connect(BtnStart,SIGNAL(clicked()),this,SLOT(startNewGame()));
	
	// initialize BtnSave
	BtnSave->setGeometry(10,110,80,40);
	BtnSave->setText("Save");
	connect(BtnSave,SIGNAL(clicked()),this,SLOT(saveGame()));
	
	// initialize BtnLoad
	BtnLoad->setGeometry(10,160,80,40);
	BtnLoad->setText("Load");
	connect(BtnLoad,SIGNAL(clicked()),this,SLOT(loadGame()));
}

void MainWindow::changeMode()
{
	static const QString ModeDes[3]={QStringLiteral("HUMANxHUMAN"),QStringLiteral("HUMANxCOM"),QStringLiteral("COMxHUMAN")};
	modenow=(modenow+1)%3;
	BtnModeSelect->setText(ModeDes[modenow]);
}

void MainWindow::startNewGame()
{
	ECPlayerType list[2]={
		modenow&2?ECPlayerType::BOT:ECPlayerType::HUMAN,
		modenow&1?ECPlayerType::BOT:ECPlayerType::HUMAN
	};
	game->newGame(list);
}

void MainWindow::saveGame()
{
	QJsonObject json=game->saveAsJson();
	QFile f("save.json");
	if(!f.open(QIODevice::WriteOnly))
	{
		qWarning("Couldn't open save.json");
		return;
	}
	f.write(QJsonDocument(json).toJson());
}

void MainWindow::loadGame()
{
	QFile f("save.json");
	if(!f.open(QIODevice::ReadOnly))
	{
		qWarning("Couldn't open save.json");
		return;
	}
	QByteArray barray=f.readAll();
	QJsonDocument jsondoc(QJsonDocument::fromJson(barray));
	game->loadFromJson(jsondoc.object());
}

MainWindow::~MainWindow()
{
}

