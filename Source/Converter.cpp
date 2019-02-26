#include "Converter.h"

Converter::Converter() : envelope()
{
    // for testing purposes ONLY.
    // make a simple profile (just the first chunk) for a simple sound.
    // *******************
    profile[0][(int)referenceFrequency] = 500.0f;
    profile[0][(int)(2 * referenceFrequency)] = 300.0f;
    profile[0][(int)(4 * referenceFrequency)] = 200.0f;
    profile[0][(int)(6 * referenceFrequency)] = 100.0f;
    // *******************
}

Converter::~Converter() {
}

