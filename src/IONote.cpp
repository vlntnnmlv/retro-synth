#include "IONote.h"

IONote::IONote()
{
}

IONote::~IONote()
{
}

std::set<Note> IONote::get_current_notes()
{
    return std::set<Note>();
}

void IONote::set_input_mode(InputMode input_mode)
{
    m_input_mode = input_mode;
}