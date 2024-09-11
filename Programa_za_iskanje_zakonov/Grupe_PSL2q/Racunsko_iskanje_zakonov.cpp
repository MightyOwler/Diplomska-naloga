#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <array>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>
#include <utility> // For std::pair
#include <numeric>
#include <map>
#include <cmath>

// Function to multiply two 2x2 matrices
std::array<std::array<int, 2>, 2> multiplyMatrices(
    const std::array<std::array<int, 2>, 2>& a,
    const std::array<std::array<int, 2>, 2>& b) {
    
    std::array<std::array<int, 2>, 2> result = {{
        {0, 0},
        {0, 0}
    }};

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    
    return result;
}

// Function to apply modulus to a 2x2 matrix
std::array<std::array<int, 2>, 2> modulusMatrices(
    const std::array<std::array<int, 2>, 2>& a,
    int mod) {
    
    std::array<std::array<int, 2>, 2> result;
    
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result[i][j] = a[i][j] % mod;
        }
    }
    
    return result;
}

bool y_before_Y(const std::string& s) {
    bool seen_y = false;
    for (char c : s) {
        if (c == 'y') seen_y = true;
        if (c == 'Y' && !seen_y) return false;
    }
    return true;
}

std::vector<std::string> generate_words(int n) {
    std::vector<char> elements = {'x', 'y', 'X', 'Y'};
    std::vector<std::string> results;

    auto is_inverse = [](char a, char b) {
        return (a == 'x' && b == 'X') || (a == 'X' && b == 'x') ||
               (a == 'y' && b == 'Y') || (a == 'Y' && b == 'y');
    };

    std::queue<std::pair<std::string, int>> queue;
    for (char e : elements) {
        queue.push({std::string(1, e), 1});
    }

    while (!queue.empty()) {
        auto [current, len] = queue.front();
        queue.pop();

        if (len == n) {
            results.push_back(current);
        } else {
            char last_char = current.back();
            for (char next : elements) {
                if (!is_inverse(last_char, next)) {
                    queue.push({current + next, len + 1});
                }
            }
        }
    }

    return results;
}

std::vector<std::string> generate_words_smarter(int n) {
    if (n <= 0) return {};
    std::vector<std::string> previous_words = generate_words(n - 1);
    std::vector<std::string> results;

    for (auto& word : previous_words) {
        if (word[0] != 'X' && y_before_Y(word)) {
            results.push_back("x" + word);
        }
    }

    return results;
}



// Function to print a 2x2 matrix

// Define a structure to represent a 2x2 matrix
struct Matrix2x2 {
    int a11, a12, a21, a22;

    Matrix2x2(int val1 = 0, int val2 = 0, int val3 = 0, int val4 = 0)
        : a11(val1), a12(val2), a21(val3), a22(val4) {}

    // Function to multiply two matrices
    Matrix2x2 multiply(const Matrix2x2& other) const {
        return Matrix2x2(
            a11 * other.a11 + a12 * other.a21,
            a11 * other.a12 + a12 * other.a22,
            a21 * other.a11 + a22 * other.a21,
            a21 * other.a12 + a22 * other.a22
        );
    }

    // Function to take modulo of a matrix
    Matrix2x2 mod(int p) const {
        return Matrix2x2(
            a11 % p, a12 % p,
            a21 % p, a22 % p
        );
    }

};

std::ostream& operator<<(std::ostream& os, const Matrix2x2& m) {
    os << "[[" << m.a11 << ", " << m.a12 << "], [" 
              << m.a21 << ", " << m.a22 << "]]";
    return os;
}


// Forward declarations for utility functions
Matrix2x2 matrixInverse(const Matrix2x2& matrix, int p);
int modularInverse(int a, int p);


// Function to calculate determinant of a 2x2 matrix
int determinant(const Matrix2x2& m) {
    return m.a11 * m.a22 - m.a12 * m.a21;
    
}

// Function to perform modular operation
int mod_p(int x, int p) {
    return ((x % p) + p) % p;
}

// Function to generate SL(2, p)
std::vector<Matrix2x2> generate_sl_2_p(int p) {
    std::vector<Matrix2x2> matrices;
    for (int a11 = 0; a11 < p; ++a11) {
        for (int a12 = 0; a12 < p; ++a12) {
            for (int a21 = 0; a21 < p; ++a21) {
                for (int a22 = 0; a22 < p; ++a22) {
                    Matrix2x2 m = {a11, a12, a21, a22};
                    if (mod_p(determinant(m), p) == 1) {
                        matrices.push_back(m);
                    }
                }
            }
        }
    }
    return matrices;
}

