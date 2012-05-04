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
#include <splashouille/Sound.hpp>

#include <titrus/Main.hpp>

using namespace titrus;

/**
 * Initialize the menu  handler
 * @return true if every objects are found (todo)
 */
bool Menu::init(splashouille::Engine * _engine)
{
    o_level1[0]     = _engine->getLibrary()->getObjectById("play");
    o_level1[1]     = _engine->getLibrary()->getObjectById("scores");
    o_level1[2]     = _engine->getLibrary()->getObjectById("multi");
    o_level1[3]     = _engine->getLibrary()->getObjectById("options");
    o_level1[4]     = _engine->getLibrary()->getObjectById("credit");
    o_level1[5]     = _engine->getLibrary()->getObjectById("quit");

    o_gametype[0]   = _engine->getLibrary()->getObjectById("typea");
    o_gametype[1]   = _engine->getLibrary()->getObjectById("typeb");

    o_speed[0]      = _engine->getLibrary()->getObjectById("speed0");
    o_speed[1]      = _engine->getLibrary()->getObjectById("speed1");
    o_speed[2]      = _engine->getLibrary()->getObjectById("speed2");
    o_speed[3]      = _engine->getLibrary()->getObjectById("speed3");
    o_speed[4]      = _engine->getLibrary()->getObjectById("speed4");
    o_speed[5]      = _engine->getLibrary()->getObjectById("speed5");
    o_speed[6]      = _engine->getLibrary()->getObjectById("speed6");
    o_speed[7]      = _engine->getLibrary()->getObjectById("speed7");
    o_speed[8]      = _engine->getLibrary()->getObjectById("speed8");
    o_speed[9]      = _engine->getLibrary()->getObjectById("speed9");

    o_high[0]       = _engine->getLibrary()->getObjectById("high0");
    o_high[1]       = _engine->getLibrary()->getObjectById("high1");
    o_high[2]       = _engine->getLibrary()->getObjectById("high2");
    o_high[3]       = _engine->getLibrary()->getObjectById("high3");
    o_high[4]       = _engine->getLibrary()->getObjectById("high4");
    o_high[5]       = _engine->getLibrary()->getObjectById("high5");

    o_options[0]    = _engine->getLibrary()->getObjectById("touch");
    o_options[1]    = _engine->getLibrary()->getObjectById("audio");

    o_audio[0]      = _engine->getLibrary()->getObjectById("audio0");
    o_audio[1]      = _engine->getLibrary()->getObjectById("audio1");
    o_audio[2]      = _engine->getLibrary()->getObjectById("audio2");
    o_audio[3]      = _engine->getLibrary()->getObjectById("audio3");
    o_audio[4]      = _engine->getLibrary()->getObjectById("audio4");

    o_touch[0]      = _engine->getLibrary()->getObjectById("touch0");
    o_touch[1]      = _engine->getLibrary()->getObjectById("touch1");
    o_touch[2]      = _engine->getLibrary()->getObjectById("touch2");
    o_touch[3]      = _engine->getLibrary()->getObjectById("touch3");
    o_touch[4]      = _engine->getLibrary()->getObjectById("touch4");

    a_menu          = _engine->getLibrary()->getAnimationById("a_menu");

    touch[0]        = 0;
    touch[1]        = 120;
    touch[2]        = 100;
    touch[3]        = 75;
    touch[4]        = 50;

    updateOption();

    return true;
}

/**
 * Change the level1 selection
 * @param _level1 is the new level1 selected menu
 * @param _timeStampInMilliSeconds is the unused timestamp
 */
void Menu::updateLevel1(int _level1, int _timeStampInMilliSeconds UNUSED)
{
    if (_level1>=0 && _level1<6)
    {
        if (level1!=_level1) { o_level1[level1]->changeFashion("normal"); }
        level1 = _level1;
        o_level1[level1]->changeFashion("selected");
    }
}

/**
 * Change the option selection
 * @param _level1 is the new level1 selected menu
 * @param _timeStampInMilliSeconds is the unused timestamp
 */
