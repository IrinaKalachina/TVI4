#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

// Новая функция для вычисления пригодности
int calculate_fitness(int* individual, int len) {
    int fit = 0;
    for (int j = 0; j < len; j++) {
        fit += individual[j];
    }
    return fit;
}

void create_population(int N, int len, int**& popul, int*& fitness, int& best) {
    srand(time(0));
    for (int i = 0; i < N; i++) {
        popul[i] = new int[len];
        for (int j = 0; j < len; j++) {
            popul[i][j] = rand() % 2;
        }
        fitness[i] = calculate_fitness(popul[i], len);
        if (fitness[i] > fitness[best]) {
            best = i;
        }
    }
}

void proportional_selection(int N, int len, int**& popul, int**& selec, int*& fitness) {
    int total_fitness = 0;
    for (int i = 0; i < N; i++) {
        total_fitness += fitness[i];
    }

    double* probabilities = new double[N];
    for (int i = 0; i < N; i++) {
        probabilities[i] = (double)fitness[i] / total_fitness;
    }

    for (int i = 0; i < 2 * N; i++) {
        double r = (double)rand() / RAND_MAX;
        double cumulative = 0.0;
        int selected = 0;

        for (int j = 0; j < N; j++) {
            cumulative += probabilities[j];
            if (r <= cumulative) {
                selected = j;
                break;
            }
        }

        selec[i] = new int[len];
        for (int j = 0; j < len; j++) {
            selec[i][j] = popul[selected][j];
        }
    }

    delete[] probabilities;
}

void rank_selection(int N, int len, int**& popul, int**& selec, int*& fitness) {
    int* indices = new int[N];
    for (int i = 0; i < N; i++) {
        indices[i] = i;
    }

    sort(indices, indices + N, [&](int a, int b) { return fitness[a] > fitness[b]; });

    double* probabilities = new double[N];
    double total_rank = N * (N + 1) / 2.0;

    for (int i = 0; i < N; i++) {
        probabilities[i] = (double)(N - i) / total_rank;
    }

    for (int i = 0; i < 2 * N; i++) {
        double r = (double)rand() / RAND_MAX;
        double cumulative = 0.0;
        int selected = 0;

        for (int j = 0; j < N; j++) {
            cumulative += probabilities[j];
            if (r <= cumulative) {
                selected = indices[j];
                break;
            }
        }

        selec[i] = new int[len];
        for (int j = 0; j < len; j++) {
            selec[i][j] = popul[selected][j];
        }
    }

    delete[] indices;
    delete[] probabilities;
}

void tournir_selection(int N, int len, int**& popul, int**& selec, int*& fitness, int tournir) {
    int *tournir_list = new int[2 * N]();

    for (int i = 0; i < 2 * N; i++) {
        int first = rand() % N;

        for (int t = 1; t < tournir; t++) {
            int competitor = rand() % N;
            if (fitness[competitor] > fitness[first]) {
                first = competitor;
            }
        }

        tournir_list[i] = first;
    }

    for (int i = 0; i < 2 * N; i++) {
        selec[i] = new int[len]();
        int a = tournir_list[i];
        for (int j = 0; j < len; j++) {
            selec[i][j] = popul[a][j];
        }
    }

    delete[] tournir_list;
}

