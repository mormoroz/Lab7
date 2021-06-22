
#include <iostream>
#include <string>
#include <cstring> // использование функций для работы со строками
#include <algorithm> // испольование функции swap()
#include <fstream> // работа с файлами
#include <Windows.h> // для русификации 
#include <locale>
#include <deque> // контейнер deque
using namespace std;

// ---------------------------------------------- Вспомогательные функции (начало)
// функция вывода меню
void print_menu() {
    cout << "1. Добавить новый элемент в начало" << endl;
    cout << "2. Добавить новый элемент в конец" << endl;
    cout << "3. Удалить элемент из начала списка" << endl;
    cout << "4. Распечатать базу данных" << endl;
    cout << "5. Поиск врача по ФИО" << endl;
    cout << "6. Фильтр по квалификации и специальности (найти врачей заданной специальности и квалификации)" << endl;
    cout << "7. Сортировать по алфавиту" << endl;
    cout << "8. Выход из программы" << endl;
    cout << ">";
}
// функция проверки корректности ввода пункта меню / специальности / квалификации
int check_input(char choice[]) {
    int var = -1;
    int str;
    bool flag = false;
    while (flag == false) {
        try {
            cin >> str;
            if (strcmp(choice, "spec") == 0) {
                if (!cin.fail()) {
                    if (1 <= str && str <= 3) {
                        flag = true;
                        break;
                    }
                    else {
                        throw invalid_argument("Ошибка: введенное число не соответствует диапазону! Попробуйте снова\n>");
                    }
                }
                else {
                    throw invalid_argument("Ошибка: на вход ожидалось целое число! Попробуйте снова\n>");
                }
            }
            if (strcmp(choice, "qual") == 0) {
                if (!cin.fail()) {
                    if (0 <= str && str <= 2) {
                        flag = true;
                        break;
                    }
                    else {
                        throw invalid_argument("Ошибка: введенное число не соответствует диапазону! Попробуйте снова\n>");
                    }
                }
                else {
                    throw invalid_argument("Ошибка: на вход ожидалось целое число! Попробуйте снова\n>");
                }
            }
        }
        catch (invalid_argument e) {
            cout << e.what();
            cin.clear();
            cin.ignore(cin.rdbuf()->in_avail());
        };
    }
    return str;
}



// функция ввода пункта меню / специальности / квалификации
int get_variant() {
    bool flag = false;
    int ans;
    while (flag == false) {
        try {
            cin >> ans;
            if (!cin.fail()) {
                if (1 <= ans && ans <= 8) {
                    flag = true;
                    break;
                }
                else {
                    throw invalid_argument("Ошибка: введенное число не соответствует диапазону! Попробуйте снова\n>");
                }
            }
            else {
                throw invalid_argument("Ошибка: на вход ожидалось целое число! Попробуйте снова\n>");
            }
        }
        catch (invalid_argument e) {
            cout << e.what();
            cin.clear();
            cin.ignore(cin.rdbuf()->in_avail());
        };
    }
    return ans;
}

// функция вывода шапки базы данных для врачей
void print_title_doc() {
    cout << "№:\t";
    cout << "Специальность:\t";
    cout << "Квалификация:\t";
    cout << "ФИО врача:\t";
    cout << endl;
    cout << "-------------------------------------------------------------------------" << endl;
}


// ---------------------------------------------- Класс Full_name (начало) 
class Full_name {
private:
    char surname[50]; // фамилия
    char name[50];  // имя
    char patronymic[50]; // отчество
public:
    Full_name()
    {
        cout << "Full_name init" << endl;
    }
    friend class Doctor;
    friend class Clinic;
    friend bool operator == (const Full_name&, const Full_name&);

    // перезагрузка оператора >> для ФИО
    friend istream& operator >> (istream& input, Full_name& fio) {
        cout << "Введите ФИО : ";
        cin >> fio.surname;
        cin >> fio.name;
        cin >> fio.patronymic;
        return input;
    }

