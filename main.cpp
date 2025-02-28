#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

void create_population(int N, int len, int**& popul, int*& fitness, int& best) {
    srand(time(0));
    for (int i = 0; i < N; i++) {
        popul[i] = new int[len];
        for (int j = 0; j < len; j++) {
            popul[i][j] = rand() % 2;
            fitness[i] += popul[i][j];
        }
        // Определение индекса индивида с наибольшей функцией пригодности
        if (fitness[i] > fitness[best]) {
            best = i;
        }
    }
}

void tournir_selection(int N, int len, int**& popul, int**& selec, int*& fitness, int tournir) {
    int *tournir_list = new int[2 * N]();

    for (int i = 0; i < 2 * N; i++) {
        int first = rand() % N; // Выбираем первого участника

        for (int t = 1; t < tournir; t++) { // Выбираем оставшихся участников турнира
            int competitor = rand() % N;
            if (fitness[competitor] > fitness[first]) {
                first = competitor;
            }
        }

        tournir_list[i] = first;
    }

    //вывод массива победителей турнирной селекции
    /*cout << "Турнирная селекция"<< endl;
    for (int i = 0; i < 2*N; i++) {
        cout << tournir_list[i] << " ";
    }
    cout << endl;*/

    for (int i = 0; i < 2 * N; i++) {
        selec[i] = new int[len]();
        int a = tournir_list[i];
        for (int j = 0; j < len; j++) {
            selec[i][j] = popul[a][j];
        }
    }

    delete[] tournir_list;
}

void one_offspring(int N, int len, int**& selec, int**& offsp) { //одноточечное скрещивание
    //cout << "Скрещивание"<< endl;
    int* scr_index = new int[N]();
    for (int i = 0; i < N; i++) {
        int scr = rand() % (len-1)+1;
        scr_index[i] = scr; // Массив индексов для скрещивания
        //cout << scr_index[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < N; i++) {
        offsp[i] = new int[len];
    }
    for (int i = 0; i < N;i++) {
        for (int j = 0; j < scr_index[i]; j++) {
            offsp[i][j] = selec[2*i][j];
        }
        for (int j = scr_index[i]; j < len; j++) {
            offsp[i][j] = selec[2*i+1][j];
        }
    }
    delete[] scr_index;
}

void weak_mutation(int N, int len, int**& offsp, int**& mut, int*& mut_fit, int& best) {
    //cout << "Мутация"<< endl;
    for (int i = 0; i < N; i++) {
        mut[i] = new int[len];
        for (int j = 0; j < len; j++) {
            mut[i][j] = offsp[i][j]; // Сначала копируем
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < len; j++) {
            if (rand() % len == 0) { // Вероятность 1/len
                mut[i][j] = 1 - mut[i][j]; // Инверсия гена
            }
            mut_fit[i] += mut[i][j];
        }
        if (mut_fit[i] > mut_fit[best]) {
            best = i;
        }
    }
}

void Population(int N, int len, int**& population, int*& fitness, int& best_index, int tournir) {
    int epoch_count=1;
    if (best_index != len) {
        while (best_index != len) {
            cout << "ЭПОХА: " << epoch_count << endl;
            // Селекция
            int** selection = new int*[2 * N];
            tournir_selection(N,len, population, selection,fitness, tournir);

            /*for (int i = 0; i < 2 * N; i++) {
                for (int j = 0; j < len; j++) {
                    cout << selection[i][j] << " ";
                }
                cout << endl;
            }
            cout << endl;*/

            //Скрещивание
            int** offspring = new int*[N];
            one_offspring(N,len,selection,offspring);
            /*for (int i = 0; i < N; i++) {
                for (int j = 0; j < len; j++) {
                    cout << offspring[i][j] << " ";
                }
                cout << endl;
            }
            cout << endl;*/

            // Мутация
            int bestIndex = 0; // Индекс лучшего индивида
            int** mutation = new int*[N];
            int* mutation_fitness = new int[N]();

            weak_mutation(N,len,offspring,mutation,mutation_fitness,bestIndex);

            for (int i = 0; i < N; i++) {
                for (int j = 0; j < len; j++) {
                    cout << mutation[i][j] << " ";
                }
                cout << "\t" << mutation_fitness[i];
                cout << endl;
            }

            // Вывод лучшего индивида
            cout << "Лучший индивид: ";
            for (int j = 0; j < len; j++) {
                cout << mutation[bestIndex][j] << " ";
            }
            cout << "\t" << mutation_fitness[bestIndex] << endl;

            for (int i = 0; i < N; i++) {
                for (int j = 0; j < len; j++) {
                    population[i][j] = mutation[i][j];
                }
            }
            best_index = mutation_fitness[bestIndex];

            // Освобождение памяти
            for (int i = 0; i < N; i++) {
                delete[] selection[i];
                delete[] offspring[i];
                delete[] mutation[i];
            }
            delete[] selection;
            delete[] offspring;
            delete[] mutation;
            epoch_count++;
        }
    }
    else {
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
        cout << "\t" << fitness[best_index] << endl;
    }
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    int N = 10;    // Количество индивидов
    int len = 8;  // Длина генома
    int tournir = 2; // Размер турнира
    if (N <= tournir) {
        cout << "Ошибка размера турнира!"<< endl;
    }
    srand(time(0));
    int** population = new int*[N];
    int* fitness = new int[N](); // Массив для хранения суммы строк (функция пригодности)
    int best_index = 0; // Индекс лучшего индивида

    create_population(N, len, population, fitness, best_index);

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
    cout << "\t" << fitness[best_index] << endl;

    Population(N, len, population, fitness, best_index, tournir);

    for (int i = 0; i < N; i++) {
        delete[] population[i];
        }
        delete[] population;
        delete[] fitness;

    return 0;
}
