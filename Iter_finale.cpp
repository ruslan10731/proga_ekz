#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <iterator>
using namespace std;

class Matrix{
private:
    double** data;
public:
    int rows;
    int cols;
    Matrix() : rows(0), cols(0) {}

    // Конструктор Matrix(n, m) - создает матрицу размера n x m
    Matrix(int n, int m) : rows(n), cols(m){
        data = new double* [rows];
        for (int i = 0; i < rows; i++){
            data[i] = new double[cols];
        }
    }

    // Конструктор Matrix(const Matrix&) - copy
    Matrix(const Matrix& other){
        rows = other.rows;
        cols = other.cols;
        data = other.data;
    }

    // Конструктор Matrix(Matrix&&) - move
    Matrix(Matrix&& other) noexcept{
        rows = other.rows;
        cols = other.cols;
        data = move(other.data);
    }

    /* Конструктор Matrix(n, m, val) - создает матрицу 
    размера n x m, заполненную числом val */
    Matrix(int n, int m, double val) : rows(n), cols(m){
        data = new double* [rows];
        for (int i = 0; i < rows; i++){
            data[i] = new double[cols];
            for (int j = 0; j < cols; j++){
                data[i][j] = val;
            }
        }
    }

    /* Конструктор вида (См. std::initializer_list):
    Matrix m {
    { 1, 2, 3 },
    { 4, 5, 6 },
    { 7, 8, 9 }
    }; */
    Matrix(initializer_list<initializer_list<double>> list){
        rows = list.size();
        cols = 0;
        for (auto& x : list)
            if (x.size() > cols)
                cols = x.size();
        data = new double* [rows];
        auto it = list.begin();
        for (int i = 0; i < rows; i++, it++){
            data[i] = new double[cols];
            copy(it->begin(), it->end(), data[i]);
        }
    }

    // Статические методы:

    // Identity(n, m) - возвращает матрицу с единицами по диагонали
    static Matrix Identity(int n, int m){
        Matrix identity(n, m);
        for (int i = 0; i < min(n, m); ++i){
            identity.data[i][i] = 1.0;
        }
        return identity;
    }

    // Zero(n, m) - возвращает матрицу, заполненную нулями
    static Matrix Zero(int n, int m){
        return Matrix(n, m, 0.0);
    }

    /* Random(n, m) - возвращает матрицу, заполненную случайными числами 
    (см. std::uniform_real_distribution для генерации случайных чисел 
    с плавающей запятой) */
    static Matrix Random(int n, int m){
        Matrix randomMatrix(n, m);
        random_device rd;
        mt19937 generator(rd());
        uniform_real_distribution<double> distribution(0.0, 1.0);
        for (int i = 0; i < n; ++i){
            for (int j = 0; j < m; ++j){
                randomMatrix.data[i][j] = distribution(generator);
            }
        }
        return randomMatrix;
    }

    /* FromString(str) - парсит строку и возвращает матрицу. 
    Формат как в питончике: [[1, 2, 3], [4, 5, 6], [7, 8, 9]] */
    static Matrix FromString(const string& str){
        Matrix matrix;
        stringstream ss(str);
        string token;

        matrix.rows = 0;
        while (getline(ss, token, '[')){
            if (token.empty()) continue;
            double** tmp_matrix = matrix.data;
            matrix.data = new double* [matrix.rows + 1];
            for (int i = 0; i < matrix.rows; ++i){
                matrix.data[i] = tmp_matrix[i];
            }
            stringstream row_ss(token);
            matrix.cols = 0;
            while (getline(row_ss, token, ',')){
                try{
                    double val = stod(token);
                    double* tmp_row = matrix.data[matrix.rows];
                    matrix.data[matrix.rows] = new double[matrix.cols + 1];
                    for (int i = 0; i < matrix.cols; ++i){
                        matrix.data[matrix.rows][i] = tmp_row[i];
                    }
                    matrix.data[matrix.rows][matrix.cols] = val;
                    matrix.cols++;
                }
                catch (invalid_argument& e){}
            }
            matrix.rows++;
        }
        return matrix;
    }

    // Методы:
    double operator()(int i, int j) const{
        return data[i - 1][j - 1];
    }

