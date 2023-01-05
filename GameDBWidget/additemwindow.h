#ifndef ADDITEMWINDOW_H
#define ADDITEMWINDOW_H

#include <QFileDialog>
#include <QMessageBox>

#include "game.h"

namespace Ui {
class AddItemWindow;
}

class AddItemWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemWindow(QWidget *parent = nullptr, const int &rowsCount = 0);
    ~AddItemWindow();

    Game getGame() const;

private:
    Ui::AddItemWindow *ui;

    Game mGame;
    int mRowsCount;

    /**
     * This function checks if all the boxes have been filled out.
     * If yes it fills the mGame object with the acquired data and closes the form with the accept() function.
     * (The game object is then processed in the main window)
     */
    void convertInputToXML();

    /**
     * Opens a dialog in which the user can select the desired image.
     */
    void searchForFile();
    void rejectForm();
};

#endif // ADDITEMWINDOW_H