// Function to perform scalar multiplication of a matrix mod p
Matrix2x2 scalar_multiple(int lambda, const Matrix2x2& m, int p) {
    return {
        mod_p(lambda * m.a11, p),
        mod_p(lambda * m.a12, p),
        mod_p(lambda * m.a21, p),
        mod_p(lambda * m.a22, p)
    };
}

// Function to check if two matrices are equal
bool matrix_equals(const Matrix2x2& m1, const Matrix2x2& m2) {
    return m1.a11 == m2.a11 && m1.a12 == m2.a12 &&
           m1.a21 == m2.a21 && m1.a22 == m2.a22;
}

// Function to remove scalar multiples of a matrix from a list
std::vector<Matrix2x2> remove_scalar_multiples(const Matrix2x2& m, std::vector<Matrix2x2>& lst, int p) {
    std::vector<Matrix2x2> result;
    for (auto& hd : lst) {
        bool to_remove = false;
        for (int lambda = 1; lambda < p; ++lambda) {
            if (matrix_equals(hd, scalar_multiple(lambda, m, p))) {
                to_remove = true;
                break;
            }
        }
        if (!to_remove) {
            result.push_back(hd);
        }
    }
    return result;
}

// Function to generate PSL(2, p)
std::vector<Matrix2x2> generate_psl_2_p(int p) {
    std::vector<Matrix2x2> sl_2_p = generate_sl_2_p(p);
    std::vector<Matrix2x2> psl_2_p;

    while (!sl_2_p.empty()) {
        Matrix2x2& hd = sl_2_p.back();
        psl_2_p.push_back(hd);
        sl_2_p.pop_back(); // Remove the last element
        sl_2_p = remove_scalar_multiples(hd, sl_2_p, p);
    }

    return psl_2_p;
}

// Function to print a 2x2 matrix
void printMatrix(const Matrix2x2& m) {
    std::cout << '[' << m.a11 << ", " << m.a12 << "; " << m.a21 << ", " << m.a22 << ']' << std::endl;
}


namespace fs = std::filesystem;

void save_psl_2_p_group(const std::vector<Matrix2x2>& group, int p, const fs::path& folder) {
    fs::path file_path = folder / ("PSL_2_" + std::to_string(p) + ".txt");
    std::ofstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << file_path << std::endl;
        return;
    }

    for (const auto& matrix : group) {
        file << '[' << matrix.a11 << ", " << matrix.a12 << "; " << matrix.a21 << ", " << matrix.a22 << "]\n";
    }

    file.close();
}

void write_words_to_file(const std::vector<std::string>& group, int n, const fs::path& folder) {
    fs::path file_path = folder / ("Besede dolzine " + std::to_string(n) + ".txt");
    std::ofstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << file_path << std::endl;
        return;
    }

    for (const auto& word : group) {
        file << word << "\n";
    }

    file.close();
}

void write_matrix_pairs_to_file(const std::vector<std::pair<Matrix2x2, Matrix2x2>>& pairs, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& pair : pairs) {
        file << "[[" << pair.first.a11 << ", " << pair.first.a12 << "; " 
             << pair.first.a21 << ", " << pair.first.a22 << "], ["
             << pair.second.a11 << ", " << pair.second.a12 << "; " 
             << pair.second.a21 << ", " << pair.second.a22 << "]]\n";
    }

    file.close();
}


std::vector<Matrix2x2> read_matrices_from_file(const std::string& filename) {
    std::vector<Matrix2x2> matrices;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return matrices;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char open_bracket, close_bracket, comma, semicolon;
        Matrix2x2 m;

        // Extract the integers from a line assuming the format [a11, a12; a21, a22]
        if ((iss >> open_bracket >> m.a11 >> comma >> m.a12 >> semicolon >> m.a21 >> comma >> m.a22 >> close_bracket) &&
            open_bracket == '[' && comma == ',' && semicolon == ';' && close_bracket == ']') {
            matrices.push_back(m);
        } else {
            std::cerr << "Invalid line format: " << line << std::endl;
        }
    }

    file.close();
    return matrices;
}

