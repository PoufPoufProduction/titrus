/*
Copyright 2011 JohannC

This file is part of TITRUS.

TITRUS is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

TITRUS is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
TITRUS.  If not, see http://www.gnu.org/licenses/
*/

#include <splashouille/Defines.hpp>
#include <splashouille/Engine.hpp>
#include <splashouille/Library.hpp>
#include <splashouille/Style.hpp>
#include <splashouille/Fashion.hpp>
#include <splashouille/Crowd.hpp>

#include <titrus/Main.hpp>

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

using namespace titrus;


const int Game::blocks[] =   {  21, 11, 12, 13, 11, 12, 22, 32, 12, 22, 21, 20, 22, 21, 11, 1,
                                11, 12, 21, 22, 11, 12, 21, 22, 11, 12, 21, 22, 11, 12, 21, 22,
                                11, 12, 22, 23, 21, 11, 12, 2,  11, 12, 22, 23, 21, 11, 12, 2,
                                21, 22, 12, 13, 22, 12, 11, 1,  21, 22, 12, 13, 22, 12, 11, 1,
                                21, 22, 23, 12, 2,  12, 22, 13, 11, 12, 13, 22, 2,  12, 22, 11,
                                10, 11, 12, 13, 2,  12, 22, 32, 10, 11, 12, 13, 1,  11, 21, 31,
                                10, 11, 12, 22, 2,  12, 22, 21, 23, 22, 21, 11, 31, 21, 11, 12
                              };

const std::string Game::bgs[]   = { "t_bg00", "t_bg01", "t_bg02", "t_bg05" };
#define nbBackgrounds  4

/**
 * Initialize the game handler
 * @return true if every objects are found
 */
bool Game::init(splashouille::Engine * _engine)
{
    char    msg[10];
    // GET THE INTERACTIVE OBJECT
    o_score[0]  = _engine->getLibrary()->getObjectById("score0");
    o_score[1]  = _engine->getLibrary()->getObjectById("score1");
    o_score[2]  = _engine->getLibrary()->getObjectById("score2");
    o_score[3]  = _engine->getLibrary()->getObjectById("score3");

    o_lines[0]  = _engine->getLibrary()->getObjectById("lines0");
    o_lines[1]  = _engine->getLibrary()->getObjectById("lines1");
    o_lines[2]  = _engine->getLibrary()->getObjectById("lines2");

    o_popup[0]  = _engine->getLibrary()->getObjectById("popup2");
    o_popup[1]  = _engine->getLibrary()->getObjectById("popup3");
    o_popup[2]  = _engine->getLibrary()->getObjectById("popup4");

    o_land      = _engine->getLibrary()->getObjectById("_land");

    a_grid      = _engine->getLibrary()->getAnimationById("grid");
    a_effects   = _engine->getLibrary()->getAnimationById("effects");
    a_preview   = _engine->getLibrary()->getAnimationById("preview");
    a_bg        = _engine->getLibrary()->getAnimationById("gridbg");
    a_cinematic = _engine->getLibrary()->getAnimationById("cinematic");

    for (int i=0; i<10; i++) for (int j=0; j<18; j++)
    {
        o_tiles[i][j] = 0;

        snprintf(msg, 10, "b%03d", (j*10)+i);
        o_borders[i][j] = _engine->getLibrary()->getObjectById(msg);
    }
    for (int i=0; i<4; i++)                             { o_active[i] = 0; o_preview[i] = 0; }

    return true;
}

/**
 * Update the grid borders
 */
void Game::updateBorders()
{
    const int       n[] = { -1, 0, 0, -1, 1, 0, 0, 1, -1, 1, -1, -1, 1, -1, 1, 1 };
    int             nb;
    int             nx, ny;
    int             v;
    for (int i=0; i<10; i++) for (int j=0; j<18; j++) if (o_tiles[i][j]) { o_borders[i][j]->setTileIndex(0); } else
    {
        // LOOK FOR THE ADJACENT NEIGHBOORS
        nb = v = 0;
        for (int k=0; k<4; k++)
        {
            nx = i + n[k*2]; ny = j + n[k*2+1];
            if ( (nx<0)||(nx>=10)||(ny<0)||(ny>=18)||(o_tiles[nx][ny])) { nb++; v|=(1<<k); }
        }

        // LOOK FOR THE DIAGONAL NEIGHBOORS
        if (nb==0)
        {
            v = 0;
            for (int k =0; k<4; k++)
            {
                nx = i+n[k*2+8]; ny = j+n[k*2+9];
                if ( (nx>=0)&&(nx<10)&&(ny>=0)&&(ny<18)&&(o_tiles[nx][ny])) { v|=(1<<k); }
            }
            v +=16;
        }
        else
        {
        // TO DO
        }

        o_borders[i][j]->setTileIndex(v);
    }
}


