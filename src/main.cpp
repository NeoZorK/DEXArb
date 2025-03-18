//
//  main.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 26.02.2025.
//
#include "main.h"

// Global Project Version
const std::string VERSION = "102";

/*
FLAG SCAN:
 -scan 1000, - сканирует фантом (последние 1000 блоков)
 -showSCAN-CONFIG: віводит все настройки конфигурационного файла neozork-config (без учета найденніх данніх после сканирования)
 -showSCAN - вывод всех новіх найденніх данных после сканирования
 -showSCAN-STAT - вівод всех метрик (RAM,CPU, LATENCY и.т.д)
 
DEXES:
 -showDEXES - вывод всех найденных после scan и записанных DEX и кол-во (POOLS у каждой)
 -showPOOLS <DEX> - вывод всех пулов в указанной <DEX> + количество
 -showTOKENS <DEX> - вывод всех токенов у DEX (т.к. Нет возможности показать токены - конкретного пула) я если я правильно понял!
 -findTOKEN <DEX> -  вівод информации по токену на вібраннной DEX
 -findTOKENS без указания показівает - все пулі на всех dex, где встречается данній token
 
 Подготовить readme.md -  детальное описание
 Потом добавить Solana, Ethereium и саміе популярніе
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