    double& operator()(int i, int j){
        return data[i - 1][j - 1];
    }

    bool operator==(const Matrix& other) const{
        if (rows != other.rows || cols != other.cols){
            return false;
        }
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                if (data[i][j] != other.data[i][j]){
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const{
        return !(*this == other);
    }


    Matrix operator-() const{
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                result.data[i][j] = -data[i][j];
            }
        }
        return result;
    }

    Matrix transpose() const{
        Matrix transposed(cols, rows);
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                transposed.data[j][i] = data[i][j];
            }
        }
        return transposed;
    }

    double sum() const{
        double total = 0.0;
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                total += data[i][j];
            }
        }
        return total;
    }

    Matrix operator+(const Matrix& other) const{
        Matrix result(rows, cols);
        if (rows != other.rows || cols != other.cols){
            return *this;
        }
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const{
        Matrix result(rows, cols);
        if (rows != other.rows || cols != other.cols){
            return *this;
        }
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const{
        Matrix result(rows, cols);
        if (cols != other.rows){
            return *this;
        }
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < other.cols; ++j){
                for (int k = 0; k < cols; ++k){
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    Matrix RemoveColRow(Matrix src, int rows, int cols, int row, int col) const{
        int di, dj;
        Matrix result(rows - 1, cols - 1);
        di = 0;
        for (int i = 0; i < rows - 1; i++){
            if (i == row)
                di = 1;
            dj = 0;
            for (int j = 0; j < cols - 1; j++){
                if (j == col)
                    dj = 1;
                result.data[i][j] = src.data[i + di][j + dj];
            }
        }
        return result;
    }

    double Determ(Matrix src, int m) const{
        int k = 1;
        double det = 0;
        if (m < 1)
            return 0;
        if (m == 1){
            det = src.data[0][0];
            return det;
        }
        if (m == 2){
            det = src.data[0][0] * src.data[1][1] - (src.data[1][0] * src.data[0][1]);
            return det;
        }
        if (m > 2){
            for (int i = 0; i < m; i++){
                Matrix result = RemoveColRow(src, m, m, i, 0);
                det = det + k * src.data[i][0] * Determ(result, m - 1);
                k = -k;
            }
        }
        return det;
    }

    Matrix reverse() const{
        int m = rows;
        Matrix result(m, m);
        double det = Determ(*this, m);
        for (int i = 0; i < m; i++){
            for (int j = 0; j < m; j++){
                result.data[i][j] = Determ(RemoveColRow(*this, m, m, i, j), m - 1);
                if ((i + j) % 2 == 1)
                    result.data[i][j] = -result.data[i][j];
                result.data[i][j] = result.data[i][j] / det;
            }
        }
        return result.transpose();
    }

    Matrix operator/(const Matrix& other) const{
        return *this * other.reverse();
    }

    Matrix operator+(double scalar) const{
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                result.data[i][j] = data[i][j] + scalar;
            }
        }
        return result;
    }


    Matrix operator-(double scalar) const{
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                result.data[i][j] = data[i][j] - scalar;
            }
        }
        return result;
    }

    Matrix operator*(double scalar) const{
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j){
                result.data[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    Matrix operator/(double scalar) const{
        Matrix result(rows, cols);
        if (scalar != 0){
            for (int i = 0; i < rows; ++i){
                for (int j = 0; j < cols; ++j){
                    result.data[i][j] = data[i][j] / scalar;
                }
            }
        }
        return result;
    }

    friend ostream& operator<<(ostream& os, const Matrix& matrix){
        os << "[";
        for (int i = 0; i < matrix.rows; ++i){
            os << "[";
            for (int j = 0; j < matrix.cols; ++j){
                os << matrix.data[i][j];
                if (j < matrix.cols - 1){
                    os << ", ";
                }
            }
            os << "]";
            if (i < matrix.rows - 1){
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    // Построковая итерация
    struct RowIterator{
        using iterator_category = random_access_iterator_tag;
        using value_type = double;
        using difference_type = int;
        using pointer = double*;
        using reference = double&;
        
        pointer rownew;

        RowIterator(pointer ptr) : rownew(ptr) {}

        reference operator*(){ return *rownew; }

        RowIterator& operator++(){
            ++rownew;
            return *this;
        }
        RowIterator& operator--(){
            --rownew;
            return *this;
        }
        RowIterator& operator+=(size_t shift){
            rownew = rownew + shift;
            return *this;
        }
        RowIterator& operator-=(size_t shift){
            rownew = rownew - shift;
            return *this;
        }

        bool operator==(const RowIterator& other) const{
            return rownew == other.rownew;
        }
        bool operator!=(const RowIterator& other) const{
            return !(*this == other);
        }

        // Оператор сложения с числом
        RowIterator operator+(int value) const{
            RowIterator result = *this;
            result.rownew += value;
            return result;
        }
    };

    // Итерация по стоблцам
    struct ColIterator{
        using iterator_category = random_access_iterator_tag;
        using value_type = double;
        using difference_type = int;
        using pointer = double*;
        using reference = double&;
        
        double** matrix_ptr;
        difference_type current_row;
        difference_type current_col;
        
        ColIterator(double** ptr, int row, int col) : matrix_ptr(ptr), current_row(row), current_col(col) {}

        reference operator*(){ return matrix_ptr[current_row][current_col]; }

        ColIterator& operator++(){
            ++current_row;
            return *this;
        }
        ColIterator& operator--(){
            --current_col;
            return *this;
        }
        ColIterator& operator+=(size_t shift){
            current_col = current_col + shift;
            return *this;
        }
        ColIterator& operator-=(size_t shift){
            current_col = current_col - shift;
            return *this;
        }

        bool operator==(const ColIterator& other) const{
            return current_row == other.current_row && current_col == other.current_col;
        }
        bool operator!=(const ColIterator& other) const{
            return !(*this == other);
        }
    };

    // Методы итераторов
    RowIterator iter_rows(int row_index){
        return RowIterator(data[row_index]);
    }

    ColIterator iter_cols(int col_index){
        return ColIterator(data, 0, col_index);
    }
};


int main(){
    string matrix_string;
    //Считывание со строки квадратной матрицы A. Например:
    //[[2, 6, 7], [1, 0, 8], [4, 3, 6]]
    getline(cin, matrix_string);
    Matrix A = Matrix::FromString(matrix_string);
    Matrix B = Matrix::Random(A.rows, A.cols);
    // Если матрица B не рандомная, то:
    //Matrix B = Matrix::FromString("[[2, 3, 4], [6, 7, 1], [3, 9, 8]]");
    Matrix result = ((A * B) - (B / A.transpose()) * A.sum()) + (B.transpose() * A / B.sum());
    cout << "Pri vypolnenii vyrazheniya ((A * B) - (B / AT) * <summa elementov A>) + (BT * A / <summa elementov B>), gde AT and BT - transponirovannye matricy, rezultat raven: " << result << endl;
    cout << endl;
    cout << A(1, 2) << endl; // обращение к элементу по индексу (счет начинается с 1, а не с 0)
    cout << A.sum() << endl; // сумма всех элементов матрицы
    cout << A << endl; // вывод самой матрицы A
    cout << B << endl; // вывод самой матрицы B
    cout << endl;
    cout << (A == B) << endl; // логический оператор '==' выводит 0
    cout << (A != B) << endl; // логический оператор '!=' выводит 1
    cout << A.transpose() << endl; // транспонированная матрица
    cout << endl;

    // Вывод первоначальной матрицы
    cout << "Dannaya matritsa dlya iteratsii:" << A << endl;

    // Итерация по строкам
    cout << "Iteratsia po strokam:" << endl;
    for (int i = 0; i < 3; ++i){
        auto checker = A.iter_rows(i) + 3;
        auto row_iter = A.iter_rows(i);
        while (row_iter != checker){
            cout << *row_iter << ' ';
            ++row_iter;
        }
    }
    cout << endl;

    // Итерация по столбцам
    cout << "Iteratsia po stolbtcam:" << endl;
    for (int j = 0; j < 3; ++j){
        auto col_iter = A.iter_cols(j);
        for (int i = 0; i < 3; ++i){
            cout << *col_iter << ' ';
            ++col_iter;
        }
    }
    return 0;
}
