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

    void convertInputToXML();
    void searchForFile();
    void rejectForm();
};

#endif // ADDITEMWINDOW_H