// Function to create pairs of matrices
std::vector<std::pair<Matrix2x2, Matrix2x2>> create_matrix_pairs(const std::vector<Matrix2x2>& lst) {
    std::vector<std::pair<Matrix2x2, Matrix2x2>> pairs;
    for (auto it1 = lst.begin(); it1 != lst.end(); ++it1) {
        for (auto it2 = it1 + 1; it2 != lst.end(); ++it2) {
            pairs.emplace_back(*it1, *it2);
            pairs.emplace_back(*it2, *it1);
        }
    }
    return pairs;
}

std::vector<std::pair<Matrix2x2, Matrix2x2>> read_matrix_pairs_from_file(const std::filesystem::path& filepath) {
    std::vector<std::pair<Matrix2x2, Matrix2x2>> matrix_pairs;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filepath.string());
    }

    size_t estimated_number_of_matrix_pairs = 30000;
    matrix_pairs.reserve(estimated_number_of_matrix_pairs);

    std::string line;
    while (std::getline(file, line)) {
        std::replace(line.begin(), line.end(), '[', ' ');
        std::replace(line.begin(), line.end(), ']', ' ');
        std::replace(line.begin(), line.end(), ';', ' ');
        std::replace(line.begin(), line.end(), ',', ' ');

        std::istringstream iss(line);
        
        int a11, a12, a21, a22, b11, b12, b21, b22;
        if (!(iss >> a11 >> a12 >> a21 >> a22 >> b11 >> b12 >> b21 >> b22)) {
            std::cerr << "Failed to parse the line: " << line << std::endl;
            continue; // Skip malformed lines
        }
        
        Matrix2x2 mat1(a11, a12, a21, a22);
        Matrix2x2 mat2(b11, b12, b21, b22);
        matrix_pairs.emplace_back(mat1, mat2);
    }

    return matrix_pairs;
}


void process_and_save_matrices(const std::filesystem::path& base_folder, int start, int end) {
    std::filesystem::path psl_folder = base_folder / "PSL_2";
    std::filesystem::path psl_matrices_folder = base_folder / "Pari_PSL_2_matrik";

    // Ensure the output directory exists
    std::filesystem::create_directories(psl_matrices_folder);

    for (int i = start; i <= end; ++i) {
        // Construct input filename within PSL_2 folder
        std::filesystem::path input_path = psl_folder / ("PSL_2_" + std::to_string(i) + ".txt");

        // Read matrices from the file
        auto matrices = read_matrices_from_file(input_path.string());
        auto pairs = create_matrix_pairs(matrices);  // Assuming you have this function defined.

        // Construct output filename within Pari_PSL_2_matrik folder
        std::filesystem::path output_path = psl_matrices_folder / ("pari_matrik_PSL_2_" + std::to_string(i) + ".txt");

        // Save matrix pairs to the file
        write_matrix_pairs_to_file(pairs, output_path.string());
    }
}


// The eval_expression function
Matrix2x2 eval_expression(int p, const std::string& word, const Matrix2x2& matA, const Matrix2x2& matB) {
    Matrix2x2 result(1, 0, 0, 1); // Start with the identity matrix
    Matrix2x2 invA = matrixInverse(matA, p);
    Matrix2x2 invB = matrixInverse(matB, p);

    // Iterate over the word and perform matrix multiplications
    for (char ch : word) {
        if (ch == 'x') {
            result = result.multiply(matA).mod(p);
        } else if (ch == 'y') {
            result = result.multiply(matB).mod(p);
        } else if (ch == 'X') {
            result = result.multiply(invA).mod(p);
        } else if (ch == 'Y') {
            result = result.multiply(invB).mod(p);
        } else {
            throw std::invalid_argument("Invalid character in expression.");
        }
    }
    
    // Take modulo p to ensure all elements are within range 0 to p-1
    return result.mod(p);
}

