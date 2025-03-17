//
//  input.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 17.03.2025.
//

#include "main.h"


// Parse Arguments
STRUCT_Input_Flags Parse_Arguments(int argc, char* argv[]) {
    
    // Save Flags
    STRUCT_Input_Flags flags;
    std::vector<std::string> args(argv + 1, argv + argc);

    // INPUT FOR
    for (const auto& arg : args) {
        if (arg == "-show") {
            flags.show = true;
        }
        else if (arg == "-showSCAN") {
            flags.showSCAN = true;
        }
        else if (arg == "-showDEXES") {
            flags.showDEXES = true;
        }
        else if (arg == "-showPOOLS") {
            flags.showPOOLS = true;
        }
        else if (arg == "-showTOKENS") {
            flags.showTOKENS = true;
        }
        else if (arg == "-findTOKEN") {
            flags.findTOKEN = true;
        }
        else if (arg == "-findTOKENS") {
            flags.findTOKENS = true;
        }
    }

    return flags;
}
