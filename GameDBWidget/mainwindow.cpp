#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additemwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    // Actions
    //     [=]() { this -> openXML(false); });
    connect(ui -> actionAbout, &QAction::triggered, this, &MainWindow::openAboutForm);
    connect(ui -> actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui -> actionAdd, &QAction::triggered, this, &MainWindow::openAddForm);
    connect(ui -> actionRemove, &QAction::triggered, this, &MainWindow::removeGameFromTable);

    // Buttons
    connect(ui -> addButton, &QPushButton::clicked, this, &MainWindow::openAddForm);

    // LineEdit
    connect(ui -> searchLE, &QLineEdit::textChanged, this, &MainWindow::searchTable);

    // Table setup
    ui -> mainTable -> setColumnCount(7);
    ui -> mainTable -> setHorizontalHeaderLabels(QStringList{"Cover",
                                                             "Name",
                                                             "Studio",
                                                             "Genre",
                                                             "Release date",
                                                             "CoverPath",
                                                             "ID"});

    ui -> mainTable -> setColumnHidden(COVERPATH, true);
    ui -> mainTable -> setColumnHidden(ID, true);

    ui -> mainTable -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
    ui -> mainTable -> verticalHeader() -> setDefaultSectionSize(150);
    ui -> mainTable -> verticalHeader() -> setMinimumSectionSize(150);

    filePath = QCoreApplication::applicationDirPath() + "/data.xml";

    // Import if the user desires
    if (!QFile::exists(filePath))
    {
        QMessageBox msgBox;
        msgBox.setText("There is no saved GameDB database on your device.");
        msgBox.setInformativeText("Would you like to import?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        switch (ret)
        {
        case QMessageBox::Yes:
            openXML(true);
            break;
        case QMessageBox::No:
            saveXML();
            break;
        default:
            break;
        }
    }

    openXML(false);
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

    //    w.setModal(true);
    addGameToTable(w.getGame());

    //    Game g(0,"E:/Pictures/Doggos/BlueBW.jpg","idk","idk","idk","idk");
    //    addGameToTable(g);

    loadImages();
    saveImages();
    saveXML();
}

void MainWindow::openAboutForm()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About GameDB");
    msgBox.setText("GameDB is a term project for the AP7MP."
                   "\nIt allows users to store information about their favorite games."
                   "\nDeveloped by Michael Pluskal.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
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
    ui -> mainTable -> setItem(row, COVERPATH, new QTableWidgetItem(game.coverPath()));
    ui -> mainTable -> setItem(row, ID, new QTableWidgetItem(QString::number(game.id())));
}

void MainWindow::removeGameFromTable()
{
    if (ui -> mainTable -> currentRow() == -1)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("No item selected");
        msgBox.setText("You haven't selected any item.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        return;
    }

    QString itemID = ui -> mainTable -> item(ui -> mainTable -> currentRow(), ID) -> text();

    openXML(false);

    for (int i = 0; i < ui -> mainTable -> rowCount(); i++)
    {
        if (ui -> mainTable -> item(i, ID) -> text() == itemID)
        {
            ui -> mainTable -> removeRow(i);
            break;
        }
    }

    saveXML();
    searchTable();
}

void MainWindow::openXML(bool import)
{
    if (import)
    {
        auto filename = QFileDialog::getOpenFileName(this,
                                                     "Opened data",
                                                     QDir::rootPath(),
                                                     "XML file (*.xml)");
        if (filename.isEmpty())
            return;

        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, "Error", file.errorString());
            return;
        }

        ui -> mainTable -> setRowCount(0);

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

        saveXML();

        copyImportedImages();
    }
    else
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, "Error", file.errorString());
            return;
        }

        ui -> mainTable -> setRowCount(0);

        QDomDocument doc;
        doc.setContent(&file);
        file.close();

        auto root = doc.firstChild().toElement();
        auto ix = root.firstChild().toElement();

        int id;
        QString coverPath, name, studio, genre, releaseDate;

        while(!ix.isNull())
        {
            id = ix.attribute("id").toInt();
            coverPath = ix.firstChild().toElement().text();
            name = ix.firstChild().nextSibling().toElement().text();
            studio = ix.firstChild().nextSibling().nextSibling().toElement().text();
            genre = ix.firstChild().nextSibling().nextSibling().nextSibling().toElement().text();
            releaseDate = ix.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().toElement().text();

            addGameToTable(Game(id, coverPath, name, studio, genre, releaseDate));

            ix = ix.nextSibling().toElement();
        }
    }

    loadImages();
}

