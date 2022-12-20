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
    explicit AddItemWindow(QWidget *parent = nullptr);
    ~AddItemWindow();

    Game getGame() const;

private:
    Ui::AddItemWindow *ui;

    void convertInputToXML();
    void searchForFile();
    void rejectForm();

    Game mGame;
};

#endif // ADDITEMWINDOW_H
