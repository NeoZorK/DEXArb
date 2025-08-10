# Wine Docker для запуска Windows приложений

## 🚀 Быстрый старт

### 1. Запуск контейнера
```bash
./scripts/wine-docker.sh start
```

### 2. Проверка статуса
```bash
./scripts/wine-docker.sh status
```

### 3. Запуск тестов
```bash
./scripts/test-wine.sh
```

### 4. Остановка контейнера
```bash
./scripts/wine-docker.sh stop
```

## 📋 Что работает

✅ **Wine 6.0.3** - полностью функционален  
✅ **Batch файлы** - выполняются корректно  
✅ **MinGW компилятор** - для создания Windows приложений  
✅ **Windows Registry** - доступен и работает  
✅ **Файловая система** - монтирование работает  
✅ **Консольные приложения** - запускаются без проблем  

## ⚠️ Ограничения

- **wine32 недоступен** на ARM64 архитектуре (это нормально)
- **GUI приложения** могут не работать из-за отсутствия X11 сервера
- **32-битные приложения** могут иметь проблемы совместимости

## 🧪 Тестовые файлы

### Простой тест
```bash
docker exec -it wine-ubuntu wine cmd /c /app/test-simple.bat
```

### Расширенный тест
```bash
docker exec -it wine-ubuntu wine cmd /c /app/wine-test.bat
```

### C++ приложение
```bash
docker exec -it wine-ubuntu wine /app/simple-test.exe
```

## 🔧 Компиляция Windows приложений

### C++ в Windows EXE
```bash
docker exec -it wine-ubuntu x86_64-w64-mingw32-g++ -o /app/output.exe /app/source.cpp
```

### C в Windows EXE
```bash
docker exec -it wine-ubuntu x86_64-w64-mingw32-gcc -o /app/output.exe /app/source.c
```

## 📁 Структура проекта

```
├── Dockerfile.wine              # Docker образ с Wine
├── docker-compose.wine.yml      # Docker Compose конфигурация
├── scripts/
│   ├── wine-docker.sh          # Управление контейнером
│   └── test-wine.sh            # Тестирование функциональности
├── app/                         # Windows приложения
│   ├── test-simple.bat         # Простой batch тест
│   ├── wine-test.bat           # Расширенный тест
│   ├── simple-test.cpp         # C++ исходный код
│   └── simple-test.exe         # Скомпилированное приложение
└── docs/
    └── WINE_DOCKER_USAGE.md    # Подробная документация
```

## 🎯 Примеры использования

### Запуск Windows приложения
```bash
# Скопировать приложение в контейнер
docker cp my-app.exe wine-ubuntu:/app/

# Запустить через Wine
docker exec -it wine-ubuntu wine /app/my-app.exe
```

### Выполнение batch скрипта
```bash
docker exec -it wine-ubuntu wine cmd /c /app/your-script.bat
```

### Интерактивная командная строка
```bash
docker exec -it wine-ubuntu wine cmd
```

## 🔍 Диагностика

### Проверка версии Wine
```bash
docker exec -it wine-ubuntu wine --version
```

### Проверка переменных окружения
```bash
docker exec -it wine-ubuntu printenv | grep WINE
```

### Доступ к файловой системе
```bash
docker exec -it wine-ubuntu ls -la /app/
```

## 📚 Документация

- **Основное руководство**: `docs/WINE_DOCKER_USAGE.md`
- **Быстрый старт**: `QUICK_START_WINE.md`
- **Тестирование**: `scripts/test-wine.sh`

## 🆘 Устранение неполадок

### Контейнер не запускается
```bash
# Проверить Docker
docker --version
docker-compose --version

# Пересобрать образ
./scripts/wine-docker.sh stop
./scripts/wine-docker.sh start
```

### Wine не работает
```bash
# Проверить логи
docker logs wine-ubuntu

# Войти в контейнер
./scripts/wine-docker.sh shell
```

### Проблемы с файлами
```bash
# Проверить монтирование
docker exec -it wine-ubuntu ls -la /app/

# Перезапустить контейнер
./scripts/wine-docker.sh restart
```

## 🌟 Особенности

- **Автоматическая инициализация** Wine при первом запуске
- **Готовые инструменты** для компиляции Windows приложений
- **Тестовые файлы** для проверки функциональности
- **Подробная документация** на русском и английском языках
- **Скрипты управления** для удобного использования

## 📞 Поддержка

При возникновении проблем:
1. Запустите `./scripts/test-wine.sh` для диагностики
2. Проверьте логи контейнера
3. Обратитесь к документации в папке `docs/`
4. Проверьте совместимость вашего приложения с Wine

---

**🎉 Поздравляем!** Ваш Wine Docker контейнер готов к использованию для запуска Windows приложений на macOS/Linux.