/**
 * Update the background
 */
void Game::updateBg(int _lastNbLines, int _newNbLines, bool _gameType, int _speed)
{
    if (_gameType)
    {
        if (_newNbLines+_lastNbLines==0) { a_bg->changeTimeline(bgs[_speed%nbBackgrounds]); }
    }
    else
    {
        if (_newNbLines/10!=_lastNbLines/10 || _newNbLines+_lastNbLines==0)
            { a_bg->changeTimeline(bgs[(_newNbLines/10)%nbBackgrounds]); }
    }

}

/**
 * Update the score and the number of lines values
 * @param _score is the current score
 * @param _lines is the number of lines
 */
void Game::updateScoreNLines(int _score, int _lines)
{
    // THE SCORE
    for (int i=0; i<4; i++)
    {
        if (_score || !i)
        {
            o_score[3-i]->getStyle()->setDisplay(true);
            o_score[3-i]->getStyle()->setPosition(32*(_score%10), 0);
            _score=_score/10;
        }
        else
        {
            o_score[3-i]->getStyle()->setDisplay(false);
        }
    }

    // THE NUMBER OF LINES
    for (int i=0; i<3; i++)
    {
        if (_lines || !i)
        {
            o_lines[2-i]->getStyle()->setDisplay(true);
            o_lines[2-i]->getStyle()->setPosition(32*(_lines%10), 0);
            _lines=_lines/10;
        }
        else
        {
            o_lines[2-i]->getStyle()->setDisplay(false);
        }
    }
}

/**
 * Rotate the current falling o_tiles
 * @param _clock is true for clockwise
 */
void Game::rotate(bool _clock)
{
    if (o_active[0])
    {
        bool    ret         = true;
        int     down        = 0;
        int     left        = 0;
        int     tileId      = order[orderId-2]-'A';
        int     newRotation = (rotation+(_clock?1:3))%4;

        // TRY THE ROTATION
        for (int i=0; i<4; i++)
        {
            int x = pos[0]   + (blocks[tileId*16+newRotation*4+i]%10);
            int y = pos[1]-1 + (blocks[tileId*16+newRotation*4+i]/10);
            if (o_tiles[x][y] || (y>17)) { ret = false; }
            if (x<0) { left = 1; }
            if (x>9) { left = -1; }
            if (y<0) { down = 1; }
        }

        // TRY THE ROTATION WITH A MOVE DOWN
        if (down || left)
        {
            ret = true;
            for (int i=0; i<4; i++)
            {
                int x = left + pos[0] + (blocks[tileId*16+newRotation*4+i]%10);
                int y = down + pos[1]-1 + (blocks[tileId*16+newRotation*4+i]/10);
                if (o_tiles[x][y] || (x<0) || (x>9) || (y>17) || (y<0)) { ret = false; }
            }
        }

        // PERFORM THE ROTATION
        if (ret)
        {
            rotation = newRotation;
            for (int i=0; i<4; i++)
            {
                o_active[i]->getStyle()->setRelativeLeft(32*((blocks[tileId*16+newRotation*4+i]%10)+left));
                o_active[i]->getStyle()->setRelativeTop(32*((blocks[tileId*16+newRotation*4+i]/10)+down));
            }
        }
    }
}

/**
 * Move the current falling o_tiles
 * @param _x is the x-axis movement value
 * @param _y is the y-axis movement value
 * @param _force is true for updating the o_tiles anyway (used for the first appearance)
 * @return true if the move is good
 */
