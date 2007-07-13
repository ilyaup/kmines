/*
    Copyright 2007 Dmitry Suzdalev <dimsuz@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>

class MineFieldItem;

/**
 * Graphics scene for KMines game
 */
class KMinesScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * Constructs scene
     */
    explicit KMinesScene( QObject* parent );
    /**
     * Resizes scene to given dimensions
     */
    void resizeScene(int width, int height);
private:
    /**
     * Draws scene background
     * Reimplemented from QGraphicsScene
     */
    virtual void drawBackground( QPainter*, const QRectF& );
    /**
     * Game field graphics item
     */
    MineFieldItem* m_fieldItem;
};

class QResizeEvent;

class KMinesView : public QGraphicsView
{
public:
    KMinesView( KMinesScene* scene, QWidget *parent );
private:
    virtual void resizeEvent( QResizeEvent *ev );

    KMinesScene* m_scene;
};
#endif
