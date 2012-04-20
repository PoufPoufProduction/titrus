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

#ifndef TITRUS_MENU_HPP_
#define TITRUS_MENU_HPP_

namespace splashouille
{
class Animation;
class Object;
class Engine;
}

namespace titrus
{
class Titrus;

/**
 * Class menu handles all the menu interactions
 */
class Menu
{
private:
    Titrus *                    titrus;                     // The titrus parent

    splashouille::Object *      o_level1[6];                // The level1 menu: Play, Scores, Credit, Multi, etc.
    splashouille::Object *      o_gametype[2];              // The level2 menu: Type-A or Type-B
    splashouille::Object *      o_speed[10];                // The speed selector
    splashouille::Object *      o_high[6];                  // The high selector
    splashouille::Object *      o_options[2];               // The options
    splashouille::Object *      o_audio[5];                 // The audio values
    splashouille::Object *      o_touch[5];                 // The touch values
    splashouille::Animation *   a_menu;                     // The menu panel

    int                         level1;                     // The selected level1 menu
    int                         option;                     // The selected options
    int                         touch[5];                   // The touch value

    /** Update the different menu parts */
    void                        updateLevel1(int _level1, int _timeStampInMilliSeconds);
    void                        updateGameType(bool _gameType, int _timeStampInMilliSeconds);
    void                        updateSpeed(int _speed, int _timeStampInMilliSeconds);
    void                        updateHigh(int _speed, int _timeStampInMilliSeconds);
    void                        updateOption(int _option = -1, int _value = -1, int _timeStampInMilliSeconds =-1);


public:
    Menu(Titrus * _titrus):titrus(_titrus), level1(0), option(0) {}

    /** Initialize the menu class handler */
    bool                        init(splashouille::Engine * _engine);

    /** Menu event handling */
    void                        onLevel1(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel2(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel3(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel4(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel5(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel13(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel14(int _frame, int _timeStampInMilliSeconds);
};

}


#endif
