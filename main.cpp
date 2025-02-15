#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

void create_arr(int len, int N, int tournir) {
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
        cout << population[best_index][j] << " ";
    }
    cout << "\t" << best_fitness << endl;

    // Селекция
    int *tournir_list = new int[2*N]();
    int k, p = len+10;
    for (int i = 0; i < 2*N; i++) {
        do{
            k = rand() % (N-1);
            p = rand() % (N-1);
        } while (p == k);
        if (fitness[k] >= fitness[p]){
            tournir_list[i]=k;
        }
        else{tournir_list[i]=p;}
    }
    //вывод массива победителей турнирной селекции
    /*for (int i = 0; i < 2*N; i++) {
        cout << tournir_list[i] << " ";
    }
    cout << endl;*/

    // Создание массива после селекции
    int** selection = new int*[2 * N];
    for (int i = 0; i < 2 * N; i++) {
        selection[i] = new int[len]();
        int a = tournir_list[i];
        for (int j = 0; j < len; j++) {
            selection[i][j] = population[a][j];
        }
    }

    for (int i = 0; i < 2 * N; i++) {
        for (int j = 0; j < len; j++) {
            cout << selection[i][j] << " ";
        }
        cout << endl;
    }

    //Скрещивание
    /*int* scr_index = new int[(N/2)-1]();
    for (int i = 0; i < (N/2); i++) {
        int scr = rand() % (len-1)+1;
        scr_index[i] = scr; // Массив индексов для скрещивания
        cout << scr_index[i] << " ";
    }
    //int** offspring = new int*[N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < len; j++) {

        }
    }*/

    // Освобождение памяти
    for (int i = 0; i < N; i++) {
        delete[] population[i];
    }
    for (int i = 0; i < 2 * N; i++) {
        delete[] selection[i];
    }
    delete[] population;
    delete[] fitness;
    delete[] tournir_list;
    delete[] selection;
    //delete[] scr_index;
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    int N = 5; // Количество строк
    int len = 8; // Длина каждой строки
    int tournir = 2; // Количество индивидов, принимающих участие в турнире
    create_arr(len, N, tournir);
    return 0;
}