    // перегрузка оператора << для ФИО
    friend ostream& operator << (ostream& output, Full_name& fio) {
        cout << fio.surname << " " << fio.name << " " << fio.patronymic;
        return output;
    }
};

// перезагрузка оператора ==
bool operator == (const Full_name& fio1, const Full_name& fio2) {
    bool ans = false;
    if (strcmp(fio1.surname, fio2.surname) == 0 &&
        strcmp(fio1.name, fio2.name) == 0 &&
        strcmp(fio1.patronymic, fio2.patronymic) == 0) {
        ans = true;
    }
    return ans;
}



// ---------------------------------------------- Класс Clinic (начало)
class Clinic {
private:
    int specialty; // специальность (1-терапевт, 2-кардиолог, 3-стоматолог)
    int qualification; // квалификация (0-высшая, 1-первая, 2-вторая)
    Full_name fio;
public:
    friend class Doctor;
    friend class Full_name;
    // перезагрузка оператора >>
    friend std::istream& operator >> (std::istream& input, Clinic& doc)
    {
        cout << "Специализация: ";
        char choice1[] = "spec";
        doc.specialty = check_input(choice1);
        cout << "Квалификация: ";
        char choice2[] = "qual";
        doc.qualification = check_input(choice2);
        cin >> doc.fio;
        return input;
    }
    friend ostream& operator << (ostream& output, Clinic& doc)
    {
        cout << doc.specialty << "\t\t";
        cout << doc.qualification << "\t\t";
        cout << doc.fio << "\t\t";
        cout << endl;
        return output;
    }
};



// ---------------------------------------------- Класс Clinic (конец)



// ---------------------------------------------- Класс Doctor (начало)

class Doctor {
private:
    std::deque<Clinic> clinics;
public:
    Doctor() {
        deque<Clinic> myDeque(0);
    }
    void reading_DB_from_file();
    void print_DB();
    void search_by_full_name();
    void search_by_spec_and_qual();
    void sort_by_alphabet();
    void writing_DB_to_file();
    void add_begin();
    void add_end();
    void delete_begin();
};




// функция чтения базы данных из файла 
void Doctor::reading_DB_from_file() {
    fstream f_in;
    f_in.open("BD1.txt", ios::in);
    if (f_in.peek() == EOF) {
        cout << "Файл пуст. Приступаем к заполнению БД." << endl;
    }
    else {
        cout << "В БД уже есть записи. Считываю и распечатываю данные." << endl;
        int id; // номер доктора в таблице
        while (!f_in.eof()) {
            f_in >> id;
            Clinic new_clinic;
            f_in >> new_clinic.specialty >> new_clinic.qualification;
            f_in >> new_clinic.fio.surname >> new_clinic.fio.name >> new_clinic.fio.patronymic;
            clinics.push_back(new_clinic);
        }
        clinics.pop_back();
        print_DB();
    }
    f_in.close();
}

//добавление в начало списка
void Doctor::add_begin() {
    Clinic new_clinic;
    cout << "ФИО врача: ";
    cin >> new_clinic.fio.surname;
    cin >> new_clinic.fio.name;
    cin >> new_clinic.fio.patronymic;
    cout << "Специализация(1-терапевт, 2-кардиолог, 3-стоматолог): ";
    cin >> new_clinic.specialty;
    cout << "Квалификация(0-высшая, 1-первая, 2-вторая): ";
    cin >> new_clinic.qualification;
    clinics.push_front(new_clinic);
}

//добавление в конец очереди
void Doctor::add_end() {
    Clinic new_clinic;
    cout << "ФИО врача: ";
    cin >> new_clinic.fio.surname;
    cin >> new_clinic.fio.name;
    cin >> new_clinic.fio.patronymic;
    cout << "Специализация(1-терапевт, 2-кардиолог, 3-стоматолог): ";
    cin >> new_clinic.specialty;
    cout << "Квалификация(0-высшая, 1-первая, 2-вторая): ";
    cin >> new_clinic.qualification;
    clinics.push_back(new_clinic);
}

