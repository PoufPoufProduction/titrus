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
#include <splashouille/Image.hpp>

#include <titrus/Main.hpp>

#include <libconfig.h++>

using namespace titrus;

const int Score::ascii[] = {   'A','B','C','D','E','F','G','H','I','J','K','L','M',
                                'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

/**
 * Initialize the scores handler
 * @return true if every objects are found
 */
bool Score::init(splashouille::Engine * _engine)
{
    a_overview  = _engine->getLibrary()->getAnimationById("a_menu_best");
    a_best      = _engine->getLibrary()->getAnimationById("gamescore");
    return true;
}

/**
 * Update the best scores overview
 * @param _root is the root of the settings configuration file
 */
void Score::update(libconfig::Setting & _root)
{
    // UPDATE THE 10 BEST SCORES (5 FOR TYPE-A + 5 FOR TYPE-B)
    for (int i=0; i<10; i++)
    {
        libconfig::Setting &    setting = _root["score"][i];
        std::string             player = setting["player"];
        int                     score = setting["score"];

        // UPDATE THE PLAYER NAME
        for (int j=0; j<3; j++)
        {
            char vId[32];
            snprintf(vId, 32, "player%d%d", i, j);
            splashouille::Image* l = titrus->getEngine()->getLibrary()->getImageById(vId);
            if (l) { l->setTileIndex(player[j]); }
        }

        // UPDATE THE SCORE VALUE
        for (int j=3; j>=0; j--)
        {
            char vId[32];
            snprintf(vId, 32, "score%d%d", i, j);
            splashouille::Image* l = titrus->getEngine()->getLibrary()->getImageById(vId);
            if (l) { l->setTileIndex(score?('0'+score%10):' '); }
            score = score/10;
        }
    }
}

void Score::updateName(int _position, const int * _name)
{
    for (int i=0; i<3; i++)
    {
        char vId[32];
        snprintf(vId, 32, "player%d", i);
        splashouille::Image* l = titrus->getEngine()->getLibrary()->getImageById(vId);
        if (l) { l->setTileIndex( (_position==i)?_name[i]:ascii[_name[i]]); }
    }
}

/** Update the splashouille objects */
void Score::openOverview()  { a_overview->changeTimeline("t_level1ToBest"); }

/** Update the splashouille objects */
bool Score::openBest(int _score)
{
    int best = titrus->getSettings().getRoot()["score"][titrus->getGameType()?9:4]["score"];
    bool ret = (_score>best);

    if (ret)
    {
        a_best->changeTimeline("intro");
        position = 0;
        name[0] = name[1] = name[2] = 0;
        updateName(position, name);

        score = _score;
        for (int i=3; i>=0; i--)
        {
            char vId[32];
            snprintf(vId, 32, "best%d", i);
            splashouille::Image* l = titrus->getEngine()->getLibrary()->getImageById(vId);
            if (l) { l->setTileIndex(_score?('0'+_score%10):' '); }
            _score = _score/10;
        }
    }

    return ret;
}

/** Ask the best scoring player name */
void Score::onLevel11(int _frame UNUSED, int _timeStampInMilliSeconds UNUSED)
{
    Uint8 *keystates = titrus->getKeyboard().SDL_GetKeyState(NULL);

    if( keystates[ SDLK_UP ] )                  { name[position]= (name[position]+25)%26;}
    if( keystates[ 'x' ] || keystates[ 'X' ] )  { if (position>0) { position--; } }
    if( keystates[ 'c' ] || keystates[ 'C' ] )  { if (position<2) { position++; } }
    if( keystates[ SDLK_LEFT ] )                { if (position>0) { position--; } }
    if( keystates[ SDLK_RIGHT ] )               { if (position<2) { position++; } }
    if( keystates[ SDLK_DOWN ] )                { name[position]= (name[position]+1)%26;}

    if( keystates[' '])
    {
        position = 3;
        titrus->getGame().close();

        int i = titrus->getGameType()?9:4;
        while(i%5)
        {
            int up = titrus->getSettings().getRoot()["score"][i-1]["score"];
            if (up<score)
            {
                titrus->getSettings().getRoot()["score"][i]["score"] = (int)titrus->getSettings().getRoot()["score"][i-1]["score"];
                titrus->getSettings().getRoot()["score"][i]["player"] = (const std::string&) titrus->getSettings().getRoot()["score"][i-1]["player"];
            }
            else break;
            i--;
        };
        char nameStr[4];
        snprintf(nameStr, 4, "%c%c%c", ascii[name[0]], ascii[name[1]], ascii[name[2]]);

        titrus->getSettings().getRoot()["score"][i]["score"] = score;
        titrus->getSettings().getRoot()["score"][i]["player"] = nameStr;

        update(titrus->getSettings().getRoot());

    }

    updateName(position, name);
}

/** Scores overview event handling: just wait a key down for going back to the menu */
void Score::onLevel10(int _frame UNUSED, int _timeStampInMilliSeconds UNUSED)
{
    Uint8 *keystates = titrus->getKeyboard().SDL_GetKeyState(NULL);
    if( keystates[SDLK_ESCAPE] || keystates[' ']) { a_overview->changeTimeline("t_bestToLevel1"); }
}