bool Game::move(int _x, int _y, bool _force)
{
    bool ret = true;

    if (o_active[0])
    {
        // CHECK IF THE MOVE IF AVAILABLE
        for (int i=0; i<4; i++)
        {
            int x = pos[0]+_x+(o_active[i]->getStyle()->getRelativeLeft()/32);
            int y = pos[1]-1+_y+(o_active[i]->getStyle()->getRelativeTop()/32);
            if (o_tiles[x][y] || (x<0) || (x>9) || (y<0) || (y>17)) { ret = false; }
        }

        // PERFORM MOVE
        if (ret || _force)
        {
            pos[0] += _x;
            pos[1] += _y;

            for (int i=0; i<4; i++)
            {
                o_active[i]->getStyle()->setLeft(32*pos[0]);
                o_active[i]->getStyle()->setTop(32*(pos[1]-1));
            }
        }
    }

    return ret;
}

/**
 * Check the lines to delete
 * @return the number of lines to delete
 */
int Game::checkLinesToDelete()
{
    int nbLines = 0;

    for (int i=0; i<4; i++) { linesToDelete[i] = -1; }
    for (int j=0; j<18; j++)
    {
        bool isLineFull = true;
        for (int i=0; i<10; i++) { isLineFull&=(o_tiles[i][j]!=0); }
        if (isLineFull) { linesToDelete[nbLines++] = j; }
    }

    return nbLines;
}

void Game::onLevel7(int _frame UNUSED, int _timeStampInMilliSeconds UNUSED)
{
    updateScoreNLines(score, lines);
    a_effects->changeTimeline("intro");
}

void Game::onLevel9(int _frame UNUSED, int _timeStampInMilliSeconds)
{
    checkGrid();

    // REMOVE THE DELETED o_tiles
    for (int j=0; j<4; j++)
    {
        if (linesToDelete[j]>=0)
        {
            for (int i=0; i<10; i++)
            {
                splashouille::Object * obj = o_tiles[i][linesToDelete[j]];
                a_grid->getCrowd()->dropObject(obj->getId());
                titrus->getEngine()->getLibrary()->deleteObject(obj->getId());
                o_tiles[i][linesToDelete[j]] = 0;
            }
        }
    }
    for (int i=0; i<4; i++) { o_active[i] = 0; }

    // DOWN THE UPPER o_tiles
    for (int j=17; j>=0; j--)
    {
        int idDelete = 0;
        for (int i=0; i<4; i++) { if (j<linesToDelete[i] && linesToDelete[i]>=0) { idDelete++; } }

        if (idDelete)
        {
            for (int i=0; i<10; i++)
            {
                if (o_tiles[i][j])
                {
                    int now=_timeStampInMilliSeconds
                            - o_tiles[i][j]->getInitialTimestamp() - titrus->getEngine()->getInitialTimestamp();
                    splashouille::Style * s = o_tiles[i][j]->getFashion()->addTransition(now, now+250, -5, 1);
                    s->setTop((j+idDelete)*32);

                    o_tiles[i][j+idDelete] = o_tiles[i][j];
                    o_tiles[i][j]=0;
                }
            }
        }
    }

    for (int j=0; j<4; j++) { linesToDelete[j] = -1; }

    checkGrid();
    updateBorders();

    // IS TYPE-B GAME FINISHED ?
    if (titrus->getGameType() && !lines)    { finish(); }
    else                                    { a_effects->changeTimeline("wait"); }
}

void Game::checkGrid()
{
    for (int j=17; j>=0; j--)
    {
        for (int i=0; i<10; i++)
        {
            if (o_tiles[i][j])
            {
                if ((o_tiles[i][j]->getStyle()->getLeft()/32 != i) &&
                    (o_tiles[i][j]->getStyle()->getTop()/32 !=j))
                {
                    std::cout<<"ERROR ("<<i<<","<<j<<") ("<<o_tiles[i][j]->getId()<<") ["<<tileIndex<<"]"<<std::endl;
                }
            }
        }
    }

}