// Utility function to compute the modular inverse of an integer using the Extended Euclidean Algorithm
int modularInverse(int a, int p) {
    int m0 = p, t, q;
    int x0 = 0, x1 = 1;

    if (p == 1)
        return 0;

    // Apply extended Euclid Algorithm
    while (a > 1) {
        // q is quotient
        q = a / p;
        t = p;

        // p is remainder now, process same as Euclid's algo
        p = a % p;
        a = t;

        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    // Make x1 positive
    if (x1 < 0)
        x1 += m0;

    return x1;
}

Matrix2x2 matrixInverse(const Matrix2x2& matrix, int p) {
    // Ensure p is prime, this algorithm relies on p being prime
    int det = (matrix.a11 * matrix.a22 - matrix.a12 * matrix.a21) % p;
    if (det < 0) det += p; // Make sure det is positive modulo p

    int det_inv = modularInverse(det, p);
    if (det == 0 || det_inv == 0) { // Check for non-invertibility
        throw std::runtime_error("Matrix is not invertible.");
    }

    // Compute the inverse using the adjugate method and modular arithmetic
    return Matrix2x2(
        det_inv * matrix.a22 % p,
        (p - det_inv * matrix.a12 % p) % p, // We subtract from p to handle negative values
        (p - det_inv * matrix.a21 % p) % p,
        det_inv * matrix.a11 % p
    );
}


bool is_scalar_identity(const Matrix2x2& matrix) {
    return (matrix.a11 == matrix.a22 && matrix.a11 != 0 && matrix.a12 == 0 && matrix.a21 == 0);
}

// Function to evaluate a single word on all matrix pairs
std::vector<std::pair<std::string, Matrix2x2>> evaluate_single_word(const std::string& word, const std::vector<std::pair<Matrix2x2, Matrix2x2>>& matrix_pairs, int p) {
    std::vector<std::pair<std::string, Matrix2x2>> evaluated_pairs;
    bool all_identities = true;

    for (const auto& [matA, matB] : matrix_pairs) {
        Matrix2x2 result = eval_expression(p, word, matA, matB);
        if (!is_scalar_identity(result)) {
            all_identities = false;
            break;
        }
        evaluated_pairs.emplace_back(word, result);
    }

    if (!all_identities) {
        evaluated_pairs.clear();
    }
    return evaluated_pairs;
}

// Function to evaluate a list of words on all matrix pairs
std::vector<std::pair<std::string, std::vector<Matrix2x2>>> evaluate_words_on_tuples(const std::vector<std::string>& words, const std::vector<std::pair<Matrix2x2, Matrix2x2>>& matrix_pairs, int p) {
    std::vector<std::pair<std::string, std::vector<Matrix2x2>>> all_evaluations;

    for (const auto& word : words) {
        auto single_word_evaluation = evaluate_single_word(word, matrix_pairs, p);
        if (!single_word_evaluation.empty()) {
            std::vector<Matrix2x2> results;
            for (const auto& [_, mat] : single_word_evaluation) {
                results.push_back(mat);
            }
            all_evaluations.emplace_back(word, results);
        }
    }
    return all_evaluations;
}


bool is_word_scalar_identity(const std::string& word, const std::vector<std::pair<Matrix2x2, Matrix2x2>>& matrix_pairs, int p) {
    for (const auto& [matA, matB] : matrix_pairs) {
        Matrix2x2 result = eval_expression(p, word, matA, matB);
        if (!is_scalar_identity(result)) {
            return false; // Early return on the first non-identity result
        }
    }
    return true; // All results were scalar identities
}

bool are_all_words_scalar_identities(const std::vector<std::string>& words, const std::vector<std::pair<Matrix2x2, Matrix2x2>>& matrix_pairs, int p) {
    for (const auto& word : words) {
        if (!is_word_scalar_identity(word, matrix_pairs, p)) {
            return false; // Early return if any word is not a scalar identity
        }
    }
    return true; // All words resulted in scalar identities
}

std::vector<std::string> read_words_from_file(const std::filesystem::path& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return {};
    }

    // Get file size and reserve vector capacity
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<std::string> words;
    words.reserve(fileSize / 18732); // Estimate number of words

    std::string word;
    while (file >> word) {
        words.push_back(std::move(word)); // Use std::move to avoid copying the string
    }
    return words;
}

// Function to evaluate words on all pairs of matrices
std::vector<std::pair<std::string, Matrix2x2>> evaluate_words_on_matrices(const std::vector<std::string>& words, const std::vector<std::pair<Matrix2x2, Matrix2x2>>& matrix_pairs, int p) {
    std::vector<std::pair<std::string, Matrix2x2>> evaluations;
    for (const auto& word : words) {
        for (const auto& [matA, matB] : matrix_pairs) {
            Matrix2x2 result = eval_expression(p, word, matA, matB);
            evaluations.emplace_back(word, result);
        }
    }
    return evaluations;
}


