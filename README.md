# URL Shortener Service

Простой сервис для сокращения URL с возможностью редиректа на оригинальный URL. Реализован на C++ с использованием **[userver](https://github.com/avito-tech/cpp_userver)**.

---

## Основные возможности

* Создание короткого URL для заданного длинного URL.
* Редирект с короткого URL на исходный URL.
* Проверка формата исходного URL.
* Обработка ошибок (некорректный URL, несуществующий короткий URL, ошибки базы данных).
* Хранение данных в PostgreSQL.

---

## Архитектура

* `handlers/` – обработчики HTTP-запросов (`create_short_url`, `redirect`).
* `service/` – логика работы с короткими URL (`ShortenerService`).
* `db/` – репозитории для работы с PostgreSQL (`RepositoryPostgres`).
* `utils/` – вспомогательные функции: проверка URL, генерация ID, обработка ошибок.
* `main.cpp` – точка входа, регистрация обработчиков.

---

## API

### 1. Создание короткого URL

**Endpoint:** `POST /create`
**Content-Type:** `application/json`

**Пример запроса:**

```bash
curl -X POST http://localhost:8080/create \
     -H "Content-Type: application/json" \
     -d '{"original_url": "https://example.com"}'
```

**Пример ответа:**

```json
{
  "short_url": "8521303url"
}
```

---

### 2. Редирект на оригинальный URL

**Endpoint:** `GET /redirect/{short_url}`

**Пример запроса:**

```bash
curl -X GET http://localhost:8080/redirect/8521303url
```

* Если короткий URL найден, возвращается HTTP 302 с заголовком `Location` на оригинальный URL.
* Если короткий URL не найден, возвращается HTTP 404.
* Если формат короткого URL некорректный, возвращается HTTP 400.

---

## Примечания

* Убедитесь, что база данных создана и настроена таблица для хранения URL.
* Короткие URL формируются автоматически через генератор ID.
* Проверка валидности URL применяется только при создании короткого URL.

---

## Пример использования

```bash
# Создать короткий URL
curl -X POST http://localhost:8080/create \
     -H "Content-Type: application/json" \
     -d '{"original_url": "https://example.com"}'

# Редирект на оригинальный URL
curl -X GET http://localhost:8080/redirect/8521303url
```
