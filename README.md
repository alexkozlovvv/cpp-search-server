# Search-server

Учебный проект для закрепления навыков использования базовых понятий языка и стандартной библиотеки. С его помощью осуществляется возможность добавления, хранения и поиска текстовых документов по ключевым словам запроса.

## Структура проекта

```mermaid
    classDiagram
    class SearchServer {
        - const set~string~ stop_words_
        - map~int, DocumentData~ documents_
        - vector<int> ids_in_order_of_addition_
        ...
        + SearchServer(const string& stop_words_text)
        + SearchServer~StringContainer~(const StringContainer& stop_words)
        + AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings)
        + FindTopDocuments~Predicate~(const string& raw_query, Predicate predicate) vector~Document~
       ...()
    }
    class Document {
        + int id
        + double relevance
        + int rating
        + Document()
        + Document(int id, double relevance, int rating)
    }
    class RequestQueue { 
        - deque<QueryResult> requests_
        - const static int min_in_day_$
        - const SearchServer& local_search_server_
        - int empty_result_query_
        - uint32_t current_minut_
        + RequestQueue(const SearchServer& search_server)
        + AddFindRequest~Predicate~(const string& raw_query, Predicate predicate) vector~Document~
        ...()
    }
    Document <.. SearchServer
    RequestQueue --o SearchServer
```
<br>

### Основные структурные элементы:
- поисковый сервер - хранит стоп слова, информацию о добавленных документах и частоту упоминания конкретного слова в каждом документе
- 

## Download

Скачать репозиторий можно с помощью команды:

```
git clone git@github.com:alexkozlovvv/cpp-search-server.git
```

## Usage

На данном этапе проект не предполагает интерактивного взаимодействия с пользователем поскольку отсутствует обработка входных команд. Сейчас они задаются статически в теле программы. 

Алгоритм работы программы:
- Создается объект поискового сервера;
- С помощью контруктора передаются стоп слова; 
- Далее создается объект очереди запросов.

## Дальнейшее развитие

Необходимо добавить возможность интерактивного взаимодействия с пользователем с помощью терминала.
Добавить тесты для модульного тестирования компонентов архитектуры.


