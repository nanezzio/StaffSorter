#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

struct Person {
    Person() : age(0), job_id(0), position_id(0) {}
    Person(const std::string& _lastName, const std::string& _firstName, int _age, int _job_id, int _position_id)
        : lastName(_lastName), firstName(_firstName), age(_age), job_id(_job_id), position_id(_position_id) {}

    std::string lastName;
    std::string firstName;
    int age;
    int job_id;
    int position_id;
};

struct Job {
    Job() : id(0) {}
    Job(const std::string& _name, int _id) : name(_name), id(_id) {}

    std::string name;
    int id;
};

struct Position {
    Position() : id(0) {}
    Position(const std::string& _name, int _id) : name(_name), id(_id) {}

    std::string name;
    int id;
};

bool compareByName(const Person& person1, const Person& person2) {
    if (person1.lastName == person2.lastName) {
        return person1.firstName < person2.firstName;
    }
    return person1.lastName < person2.lastName;
}

class PersonsList {
public:
    void addPerson(const Person& person) {
        persons.push_back(person);
    }

    void addPosition(const Position& position) {
        positionsMap[position.id] = position;
    }

    void addJob(const Job& job) {
        jobsMap[job.id] = job;
    }

    void print() {
        for (const auto& p : persons) {
            std::string jobName = jobsMap.count(p.job_id) ? jobsMap[p.job_id].name : "Unknown";
            std::string posName = positionsMap.count(p.position_id) ? positionsMap[p.position_id].name : "Unknown";

            std::cout << p.lastName << " " << p.firstName
                << ", Возраст: " << p.age
                << ", Работа: " << jobName
                << ", Должность: " << posName << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
    }

    void sortByName() {
        std::stable_sort(persons.begin(), persons.end(), compareByName);
    }

    void sortByAge() {
        std::stable_sort(persons.begin(), persons.end(), [](const Person& a, const Person& b) {
            return a.age < b.age;
            });
    }

    void sortByJob();

private:
    std::vector<Person> persons;
    std::map<int, Job> jobsMap;
    std::map<int, Position> positionsMap;

    struct JobComparator {
        const std::map<int, Job>& jobs;

        JobComparator(const std::map<int, Job>& jobsMapRef) : jobs(jobsMapRef) {}

        bool operator()(const Person& p1, const Person& p2) const {
            auto it1 = jobs.find(p1.job_id);
            auto it2 = jobs.find(p2.job_id);

            if (it1 == jobs.end() || it2 == jobs.end()) {
                return p1.job_id < p2.job_id;
            }

            if (it1->second.name != it2->second.name) {
                return it1->second.name < it2->second.name;
            }

            if (p1.lastName != p2.lastName) {
                return p1.lastName < p2.lastName;
            }
            return p1.firstName < p2.firstName;
        }
    };
};

void PersonsList::sortByJob() {
    JobComparator comparator(jobsMap);
    std::stable_sort(persons.begin(), persons.end(), comparator);
}

int main() {
    setlocale(LC_ALL, "Russian");

    PersonsList list;

    list.addJob(Job("Yandex", 1));
    list.addJob(Job("Google", 2));
    list.addJob(Job("Apple", 3));
    list.addJob(Job("Amazon", 4));

    list.addPosition(Position("Разработчик", 10));
    list.addPosition(Position("Менеджер", 20));
    list.addPosition(Position("Директор", 30));

    list.addPerson(Person("Иванов", "Иван", 30, 2, 10));
    list.addPerson(Person("Петров", "Петр", 25, 1, 20));
    list.addPerson(Person("Сидоров", "Алексей", 40, 4, 30));
    list.addPerson(Person("Смирнова", "Анна", 28, 3, 10));
    list.addPerson(Person("Кузнецов", "Дмитрий", 35, 1, 10));

    std::cout << "=== Исходный список ===" << std::endl;
    list.print();

    std::cout << "=== Сортировка по имени ===" << std::endl;
    list.sortByName();
    list.print();

    std::cout << "=== Сортировка по возрасту ===" << std::endl;
    list.sortByAge();
    list.print();

    std::cout << "=== Сортировка по месту работы (по названию) ===" << std::endl;
    list.sortByJob();
    list.print();

    return 0;
}