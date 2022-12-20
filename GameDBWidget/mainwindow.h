#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDirIterator>
#include <QXmlStreamReader>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDomDocument>
#include <QDomElement>

#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Game;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openAddForm();

private:
    Ui::MainWindow *ui;

    enum
    {
        COVER,
        NAME,
        STUDIO,
        GENRE,
        RELEASEDATE
    };

    void addGameToTable(const Game &game);
    void removeGameFromTable();

    void openXML();
    void saveXML();
};
#endif // MAINWINDOW_H