void Menu::updateOption(int _option, int _value, int _timeStampInMilliSeconds UNUSED)
{
    if (_option<2)
    {
        // THE OPTION HIGHLIGHT
        bool update = true;
        if (_option>=0)
        {
            if (option!=_option) { o_options[option]->changeFashion("normal"); } else { update = false; }
            option = _option;
        }
        if (update) { o_options[option]->changeFashion("selected"); }


        // THE VALUE HIGHLIGHT
        int t = titrus->getSettings().getRoot()["touch"];
        int a = titrus->getSettings().getRoot()["audio"];
        update = true;

        if (_value>=0)
        {
            switch(option)
            {
            case 0:
                if (t!=_value) { o_touch[t]->changeFashion("normal"); } else { update=false; }
                t = _value;
                break;
            case 1:
                if (a!=_value) { o_audio[a]->changeFashion("normal"); } else { update=false; }
                a = _value;
                break;
            }
        }

        if (update)
        {
            titrus->getKeyboard().SDL_EnableKeyRepeat(touch[t]*2, touch[t]);
            splashouille::Sound::changeVolume((float)a/4);
            titrus->getEngine()->getLibrary()->getObjectById("menusignes")->changeFashion(a?"sound":"nosound");

            o_touch[t]->changeFashion("selected");
            o_audio[a]->changeFashion("selected");
            titrus->getSettings().getRoot()["touch"] = t;
            titrus->getSettings().getRoot()["audio"] = a;
        }

    }
}

/**
 * Change the game type
 * @param _gameType is the game type
 * @param _timeStampInMilliSeconds is the unused timestamp
 */
void Menu::updateGameType(bool _gameType, int _timeStampInMilliSeconds UNUSED)
{
    if (_gameType!=titrus->getGameType()) { o_gametype[titrus->getGameType()]->changeFashion("normal"); }
    titrus->setGameType(_gameType);
    o_gametype[titrus->getGameType()]->changeFashion("selected");
}

/**
 * Change the speed
 * @param _speed is the new speed
 * @param _timeStampInMilliSeconds is the unused timestamp
 */
void Menu::updateSpeed(int _speed, int _timeStampInMilliSeconds UNUSED)
{
    if (_speed>=0 && _speed<10)
    {
        if (_speed!=titrus->getSpeed()) { o_speed[titrus->getSpeed()]->changeFashion("normal"); }
        titrus->setSpeed(_speed);
        o_speed[titrus->getSpeed()]->changeFashion("selected");
    }
}

/**
 * Change the highness
 * @param _speed is the new speed
 * @param _timeStampInMilliSeconds is the unused timestamp
 */
void Menu::updateHigh(int _high, int _timeStampInMilliSeconds UNUSED)
{
    if (_high>=0 && _high<6)
    {
        if (_high!=titrus->getHigh()) { o_high[titrus->getHigh()]->changeFashion("normal"); }
        titrus->setHigh(_high);
        o_high[titrus->getHigh()]->changeFashion("selected");
    }
}

/**
 * Handle the level1 event
 * @param _event
 * @param _timeStampInMilliSeconds is the unused timestamp
 * @return true
 */
void Menu::onLevel1(int _frame UNUSED, int _timeStampInMilliSeconds UNUSED)
{
    Uint8 *keystates = titrus->getKeyboard().SDL_GetKeyState(NULL);

    if( keystates[' '])
    {
        switch(level1)
        {
            case 0:     a_menu->changeTimeline("t_level1ToLevel2"); break;
            case 1:     titrus->getScore().openOverview(); break;
            case 3:     a_menu->changeTimeline("t_level1ToLevel14"); break;
            case 4:     a_menu->changeTimeline("t_level1ToCredits"); break;
            case 5:     a_menu->changeTimeline("t_level1ToEnd");break;
            default:    break;
        };
    }
    else
    {
        int value = level1;

        if( keystates[ SDLK_DOWN ] )    { if (value<4) { value += 2; } }
        if( keystates[ SDLK_UP ] )      { if (value>1) { value -= 2; } }
        if( keystates[ SDLK_LEFT ] )    { value &= 254; }
        if( keystates[ SDLK_RIGHT ] )   { value |= 1; }

        updateLevel1(value, _timeStampInMilliSeconds);
    }
}

/**
 * Handle the game type selector event
 * @param _event
 * @param _timeStampInMilliSeconds is the unused timestamp
 * @return true
 */
void Menu::onLevel2(int _frame UNUSED, int _timeStampInMilliSeconds UNUSED)
{
    Uint8 *keystates = titrus->getKeyboard().SDL_GetKeyState(NULL);

    if( keystates[SDLK_ESCAPE])     { a_menu->changeTimeline("t_level2ToLevel1"); }
    else if( keystates[' '])        { a_menu->changeTimeline("t_level2ToLevel3"); }
    else
    {
        int value = titrus->getGameType();
        if( keystates[ SDLK_LEFT ] )    { value =0; }
        if( keystates[ SDLK_RIGHT ] )   { value = 1; }
        updateGameType(value, _timeStampInMilliSeconds);
    }
}

