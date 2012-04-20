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

#ifndef TITRUS_MAIN_HPP_
#define TITRUS_MAIN_HPP_

#include <string>
#include <splashouille/Engine.hpp>
#include <SDL.h>

#include <utility/Configuration.hpp>
#include <utility/SDL_Keyboard.hpp>
#include <titrus/Score.hpp>
#include <titrus/Menu.hpp>
#include <titrus/Game.hpp>

namespace titrus
{

typedef enum
{
    level0              = 0,                // NOT INTERACTIVE LEVEL
    level1              = 10,               // LEVEL1 SELECTOR (PLAY,SCORES,CREDIT,ETC.)
    level2              = 20,               // GAMETYPE SELECTION
    level3              = 30,               // SPEED SELECTION
    level4              = 40,               // HIGH SELECTION
    level5              = 50,               // LAUNCH BUTTON
    level6              = 60,               // QUIT
    level7              = 70,               // THE GAME: INITIALISATION
    level8              = 80,               // THE GAME: INTERACTIVE
    level9              = 90,               // THE GAME: REMOVE LINE
    level10             = 100,              // SCORE MENU
    level11             = 110,              // NEW BEST SCORE
    level12             = 120,              // NEED TO ASK THE BEST SCORING PLAYER'S NAME
    level13             = 130,              // CREDITS
    level14             = 140,              // OPTIONS
    last                = 200
} gameState;

class Titrus : public splashouille::Engine::Listener
{
private:
    typedef void (Titrus::*p_onFrame)(int _frame, int _timeStampInMilliSeconds);
    typedef bool (Titrus::*p_onEvent)(SDL_Event & _event, int _timeStampInMilliSeconds);

private:
    splashouille::Engine *      engine;                         // The splashouille engine
    utility::SDL_Keyboard       keyboard;                       // The SDL keyboard handler

    int                         screenSize[2];                  // The screen size
    int                         screenDepth;                    // The screen depth
    bool                        gameType;                       // The gameType false=typeA, true=typeB
    int                         speed;                          // The game speed
    int                         high;                           // The type B high

    // -------------- CALLBACKS PART -----------------------------
    p_onFrame                   onFrameMethods[last];           // onFrame callbacks regarding the state
    p_onEvent                   onEventMethods[last];           // onEvent Callbacks regarding the state
    void                        initStateMethods();             // Init the callbacks vector

    // -------------- TITRUS COMPONENTS -----------------------------
    utility::Configuration      settings;                       // The titrus settings (volume, bestscores, etc.)
    Score                       score;                          // The score handler
    Menu                        menu;                           // The menu handler
    Game                        game;                           // The game handler

    /** Forward methods */
    void                        onLevel1(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel2(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel3(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel4(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel5(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel6(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel7(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel8(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel9(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel10(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel11(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel12(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel13(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel14(int _frame, int _timeStampInMilliSeconds);

public:
    Titrus():
        engine(0), screenDepth(32), gameType(false), speed(0), high(0), settings("titrus"),
        score(this), menu(this), game(this)
    {
        screenSize[0] = 480;
        screenSize[1] = 640;
        initStateMethods();
    }

    /** Accessors */
    Score &                     getScore()              { return score; }
    Menu &                      getMenu()               { return menu; }
    Game &                      getGame()               { return game; }
    utility::Configuration &    getSettings()           { return settings; }
    utility::SDL_Keyboard &     getKeyboard()           { return keyboard; }
    splashouille::Engine *      getEngine() const       { return engine; }
    bool                        getGameType() const     { return gameType; }
    int                         getSpeed() const        { return speed; }
    int                         getHigh() const         { return high; }

    void                        setGameType(bool _g)    { gameType = _g; }
    void                        setSpeed(int _s)        { speed = _s; }
    void                        setHigh(int _h)         { high = _h; }

    /**
     * Callback called each second
     * @param _frame is the frame number from the beginning of the animation
     * @param _frameSec is the number of frames played during the last second
     * @param _second is the current second
     */
    void onSecond(int _frame, int _frameSec, int _second);

    /**
     * Callback called on each frame
     * @param _frame is the frame number from the beginning of the animation
     * @param _timeStampInMilliSeconds is the current timestamp
     */
    void onFrame(int _frame, int _timeStampInMilliSeconds);

    /**
     * The onEvent Callback
     * @param _event is the SDL_Event
     */
    bool onEvent(SDL_Event & _event, int _timeStampInMilliSeconds);

    /**
     * Callback on the quit event
     * @return true if the event is consumed
     */
    bool onStop();

    /**
     * Run the player
     */
    void run();
};
}

#endif