void one_offspring(int N, int len, int**& selec, int**& offsp) {
    int* scr_index = new int[N]();
    for (int i = 0; i < N; i++) {
        int scr = rand() % (len-1)+1;
        scr_index[i] = scr;
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

void two_offspring(int N, int len, int**& selec, int**& offsp) {
    int** scr_index = new int*[N];

    for (int i = 0; i < N; i++) {
        scr_index[i] = new int[2];
        int point1 = rand() % (len - 1) + 1;
        int point2 = rand() % (len - 1) + 1;
        if (point1 > point2) {
            swap(point1, point2);
        }
        scr_index[i][0] = point1;
        scr_index[i][1] = point2;
        cout << scr_index[i][0] << " " << scr_index[i][1] << endl;
    }

    for (int i = 0; i < N; i++) {
        offsp[i] = new int[len];
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < scr_index[i][0]; j++) {
            offsp[i][j] = selec[2 * i][j];
        }
        for (int j = scr_index[i][0]; j < scr_index[i][1]; j++) {
            offsp[i][j] = selec[2 * i + 1][j];
        }
        for (int j = scr_index[i][1]; j < len; j++) {
            offsp[i][j] = selec[2 * i][j];
        }
    }

    for (int i = 0; i < N; i++) {
        delete[] scr_index[i];
    }
    delete[] scr_index;
}

void uniform_offspring(int N, int len, int**& selec, int**& offsp) {
    for (int i = 0; i < N; i++) {
        offsp[i] = new int[len];
    }
    for (int i = 0; i < N;i++) {
        for (int j = 0; j < len; j++) {
            if (rand() % 2 == 0) {
                offsp[i][j] = selec[2*i][j];
            }
            else {
                offsp[i][j] = selec[2*i+1][j];
            }
        }
    }
}

void weak_mutation(int N, int len, int**& offsp, int**& mut, int*& mut_fit, int& best) {
    for (int i = 0; i < N; i++) {
        mut[i] = new int[len];
        for (int j = 0; j < len; j++) {
            mut[i][j] = offsp[i][j];
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < len; j++) {
            if (rand() % (3*len) == 0) {
                mut[i][j] = 1 - mut[i][j];
            }
        }
        mut_fit[i] = calculate_fitness(mut[i], len);
        if (mut_fit[i] > mut_fit[best]) {
            best = i;
        }
    }
}

void average_mutation(int N, int len, int**& offsp, int**& mut, int*& mut_fit, int& best) {
    for (int i = 0; i < N; i++) {
        mut[i] = new int[len];
        for (int j = 0; j < len; j++) {
            mut[i][j] = offsp[i][j];
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < len; j++) {
            if (rand() % len == 0) {
                mut[i][j] = 1 - mut[i][j];
            }
        }
        mut_fit[i] = calculate_fitness(mut[i], len);
        if (mut_fit[i] > mut_fit[best]) {
            best = i;
        }
    }
}

void strong_mutation(int N, int len, int**& offsp, int**& mut, int*& mut_fit, int& best) {
    for (int i = 0; i < N; i++) {
        mut[i] = new int[len];
        for (int j = 0; j < len; j++) {
            mut[i][j] = offsp[i][j];
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < len; j++) {
            if (min(3*(rand() % len), 1) == 0) {
                mut[i][j] = 1 - mut[i][j];
            }
        }
        mut_fit[i] = calculate_fitness(mut[i], len);
        if (mut_fit[i] > mut_fit[best]) {
            best = i;
        }
    }
}

void create_new_population(int N, int len, int**& popul, int**& mut, int*& new_fit, int& best) {
    int** massiv = new int*[2 * N];
    int* fite = new int[2 * N]();

    for (int i = 0; i < 2 * N; i++) {
        massiv[i] = new int[len];
        for (int j = 0; j < len; j++) {
            if (i < N) {
                massiv[i][j] = popul[i][j];
            } else {
                massiv[i][j] = mut[i - N][j];
            }
        }
        fite[i] = calculate_fitness(massiv[i], len);
    }

    for (int i = 0; i < 2 * N - 1; i++) {
        for (int j = 0; j < 2 * N - i - 1; j++) {
            if (fite[j] < fite[j + 1]) {
                swap(fite[j], fite[j + 1]);
                swap(massiv[j], massiv[j + 1]);
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < len; j++) {
            popul[i][j] = massiv[i][j];
        }
        new_fit[i] = fite[i];
    }

    best = 0;  // Индекс лучшего теперь всегда 0 после сортировки

    for (int i = 0; i < 2 * N; i++) {
        delete[] massiv[i];
    }
    delete[] massiv;
    delete[] fite;
}

void Population(int N, int len, int**& population, int*& fitness, int& best_index, int tournir) {
    int epoch_count = 1;
    while (fitness[best_index] != len) {  // Изменено условие на использование значения fitness
        cout << "ПОКОЛЕНИЕ: " << epoch_count << endl;
        int** selection = new int*[2 * N];
        tournir_selection(N,len, population, selection,fitness, tournir);

        int** offspring = new int*[N];
        one_offspring(N,len,selection,offspring);

        int bestIndex = 0;
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

        cout << "Лучший индивид: ";
        for (int j = 0; j < len; j++) {
            cout << mutation[bestIndex][j] << " ";
        }
        cout << "\t" << mutation_fitness[bestIndex] << endl;

        create_new_population(N, len, population, mutation, fitness, best_index);

        for (int i = 0; i < N; i++) {
            delete[] selection[i];
            delete[] offspring[i];
            delete[] mutation[i];
        }
        delete[] selection;
        delete[] offspring;
        delete[] mutation;
        delete[] mutation_fitness;
        epoch_count++;
    }
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    int N = 5;
    int len = 8;
    int tournir = 2;
    if (N <= tournir) {
        cout << "Ошибка размера турнира!"<< endl;
        return 1;
    }

    int** population = new int*[N];
    int* fitness = new int[N]();
    int best_index = 0;

    create_population(N, len, population, fitness, best_index);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < len; j++) {
            cout << population[i][j] << " ";
        }
        cout << "\t" << fitness[i];
        cout << endl;
    }

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