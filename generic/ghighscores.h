/*
    This file is part of the KDE games library
    Copyright (C) 2001-02 Nicolas Hadacek (hadacek@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef G_HIGHSCORES_H
#define G_HIGHSCORES_H

#include <qvaluelist.h>
#include <qwidget.h>

#include <kurl.h>

#include "ghighscores_item.h"


class QWidget;
class QTabWidget;

namespace KExtHighscores
{

/**
 * Get the current game type.
 */
uint gameType();

/**
 * Set the current game type.
 */
void setGameType(uint gameType);

/**
 * This abstrat class is the base class of the highscores
 * configuration widget (@see createConfigWidget).
 */
class ConfigWidget : public QWidget
{
 Q_OBJECT
 public:
    ConfigWidget(QWidget *parent) : QWidget(parent) {}

    /**
     * Load the settings.
     */
    virtual void load() = 0;

    /**
     * Save the settings.
     */
    virtual bool save() = 0;

    /**
     * @return the title.
     */
    virtual QString title() const = 0;

    /**
     * @return the icon.
     */
    virtual QString icon() const = 0;

 signals:
    void modified();
};

/**
 * @return a @ref ConfigWidget for configuration of the highscores.
 */
ConfigWidget *createConfigWidget(QWidget *parent);

/**
 * Show the highscores lists.
 */
void showHighscores(QWidget *parent);

/**
 * Show scores for a multiplayers game.
 *
 * Usage example:
 * <pre>
 * QValueList<KExtHighscores::Score> scores(2);
 * scores[0].setType(KExtHighscores::Won);
 * scores[0].setData("score", score1);
 * scores[0].setData("name", player1);
 * scores[1].setType(KExtHighscores::Lost);
 * scores[1].setData("score", score2);
 * scores[1].setData("name", player2);
 * KExtHighscores::showMultipleScores(scores, widget);
 * </pre>
 */
void showMultipleScores(const QValueList<Score> &scores,
                        QWidget *parent);

/**
 * Submit the score generated by @ref newScore.
 * See @ref Highscores for usage example.
 *
 * @param widget a widget used as parent for error message box.
 */
void submitScore(const Score &score, QWidget *widget);

/**
 * @return the last score in the local list of highscores (the worst
 * score if the number of entries is less than the maximum).
 */
Score lastScore();

/**
 * @return the first score in the local list of highscores (the worst possible
 * score if there is no entry).
 */
Score firstScore();

/**
 * This class manages highscores and players entries (several players can
 * share the same highscore list if the libkdegames library is built to
 * support a common highscores file).
 *
 * You need one instance of this class during the application lifetime ; in
 * main() just insert <pre> KExtHighscores::Highscores highscores; </pre>
 * with the needed arguments.
 * Replace <pre>Highscores</pre> by the derived class if you need to
 * reimplement some of the default methods.
 *
 * This class has three functions :
 * <ul>
 * <li> Update the highscores list when new entries are submitted </li>
 * <li> Display the highscores list and the players list </li>
 * <li> Send query to an optionnal web server to support world-wide
 *      highscores </li>
 * </ul>
 *
 * The highscores and the players lists contain several items (represented by
 * @ref Item).
 *
 * The highscores list contains by default :
 * <ul>
 * <li> "id" : the index of the player (internal and not shown) </li>
 * <li> "name" : the player name (automatically set) </li>
 * <li> "score" : the score value </li>
 * <li> "date" : the time and date of the highscore (automatically set) </li>
 * </ul>
 * You can add an item or replace the default item for the score value
 * (for e.g. displaying it differently) by calling @ref setItem in a
 * reimplementation of the constructor.
 *
 * The players list contains :
 * <ul>
 * <li> "name" : the player name (as defined by the user in the configuration
 *      dialog) </li>
 * <li> "nb game" : the number of games </li>
 * <li> "success" : the number of successes (only if @p trackLostGame is set
 *      in the constructor) </li>
 * <li> "mean score" : the mean score </li>
 * <li> "best score" : the best score </li>
 * <li> "black mark" : the number of black marks (only if @p trackBlackMark is
 *      set in the constructor) ; black marks should be submitted if the user
 *      aborts a game and you consider this practice is somehow cheating the
 *      statistics </li>
 * <li> "date" : the best score time and date </li>
 * <li> "comment" : the player comment (as defined by the user in the
 *      configuration dialog) </li>
 * </ul>
 * You can replace the best score and the mean score item with @ref setItem.
 *
 * To submit a new score at game end, just construct a @ref Score, set the
 * score data and then call @ref submitScore.
 * <pre>
 *     KExtHighscores::Score score(KExtHighscores::Won);
 *     score.setData("score", myScore);
 *     KExtHighscores::submitScore(score, widget); // delete score
 * </pre>
 * You can only set the score value ("name" and "date" are set automatically)
 * and the value of the items you have (optionnally) added with @ref setItem.
 */
