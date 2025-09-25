#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <limits> // для numeric_limits
using namespace std;

// Базовый класс "Артефакт"
class Artefact {
public:
    string content; // Содержание
    int year;       // Год создания

    Artefact(const string& c, int y) : content(c), year(y) {}

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
class ArtefactContainer {
private:
    vector<unique_ptr<Artefact>> container;

public:
    void addObject(unique_ptr<Artefact> obj) {
        container.push_back(move(obj));
    }

    void printContainer() const {
        if (container.empty()) {
            cout << "Контейнер пуст!" << endl;
        }
        else {
            for (const auto& obj : container) {
                obj->print();
            }
        }
    }

    void removeObject() {
        cout << "Выберите поле для удаления объектов:\n";
        cout << "1 - Содержание\n2 - Автор (только афоризмы)\n3 - Страна (только пословицы)\n4 - Год\n";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очищаем буфер

        string keyword;
        int yearKey = 0;
        if (choice == 4) {
            cout << "Введите год: ";
            cin >> yearKey;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cout << "Введите ключевое слово: ";
            getline(cin, keyword);
        }

        container.erase(
            remove_if(container.begin(), container.end(),
                [&](const unique_ptr<Artefact>& obj) {
                    switch (choice) {
                    case 1: return obj->content.find(keyword) != string::npos;
                    case 2:
                        if (Aphorism* a = dynamic_cast<Aphorism*>(obj.get()))
                            return a->author.find(keyword) != string::npos;
                        return false;
                    case 3:
                        if (Proverb* p = dynamic_cast<Proverb*>(obj.get()))
                            return p->country.find(keyword) != string::npos;
                        return false;
                    case 4: return obj->year == yearKey;
                    default:
                        cout << "Неверный выбор поля!" << endl;
                        return false;
                    }
                }),
            container.end());
    }
};

// Функция для добавления объекта
void addObject(ArtefactContainer& artefactContainer) {
    int type;
    cout << "Выберите тип объекта: 1 - Афоризм, 2 - Пословица: ";
    cin >> type;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string content;
    cout << "Введите содержание: ";
    getline(cin, content);

    int year;
    cout << "Введите год создания: ";
    cin >> year;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (type == 1) {
        string author;
        cout << "Введите автора: ";
        getline(cin, author);
        artefactContainer.addObject(make_unique<Aphorism>(content, author, year));
    }
    else if (type == 2) {
        string country;
        cout << "Введите страну: ";
        getline(cin, country);
        artefactContainer.addObject(make_unique<Proverb>(content, country, year));
    }
    else {
        cout << "Неверный тип объекта!" << endl;
    }
}

// Главная функция
int main() {
    setlocale(0, "");
    ArtefactContainer artefactContainer;

    cout << "Программа Кладезь мудрости\n";
    cout << "Доступные команды: ADD, REM, PRINT, EXIT\n";

    string command;
    while (true) {
        cout << "\nВведите команду: ";
        getline(cin, command); // теперь корректно читаем команду

        if (command == "ADD") addObject(artefactContainer);
        else if (command == "REM") artefactContainer.removeObject();
        else if (command == "PRINT") artefactContainer.printContainer();
        else if (command == "EXIT") break;
        else cout << "Неизвестная команда!" << endl;
    }

    return 0;
}