void Game::stopTile(int _timeStampInMilliSeconds)
{
    // MAKE THE CURRENT TILE STATIC
    for (int i=0; i<4; i++)
    {
        int x = pos[0]+(o_active[i]->getStyle()->getRelativeLeft()/32);
        int y = pos[1]-1+(o_active[i]->getStyle()->getRelativeTop()/32);
        o_tiles[x][y] = o_active[i];
        o_active[i]->getStyle()->setRelativeLeft(0);
        o_active[i]->getStyle()->setRelativeTop(0);
        o_active[i]->getStyle()->setLeft(x*32);
        o_active[i]->getStyle()->setTop(y*32);
        o_active[i]->changeFashion("static");
    }
    checkGrid();

    // CHUNK
    a_effects->getCrowd()->insertObject(0, o_land);

    // ARE SOME LINES FULL ?
    int nbLines = checkLinesToDelete();
    if (nbLines)
    {

        // CHANGE THE FASHION OF THE DELETED LINES o_tiles
        for (int j=0; j<nbLines; j++)
        {
            if (linesToDelete[j]>=0)
            {
                for (int i=0; i<10; i++)
                {
                    o_tiles[i][linesToDelete[j]]->changeFashion("delete");
                }
            }
        }

        // MOVE TO A TRANSITION TIMELINE (DEPENDING ON THE NUMBER OF LINES)
        int h = linesToDelete[0]*32;
        if (h>500) { h = 500; }
        switch(nbLines)
        {
            case 1: a_effects->changeTimeline("line1"); break;
            case 2: o_popup[0]->getStyle()->setTop(h); a_effects->changeTimeline("line2"); break;
            case 3: o_popup[1]->getStyle()->setTop(h); a_effects->changeTimeline("line3"); break;
            case 4: o_popup[2]->getStyle()->setTop(h); a_effects->changeTimeline("line4"); break;
            default: break;
        }

        // UPDATE SCORE AND LINES
        updateBg(lines, lines+nbLines, titrus->getGameType(), currentSpeed);
        int p = 1;
        for (int i=1; i<nbLines; i++) { p = p*2; }
        if (titrus->getGameType()) { p = p*(titrus->getHigh()+1); }
        score += (1+(currentSpeed/2))*p*nbLines;
        if (titrus->getGameType()) { lines -= nbLines; } else { lines +=nbLines; }
        if (lines<0) { lines = 0; }
        updateScoreNLines(score, lines);

        // UPDATE THE SPEED
        if (!titrus->getGameType() && lines/10>currentSpeed)
        {
            currentSpeed++;
        }
    }
    else
    {
        next(_timeStampInMilliSeconds);
    }
}

void Game::finish()
{
    a_bg->changeTimeline("outro");

    if (!titrus->getGameType())
    {
        if (lines>150)  { a_cinematic->changeTimeline("level2"); } else
        if (lines>100)  { a_cinematic->changeTimeline("level1"); } else
                        { a_effects->changeTimeline("gameover"); }
    }
    else
    {
        if (lines)      { a_effects->changeTimeline("gameover"); } else
        if (titrus->getHigh()*2 + currentSpeed == 19)   { a_cinematic->changeTimeline("level3"); } else
        if (titrus->getHigh()*2 + currentSpeed > 15)    { a_cinematic->changeTimeline("level2"); } else
        if (titrus->getHigh()*2 + currentSpeed > 10)    { a_cinematic->changeTimeline("level1"); } else
                        { a_effects->changeTimeline("gameover2"); }
    }
}

void Game::close() { a_effects->changeTimeline("outro"); }

void Game::onLevel12(int _frame UNUSED, int _timeStampInMilliSeconds UNUSED)
{
    if ( (lines&&titrus->getGameType()) || (!titrus->getScore().openBest(score)) ){ close(); }
}

void Game::onLevel8(int _frame UNUSED, int _timeStampInMilliSeconds)
{
    if (!o_active[0])
    {
        // NEXT TILE
        next(_timeStampInMilliSeconds);
    }
    else
    if (nextDown<_timeStampInMilliSeconds)
    {
        // MOVE THE CURRENT TILE DOWN
        if (move(0, 1))
        {
            nextDown = _timeStampInMilliSeconds + 1000/(currentSpeed+1);
        }
        else
        {
            stopTile(_timeStampInMilliSeconds);
        }
    }
    else
    {
        // MOVE THE TILE REGARDING THE KEY INPUT
        Uint8 *keystates = titrus->getKeyboard().SDL_GetKeyState( NULL );

        if( keystates[ SDLK_UP ] )                  { rotate(); }
        if( keystates[ 'x' ] || keystates[ 'X' ] )  { rotate(false); }
        if( keystates[ 'c' ] || keystates[ 'C' ] )  { rotate(true); }
        if( keystates[ SDLK_LEFT ] )                { move(-1,0); }
        if( keystates[ SDLK_RIGHT ] )               { move(1,0); }
        if( keystates[ SDLK_DOWN ] )
        {
            if (move(0, 1))
            {
                nextDown = _timeStampInMilliSeconds + 1000/(currentSpeed+1);
            }
            else
            {
                stopTile(_timeStampInMilliSeconds);
            }
        }
    }
}