class Highscores
{
 public:
    /**
     * Constructor
     *
     * @param version the game version which is sent to the web server (it can
     *        be useful for backward compatibility on the server side).
     * @param baseURL the web server url (an empty url means that world-wide
     *        highscores are not available)
     * @param nbGameTypes the number of different game types (usually one) ;
     *        for e.g. KMines has easy, normal and expert levels.
     * @param maxNbEntries the maximum numbers of highscores entries (by game
     *        types)
     */
    Highscores(const QString &version = QString::null,
               const KURL &baseURL = KURL(),
               uint nbGameTypes = 1, uint maxNbEntries = 10);
    virtual ~Highscores();

    /**
     * Set if the number of lost games should be track for the world-wide
     * highscores statistics. By default, there is no tracking.
     *
     * Note: should be called at construction time.
     */
    void setTrackLostGames(bool track);

    /**
     * Set if the number of black marks should be tracked for the world-wide
     * highscores statistics. By default, there is no tracking.
     *
     * Note: should be called at construction time.
     */
    void setTrackBlackMarks(bool track);

    /**
     * Set if the statistics tab should be shown in the highscores dialog.
     * You only want to show this tab if it makes sense to lose or to win the
     * game (for e.g. it makes no sense for a tetris game but it does for a
     * minesweeper game).
     *
     * Note: should be called at construction time.
     */
    void showStatistics(bool show);

    /**
     * Set the ranges for the score histogram.
     *
     * Note: should be called at construction time.
     */
    void setScoreHistogram(const QMemArray<uint> &scores, bool bound,
                           bool showMaxPixmap = true);

    /**
     * @return true is the first score is strictly worse than the second one.
     * By default return s1.score()<s2.score(). You can reimplement
     * this method if additionnal items added to @ref Score can further
     * differentiate the scores (for e.g. the time spent).
     *
     * Note that you do not need to use directly this method, simply write
     * <pre>s1<s2</pre> since @ref Score::operator< calls this method.
     */
    virtual bool isStrictlyLess(const Score &s1, const Score &s2) const;

    /**
     * Possible type of label (@see gameTypeLabel).
     * @p Standard label used in config file.
     * @p I18N label used to display the game type.
     * @p WW label used when contacting the world-wide highscores server.
     * @p Icon label used to load the icon corresponding to the game type.
     */
    enum LabelType { Standard, I18N, WW, Icon };

    /**
     * @return the label corresponding to the game type. The default
     * implementation works only for one game type : you need to reimplement
     * this method if the number of game types is more than one.
     */
    virtual QString gameTypeLabel(uint gameType, LabelType type) const;

    /**
     * Add an item to the score. It will add a column to the highscores list.
     * This method should be called at construction time.
     *
     * If @p name is "score", "best score" or "mean score", the default item
     * will be replaced by the given one.
     */
    void setItem(const QString &name, Item *item);

    /**
     * This method is called once for each player (ie for each user). You
     * can reimplement it to convert old style highscores to the new mechanism
     * (@see submitLegacyScore). By default this method does nothing.
     *
     * @param gameType the game type
     */
    virtual void convertLegacy(uint gameType) { Q_UNUSED(gameType); }

    /**
     * This method should be called from @ref convertLegacy. It is used
     * to submit an old highscore (it will not be send over the network).
     * For each score do something like:
     * <pre>
     * KExtHighscores::Score score(KExtHighscores::Won);
     * score.setData("score", oldScore);
     * score.setData("name", name);
     * KExtHighscores::submitLegacyScore(score);
     * </pre>
     * Note that here you can set the player "name" and the highscore "date"
     * if they are known.
     */
    void submitLegacyScore(const Score &score) const;

    /**
     * This method is called before submitting a score to the world-wide
     * highscores server. You can reimplement this method to add an entry
     * with @ref addToQueryURL. By default this method does nothing.
     *
     * @param score the score to be submitted.
     */
    virtual void additionnalQueryItems(KURL &url, const Score &score) const
        { Q_UNUSED(url); Q_UNUSED(score); }

    /**
     * Add an entry to the url to be submitted (@see additionnalQueryItems).
     *
     * @param item the item name
     * @param content the item content
     */
    static void addToQueryURL(KURL &url, const QString &item,
                              const QString &content);

    /**
     * Called when a score has been submitted. By default, this method
     * does nothing. It's there for future extensions.
     */
    virtual void scoreSubmitted(const Score &score) { Q_UNUSED(score); }

    /**
     * Called before the highscores are shown. By default it does nothing.
     * It's there for future extensions.
     */
    virtual void additionnalTabs(QTabWidget *widget) { Q_UNUSED(widget); }

 private:
    Highscores(const Highscores &);
    Highscores &operator =(const Highscores &);
};

}; // namespace

#endif
