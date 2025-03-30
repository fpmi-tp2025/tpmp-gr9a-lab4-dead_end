#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <assert.h>
#include "../include/MusicShopDB.h"

// Константы из вашего кода
#define ATTEMPTS_AMOUNT 3
#define OWNER_RIGHTS 1
#define CLIENT_RIGHTS 0

// Глобальные переменные из вашего кода
extern const char* ownerLogin;
extern const char* ownerPassword;
extern int accessRights;

// Объявляем функции, которые определены в MusicShopDB.c
void Authorization(void);
int checkDate(char date[]);

// ================== ТЕСТОВЫЕ ФУНКЦИИ ==================

void setup_test_db(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        exit(1);
    }

    const char *sql =
        "CREATE TABLE CompactDisk(id INTEGER PRIMARY KEY, date TEXT, company TEXT, price REAL);"
        "CREATE TABLE MusicalComposition(name TEXT, author TEXT, performer TEXT, compactID INTEGER);"
        "CREATE TABLE Trade(date TEXT, compactID INTEGER, amount INTEGER, code INTEGER);"
        "CREATE TABLE TradeCodeInfo(code INTEGER PRIMARY KEY, info TEXT);"
        
        "INSERT INTO CompactDisk VALUES(1, '2020.01.01', 'Sony', 10.5);"
        "INSERT INTO CompactDisk VALUES(2, '2020.02.15', 'Warner', 15.75);"
        "INSERT INTO CompactDisk VALUES(3, '2021.03.20', 'Universal', 12.0);"
        
        "INSERT INTO MusicalComposition VALUES('Song1', 'Author1', 'Performer1', 1);"
        "INSERT INTO MusicalComposition VALUES('Song2', 'Author2', 'Performer1', 2);"
        "INSERT INTO MusicalComposition VALUES('Song3', 'Author3', 'Performer2', 3);"
        
        "INSERT INTO Trade VALUES('2020.03.01', 1, 5, 1);"
        "INSERT INTO Trade VALUES('2020.03.02', 2, 3, 1);"
        "INSERT INTO Trade VALUES('2020.03.03', 1, 2, 2);"
        "INSERT INTO Trade VALUES('2021.04.01', 3, 4, 1);"
        
        "INSERT INTO TradeCodeInfo VALUES(1, 'Sale');"
        "INSERT INTO TradeCodeInfo VALUES(2, 'Purchase');";
    
    char *err_msg = 0;
    rc = sqlite3_exec(*db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        exit(1);
    }
}

void test_authorization() {
    printf("\n=== ТЕСТ АВТОРИЗАЦИИ ===\n");
    
    // Тест 1: Вход владельца
    printf("Тест 1: Вход владельца\n");
    // Мокаем ввод
    freopen("test_input_owner.txt", "w+", stdin);
    fprintf(stdin, "Admin\n1111\n");
    rewind(stdin);
    
    Authorization();
    assert(accessRights == OWNER_RIGHTS);
    fclose(stdin);
    
    // Тест 2: Вход клиента
    printf("Тест 2: Вход клиента\n");
    freopen("test_input_client.txt", "w+", stdin);
    fprintf(stdin, "Client\nanypass\n");
    rewind(stdin);
    
    Authorization();
    assert(accessRights == CLIENT_RIGHTS);
    fclose(stdin);
    
    // Восстанавливаем stdin
    freopen("/dev/tty", "r", stdin);
    
    // Удаляем временные файлы
    remove("test_input_owner.txt");
    remove("test_input_client.txt");
    
    printf("Тесты авторизации пройдены!\n");
}

void test_date_validation() {
    printf("\n=== ТЕСТ ПРОВЕРКИ ДАТЫ ===\n");
    
    assert(checkDate("2020.01.01") == 1);
    assert(checkDate("2020.02.29") == 1); // Високосный год
    assert(checkDate("2021.12.31") == 1);
    
    assert(checkDate("2020.02.30") == 0);
    assert(checkDate("1999.01.01") == 0); // Слишком ранний год
    assert(checkDate("2020.13.01") == 0); // Неправильный месяц
    assert(checkDate("2020/01/01") == 0); // Неправильный формат
    
    printf("Тесты проверки даты пройдены!\n");
}

void test_database_requests(sqlite3 *db) {
    printf("\n=== ТЕСТ ЗАПРОСОВ К БАЗЕ ДАННЫХ ===\n");
    
    printf("1. Информация о продажах:\n");
    AllCompactTradeInfo(db);
    
    printf("\n2. Самый продаваемый диск:\n");
    GetMostSoldCompactInfo(db);
    
    printf("\n3. Самый популярный исполнитель:\n");
    GetMostPopularPerformerTradeInfo(db);
    
    printf("\n4. Продажи за период:\n");
    GetPeriodTrade(db, "2020.01.01", "2021.12.31");
    
    printf("\n5. Информация о диске с ID=1:\n");
    GetCompactSoldInfo(db, 1);
    
    printf("\nПроверьте вывод запросов выше!\n");
}

void test_crud_operations(sqlite3 *db) {
    printf("\n=== ТЕСТ ОПЕРАЦИЙ CRUD ===\n");
    
    printf("1. Тест добавления (введите тестовые данные):\n");
    Insert(db, NULL, 0);
    
    printf("\n2. Тест обновления (введите тестовые данные):\n");
    Update(db, NULL, 0);
    
    printf("\n3. Тест удаления (введите тестовые данные):\n");
    Delete(db, NULL, 0);
    
    printf("\nПроверьте результаты операций!\n");
}

int main() {
    sqlite3 *db;
    
    printf("=== НАЧАЛО ТЕСТИРОВАНИЯ ===\n");
    
    setup_test_db(&db);
    
    test_authorization();
    test_date_validation();
    test_database_requests(db);
    test_crud_operations(db);
    
    sqlite3_close(db);
    printf("\n=== ТЕСТИРОВАНИЕ ЗАВЕРШЕНО ===\n");
    
    return 0;
}