//удаление из начала списка
void Doctor::delete_begin() {
    clinics.pop_front();
    cout << "Успешное удаление!" << endl;
    print_DB();
}


// функция вывода базы данных
void Doctor::print_DB() {
    print_title_doc();
    for (int i = 0; i < clinics.size(); i++) {
        cout << i << "\t";
        cout << clinics[i];
    }
    cout << endl;
}

// функция поиска докторов по ФИО
void Doctor::search_by_full_name() {
    cout << "Введите ФИО интересующего врача: ";
    Full_name new_full_name;
    cin >> new_full_name.surname;
    cin >> new_full_name.name;
    cin >> new_full_name.patronymic;
    int flag = 0; // флаг для подсчета кол-ва совпадений
    for (int i = 0; i < clinics.size(); i++) {
        if (clinics[i].fio == new_full_name) {
            flag += 1;
            cout << "Найдено совпадение:" << endl;
            print_title_doc();
            cout << i << "\t";
            cout << clinics[i];
        }
    }
    if (flag == 0) {
        cout << "Не найдено совпадений :(" << endl;
    }
}

// функция поиска докторов по специальности и квалификации 
void Doctor::search_by_spec_and_qual() {
    cout << "Введите специальность: ";
    char choice1[] = "spec";
    int new_spec = get_variant();
    cout << "Введите квалификацию: ";
    char choice2[] = "qual";
    int new_qual = get_variant();
    int flag = 0; // флаг для подсчета кол-ва совпадений
    for (int i = 0; i < clinics.size(); i++) {
        if (clinics[i].specialty == new_spec && clinics[i].qualification == new_qual) {
            flag += 1;
            cout << "Найдено совпадение:" << endl;
            print_title_doc();
            cout << i << "\t";
            cout << clinics[i];
        }
    }
    if (flag == 0) {
        cout << "Не найдено совпадений :(" << endl;
    }
}

// функция сортировки базы данных по фамилии доктора
void Doctor::sort_by_alphabet() {
    cout << "Отсортированная база данных по фамилии врача: " << endl;
    // Сортировка
    for (int i = 0; i < clinics.size() - 1; i++) {
        for (int j = 0; j < clinics.size() - i - 1; j++) {
            if (clinics[j].fio.surname[0] > clinics[j + 1].fio.surname[0]) {
                swap(clinics[j], clinics[j + 1]);
            }
        }
    }
    print_DB();
}

// функция записи базы данных в файл
void Doctor::writing_DB_to_file() {
    fstream f_out;
    f_out.open("BD1.txt", ios::out);
    for (int i = 0; i < clinics.size(); i++) {
        f_out << i << " ";
        f_out << clinics[i].specialty << " ";
        f_out << clinics[i].qualification << " ";
        f_out << clinics[i].fio.surname << " " << clinics[i].fio.name << " " << clinics[i].fio.patronymic << " ";
    }
    f_out << endl;
    f_out.close();
}


// функция работы меню
void menu(Doctor* first_clinic) {
    int var; // выбранный пункт меню
    do {
        print_menu(); // выводим меню на экран
        //char choice[] = "menu";
        var = get_variant(); // получаем номер выбранного пункта меню
        switch (var) {
        case 1:
            first_clinic->add_begin();
            break;
        case 2:
            first_clinic->add_end();
            break;
        case 3:
            first_clinic->delete_begin();
            break;
        case 4:
            first_clinic->print_DB();
            break;
        case 5:
            first_clinic->search_by_full_name();
            break;
        case 6:
            first_clinic->search_by_spec_and_qual();
            break;
        case 7:
            first_clinic->sort_by_alphabet();
            break;
        }
        if (var != 8)
            system("pause"); // задерживаем выполнение
    } while (var != 8);
}
// ---------------------------------------------- Класс Doctor (конец)
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    Doctor first_doctor;
    first_doctor.reading_DB_from_file();
    menu(&first_doctor);
    first_doctor.writing_DB_to_file();
}






