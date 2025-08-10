# Немедленные действия для модернизации NeoZorKDEXArb

## 🚀 Приоритетные задачи (1-2 недели)

### 1. Исправление предупреждений компилятора

#### 1.1 Устранение глобальных переменных
```cpp
// Проблема: g_logger в modern_utils.cpp
// Решение: Dependency Injection

// Создать:
include/di/logger_factory.h
src/di/logger_factory.cpp

// Заменить глобальную переменную на:
class Application {
private:
    std::unique_ptr<modern::Logger> logger_;
public:
    Application(std::unique_ptr<modern::Logger> logger);
};
```

#### 1.2 Оптимизация структур данных
```cpp
// Проблема: padding в modern::Logger и JsonValue
// Решение: Переупорядочить поля

class Logger {
private:
    std::ostream& output_;     // 8 bytes (pointer)
    std::string prefix_;       // 24 bytes (string)
    Level level_;              // 4 bytes (enum)
    // Padding: 4 bytes
};
```

#### 1.3 Исправление путей включения
```cmake
# В CMakeLists.txt заменить:
target_include_directories(NeoZorKDEXArb PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# На:
target_include_directories(NeoZorKDEXArb PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
```

### 2. Разделение main.cpp

#### 2.1 Создание CLI модуля
```bash
mkdir -p src/cli include/cli
```

```cpp
// include/cli/command_parser.h
class CommandParser {
public:
    struct ParsedCommand {
        std::string flag;
        std::string blockchain;
        std::string value;
        bool is_valid;
    };
    
    static ParsedCommand parse(int argc, char* argv[]);
    static bool validate_command(const ParsedCommand& cmd);
};
```

```cpp
// include/cli/help_display.h
class HelpDisplay {
public:
    static void show_help();
    static void show_version();
    static void show_usage_examples();
};
```

#### 2.2 Создание Application класса
```cpp
// include/core/application.h
class Application {
private:
    std::unique_ptr<modern::Logger> logger_;
    std::unique_ptr<ConfigManager> config_manager_;
    
public:
    Application();
    int run(int argc, char* argv[]);
    
private:
    int handle_help_command();
    int handle_version_command();
    int handle_scan_command(const std::string& blockchain, const std::string& blocks);
    int handle_show_command(const std::string& type, const std::string& blockchain);
};
```

### 3. Создание интерфейсов

#### 3.1 Базовые интерфейсы
```cpp
// include/interfaces/iblockchain_scanner.h
class IBlockchainScanner {
public:
    virtual ~IBlockchainScanner() = default;
    virtual Result<std::vector<DexInfo>> scan_blockchain(
        const std::string& blockchain, 
        uint64_t start_block, 
        uint64_t end_block) = 0;
};

// include/interfaces/idex_analyzer.h
class IDexAnalyzer {
public:
    virtual ~IDexAnalyzer() = default;
    virtual Result<std::vector<PoolInfo>> analyze_dex_pools(
        const std::string& blockchain, 
        const std::string& dex_name) = 0;
};

// include/interfaces/irpc_client.h
class IRpcClient {
public:
    virtual ~IRpcClient() = default;
    virtual Result<std::string> make_request(
        const std::string& endpoint, 
        const std::string& method, 
        const std::string& params) = 0;
};
```

### 4. Улучшение тестирования

#### 4.1 Создание C++ unit тестов
```bash
mkdir -p tests/cpp tests/integration tests/performance
```

```cpp
// tests/cpp/test_command_parser.cpp
#include <gtest/gtest.h>
#include "cli/command_parser.h"

class CommandParserTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CommandParserTest, ParseHelpCommand) {
    const char* argv[] = {"program", "-h"};
    auto result = CommandParser::parse(2, const_cast<char**>(argv));
    
    EXPECT_TRUE(result.is_valid);
    EXPECT_EQ(result.flag, "-h");
    EXPECT_TRUE(result.blockchain.empty());
    EXPECT_TRUE(result.value.empty());
}
```

#### 4.2 Настройка Google Test
```cmake
# В CMakeLists.txt добавить:
find_package(GTest REQUIRED)

# Для тестов:
target_link_libraries(NeoZorKDEXArbTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main
)
```

### 5. Модернизация до C++23

#### 5.1 Использование std::expected
```cpp
// Заменить Result<T> на std::expected
#include <expected>

// Было:
Result<std::vector<DexInfo>> scan_blockchain(...)

// Стало:
std::expected<std::vector<DexInfo>, std::string> scan_blockchain(...)
```

#### 5.2 Использование std::string_view
```cpp
// Заменить const std::string& на std::string_view
// Было:
void log(const std::string& message)

// Стало:
void log(std::string_view message)
```

#### 5.3 Использование std::format
```cpp
// Заменить конкатенацию строк на std::format
// Было:
std::string message = "Error: " + error + " at line " + std::to_string(line);

// Стало:
std::string message = std::format("Error: {} at line {}", error, line);
```

## 📋 План выполнения

### Неделя 1
- [ ] Исправить предупреждения компилятора
- [ ] Создать CLI модуль
- [ ] Настроить Google Test
- [ ] Создать базовые интерфейсы

### Неделя 2
- [ ] Разделить main.cpp
- [ ] Создать Application класс
- [ ] Написать C++ unit тесты
- [ ] Обновить CMake конфигурацию

## 🎯 Ожидаемые результаты

### После выполнения:
- ✅ Устранены все предупреждения компилятора
- ✅ Улучшена архитектура (разделение ответственности)
- ✅ Добавлены интерфейсы для тестирования
- ✅ Увеличено покрытие тестами
- ✅ Использование современного C++23

### Метрики улучшения:
- **Размер main.cpp**: с 279 до ~50 строк
- **Покрытие тестами**: с 47 Python тестов до 47+ C++ тестов
- **Предупреждения**: с 5 до 0
- **Архитектура**: модульная вместо монолитной

## 🔧 Команды для выполнения

```bash
# 1. Создание структуры
mkdir -p src/cli src/core src/di include/cli include/core include/di include/interfaces
mkdir -p tests/cpp tests/integration tests/performance

# 2. Обновление зависимостей
# Добавить в vcpkg.json:
{
  "name": "gtest",
  "version>=": "1.14.0"
}

# 3. Сборка и тестирование
cd build && cmake .. && make -j$(sysctl -n hw.ncpu)
ctest --verbose
```

## 📚 Дополнительные ресурсы

- [C++23 Features](https://en.cppreference.com/w/cpp/23)
- [Google Test Documentation](https://google.github.io/googletest/)
- [Modern CMake Practices](https://cliutils.gitlab.io/modern-cmake/)
- [Dependency Injection in C++](https://github.com/google/fruit)
