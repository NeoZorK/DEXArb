# Ubuntu Container Runner для NeoZorKDEXArb

Этот документ описывает, как запустить приложение NeoZorKDEXArb в Ubuntu Linux контейнере с использованием Apple Container CLI.

## 🚀 Быстрый старт

### Предварительные требования

1. **Apple Container CLI** - должен быть установлен и доступен в системе
2. **macOS** - для использования Apple Container CLI
3. **Интернет соединение** - для загрузки Ubuntu образа и зависимостей

### Запуск приложения

```bash
# Показать справку
./scripts/container/run-ubuntu-container.sh --help

# Запустить приложение (покажет справку)
./scripts/container/run-ubuntu-container.sh

# Показать версию
./scripts/container/run-ubuntu-container.sh -- -v

# Показать DEXes для Ethereum
./scripts/container/run-ubuntu-container.sh -- -showDEXES ethereum

# Просканировать Ethereum (1000 блоков)
./scripts/container/run-ubuntu-container.sh -- -scan ethereum 1000
```

## 🔧 Сборка для Ubuntu

### Автоматическая сборка

Скрипт автоматически собирает приложение для Ubuntu при первом запуске:

```bash
# Принудительная пересборка
./scripts/container/run-ubuntu-container.sh --build
```

### Процесс сборки

1. **Загрузка Ubuntu 24.04 (Noble)** - современная версия с glibc
2. **Установка зависимостей:**
   - build-essential (компилятор GCC)
   - cmake (сборка)
   - libcurl4-openssl-dev (HTTP клиент)
   - nlohmann-json3-dev (JSON парсинг)
   - git, wget, software-properties-common
3. **Обновление CMake** до версии 3.28+ (если необходимо)
4. **Сборка приложения** с помощью CMake
5. **Установка runtime зависимостей** (libcurl4, libstdc++6)

## 📁 Структура файлов

```
build-ubuntu/
├── bin/
│   └── NeoZorKDEXArb          # Исполняемый файл для Ubuntu
├── CMakeFiles/                 # Файлы CMake
└── ...                         # Другие файлы сборки
```

## 🌐 Поддерживаемые команды

### Основные команды

| Команда | Описание | Пример |
|---------|----------|---------|
| `-h`, `-help` | Показать справку | `./run-ubuntu-container.sh -- -h` |
| `-v`, `-version` | Показать версию | `./run-ubuntu-container.sh -- -v` |
| `-scan` | Сканировать блоки | `./run-ubuntu-container.sh -- -scan ethereum 5000` |
| `-showDEXES` | Показать DEXes | `./run-ubuntu-container.sh -- -showDEXES bsc` |
| `-showPOOLS` | Показать пулы | `./run-ubuntu-container.sh -- -showPOOLS ethereum uniswap` |
| `-showTOKENS` | Показать токены | `./run-ubuntu-container.sh -- -showTOKENS ethereum uniswap` |

### Поддерживаемые блокчейны

- **Ethereum** - основная сеть
- **BSC** - Binance Smart Chain
- **Polygon** - Polygon Network
- **Avalanche** - Avalanche C-Chain
- **Fantom** - Fantom Opera
- **Solana** - Solana Network

## ⚡ Производительность

### Рекомендации по сканированию

- **Тестирование**: 1,000 - 5,000 блоков
- **Обнаружение**: 10,000 - 50,000 блоков
- **Глубокий анализ**: 100,000+ блоков

### Оптимизация

- Используйте небольшие диапазоны блоков для тестирования
- Увеличивайте диапазон постепенно
- Мониторьте использование ресурсов

## 🐛 Устранение неполадок

### Частые проблемы

1. **CMake версия слишком старая**
   - Скрипт автоматически обновляет CMake до 3.28+
   - Используется Ubuntu 24.04 (Noble) по умолчанию

2. **Отсутствуют зависимости**
   - Скрипт автоматически устанавливает все необходимые пакеты
   - Проверьте интернет соединение

3. **Ошибки сборки**
   - Убедитесь, что у вас достаточно места на диске
   - Проверьте права доступа к директории

### Логи

Все операции логируются с временными метками:
```
[2025-08-10 21:59:39] INFO: Ubuntu build completed: build-ubuntu/bin/NeoZorKDEXArb
[2025-08-10 21:59:39] INFO: Starting NeoZorKDEXArb in Ubuntu Linux container...
```

## 🔄 Обновления

### Пересборка приложения

```bash
# Удалить старую сборку
rm -rf build-ubuntu/

# Пересобрать
./scripts/container/run-ubuntu-container.sh --build
```

### Обновление скрипта

```bash
# Сделать скрипт исполняемым
chmod +x scripts/container/run-ubuntu-container.sh

# Проверить версию Apple Container CLI
container --version
```

## 📚 Дополнительные ресурсы

- [Alpine Container Runner](ALPINE_CONTAINER_RUNNER.md) - альтернативный способ запуска
- [Build and Usage](BUILD_AND_USAGE.md) - общая информация о сборке
- [Project Description](../development/PROJECT_DESCRIPTION.md) - описание проекта

## 🤝 Поддержка

При возникновении проблем:

1. Проверьте логи выполнения
2. Убедитесь в наличии всех зависимостей
3. Проверьте версию Apple Container CLI
4. Обратитесь к документации проекта

---

**Создано**: 26.02.2025  
**Автор**: Rostyslav S.  
**Версия**: 1.0.7
