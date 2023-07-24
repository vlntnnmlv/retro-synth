#pragma once

#include <SDL2/SDL.h>
#include <vector>

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

struct Note
{
    NoteType type;
    int      octave;

    mutable float timeOn;
    mutable float timeOff;
    mutable bool  active;

    Note()
    {
        type    = UNKNOWN;
        octave  = 0;
        timeOn  = 0.0f;
        timeOff = 0.0f;
        active  = false;
    }

    float get_frequency() const
    {
        return 27.5f * std::pow(2, octave) * std::pow(2, type / 12.0f); 
    }

    void setOn(float time)
    {
        timeOn = time;
    }

    void setOff(float time)
    {
        timeOff = time;
    }

    bool operator=(const Note& other) const
    {
        return type == other.type && octave == other.octave;
    }

    bool operator<(const Note& other) const
    {
        return type < other.type && octave <= other.octave;
    }
};