#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additemwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Buttons
    //    connect(ui -> refreshButton, &QPushButton::clicked, this, &MainWindow::getXMLFiles);
    connect(ui -> addButton, &QPushButton::clicked, this, &MainWindow::openAddForm);
    connect(ui -> actionAdd, &QAction::triggered, this, &MainWindow::openAddForm);
    connect(ui -> actionRemove, &QAction::triggered, this, &MainWindow::removeGameFromTable);

    // Table setup
    ui -> mainTable -> setColumnCount(5);
    ui -> mainTable -> setHorizontalHeaderLabels(QStringList{"Cover",
                                                             "Name",
                                                             "Studio",
                                                             "Genre",
                                                             "Release date"});

    ui -> mainTable -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
    //    ui -> mainTable -> verticalHeader() -> setDefaultSectionSize(100);
    //    ui -> mainTable -> verticalHeader() -> setMinimumSectionSize(100);

    ui -> mainTable -> setRowHeight(0, 100);
    ui -> mainTable -> setColumnWidth(0, 100);


    //    QPixmap pix("E:/Downloads/header.jpg");
    //    pix.save("E:/Downloads/header.bmp");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openAddForm()
{
    AddItemWindow w(this);

    if (w.exec() == QDialog::Rejected)
        return;

    w.setModal(true);
    addGameToTable(w.getGame());
}

void MainWindow::addGameToTable(const Game &game)
{
    const int row = ui -> mainTable -> rowCount();
    ui -> mainTable -> insertRow(row);
    ui -> mainTable -> setItem(row, COVER, new QTableWidgetItem(game.coverPath()));
    ui -> mainTable -> setItem(row, NAME, new QTableWidgetItem(game.name()));
    ui -> mainTable -> setItem(row, STUDIO, new QTableWidgetItem(game.studio()));
    ui -> mainTable -> setItem(row, GENRE, new QTableWidgetItem(game.genre()));
    ui -> mainTable -> setItem(row, RELEASEDATE, new QTableWidgetItem(game.releaseDate()));
}

void MainWindow::removeGameFromTable()
{
    ui -> mainTable -> removeRow(ui -> mainTable -> currentRow());
}

































