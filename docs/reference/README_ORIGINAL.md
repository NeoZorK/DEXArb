**NeoZorKDEXArb** — is a console application written in C++, designed for scanning, analyzing, and potentially utilizing arbitrage opportunities on decentralized exchanges (DEX) across various blockchains such as Ethereum, Fantom, Binance Smart Chain (BSC), Polygon, Avalanche, and Solana. The program provides users with tools for discovering DEX factory contracts, analyzing liquidity pools, tokens, trading statistics, as well as executing queries to blockchains through free RPC nodes. The project focuses on cross-platform compatibility, performance, and minimal dependencies, making it easy to deploy and test.

All paid nodes (e.g., Infura) are excluded and replaced with free public RPC nodes:

Ethereum: rpc.ankr.com/eth, eth.llamarpc.com
Fantom: rpc.ftm.tools, rpc.ankr.com/fantom
BSC: bsc-dataseed.binance.org, rpc.ankr.com/bsc
Polygon: polygon-rpc.com, rpc.ankr.com/polygon
Avalanche: rpc.ankr.com/avalanche, api.avax.network/ext/bc/C/rpc
Solana: api.mainnet-beta.solana.com, solana-mainnet.rpc.extrnode.com

#### Main Features
1. **Blockchain Scanning**:
   - The program scans a specified block range (from 1000 to 1,000,000) to find DEX factory contracts (e.g., Uniswap, PancakeSwap).
   - Uses multithreading to accelerate the process by distributing blocks across threads.
   - Identifies pool creation events (PairCreated, PoolCreated) by their signatures.

2. **DEX Analysis**:
   - Collects data on liquidity pools, tokens, 24-hour trading volume, and total value locked (TVL).
   - Supports queries to factory contracts to obtain pool addresses and their characteristics.

3. **Configuration**:
   - Reads settings from a `neozork-config` file in JSON format, including thread count and RPC nodes.
   - Automatically creates a configuration file with free RPC nodes if it doesn't exist.

4. **Queries and Data Output**:
   - Supports commands for displaying DEX information, pools, tokens, and searching for specific tokens.
   - Outputs performance statistics (execution time, network usage, disk space) to the `neozork-scan-stat` file.

5. **Arbitrage (stub)**:
   - Includes functions for analyzing and executing arbitrage operations (not fully implemented yet).

#### How the Program Works
1. **Launch and Command Line Arguments**:
   - The program is launched with flags such as `-scan`, `-showDEXES`, `-showPOOLS`, and requires specifying a blockchain (e.g., `ethereum`) and, if necessary, additional parameters (e.g., block range).
   - Example: `./neozork -scan ethereum 1000` scans the last 1000 blocks of Ethereum.

2. **Configuration Reading**:
   - Upon startup, the program checks for the presence of the `neozork-config` file. If it doesn't exist, a file with default settings is created (3 threads, free RPC nodes).
   - The configuration contains sections for each blockchain with specified RPC nodes and their request limits.

3. **Blockchain Scanning**:
   - Uses the RPC method `eth_getBlockByNumber` to retrieve block data.
   - Analyzes transactions and logs in each block, checking for pool creation event signatures.
   - Found factory contracts are saved to the `DexInfo` list.

4. **Data Processing**:
   - For each found factory contract, the number of pools (`allPairsLength`), pool addresses (`allPairs(index)`), pool tokens (`token0`, `token1`), and liquidity (`getReserves`) are requested.
   - 24-hour trading statistics are collected via `eth_getLogs` with a filter on the `Swap` event.

5. **Multithreading**:
   - Block scanning and pool statistics collection are distributed across threads (count specified in config).
   - A mutex is used to synchronize access to shared data.

6. **Output and Saving**:
   - Scan results are displayed in the console with colored formatting (green for success, yellow for data, red for errors).
   - DEX data is saved to `neozork-config`, statistics to `neozork-scan-stat`.

7. **Performance**:
   - Execution time, network traffic volume, and disk usage are measured. Scan progress is displayed as a progress bar.

#### Technical Details
- **Language and Standard**: C++17 for modern capabilities and compatibility.
- **Dependencies**: Uses `libcurl` for HTTP requests to RPC nodes and the C++ standard library (`<thread>`, `<mutex>`, `<atomic>`) for multithreading.
- **RPC Nodes**: Only free, public nodes (e.g., `rpc.ankr.com`, `polygon-rpc.com`).
- **Cross-platform**: Supports Windows, Linux, and macOS with minimal setup.

#### Usage Examples
- **Scanning**: `./neozork -scan bsc 5000` — scans 5000 BSC blocks, finds DEXes and updates config.
- **Viewing Pools**: `./neozork -showPOOLS bsc 0xFactoryAddress` — shows pools for the specified DEX.
- **Token Search**: `./neozork -findTOKEN bsc 0xFactoryAddress 0xTokenAddress` — searches for a token in DEX pools.

#### Limitations
- Solana support is limited (only config display) due to API differences.
- Arbitrage, wallet, and profit analysis functions are currently implemented as stubs.

1. **CMake Version**: Kept at 3.28 for modern capabilities support.
2. **Source Files**: Added all `.cpp` files from `/src` so the project builds full functionality.
3. **Dependencies**:
   - `find_package(CURL REQUIRED)`: Ensures `libcurl` is available for RPC requests.
   - `find_package(Threads REQUIRED)`: Includes thread support via `<thread>` and `-pthread`.
4. **Cross-platform**:
   - **macOS**: Added `CoreFoundation` and `SystemConfiguration` frameworks for proper `libcurl` operation.
   - **Linux**: Basic configuration with `libcurl` and `pthread`.
   - **Windows**: Added `ws2_32` library (Winsock) for `libcurl` network operations.
5. **Compiler Flags**: Added warnings (`-Wall`, `/W4`) and optimization (`-O2`) for all platforms.
6. **Installation**: Added `install` for the ability to package the binary in `bin`.

#### Building the Project
1. Ensure dependencies are installed:
   - Linux: `sudo apt install libcurl4-openssl-dev cmake g++`
   - macOS: `brew install curl cmake`
   - Windows: Install CMake and CURL via vcpkg (`vcpkg install curl`) and MSVC.
2. Execute:
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```
3. Run: `./NeoZorKDEXArb` (Linux/macOS) or `NeoZorKDEXArb.exe` (Windows).
