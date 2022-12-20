#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QDate>

class Game
{
public:
    Game(const QString &coverPath = "",
         const QString &name = "",
         const QString &studio = "",
         const QString &genre = "",
         const QString &releaseDate = "");

    const QString &coverPath() const;
    void setCoverPath(const QString &newCoverPath);

    const QString &name() const;
    void setName(const QString &newName);

    const QString &studio() const;
    void setStudio(const QString &newStudio);

    const QString &genre() const;
    void setGenre(const QString &newGenre);

    const QString &releaseDate() const;
    void setReleaseDate(const QString &newReleaseDate);

private:
    QString mCoverPath;
    QString mName;
    QString mStudio;
    QString mGenre;
    QString mReleaseDate;
};

#endif // GAME_H