void write_laws_only_simplified(const std::vector<std::string>& words, const std::vector<std::pair<Matrix2x2, Matrix2x2>>& matrix_pairs, int p, int n) {
    std::filesystem::path dir = "Rezultati/Zakoni za grupo PSL_2_" + std::to_string(p);
    std::filesystem::create_directories(dir);

    std::filesystem::path file_path = dir / ("Zakoni dolzine " + std::to_string(n) + ".txt");
    std::ofstream file(file_path);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + file_path.string());
    }

    // Loop over the words and write only those that evaluate to scalar identities
    for (const auto& word : words) {
        if (is_word_scalar_identity(word, matrix_pairs, p)) {
            file << "Beseda " << word << " je zakon v grupi PSL_2(" << p << ")" << std::endl;
        }
    }

    // Close the file
    file.close();
}

void write_extended_words(int n) {
    // Generate the input and output file paths
    std::filesystem::path input_filename = "Grupe_in_besede\\Besede\\Besede dolzine " + std::to_string(n-1) + ".txt";
    std::filesystem::path output_filename = "Grupe_in_besede\\Besede\\Besede dolzine " + std::to_string(n) + ".txt";

    // Open the input file for reading
    std::ifstream input_file(input_filename);
    if (!input_file.is_open()) {
        throw std::runtime_error("Unable to open file: " + input_filename.string());
    }

    // Open the output file for writing
    std::ofstream output_file(output_filename);
    if (!output_file.is_open()) {
        throw std::runtime_error("Unable to open file: " + output_filename.string());
    }

    // Read lines from the input file and write to the output file
    std::string word;
    while (std::getline(input_file, word)) {
        // Use a switch statement to append characters that don't cancel
        switch (word.back()) {
            case 'x':
                output_file << word << 'x' << '\n';
                output_file << word << 'y' << '\n';
                output_file << word << 'Y' << '\n';
                break;
            case 'X':
                output_file << word << 'x' << '\n';
                output_file << word << 'y' << '\n';
                output_file << word << 'Y' << '\n';
                break;
            case 'y':
                output_file << word << 'x' << '\n';
                output_file << word << 'X' << '\n';
                output_file << word << 'Y' << '\n';
                break;
            case 'Y':
                output_file << word << 'x' << '\n';
                output_file << word << 'X' << '\n';
                output_file << word << 'y' << '\n';
                break;
            default:
                // Handle other characters if necessary
                break;
        }
    }

    // Close the files
    input_file.close();
    output_file.close();
}



int main() {

    // Za compilanje: g++ -std=c++17 -o .\Racunsko_iskanje_zakonov.exe .\Racunsko_iskanje_zakonov.cpp -lstdc++fs


    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

   
     int p = 5; // Prime number
        // Set n and p to the values you are working with
        // ...
    int n = 10;
    
    for (int i = 19; i <= 19; i++){
        int n = i;

        std::filesystem::path matrix_pairs_filename = "Grupe_in_besede\\Pari_PSL_2_matrik\\pari_matrik_PSL_2_" + std::to_string(p) + ".txt";
        std::filesystem::path words_filename = "Grupe_in_besede\\Besede\\Besede dolzine " + std::to_string(n) + ".txt";

       auto words =  read_words_from_file(words_filename);

        auto matrix_pairs = read_matrix_pairs_from_file(matrix_pairs_filename); // Assuming you have implemented this function
        // auto evaluations = evaluate_words_on_matrices(words, matrix_pairs, p);

        // if (n <= 8) {
        //     write_summary_to_file(evaluations, p, n);
        // }
        write_laws_only_simplified(words, matrix_pairs, p, n);

    }

    // POMEMBNO: Za grupe 19 in 23 sem datoteke izbrisal, ker je bilo skupaj več kot 1,5 
    // Stop timing
    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    // Check if duration is greater than 1000 milliseconds (1 second)
    if (duration.count() > 1000) {
        // Calculate the duration in seconds if it's longer than 1000 milliseconds
        auto duration_sec = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
        std::cout << "Time taken by main: " << duration_sec.count() << " seconds" << std::endl;
    } else {
        // Otherwise, just print the milliseconds
        std::cout << "Time taken by main: " << duration.count() << " milliseconds" << std::endl;
    }

        return 0;
    }
