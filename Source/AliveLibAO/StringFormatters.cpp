#include "stdafx_ao.h"
#include "Function.hpp"
#include "StringFormatters.hpp"
#include "Input.hpp"

namespace AO {

// TODO: Convert to InputCommand constants
const int dword_4CEE78[30] =
{
    2048,
    256,
    6,
    9,
    9,
    6,
    16,
    128,
    32,
    64,
    4096,
    16384,
    32768,
    8192,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

const char* kButtonNamesOrAtlasNums_4CEDA8[] =
{
    "esc",
    "tab",
    "alt",
    "shift",
    "shift",
    "alt",
    "space",
    "ctrl",
    "Z",
    "X",
    kAO_Up,
    kAO_Down,
    kAO_Left,
    kAO_Right,
    kAO_MountZTurn,
    kAO_15,
    "\x16",
    "\x17",
    "\x18",
    "\x19",
    "\x1A",
    "\x1B",
    "\x1C",
    "\x1D",
    "\x1E",
    "\x1F",
    "esc",
    "tab",
    kAO_Action,
    kAO_Jump_Or_Hello,
    kAO_Jump_Or_Hello,
    kAO_Action,
    kAO_Speak1,
    kAO_Speak2,
    kAO_Run,
    kAO_Sneak,
    kAO_Up,
    kAO_Down,
    kAO_Left,
    kAO_Right,
    kAO_MountZTurn,
    kAO_15,
    "\x16",
    "\x17",
    "\x18",
    "\x19",
    "\x1A", // corruption
    "\x1B", // blank
    "\x1C", // blank
    "\x1D", // blank
    "\x1E", // blank
    "\x1F", // blank
    "\x2D", // todo: might not be part of this
};


EXPORT void CC String_FormatString_450DC0(const char* pInput, char* pOutput)
{
    char* pOutIter = pOutput;
    const char* pInIter = pInput;
    while (1)
    {
        const char in_char = *pInIter;
        if (!*pInIter)
        {
            break;
        }

        if (in_char >= ' ')
        {
            *pOutIter++ = in_char;
            pInIter++;
        }
        else
        {
            const char in_char_m6 = in_char - 6;
            const char* pConverted = nullptr;
            if (in_char_m6 < 0 || in_char_m6 >= 14)
            {
                pConverted = kButtonNamesOrAtlasNums_4CEDA8[in_char_m6];
            }
            else
            {
                pConverted = Input_GetButtonString_44F1C0(static_cast<InputCommands>(dword_4CEE78[in_char_m6]));
            }

            strcpy(pOutIter, pConverted);

            if (*pOutIter)
            {
                char next_char = 0;
                do
                {
                    next_char = (pOutIter++)[1];
                } while (next_char);
                pInIter++;
            }
            else
            {
                pInIter++;
            }
        }
    }

    // Null terminate destination string
    *pOutIter = 0;
}

}

