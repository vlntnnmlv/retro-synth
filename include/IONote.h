#pragma once

#include <set>

#include "Note.h"

enum class InputMode
{
    INVALID  = 0,
    KEYBOARD = 1,
    MIDI     = 2
};

class IONote
{
    public:
        IONote();
        ~IONote();
        std::set<Note> get_current_notes();
        void           set_input_mode(InputMode input_mode);

    private:
        InputMode m_input_mode;        
};