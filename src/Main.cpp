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

#include <titrus/Main.hpp>

#include <libconfig.h++>

#include <iostream>
#include <iomanip>
#include <getopt.h>


using namespace titrus;

/**
 * Init the state methods
 */
void Titrus::initStateMethods()
{
    for (int i=0; i<last; i++) { onFrameMethods[i] = 0; onEventMethods[i] = 0; }

    onFrameMethods[level1]      = &Titrus::onLevel1;
    onFrameMethods[level2]      = &Titrus::onLevel2;
    onFrameMethods[level3]      = &Titrus::onLevel3;
    onFrameMethods[level4]      = &Titrus::onLevel4;
    onFrameMethods[level5]      = &Titrus::onLevel5;
    onFrameMethods[level6]      = &Titrus::onLevel6;
    onFrameMethods[level7]      = &Titrus::onLevel7;
    onFrameMethods[level8]      = &Titrus::onLevel8;
    onFrameMethods[level9]      = &Titrus::onLevel9;
    onFrameMethods[level10]     = &Titrus::onLevel10;
    onFrameMethods[level11]     = &Titrus::onLevel11;
    onFrameMethods[level12]     = &Titrus::onLevel12;
    onFrameMethods[level13]     = &Titrus::onLevel13;
    onFrameMethods[level14]     = &Titrus::onLevel14;
}


void Titrus::onLevel6(int _frame UNUSED, int _t UNUSED)
{
    engine->stop();
}

/**
 * Forward the state related calls to the good handler
 * @param _event is the event
 * @param _t is the timestamp
 * @return true if consumed
 */

void Titrus::onLevel1(int _frame, int _t)               { menu.onLevel1(_frame, _t); }
void Titrus::onLevel2(int _frame, int _t)               { menu.onLevel2(_frame, _t); }
void Titrus::onLevel3(int _frame, int _t)               { menu.onLevel3(_frame, _t); }
void Titrus::onLevel4(int _frame, int _t)               { menu.onLevel4(_frame, _t); }
void Titrus::onLevel5(int _frame, int _t)               { menu.onLevel5(_frame, _t); }
void Titrus::onLevel7(int _frame, int _t)               { game.onLevel7(_frame, _t); }
void Titrus::onLevel8(int _frame, int _t)               { game.onLevel8(_frame, _t); }
void Titrus::onLevel9(int _frame, int _t)               { game.onLevel9(_frame, _t); }
void Titrus::onLevel10(int _frame, int _t)              { score.onLevel10(_frame,_t); }
void Titrus::onLevel11(int _frame, int _t)              { score.onLevel11(_frame,_t); }
void Titrus::onLevel12(int _frame, int _t)              { game.onLevel12(_frame,_t); }
void Titrus::onLevel13(int _frame, int _t)              { menu.onLevel13(_frame,_t); }
void Titrus::onLevel14(int _frame, int _t)              { menu.onLevel14(_frame,_t); }

/**
 * Callback called each second
 * @param _frame is the frame number from the beginning of the animation
 * @param _frameSec is the number of frames played during the last second
 * @param _second is the current second
 */
void Titrus::onSecond(int _frame UNUSED, int _frameSec UNUSED, int _second UNUSED)
{
}

/**
 * Callback called on each frame
 * @param _frame is the frame number from the beginning of the animation
 * @param _timeStampInMilliSeconds is the current timestamp
 */
void Titrus::onFrame(int _frame, int _timeStampInMilliSeconds)
{
    p_onFrame  method = onFrameMethods[engine->getState()];
    if (method) { (*this.*method)(_frame, _timeStampInMilliSeconds); }
}

/**
 * The onEvent Callback
 * @param _event is the SDL_Event
 */
bool Titrus::onEvent(SDL_Event & _event, int _timeStampInMilliSeconds UNUSED)
{
    getKeyboard().onEvent(_event);
    return true;
}

/**
 * Callback on the quit event
 * @return true if the event is consumed
 */
bool Titrus::onStop() { return true; }



/**
 * Run the player
 */
void Titrus::run()
{
    bool rc = true;

    // READ THE SETTINGS CONFIGURATION FILE
    settings.scoreValues[5] = 500;
    settings.scoreValues[6] = 400;
    settings.scoreValues[7] = 250;
    settings.scoreValues[8] = 200;
    settings.scoreValues[9] = 150;
    settings.load();

    // INIT THE SPLASHOUILLE FRAMEWORK
    splashouille::Engine::init();

    // CREATE THE SDL WINDOW
    SDL_Surface * screen = SDL_SetVideoMode(screenSize[0], screenSize[1], screenDepth, SDL_SWSURFACE | SDL_DOUBLEBUF );
    SDL_WM_SetCaption("Titrus", "Titrus");

    // DEFINE THE WINDOW ICON
    SDL_Surface*    icon     = SDL_LoadBMP("res/img/icon.bmp");
    if (icon)
    {
        Uint32          colorKey = SDL_MapRGB(icon->format, 255, 0, 255);
        SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorKey);
        SDL_WM_SetIcon(icon, NULL);
    }

    // OPEN AND PARSE THE CONFIGURATION FILE
    libconfig::Config * configuration   = new libconfig::Config();
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));SDL_Flip(screen); 

    try { configuration->readFile("res/conf/titrus.ini"); }
        catch(libconfig::FileIOException e) { std::cerr<<e.what()<<std::endl; rc = false; }
        catch(libconfig::ParseException  e) { std::cerr<<e.what()<<std::endl; rc = false; }

    // CREATE AND PREPARE THE ANIMATION
    engine = splashouille::Engine::createEngine();
    rc = rc && engine->import(configuration);
    delete configuration;

    if (rc)
    {

        // INITIALIZE THE DIFFERENT COMPONENTS OF TITRUS
        menu.init(engine);
        game.init(engine);
        score.init(engine);
        score.update(settings.getRoot());

        //engine->setDebug(true);
        engine->setFPS(60);
        engine->addListener(this);

        engine->run(screen);
    }

    // DELETE THE ENGINE
    splashouille::Engine::deleteEngine(engine);

    // SAVE THE CONFIGURATION
    settings.save();
}

/**
 * The player main function
 * @param argc is the number of arguments
 * @param argv is the argument values
 * @return 1
 */
int main()
{

    std::cout<<"Titrus 2011"<<std::endl;

    // INSTANCE THE GAME ENGINE AND RUN!!!
    Titrus * game = new Titrus();
    game->run();

    delete game;

    return 1;

}
