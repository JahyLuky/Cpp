#include <iostream>

class vector_P {
public:
    int *arr;
    int size;
    int n;

    vector_P(int size) {
        this->arr = new int[size];
        this->size = size;
        this->n = 0;
    }

    ~vector_P() {
        delete[] this->arr;
    }

    void resize(int new_size) {
        int *new_arr = new int[new_size];
        for (int i = 0; i < this->size; ++i) {
            new_arr[i] = this->arr[i];
        }
        delete[] this->arr;
        this->arr = new_arr;
        this->size = new_size;
    }

    void append(int x) {
        if (this->n == this->size) {
            resize(this->size * 2);
        }
        this->arr[this->n++] = x;
    }

    int at(int pos) {
        return this->arr[pos];
    }

    int get_size() {
        return this->size;
    }

    void print_vector() {
        for (int i = 0; i < this->n; ++i) {
            if (i + 1 == this->size) {
                std::cout << this->arr[i] << std::endl;
            } else {
                std::cout << this->arr[i] << ", ";
            }
        }
    }
};

int main() {
    int y = 0;
    std::cout << "Zadej y: " << std::endl;
    std::cin >> y;
    if (!std::cin.good()) {
        std::cout << "Chyba" << std::endl;
        return 0;
    }
    vector_P vec = vector_P(1);
    std::cout << "Zadej prvky posloupnosti" << std::endl;
    int x = 0;
    while (true) {
        if (!(std::cin >> x)) {
            if (!std::cin.good()) {
                std::cout << "Chyba" << std::endl;
            }
            break;
        } else {
            vec.append(x);
        }
    }
    //vec.print_vector();

    int sum = 0;
    for (int i = 0; i < vec.n; ++i) {
        sum = 0;
        for (int j = i; j < vec.n; ++j) {
            sum += vec.arr[j];
            if (y == sum) {
                std::cout << sum << " -> i = " << i << ", j = " << j << std::endl;
                break;
            }
            if (y > sum) {
                continue;
            }
        }
    }

    return 1;
}