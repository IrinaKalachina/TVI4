#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

void create_arr(int len, int N) {
    srand(time(0)); // Инициализация генератора случайных чисел
    int** arr = new int*[N];
    int* sum = new int[N](); // Массив для хранения суммы строк

    for (int i = 0; i < N; i++) {
        arr[i] = new int[len];
        for (int j = 0; j < len; j++) {
            arr[i][j] = rand() % 2; // Генерация случайных 0 и 1
            sum[i] += arr[i][j]; // Подсчет суммы элементов в строке
        }
    }

    // Вывод массива с суммами строк
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < len; j++) {
            cout << arr[i][j] << " ";
        }
        cout << "\t" << sum[i];
        cout << endl;
    }

    // Освобождение памяти
    for (int i = 0; i < N; i++) {
        delete[] arr[i];
    }
    delete[] arr;
    delete[] sum;
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    int N = 10;
    int len = 8;
    create_arr(len, N);
    return 0;
}
