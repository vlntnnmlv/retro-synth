#pragma once

#include <cmath>

enum NoteType
{
    UNKNOWN = -1,
    A       = 0,
    A_SHARP = 1,
    B       = 2,
    C       = 3,
    C_SHARP = 4,
    D       = 5,
    D_SHARP = 6,
    E       = 7,
    F       = 8,
    F_SHARP = 9,
    G       = 10,
    G_SHARP = 11
};

class Note
{
    public:
        Note();

        NoteType type;
        int      octave;
        float    time_on;
        float    time_off;
        bool     active;

        float get_frequency() const;

        bool operator==(const Note& other) const;
        bool operator<(const Note& other) const;
};