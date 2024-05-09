#include "deque.h"

bool isValidName(const string& name) {
    if (name.length() < 2) {
        return false;
    }
    for (char c : name) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}

bool isValidGrade(const string& grade) {
    for (char c : grade) {
        if (!isdigit(c)) {
            return false;
        }
    }
    int value = stoi(grade);
    return (value >= 1 && value <= 10);
}

double calculateAverage(const Student& student){
    double sum = 0;
    for(const auto& grade : student.grades){
        sum += grade;
    }

    double average = sum / student.grades.size();

    return average;
}

double calculateMedian(const Student& student) {
    Student temp = student;
    std::sort(temp.grades.begin(), temp.grades.end()); // Sort the grades in ascending order

    if (temp.grades.empty()) {
        return 0.0;
    }

    size_t size = temp.grades.size();
    size_t middle = size / 2;

    if (size % 2 != 0) {
        return static_cast<double>(temp.grades[middle]);
    } else {
        return static_cast<double>(temp.grades[middle - 1] + temp.grades[middle]) / 2.0;
    }
}


void randomGradeGenerator(int number, Student& student) {
    student.grades.clear();
    srand(time(NULL));
    cout << "Namų darbų pažymiai:" << endl;
    for (int i = 0; i < number; i++) {
        int grade;
        do {
            grade = rand() % 10 + 1; 
        } while (!isValidGrade(to_string(grade))); 
        student.grades.push_back(grade);
        cout << student.grades.back() << endl;
    }
    student.finalExamGrade = rand() % 10 + 1; 
    cout << "Egzamino pažymys: "  << endl << student.finalExamGrade << endl;
}

int customRandomInt(int min, int max) {
    static bool initialized = false;
    if (!initialized) {
        std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator
        initialized = true;
    }
    return min + (std::rand() % (max - min + 1));
}

void generateNames(Student& student) {
    std::deque<std::string> firstNames = {"John", "Jane", "Michael", "Emily", "David", "Sarah", "James", "Jessica", "Daniel", "Jennifer"};
    std::deque<std::string> lastNames = {"Smith", "Johnson", "Williams", "Jones", "Brown", "Davis", "Miller", "Wilson", "Moore", "Taylor"};

    // Generate random indices to select random first and last names
    int firstNameIndex = customRandomInt(0, firstNames.size() - 1);
    int lastNameIndex = customRandomInt(0, lastNames.size() - 1);

    // Advance iterators to selected indices
    auto firstNameIter = firstNames.begin();
    std::advance(firstNameIter, firstNameIndex);

    auto lastNameIter = lastNames.begin();
    std::advance(lastNameIter, lastNameIndex);

    // Assign random names to the student
    student.firstName = *firstNameIter;
    student.lastName = *lastNameIter;

    std::cout << "Generated first name and last name: " << student.firstName << " " << student.lastName << std::endl;
}

void readFromFile(const string& filename, deque<Student>& students) {
    auto start = chrono::high_resolution_clock::now();
    ifstream file(filename);
    try {
        if (!file.is_open()) {
            throw runtime_error("Failas nebuvo rastas arba negalima atidaryti.");
        }

        string header, line, firstName, lastName;
        getline(file, header);

        while (getline(file, line)) {
            stringstream ss(line);
            ss >> firstName >> lastName;
            Student student;
            student.firstName = firstName;
            student.lastName = lastName;

            int grade;
            while (ss >> grade) {
                student.grades.push_back(grade);
            }
            if (!student.grades.empty()) {
                student.finalExamGrade = student.grades.back();
                student.grades.pop_back();
            }
            student.fin_average = calculateAverage(student) * 0.4 + student.finalExamGrade * 0.6;
            student.fin_median = calculateMedian(student) * 0.4 + student.finalExamGrade * 0.6;

            students.push_back(student);
        }
    } catch (const ifstream::failure& e) {
        cout << "Klaida nuskaitant failą: " << e.what() << endl;
        exit(EXIT_FAILURE);
    } catch (const exception& e) {
        cout << "Klaida: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    file.close();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> time = end - start;
    cout << endl;
    cout << "Skaitymo laikas " << filename << " " << time.count() << " sekundės" << endl;
    cout << endl;
}