void MainWindow::saveXML()
{
    QDomDocument doc;

    doc = xmlSetup(doc);

    QFile file (filePath);
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

QDomDocument MainWindow::xmlSetup(QDomDocument doc)
{
    auto root = doc.createElement("Games");
    doc.appendChild(root);

    for (int i = 0; i < ui -> mainTable -> rowCount(); i++)
    {
        Game game(i,
                  ui -> mainTable -> item(i, COVERPATH) -> text(),
                  ui -> mainTable -> item(i, NAME) -> text(),
                  ui -> mainTable -> item(i, STUDIO) -> text(),
                  ui -> mainTable -> item(i, GENRE) -> text(),
                  ui -> mainTable -> item(i, RELEASEDATE) -> text()
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

    return doc;
}

void MainWindow::saveImages()
{
    QString saveDir = QCoreApplication::applicationDirPath() + "/img/";

    QDir dir(saveDir);
    if (!dir.exists())
        dir.mkpath(".");

    QString unsavedImgPath, savedImgPath;

    for (int i = 0; i < ui -> mainTable -> rowCount(); i++)
    {
        unsavedImgPath = ui -> mainTable -> item(i, COVERPATH) -> text();

        QPixmap pix(unsavedImgPath);
        pix.save(saveDir + "img_" + QString::number(i) + ".bmp");

        savedImgPath = saveDir + "img_" + QString::number(i) + ".bmp";
        ui -> mainTable -> item(i, COVERPATH) -> setText(savedImgPath);
    }
}

void MainWindow::copyImportedImages()
{
    QString saveDirPath = QCoreApplication::applicationDirPath() + "/img/";
    QString importDirPath = QFileDialog::getExistingDirectory(this,
                                                              tr("Open Directory"),
                                                              "/home",
                                                              QFileDialog::ShowDirsOnly |
                                                              QFileDialog::DontResolveSymlinks);
    QDir importDir(importDirPath + "/");

    QDir saveDir(saveDirPath);
    if (!saveDir.exists())
        saveDir.mkpath(".");

    QString unsavedImgPath, savedImgPath;

    for (unsigned int i = 0; i < importDir.count() - 2; i++)
    {
        unsavedImgPath = importDirPath + "/" + "img_" + QString::number(i) + ".bmp";
        savedImgPath = saveDirPath + "img_" + QString::number(i) + ".bmp";

        QPixmap pix(unsavedImgPath);
        pix.save(savedImgPath);
    }

    saveImages();
}

void MainWindow::loadImages()
{
    QString imgPath;

    for (int i = 0; i < ui -> mainTable -> rowCount(); i++)
    {
        imgPath = ui -> mainTable -> item(i, COVERPATH) -> text();

        if (imgPath.isEmpty())
            continue;

        QImage *img = new QImage(imgPath);
        if (img == NULL)
        {
            QMessageBox::critical(this, "Error", "Out of memory!");
            return;
        }

        QTableWidgetItem *thumbnail = new QTableWidgetItem;
        if (thumbnail == NULL)
        {
            QMessageBox::critical(this, "Error", "Out of memory!");
            return;
        }

        thumbnail -> setData(Qt::DecorationRole, QPixmap::fromImage(*img).scaledToHeight(150));

        ui -> mainTable -> setItem(i, COVER, new QTableWidgetItem(*thumbnail));

        delete img;
        delete thumbnail;
    }
}

void MainWindow::searchTable()
{
    if (ui -> searchLE -> text().isEmpty())
    {
        openXML(false);
        return;
    }

    openXML(false);

    bool found = false;
    QString searchText = ui -> searchLE -> text();
    int rowCount = ui -> mainTable -> rowCount() - 1;

    for (int row = rowCount; row >= 0; row--)
    {
        for (int col = 1; col < ui -> mainTable -> columnCount() - 1; col++)
        {
            if (ui -> mainTable -> item(row, col) -> text().contains(searchText, Qt::CaseInsensitive))
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            found = false;
            continue;
        }
        else
        {
            ui -> mainTable -> removeRow(row);
        }
    }
}
