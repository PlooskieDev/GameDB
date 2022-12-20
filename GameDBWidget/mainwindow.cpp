#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additemwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    // Actions
    connect(ui -> actionOpen, &QAction::triggered, this, &MainWindow::openAddForm);
    connect(ui -> actionSave, &QAction::triggered, this, &MainWindow::saveXML);
    connect(ui -> actionAdd, &QAction::triggered, this, &MainWindow::openAddForm);
    connect(ui -> actionRemove, &QAction::triggered, this, &MainWindow::removeGameFromTable);

    // Buttons
    //    connect(ui -> refreshButton, &QPushButton::clicked, this, &MainWindow::getXMLFiles);
    connect(ui -> addButton, &QPushButton::clicked, this, &MainWindow::openAddForm);




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
    AddItemWindow w(this, ui -> mainTable -> rowCount());

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

void MainWindow::openXML()
{
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 "Opened data",
                                                 QDir::rootPath(),
                                                 "XML file (*.xml)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", file.errorString());
        return;
    }

    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    auto root = doc.firstChild().toElement();
    auto ix = root.firstChild().toElement();

    while(!ix.isNull())
    {
        int id = ix.attribute("id").toInt();
        QString coverPath = ix.firstChild().toElement().text();
        QString name = ix.firstChild().nextSibling().toElement().text();
        QString studio = ix.firstChild().nextSibling().nextSibling().toElement().text();
        QString genre = ix.firstChild().nextSibling().nextSibling().nextSibling().toElement().text();
        QString releaseDate = ix.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().toElement().text();

        addGameToTable(Game(id, coverPath, name, studio, genre, releaseDate));

        ix = ix.nextSibling().toElement();
    }
}

void MainWindow::saveXML()
{
    if (ui -> mainTable -> rowCount() == 0)
    {
        QMessageBox::critical(this, "Error", "The table is empty.");
        return;
    }

    auto filename = QFileDialog::getSaveFileName(this,
                                                 "Saved data",
                                                 QDir::rootPath(),
                                                 "XML file (*.xml)");
    if (filename.isEmpty())
        return;

    QDomDocument doc;
    auto root = doc.createElement("Games");
    doc.appendChild(root);

    for (int i = 0; i < ui -> mainTable -> rowCount(); i++)
    {
//        Game game(i,
//                  ui -> mainTable -> item(i, 1) -> text(),
//                  ui -> mainTable -> item(i, 2) -> text(),
//                  ui -> mainTable -> item(i, 3) -> text(),
//                  ui -> mainTable -> item(i, 4) -> text(),
//                  ui -> mainTable -> item(i, 5) -> text()
//                  );

        Game game(i,
                  "lol1",
                  "lol2",
                  "lol3",
                  "lol4",
                  "lol5"
                  );

        auto eGame = doc.createElement("Game");
        eGame.setAttribute("id", game.id());

        auto eCoverPath = doc.createElement("CoverPath");
        auto eName = doc.createElement("Name");
        auto eStudio = doc.createElement("Studio");
        auto eGenre = doc.createElement("Genre");
        auto eReleaseDate = doc.createElement("ReleaseDate");

        eCoverPath.appendChild(doc.createTextNode(game.coverPath()));
        eName.appendChild(doc.createTextNode(game.name()));
        eStudio.appendChild(doc.createTextNode(game.studio()));
        eGenre.appendChild(doc.createTextNode(game.genre()));
        eReleaseDate.appendChild(doc.createTextNode(game.releaseDate()));

        eGame.appendChild(eCoverPath);
        eGame.appendChild(eName);
        eGame.appendChild(eStudio);
        eGame.appendChild(eGenre);
        eGame.appendChild(eReleaseDate);

        root.appendChild(eGame);
    }

    QFile file (filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", file.errorString());
        return;
    }

    QTextStream out(&file);
    out << doc.toString();
    file.flush();
    file.close();
}

































