#include "game.h"

Game::Game(const QString &coverPath,
           const QString &name,
           const QString &studio,
           const QString &genre,
           const QString &releaseDate)
{
    mCoverPath = coverPath;
    mName = name;
    mStudio = studio;
    mGenre = genre;
    mReleaseDate = releaseDate;
}

const QString &Game::coverPath() const
{
    return mCoverPath;
}

void Game::setCoverPath(const QString &newCoverPath)
{
    mCoverPath = newCoverPath;
}

const QString &Game::name() const
{
    return mName;
}

void Game::setName(const QString &newName)
{
    mName = newName;
}

const QString &Game::studio() const
{
    return mStudio;
}

void Game::setStudio(const QString &newStudio)
{
    mStudio = newStudio;
}

const QString &Game::genre() const
{
    return mGenre;
}

void Game::setGenre(const QString &newGenre)
{
    mGenre = newGenre;
}

const QString &Game::releaseDate() const
{
    return mReleaseDate;
}

void Game::setReleaseDate(const QString &newReleaseDate)
{
    mReleaseDate = newReleaseDate;
}
