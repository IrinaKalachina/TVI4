#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

void create_arr(int len, int N) {
    srand(time(0));
    int** population = new int*[N];
    int* fitness = new int[N](); // Массив для хранения суммы строк (функция пригодности)
    int best_index = 0; // Индекс лучшего индивида

    // Генерация популяции и подсчет функции пригодности
    for (int i = 0; i < N; i++) {
        population[i] = new int[len];
        for (int j = 0; j < len; j++) {
            population[i][j] = rand() % 2; // Генерация случайных 0 и 1
            fitness[i] += population[i][j]; // Подсчет суммы элементов в строке
        }
        // Определение индекса индивида с наибольшей функцией пригодности
        if (fitness[i] > fitness[best_index]) {
            best_index = i;
        }
    }

    // Находим максимальное значение функции пригодности
    int best_fitness = fitness[best_index];

    // Вывод массива с суммами строк
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < len; j++) {
            cout << population[i][j] << " ";
        }
        cout << "\t" << fitness[i];
        cout << endl;
    }

    // Вывод лучшего индивида
    cout << "Лучший индивид: ";
    for (int j = 0; j < len; j++) {
        cout << population[best_index][j] << " "; // Вывод полной строки лучшего индивида
    }
    cout << "\t" << best_fitness << endl; // Вывод максимальной функции пригодности

    // Освобождение памяти
    for (int i = 0; i < N; i++) {
        delete[] population[i];
    }
    delete[] population;
    delete[] fitness;
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    int N = 10; // Количество строк
    int len = 8; // Длина каждой строки
    create_arr(len, N);
    return 0;
}
