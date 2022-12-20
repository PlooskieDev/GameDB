#include "additemwindow.h"
#include "ui_additemwindow.h"

AddItemWindow::AddItemWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItemWindow)
{
    ui -> setupUi(this);

    connect(ui -> addFileButton, &QPushButton::clicked, this, &AddItemWindow::searchForFile);
    connect(ui -> acceptButton, &QPushButton::clicked, this, &AddItemWindow::convertInputToXML);
    connect(ui -> rejectButton, &QPushButton::clicked, this, &AddItemWindow::rejectForm);
}

AddItemWindow::~AddItemWindow()
{
    delete ui;
}

Game AddItemWindow::getGame() const
{
    return mGame;
}

void AddItemWindow::convertInputToXML()
{
    if (ui -> pathLE -> text().isEmpty() ||
            !ui -> studioLE -> isModified() ||
            !ui -> genreLE -> isModified() ||
            !ui -> dateLE -> isModified())
    {
        QMessageBox ms;
        ms.text() = "Not all info given!";
        ms.show();

        return;
    }

    mGame.setCoverPath(ui -> pathLE -> text());
    mGame.setName(ui -> nameLE -> text());
    mGame.setStudio(ui -> studioLE -> text());
    mGame.setGenre(ui -> genreLE -> text());
    mGame.setReleaseDate(ui -> dateLE -> text());

    accept();
}

void AddItemWindow::searchForFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.jpg *.png *.bpm *.webp *.jfif *.xpm)"));
    ui -> pathLE -> setText(fileName);
}

void AddItemWindow::rejectForm()
{
    reject();
}