void Game::next(int _timeStampInMilliSeconds)
{
    bool isFinished = false;

    // Update the borders
    updateBorders();

    // Copy the preview to the grid as the current o_active
    if (o_preview[0])
    {
        a_preview->getCrowd()->clear();
        for (int i=0; i<4; i++)
        {
            o_active[i] = o_preview[i];
            o_preview[i] = 0;
            o_active[i]->changeFashion("game");
            a_grid->getCrowd()->insertObject(0, o_active[i]);
        }
        pos[0] = 3;
        pos[1] = 0;
        rotation = 0;
        nextDown = _timeStampInMilliSeconds + 500/(sqrt(currentSpeed+1)) + 1000/(currentSpeed+1);
        isFinished = !move(0,0, true);
    }

    // Add a new preview
    if (!o_preview[0])
    {
        int newTileId = order[orderId]-'A';
        orderId=(orderId+1)%LG_MAX_ORDER;
        for (int i=0; i<4; i++)
        {
            char tileId[128];
            snprintf(tileId, 128, "tile%d", tileIndex++);
            char tileName[128];
            snprintf(tileName, 128, "_tile%d", (newTileId+1));

            o_preview[i] = titrus->getEngine()->getLibrary()->copyObject(tileName, tileId);
            o_preview[i]->getStyle()->setRelativeLeft(32*(blocks[i+newTileId*16]%10));
            o_preview[i]->getStyle()->setRelativeTop(32*(blocks[i+newTileId*16]/10));
            o_preview[i]->changeFashion("preview");

            if (newTileId!=5)
            {
                o_preview[i]->getStyle()->setTop(-16);
                if (newTileId==6)         { o_preview[i]->getStyle()->setLeft(16); }
                else if (newTileId!=1)    { o_preview[i]->getStyle()->setLeft(-16); }
            }

            a_preview->getCrowd()->insertObject(0, o_preview[i]);
        }
    }

    // Is it the end ?
    if (isFinished) { finish(); }

}

void Game::init()
{
    // CLEAR THE PREVIOUS GAME
    a_preview->getCrowd()->clear();
    a_grid->getCrowd()->clear();
    for (int i=0; i<4; i++)
    {
        if (o_active[i])    { titrus->getEngine()->getLibrary()->deleteObject(o_active[i]->getId()); }
        if (o_preview[i])   { titrus->getEngine()->getLibrary()->deleteObject(o_preview[i]->getId()); }
    }
    for (int i=0; i<10; i++) for (int j=0; j<18; j++)
    {
        if (o_tiles[i][j])  { titrus->getEngine()->getLibrary()->deleteObject(o_tiles[i][j]->getId()); }
    }

    // INITIALISATION OF THE SCORE AND LINES
    score = 0;
    lines = titrus->getGameType()?25:0;
    currentSpeed = titrus->getSpeed();

    // INITIALISATION OF THE 10X18 o_tiles GRID
    srand(time(NULL));
    tileIndex = 0;
    for (int i=0; i<10; i++) for (int j=0; j<18; j++)
    {
        o_tiles[i][j] = 0;
        if (titrus->getGameType() && ((17-j)<titrus->getHigh()*2) && (rand()%2))
        {
            char tileId[128];
            snprintf(tileId, 128, "tile%d", tileIndex++);
            char tileName[32];
            snprintf(tileName, 32, "_tile%d", (rand()%7)+1);
            splashouille::Object * object = titrus->getEngine()->getLibrary()->copyObject(tileName, tileId);
            object->getStyle()->setTop(j*32);
            object->getStyle()->setLeft(i*32);
            object->changeFashion("static");
            a_grid->getCrowd()->insertObject(0, object);
            o_tiles[i][j] = object;
        }
    }

    // Initialisation of the blocks order
    for (int i=0; i<LG_MAX_ORDER; i++)    { order[i] = 'A'+rand()%7; }
    for (int i=0; i<4; i++)               { o_active[i] = 0; o_preview[i] = 0; }
    orderId = 0;

    updateBg(0, 0, titrus->getGameType(), currentSpeed);
    next();
}

