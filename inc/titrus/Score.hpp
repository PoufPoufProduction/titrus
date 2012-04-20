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

#ifndef TITRUS_SCORE_HPP_
#define TITRUS_SCORE_HPP_

namespace splashouille
{
class Animation;
class Engine;
}

namespace libconfig
{
class Setting;
}

namespace titrus
{
class Titrus;

/**
 * Class score handles the best scores overview (from main menu) and the interactive prompt at
 * the end game (when the score enters into the chart)
 */
class Score
{
private:
    Titrus *                    titrus;                         // The titrus parent
    const static int            ascii[];                        // The alphabet used characters

    splashouille::Animation *   a_best;                         // The foreground effects animation
    splashouille::Animation *   a_overview;                     // The scores overview animation

    int                         name[3];                        // The best scoring player name
    int                         position;                       // Character position
    int                         score;                          // A new best score

    /** Update the best scoring player name */
    void                        updateName(int _position, const int * _name);

public:
    Score(Titrus * _titrus):titrus(_titrus) {}

    /** Initialize the scores handler */
    bool                        init(splashouille::Engine * _engine);

    /** Update the best scores overview */
    void                        update(libconfig::Setting & _root);

    /** Scores overview event handling: just wait a key down for going back to the menu */
    void                        onLevel10(int _frame, int _timeStampInMilliSeconds);

    /** Ask the best scoring player name */
    void                        onLevel11(int _frame, int _timeStampInMilliSeconds);

    /** Update the splashouille objects */
    void                        openOverview();

    /** Ask the best scoring player name */
    bool                        openBest(int _score);

};

}


#endif
