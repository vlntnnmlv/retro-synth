#include "Note.h"

Note::Note()
{
    type     = UNKNOWN;
    octave   = 0;
    time_on  = 0.0f;
    time_off = 0.0f;
    active   = false;
}

float Note::get_frequency() const
{
    return 16.35f * std::pow(2, octave) * std::pow(2, type / 12.0f); 
}

bool Note::operator==(const Note& other) const
{
    return type == other.type && octave == other.octave;
}

bool Note::operator<(const Note& other) const
{
    return type < other.type && octave <= other.octave;
}