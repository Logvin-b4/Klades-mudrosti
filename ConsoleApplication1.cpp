#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Для std::remove_if
using namespace std;

// Базовый класс "Артефакт"
class Artefact {
public:
    string content; // Содержание
    int year;       // Год создания

    Artefact(const string& c, int y) : content(c), year(y) {}

    // Виртуальная функция для вывода информации
    virtual void print() const {
        cout << "Содержание: " << content << " | Год: " << year << endl;
    }

    virtual ~Artefact() {}
};

// Класс "Афоризм"
class Aphorism : public Artefact {
public:
    string author; // Автор

    Aphorism(const string& c, const string& a, int y) : Artefact(c, y), author(a) {}

    void print() const override {
        cout << "Афоризм | Автор: " << author << " | Содержание: " << content << " | Год: " << year << endl;
    }
};

// Класс "Пословица"
class Proverb : public Artefact {
public:
    string country; // Страна

    Proverb(const string& c, const string& ctr, int y) : Artefact(c, y), country(ctr) {}

    void print() const override {
        cout << "Пословица | Страна: " << country << " | Содержание: " << content << " | Год: " << year << endl;
    }
};

// Контейнер для хранения артефактов
vector<Artefact*> container;

// Функция для добавления объекта
void addObject() {
    cout << "Выберите тип объекта: 1 - Афоризм, 2 - Пословица: ";
    int type;
    cin >> type;
    cin.ignore(); // очистка буфера

    string content, extra;
    int year;

    cout << "Введите содержание: ";
    getline(cin, content);

    cout << "Введите год создания: ";
    cin >> year;
    cin.ignore();

    if (type == 1) {
        cout << "Введите автора: ";
        getline(cin, extra);
        container.push_back(new Aphorism(content, extra, year));
    }
    else if (type == 2) {
        cout << "Введите страну: ";
        getline(cin, extra);
        container.push_back(new Proverb(content, extra, year));
    }
    else {
        cout << "Неверный тип объекта!" << endl;
    }
}

// Функция для удаления объекта
void removeObject() {
    cout << "Выберите поле для удаления объектов:\n";
    cout << "1 - Содержание\n2 - Автор (только афоризмы)\n3 - Страна (только пословицы)\n4 - Год\n";
    int choice;
    cin >> choice;
    cin.ignore();

    string keyword;
    int yearKey = 0;
    if (choice == 4) {
        cout << "Введите год: ";
        cin >> yearKey;
        cin.ignore();
    }   
    else {
        cout << "Введите ключевое слово: ";
        getline(cin, keyword);
    }

    container.erase(
        remove_if(container.begin(), container.end(),
            [&](Artefact* obj) {
                bool toDelete = false;
                switch (choice) {
                case 1:
                    if (obj->content.find(keyword) != string::npos) toDelete = true;
                    break;
                case 2:
                    if (Aphorism* a = dynamic_cast<Aphorism*>(obj)) {
                        if (a->author.find(keyword) != string::npos) toDelete = true;
                    }
                    break;
                case 3:
                    if (Proverb* p = dynamic_cast<Proverb*>(obj)) {
                        if (p->country.find(keyword) != string::npos) toDelete = true;
                    }
                    break;
                case 4:
                    if (obj->year == yearKey) toDelete = true;
                    break;
                default:
                    cout << "Неверный выбор поля!" << endl;
                    break;
                }
                if (toDelete) delete obj;
                return toDelete;
            }),
        container.end());
}

// Функция для вывода контейнера
void printContainer() {
    if (container.empty()) {
        cout << "Контейнер пуст!" << endl;
    }
    else {
        for (auto obj : container) {
            obj->print();
        }
    }
}

int main() {
    setlocale(0, "");
    string command;

    cout << "Программа Кладезь мудрости\n";
    cout << "Доступные команды: ADD, REM, PRINT, EXIT\n";

    while (true) {
        cout << "\nВведите команду: ";
        cin >> command;

        if (command == "ADD") addObject();
        else if (command == "REM") removeObject();
        else if (command == "PRINT") printContainer();
        else if (command == "EXIT") break;
        else cout << "Неизвестная команда!" << endl;
    }

    // Очистка памяти
    for (auto obj : container) delete obj;

    return 0;
}
