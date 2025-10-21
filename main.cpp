#include <iostream>
#include <iomanip>

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;
public:
    MatrizBase(int filas = 0, int columnas = 0) : _filas(filas), _columnas(columnas) {}
    virtual ~MatrizBase() {}
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    virtual T obtenerValor(int i, int j) const = 0;
    int filas() const { return _filas; }
    int columnas() const { return _columnas; }
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
    T **_datos;
public:
    MatrizDinamica(int filas, int columnas) : MatrizBase<T>(filas, columnas) {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; i++) {
            _datos[i] = new T[this->_columnas];
        }
    }

    MatrizDinamica(const MatrizDinamica<T>& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; i++) {
            _datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
    }

    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        if (this == &otra) return *this;
        for (int i = 0; i < this->_filas; i++)
            delete[] _datos[i];
        delete[] _datos;
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; i++) {
            _datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
        return *this;
    }

    virtual ~MatrizDinamica() {
        for (int i = 0; i < this->_filas; i++) {
            delete[] _datos[i];
        }
        delete[] _datos;
    }

    void cargarValores() override {
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = static_cast<T>((i + 1) + (j * 0.5));
            }
        }
    }

    T obtenerValor(int i, int j) const override {
        return _datos[i][j];
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.filas() || this->_columnas != otra.columnas()) {
            std::cout << "No se pueden sumar: dimensiones distintas.\n";
            return nullptr;
        }
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);
        for (int i = 0; i < this->_filas; i++) {
            for (int j = 0; j < this->_columnas; j++) {
                resultado->_datos[i][j] = this->_datos[i][j] + otra.obtenerValor(i, j);
            }
        }
        std::cout << "(Suma realizada por MatrizDinamica)\n";
        return resultado;
    }

    void imprimir() const override {
        for (int i = 0; i < this->_filas; i++) {
            std::cout << "| ";
            for (int j = 0; j < this->_columnas; j++) {
                std::cout << std::setw(6) << std::fixed << std::setprecision(2) << _datos[i][j] << " ";
            }
            std::cout << "|\n";
        }
    }
};

template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
    T _datos[M][N];
public:
    MatrizEstatica() : MatrizBase<T>(M, N) {}

    void cargarValores() override {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                _datos[i][j] = static_cast<T>((i + 1) * 0.5 + (j + 1) * 0.5);
            }
        }
    }

    T obtenerValor(int i, int j) const override {
        return _datos[i][j];
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (M != otra.filas() || N != otra.columnas()) {
            std::cout << "No se pueden sumar: dimensiones distintas.\n";
            return nullptr;
        }
        MatrizEstatica<T, M, N>* resultado = new MatrizEstatica<T, M, N>();
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                resultado->_datos[i][j] = _datos[i][j] + otra.obtenerValor(i, j);
            }
        }
        std::cout << "(Suma realizada por MatrizEstatica)\n";
        return resultado;
    }

    void imprimir() const override {
        for (int i = 0; i < M; i++) {
            std::cout << "| ";
            for (int j = 0; j < N; j++) {
                std::cout << std::setw(6) << std::fixed << std::setprecision(2) << _datos[i][j] << " ";
            }
            std::cout << "|\n";
        }
    }
};

template <typename T>
MatrizBase<T>* operator+(const MatrizBase<T>& a, const MatrizBase<T>& b) {
    return a.sumar(b);
}

int main() {
    std::cout << "--- Sistema Genérico de Álgebra Lineal ---\n\n";

    std::cout << ">> Prueba con tipo FLOAT <<\n";
    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    A->cargarValores();
    std::cout << "Matriz A:\n";
    A->imprimir();

    MatrizBase<float>* B = new MatrizEstatica<float, 3, 2>();
    B->cargarValores();
    std::cout << "\nMatriz B:\n";
    B->imprimir();

    std::cout << "\nResultado C = A + B:\n";
    MatrizBase<float>* C = (*A) + (*B);
    if (C) C->imprimir();

    delete A;
    delete B;
    delete C;

    std::cout << "\n>> Prueba con tipo INT <<\n";
    MatrizBase<int>* D = new MatrizDinamica<int>(2, 2);
    D->cargarValores();
    std::cout << "Matriz D:\n";
    D->imprimir();

    MatrizBase<int>* E = new MatrizEstatica<int, 2, 2>();
    E->cargarValores();
    std::cout << "\nMatriz E:\n";
    E->imprimir();

    std::cout << "\nResultado F = D + E:\n";
    MatrizBase<int>* F = (*D) + (*E);
    if (F) F->imprimir();

    delete D;
    delete E;
    delete F;

    std::cout << "\nMemoria liberada correctamente.\n";
    std::cout << "Sistema cerrado.\n";
    return 0;
}
