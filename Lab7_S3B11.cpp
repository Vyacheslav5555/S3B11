#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

// Функция для выполнения SQL-команд
void executeSQL(sqlite3* db, const string& sql, const string& successMessage) {
    char* errorMessage = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK) {
        cerr << "Ошибка SQL: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    } else {
        cout << successMessage << endl;
    }
}

// Функция для отображения всех записей из таблицы
int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\t";
    }
    cout << endl;
    return 0;
}

int main() {
    sqlite3* db;
    string dbName = "contacts.db";

    // Открытие или создание базы данных
    int rc = sqlite3_open(dbName.c_str(), &db);
    if (rc) {
        cerr << "Не удалось открыть базу данных: " << sqlite3_errmsg(db) << endl;
        return 1;
    } else {
        cout << "База данных успешно открыта: " << dbName << endl;
    }

    // Создание таблицы
    string createTableSQL = "CREATE TABLE IF NOT EXISTS contacts ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "name TEXT NOT NULL, "
                            "phone TEXT, "
                            "age INTEGER, "
                            "salary REAL);";
    executeSQL(db, createTableSQL, "Таблица 'contacts' создана или уже существует.");

    // Добавление записей
    string insertDataSQL =
        "INSERT INTO contacts (name, phone, age, salary) VALUES ('Anna', '+7012345678', 25, 50000.0);"
        "INSERT INTO contacts (name, phone, age, salary) VALUES ('Bob', '+7023456789', 30, 60000.0);"
        "INSERT INTO contacts (name, phone, age, salary) VALUES ('Catherine', '+7034567890', 28, 55000.0);"
        "INSERT INTO contacts (name, phone, age, salary) VALUES ('David', '+7045678901', 35, 70000.0);"
        "INSERT INTO contacts (name, phone, age, salary) VALUES ('Eleanor', '+7056789012', 32, 65000.0);"
        "INSERT INTO contacts (name, phone, age, salary) VALUES ('Frank', '+7067890123', 29, 62000.0);";
    executeSQL(db, insertDataSQL, "Записи добавлены в таблицу.");

    // Вывод всех записей
    cout << "Содержимое таблицы 'contacts':" << endl;
    string selectAllSQL = "SELECT * FROM contacts;";
    sqlite3_exec(db, selectAllSQL.c_str(), callback, nullptr, nullptr);

    // Обновление записи
    string updateSQL = "UPDATE contacts SET phone = '+7000000000' WHERE name = 'Bob';";
    executeSQL(db, updateSQL, "Телефон для Bob обновлен.");

    // Удаление записи
    string deleteSQL = "DELETE FROM contacts WHERE name = 'Frank';";
    executeSQL(db, deleteSQL, "Запись для Frank удалена.");

    // Подсчет записей
    cout << "Количество записей в таблице:" << endl;
    string countSQL = "SELECT COUNT(*) AS Total FROM contacts;";
    sqlite3_exec(db, countSQL.c_str(), callback, nullptr, nullptr);

    // Сортировка записей по зарплате
    cout << "Записи, отсортированные по зарплате (убывание):" << endl;
    string sortSQL = "SELECT * FROM contacts ORDER BY salary DESC;";
    sqlite3_exec(db, sortSQL.c_str(), callback, nullptr, nullptr);

    // Закрытие базы данных
    sqlite3_close(db);
    cout << "База данных закрыта." << endl;

    return 0;
}
