#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDirIterator>
#include <QXmlStreamReader>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDomDocument>

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

    QString filePath = "";

    enum
    {
        COVER,
        NAME,
        STUDIO,
        GENRE,
        RELEASEDATE,
        COVERPATH,
        ID
    };

    void addGameToTable(const Game &game);
    void removeGameFromTable();

    void openXML(bool import);
    void saveXML();
    QDomDocument xmlSetup(QDomDocument doc);

    void saveImages();
    void copyImportedImages();
    void loadImages();

    void searchTable();
};
#endif // MAINWINDOW_H
