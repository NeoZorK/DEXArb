//
//  check_rpc_endpoints.cpp
//  NeoZorKDEXArb
//
//  Created by Rostyslav S. on 19.03.2025.
//

#include "check_rpc_endpoints.h"
#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <functional>

// Структура для хранения ответа от сервера
struct Response {
    std::string data; // Using std::string to avoid manual memory management
};

// Вспомогательная функция, работающая с ссылками
size_t processResponseData(const std::string& contents, Response& response) {
    response.data.append(contents);
    return contents.size();
}

// Callback-функция для обработки данных ответа от curl
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    Response& response = *static_cast<Response*>(userp); // Приводим к ссылке
    std::string data(static_cast<char*>(contents), size * nmemb); // Преобразуем void* в std::string
    return processResponseData(data, response); // Работаем только с ссылками
}

// Функция проверки доступности RPC-эндпоинта
void проверитьRpc(const std::string& url) {
    CURL* curlHandle = curl_easy_init();
    if (!curlHandle) {
        std::cerr << "Не удалось инициализировать curl для " << url << std::endl;
        return;
    }
    
    // Автоматическая очистка curlHandle с помощью RAII
    auto curlCleanup = [curlHandle]() { curl_easy_cleanup(curlHandle); };
    struct CurlGuard {
        std::function<void()> cleanup;
        ~CurlGuard() { cleanup(); }
    } guard{curlCleanup};

    Response response; // Автоматическое управление памятью через std::string

    // Настройка параметров curl для POST-запроса
    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_POST, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, "{\"jsonrpc\":\"2.0\",\"method\":\"eth_blockNumber\",\"params\":[],\"id\":1}");
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1L);

    // Установка заголовков с использованием современного C++ стиля
    std::vector<std::string> headerList = {"Content-Type: application/json"};
    struct curl_slist* headers = nullptr;
    for (const auto& header : headerList) {
        headers = curl_slist_append(headers, header.c_str());
    }
    curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, headers);

    // Автоматическая очистка headers
    struct HeaderGuard {
        curl_slist* ptr;
        ~HeaderGuard() { if (ptr) curl_slist_free_all(ptr); }
    } headerGuard{headers};

    // Выполнение запроса
    CURLcode res = curl_easy_perform(curlHandle);
    if (res != CURLE_OK) {
        std::cerr << "RPC " << url << ": Ошибка - " << curl_easy_strerror(res) << std::endl;
        return;
    }

    long httpCode = 0;
    curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &httpCode);
    if (httpCode == 200 && response.data.find("result") != std::string::npos) {
        std::cout << "RPC " << url << ": Доступен - Ответ: " << response.data << std::endl;
    } else {
        std::cerr << "RPC " << url << ": Недоступен или неверный ответ - HTTP " << httpCode
                  << ", Ответ: " << response.data << std::endl;
    }
}

void check_rpc_endpoints() {
    // Вектор RPC-эндпоинтов для проверки
    std::vector<std::string> rpcEndpoints{
        "https://rpc.ftm.tools/",
        "https://rpc.ankr.com/fantom",
        "https://fantom.publicnode.com",
        "https://fantom-mainnet-rpc.coinsdo.com",
        "https://fantom-mainnet.public.blastapi.io",
        "https://1rpc.io/ftm"
    };

    // Инициализация глобального curl с автоматической очисткой
    curl_global_init(CURL_GLOBAL_ALL);
    struct GlobalCurlGuard {
        ~GlobalCurlGuard() { curl_global_cleanup(); }
    } globalGuard;

    // Проверка каждого RPC-эндпоинта
    for (const auto& endpoint : rpcEndpoints) {
        std::cout << "Проверка " << endpoint << "..." << std::endl;
        проверитьRpc(endpoint);
    }

    return;
}