/**
 * Handle the speed selector event
 * @param _event
 * @param _timeStampInMilliSeconds is the unused timestamp
 * @return true
 */
void Menu::onLevel3(int _frame UNUSED, int _timeStampInMilliSeconds UNUSED)
{
    Uint8 *keystates = titrus->getKeyboard().SDL_GetKeyState(NULL);

    if( keystates[SDLK_ESCAPE])     { a_menu->changeTimeline("t_level3ToLevel2"); }
    else if( keystates[' '])        { a_menu->changeTimeline(titrus->getGameType()?"t_level3ToLevel4":"t_toLevel5"); }
    else
    {
        int value = titrus->getSpeed();
        if( keystates[ SDLK_DOWN ] )    { if (value<5) { value += 5; } }
        if( keystates[ SDLK_UP ] )      { if (value>4) { value -= 5; } }
        if( keystates[ SDLK_LEFT ] )    { if (value%5>0) { value--;} }
        if( keystates[ SDLK_RIGHT ] )   { if (value%5<4) { value++;} }
        updateSpeed(value, _timeStampInMilliSeconds);
    }

}

/**
 * Handle the high selector event
 * @param _event
 * @param _timeStampInMilliSeconds is the unused timestamp
 * @return true
 */
void Menu::onLevel4(int _frame UNUSED, int _timeStampInMilliSeconds UNUSED)
{
    Uint8 *keystates = titrus->getKeyboard().SDL_GetKeyState(NULL);

    if( keystates[SDLK_ESCAPE])         { a_menu->changeTimeline("t_level4ToLevel3"); }
    else if( keystates[' '])            { a_menu->changeTimeline("t_toLevel5"); }
    else
    {
        int value = titrus->getHigh();
        if( keystates[ SDLK_DOWN ] )    { if (value<3) { value += 3; } }
        if( keystates[ SDLK_UP ] )      { if (value>2) { value -= 3; } }
        if( keystates[ SDLK_LEFT ] )    { if (value%3>0) { value--;} }
        if( keystates[ SDLK_RIGHT ] )   { if (value%3<2) { value++;} }
        updateHigh(value, _timeStampInMilliSeconds);
    }
}

/**
 * Handle the play button event
 * @param _event
 * @param _timeStampInMilliSeconds is the unused timestamp
 * @return true
 */
void Menu::onLevel5(int _frame UNUSED, int _timeStampInMilliSeconds UNUSED)
{
    Uint8 *keystates = titrus->getKeyboard().SDL_GetKeyState(NULL);

    if( keystates[SDLK_ESCAPE])         { a_menu->changeTimeline(titrus->getGameType()?"t_level5ToLevel4":"t_level5ToLevel3"); }
    else if( keystates[' '])            { titrus->getGame().init(); a_menu->changeTimeline("t_level5toGame"); }
}

/**
 * Handle the credit event
 * @param _event
 * @param _timeStampInMilliSeconds is the unused timestamp
 * @return true
 */
void Menu::onLevel13(int _frame UNUSED, int _timeStampInMilliSeconds UNUSED)
{
    Uint8 *keystates = titrus->getKeyboard().SDL_GetKeyState(NULL);

    if( keystates[SDLK_ESCAPE] || keystates[' ']) { a_menu->changeTimeline("t_creditsToLevel1"); }
}

/**
 * Handle the credit event
 * @param _event
 * @param _timeStampInMilliSeconds is the unused timestamp
 * @return true
 */
void Menu::onLevel14(int _frame UNUSED, int _timeStampInMilliSeconds UNUSED)
{
    Uint8 *keystates = titrus->getKeyboard().SDL_GetKeyState(NULL);

    if( keystates[SDLK_ESCAPE] || keystates[' ']) { a_menu->changeTimeline("t_Level14ToLevel1"); }
    else
    {
        int value = option;
        if( keystates[ SDLK_DOWN ] )    { if (value<1) { value++; } }
        if( keystates[ SDLK_UP ] )      { if (value>0) { value--; } }

        int value2;
        switch(value)
        {
            case 0: value2 = titrus->getSettings().getRoot()["touch"]; break;
            case 1: value2 = titrus->getSettings().getRoot()["audio"]; break;
        };
        if( keystates[ SDLK_LEFT ] )    { if (value2>0) { value2--;} }
        if( keystates[ SDLK_RIGHT ] )   { if (value2<4) { value2++;} }

        updateOption(value, value2, _timeStampInMilliSeconds);
    }
}


