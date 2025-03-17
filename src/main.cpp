//
//  main.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#include "main.h"

// Global Project Version
const std::string VERSION = "101";

/*
Flag -show, - віводим всю информацию (все найденніе результаті)
Flag -showSCAN - вывод всех данных про настройки сканирования
Flag -showDEXES - вывод всех найденных и записанных DEX и кол-во (POOLS у каждой)
Flag -showPOOLS <DEX> - вывод всех пулов в указанной <DEX> + количество
Flag -showTOKENS <DEX> - вывод всех токенов у DEX (т.к. Нет возможности показать токены - конкретного пула) я если я правильно понял!
Flag -findTOKEN <DEX> -  вівод информации по токену на вібраннной DEX
Flag -findTOKENS без указания показівает - все пулі на всех dex, где встречается данній token ]
 */


//
//          MAIN FUNCTION
//
int main(int argc, char* argv[]) {
    
    // Start the timer
    StartTimeMeasure();
    
    std::cout << "Starting DEX Arbitrage, VERSION: "<<VERSION<<"\n";
    
    
    // Input Flags
    STRUCT_Input_Flags flags = Parse_Arguments(argc, argv);
    

    

    // Check Input
        if (flags.showDEXES) {
            std::cout << "showDEXES is enabled\n";
        }
        if (flags.showPOOLS) {
            std::cout << "showPOOLS is enabled\n";
        }
    
    // Example of calling functions from another file arbitrage.cpp
    findArbitrageOpportunity();
    
    // Stop the timer
    StopTimeMeasure(MICROSECONDS);
    return 0;
}

