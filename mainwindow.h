#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QPushButton>
#include "game.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
	Game *game;
	int modenow;
	QPushButton *BtnModeSelect;
	QPushButton *BtnStart;
	QPushButton *BtnSave;
	QPushButton *BtnLoad;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	
private slots:
	void changeMode();
	void startNewGame();
	void saveGame();
	void loadGame();
};
#endif // MAINWINDOW_H
