#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <Windows.h>

using namespace std;

enum Specialty { KN, IT, ME, FI, TN };

struct Student {
    string prizv;
    int kurs;
    Specialty specialty;
    int Math;
    int Fizik;
    union {
        int Program;
        int Num_met;
        int Pedagogik;
    };
};

void Create(Student* p, const int n);
void Print(Student* p, const int n);
double Search(Student* p, const int n);
void SearchStudents(Student* p, const int n);
void WriteToFile(Student* p, const int n, const char* filename);
void ReadFromFileAndPrint(const char* filename);
void SortByCourseMathName(Student* p, const int n);
bool CompareStudents(const Student& a, const Student& b);
void BuildIndexFile(Student* p, const int n, const char* indexFilename);
int BinarySearch(Student* p, const int n, const string& prizv, const int math);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int n;
    cout << "Введіть кількість студентів: ";
    cin >> n;

    Student* p = new Student[n];

    Create(p, n);

    char sortChoice;
    cout << "Бажаєте відсортувати дані перед виведенням? (y/n): ";
    cin >> sortChoice;
    if (tolower(sortChoice) == 'y') {
        SortByCourseMathName(p, n);
    }

    Print(p, n);

    double proc = Search(p, n);
    cout << "Процент учнів, які вчаться без трійок: " << proc << " %" << endl;

    cout << "Студенти, у яких з фізики 4 або 5:" << endl;
    SearchStudents(p, n);

    const char* filename = "students.bin";
    WriteToFile(p, n, filename);

    cout << "Дані з файлу:" << endl;
    ReadFromFileAndPrint(filename);

    const char* indexFilename = "index.bin";
    BuildIndexFile(p, n, indexFilename);

    cout << "=============================" << endl;
    cout << "Пошук студента за прізвищем та оцінкою з математики:" << endl;
    string searchPrizv;
    int searchMath;
    cout << "Введіть прізвище: ";
    cin >> searchPrizv;
    cout << "Введіть оцінку з математики: ";
    cin >> searchMath;

    int foundIndex = BinarySearch(p, n, searchPrizv, searchMath);
    if (foundIndex != -1) {
        cout << "Студент з прізвищем \"" << searchPrizv << "\" та оцінкою з математики " << searchMath << " знайдений на позиції " << foundIndex + 1 << endl;
    }
    else {
        cout << "Студент не знайдений." << endl;
    }

    delete[] p;
    return 0;
}

void Create(Student* p, const int n) {
    int specialty;
    for (int i = 0; i < n; i++) {
        cout << "Cтудент № " << i + 1 << ":" << endl;

        cin.ignore();
        cout << "Прізвище: "; getline(cin, p[i].prizv);
        cout << "Курс: "; cin >> p[i].kurs;
        cout << "Оцінка з математики: "; cin >> p[i].Math;
        cout << "Оцінка з фізики: "; cin >> p[i].Fizik;
        cout << "Спеціальність (0 - КН, 1 - ІН, 2 - МЕ, 3 - ФІ, 4 - ТТІ): ";
        cin >> specialty;
        p[i].specialty = static_cast<Specialty>(specialty);

        switch (p[i].specialty)
        {
        case KN:
            cout << "Оцінка з програмування: "; cin >> p[i].Program;
            break;
        case IT:
            cout << "Оцінка з чисельних методів: "; cin >> p[i].Num_met;
            break;
        case ME:
        case FI:
        case TN:
            cout << "Оцінка з педагогіки: "; cin >> p[i].Pedagogik;
            break;
        }
        cout << endl;
    }
}

void Print(Student* p, const int n) {
    cout << "============================================================================================================"
        << endl;
    cout << "| № | Прізвище | Курс | Математика | Фізика | Спеціальність | Програмування | Чисельні методи | Педагогіка |"
        << endl;
    cout << "------------------------------------------------------------------------------------------------------------"
        << endl;

    for (int i = 0; i < n; i++) {
        cout << "| " << setw(1) << right << i + 1 << " ";
        cout << "| " << setw(9) << left << p[i].prizv
            << "| " << setw(4) << right << p[i].kurs << " "
            << "| " << setw(10) << left << p[i].Math << " "
            << "| " << setw(6) << left << p[i].Fizik << " "
            << "| " << setw(14) << left << p[i].specialty;

        switch (p[i].specialty)
        {
        case KN:
            cout << "| " << setw(13) << setprecision(2) << fixed << right
                << p[i].Program << " |" << setw(18) << right << "|"
                << setw(13) << right << "|" << endl;
            break;
        case IT:
            cout << "| " << setw(15) << right << "|"
                << setw(16) << setprecision(2) << fixed << right << p[i].Num_met << " |"
                << setw(13) << right << "|" << endl;
            break;
        case ME:
        case FI:
        case TN:
            cout << "| " << setw(15) << right << "|"
                << setw(18) << right << "|"
                << setw(11) << setprecision(2) << fixed << right << p[i].Num_met << " |" << endl;
            break;
        }
    }
    cout << "============================================================================================================"
        << endl;
    cout << endl;
}

double Search(Student* p, const int n) {
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].Fizik > 3 && p[i].Num_met > 3 && p[i].Math > 3) {
            k++;
        }
    }
    return 100.0 * k / n;
}

void SearchStudents(Student* p, const int n) {
    for (int i = 0; i < n; i++) {
        if (p[i].Fizik == 4 || p[i].Fizik == 5) {
            cout << setw(3) << right << i + 1
                << ". " << p[i].prizv << endl;
        }
    }
}

void WriteToFile(Student* p, const int n, const char* filename) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу для запису." << endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(&n), sizeof(n));
    file.write(reinterpret_cast<char*>(p), n * sizeof(Student));

    file.close();
}

void ReadFromFileAndPrint(const char* filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу для читання." << endl;
        return;
    }

    int n;
    file.read(reinterpret_cast<char*>(&n), sizeof(n));

    Student* students = new Student[n];

    file.read(reinterpret_cast<char*>(students), n * sizeof(Student));

    Print(students, n);

    file.close();
}

void SortByCourseMathName(Student* p, const int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (CompareStudents(p[j], p[j + 1])) {
                swap(p[j], p[j + 1]);
            }
        }
    }
}

bool CompareStudents(const Student& a, const Student& b) {
    if (a.kurs != b.kurs)
        return a.kurs < b.kurs;
    if (a.Math != b.Math)
        return a.Math > b.Math;
    return a.prizv > b.prizv;
}

void BuildIndexFile(Student* p, const int n, const char* indexFilename) {
    ofstream file(indexFilename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття індексного файлу для запису." << endl;
        return;
    }

    for (int i = 0; i < n; ++i) {
        file.write(reinterpret_cast<const char*>(&p[i].kurs), sizeof(int));
        file.write(reinterpret_cast<const char*>(&i), sizeof(int));
    }

    file.close();
}

int BinarySearch(Student* p, const int n, const string& prizv, const int math) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (p[mid].prizv == prizv && p[mid].Math == math) {
            return mid;
        }
        else if (p[mid].prizv < prizv || (p[mid].prizv == prizv && p[mid].Math < math)) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1;
}
