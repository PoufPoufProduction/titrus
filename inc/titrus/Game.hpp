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

#ifndef TITRUS_GAME_HPP_
#define TITRUS_GAME_HPP_


#define LG_MAX_ORDER      1024

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
 * Class Game handles all the game interactions
 */
class Game
{
private:
    Titrus *                    titrus;                     // The titrus parent

    const static int            blocks[];                   // The shapes of the 7 block in the 4 orientations
    const static std::string    bgs[];                      // The different background;

    splashouille::Object *      o_score[4];                 // The score value
    splashouille::Object *      o_lines[3];                 // The number of lines
    splashouille::Object *      o_tiles[10][18];            // The tiles grid
    splashouille::Object *      o_preview[4];               // The preview tiles
    splashouille::Object *      o_active[4];                // The interactive falling tiles
    splashouille::Object *      o_popup[3];                 // The popup (double, triple and titrus)
    splashouille::Object *      o_land;                     // The land chunk
    splashouille::Animation *   a_preview;                  // The preview animation area
    splashouille::Animation *   a_grid;                     // The grid animation
    splashouille::Animation *   a_effects;                  // The foreground effects animation
    splashouille::Animation *   a_bg;                       // The grid background
    splashouille::Animation *   a_cinematic;                // The final cinematic

    int                         linesToDelete[4];
    int                         currentSpeed;
    int                         pos[2];
    int                         rotation;
    char                        order[LG_MAX_ORDER];
    int                         orderId;
    int                         score;
    int                         lines;
    int                         tileIndex;
    int                         nextDown;

    /** Update the score and the number of lines values */
    void                        updateScoreNLines(int _score, int _lines);


    void                        next(int _timeStampInMilliSeconds = 0);
    bool                        move(int _x, int _y, bool _force = false);
    void                        rotate(bool _clock = true);
    int                         checkLinesToDelete();
    void                        stopTile(int _timeStampInMilliSeconds);
    void                        updateBg(int _lastNbLines, int _newNbLines, bool _gameType, int _speed);
    void                        finish();

    void checkGrid();


public:
    Game(Titrus * _titrus):titrus(_titrus) {}

    /** Initialize the game class handler */
    bool                        init(splashouille::Engine * _engine);

    /** Initialize a new game */
    void                        init();

    /** Close the game */
    void                        close();

    /** Game event handling */
    void                        onLevel7(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel8(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel9(int _frame, int _timeStampInMilliSeconds);
    void                        onLevel12(int _frame, int _timeStampInMilliSeconds);

};

}


#endif
