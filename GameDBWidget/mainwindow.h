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
    /**
     * Opens the window where the user can create a new entry into the database.
     * If the window is closed without confirming the new item the function returns.
     */
    void openAddForm();

    /**
     * Opens a window that briefly informs the user about the app.
     */
    void openAboutForm();

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

    /**
     * Receives the game object and proceeds to add it as a new row into the table.
     *
     * @param &game The game object containing the information about the item that is to be added.
     */
    void addGameToTable(const Game &game);

    /**
     * Grabs the currently selected row's id and removes it from the table.
     */
    void removeGameFromTable();

    /**
     * Opens the XML file located in the directory of the compiled app.
     * If the import boolean parameter is true the function proceeds to ask the user
     * about the location of the already existing XML file alongside the directory containing the images.
     * It then adds each individual item to the table and displays the images.
     *
     * @param import A boolean value that decides the function should import a file or just open the already existing one.
     */
    void openXML(bool import);

    /**
     * The primary goal of this function is to set up a QDomDocument instance and then run it through another function.
     * If anything goes wrong the function throws an error message.
     */
    void saveXML();

    /**
     * Receives a QDomDocument instance (XML file) from the saveXML() function and proceeds to
     * loop through the table and convert each row to a Game object, it then creates an element
     * for each information stored within the Game object and writes it into the XML file as a new entry.
     * When done the function returns the filled QDomDocument instance.
     *
     * @param doc QDomDocument instance given from the saveXML() that is to be processed.
     * @return doc A processed QDomDocument instance.
     */
    QDomDocument xmlSetup(QDomDocument doc);

    /**
     * Checks if the folder for the images exists.
     * If yes proceeds to loop through the table, getting the image path of each item and saving it
     * (sometimes also converting to .bmp format) into the image folder so that the app can work with the image.
     */
    void saveImages();

    /**
     * If the user decides to import an already existing database.
     * This function gets every file in the given folder and saves it into the
     * image folder so that the app can work with the images.
     */
    void copyImportedImages();

    /**
     * This function takes care of the image displaying.
     * It loops through the table and puts an image (getting it from the image path) into the first column.
     */
    void loadImages();

    /**
     * This rather crude search function proceeds to run through the entire table,
     * scanning each item and comparing the search text with the value in the current cell.
     *
     * If it finds a match it skips onto the next one, leaving the current row displayed.
     * If it doesn't find a match it removes the current row from the table.
     */
    void searchTable();
};
#endif // MAINWINDOW_H
