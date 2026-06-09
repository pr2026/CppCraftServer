![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![Qt](https://img.shields.io/badge/Qt-6.0%2B-green.svg)
![Docker](https://img.shields.io/badge/Docker-Sandbox-blue.svg)
![SQLite](https://img.shields.io/badge/Database-SQLite-lightgrey.svg)
![Drogon](https://img.shields.io/badge/framework-Drogon-orange)

# CppCraft — десктопное приложение для обучения программированию на C++.

Это репозиторий для серверной части.
Проект был разработан студентами 1 курса ПМИ НИУ ВШЭ СПб. Цель проекта - разработать учебную платформу для изучения C++ с возможностью автоматической проверки решений.

## Возможности:
### Для студента:
* Регистрация
* Авторизация
* Редактор кода с подсветкой синтаксиса на C++
* Отправка решений на проверку 
* Получение результата по решениям
* Просмотр статистики по решенным заданиям

### Для преподавателя:
* Регистрация 
* Авторизация
* Возможность добавлять, редактировать и удалять задачи
* Возможность добавлять тесты к задачам
* Просмотр статистики по всем задачам

---
## Технологический стек
* **Язык:** `C++17`
* `Drogon` - веб-фреймворк
* `SQLite` - база данных
* `Docker` - изоляция выполнения кода
* `Cmake` - система сборки
---

## Требования
* компилятор `C++17`
- `Drogon` (веб-фреймворк)
- `SQLite3`
- `Docker` (для раннера)
- `CMake` (3.16+)


##  Компоненты системы
### Сервер:
- `UserController` — регистрация и аутентификация
- `TasksController` — создание, чтение, обновление и удаление задач
- `SubmissionController` — приём и проверка решений
- `StatisticsController` — сбор и выдача статистики
   
### База данных:
- **Таблицы:**
  - `users` — пользователи (id, username, password_hash, role)
  - `tasks` — задачи (id, title, description, difficulty, owner_id)
  - `tests` — тесты (id, task_id, input, expected_output)
  - `submissions` — решения (id, user_id, task_id, code, result, timestamp)

### Раннер:
- Изоляция через Docker-контейнер
- Автоматическое сравнение вывода с эталонными тестами

---

## Структура проекта
```bash
CppCraft_Server/
├── controllers/               # Контроллеры для обработки HTTP запросов           
├── database_runner/           # Модуль для работы с Docker
│   ├── src/
│   ├── include/
│   ├── test/
│   └── CMakeLists.txt          
├── models/                    # Модели данных
├── static/                    # Статические файлы
├── storage/                   # База данных
├── test/                      # Тесты
└── CMakeLists.txt
```

## Команда 
1. Балакшина Ярославна [@YaraBalak](https://github.com/YaraBalak) - сервер
2. Рязанская Вероника [@veronica2102](https://github.com/veronica2102) - раннер и базы данных
3. Ярыгина Софья [@sonya-sonya](https://github.com/sonya-sonya) - клиент
4. Виноградов Илья [@domosedy](https://github.com/domosedy) - ментор
