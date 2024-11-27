#include <cassert>
#include <iostream>
#include "Student.h"
#include "Histogram.h"
#include "ArraySequence.h"
#include "Interval.h"
#include "ShrdPtr.h"
#include "Tests.h"
#include "HashTable.h"

void TestHashTable() {
    HashTable<int, std::string> hashTable;

    // Тест: Add и Get
    hashTable.Add(1, "one");
    hashTable.Add(2, "two");
    hashTable.Add(3, "three");

    assert(hashTable.Get(1) == "one");
    assert(hashTable.Get(2) == "two");
    assert(hashTable.Get(3) == "three");

    // Тест: Перезапись существующего элемента
    hashTable.Add(1, "uno");
    assert(hashTable.Get(1) == "uno");

    // Тест: ContainsKey
    assert(hashTable.ContainsKey(1) == true);
    assert(hashTable.ContainsKey(2) == true);
    assert(hashTable.ContainsKey(999) == false);

    // Тест: Удаление элемента
    hashTable.Remove(2);
    assert(hashTable.ContainsKey(2) == false);

    // Тест: Исключение при получении несуществующего элемента
    try {
        hashTable.Get(2);
        assert(false); // Мы не должны дойти до этой строки
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "Element not found");
    }

    // Тест: Исключение при удалении несуществующего элемента
    try {
        hashTable.Remove(999);
        assert(false);
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "Element not found");
    }

    // Тест: Итератор
    hashTable.Add(4, "four");
    hashTable.Add(5, "five");

    int count = 0;
    for (const auto& item : hashTable) {
        count++;

        std::cout << "Key: " << item.first << ", Value: " << item.second << std::endl;
    }
    assert(count == 4);

    // Тест: Итератор после удаления
    hashTable.Remove(5);
    count = 0;
    for (const auto& item : hashTable) {
        count++;
    }
    assert(count == 3);

    // Тест: Увеличение таблицы (резайз)
    for (int i = 6; i < 30; ++i) {
        hashTable.Add(i, "value" + std::to_string(i));
    }
    assert(hashTable.GetCount() == 27);
    assert(hashTable.GetCapacity() >= 30);

    // Тест: Проверка всех элементов через Get
    for (int i = 6; i < 30; ++i) {
        assert(hashTable.Get(i) == "value" + std::to_string(i));
    }

    // Тест: GetAllItems
    auto allItems = hashTable.GetAllItems();
    // assert(allItems->GetLength() == hashTable.GetCount());
    std::cout<<"Getlength:" <<allItems->GetLength()<<std::endl;
    std::cout<< "getcount:"<<hashTable.GetCount()<<std::endl;
    for (int i = 0; i < allItems->GetLength(); ++i) {
        auto item = allItems->Get(i);
        assert(hashTable.Get(item.first) == item.second);
    }

    std::cout << "HT tests passed successfully!" << std::endl;
}

void test() {

    ShrdPtr<Sequence<Interval<int>>> intervals(new ArraySequence<Interval<int>>());

    intervals = intervals->Append(Interval<int>(1, 2));
    intervals = intervals->Append(Interval<int>(3, 4));
    intervals = intervals->Append(Interval<int>(5, 5));

    ShrdPtr<Histogram<int>> histogram(new Histogram<int>(intervals));

    int dob1[] = {1, 1, 2002};
    int dob2[] = {2, 2, 2003};
    int dob3[] = {3, 3, 2004};
    int dob4[] = {4, 4, 2005};
    int dob5[] = {5, 5, 2006};

    ShrdPtr<Student> s1(new Student("Alice", "Smith", 101, dob5, 1));
    ShrdPtr<Student> s2(new Student("Bob", "Johnson", 102, dob4, 2));
    ShrdPtr<Student> s3(new Student("Charlie", "Brown", 103, dob3, 3));
    ShrdPtr<Student> s4(new Student("Diana", "Evans", 104, dob2, 4));
    ShrdPtr<Student> s5(new Student("Eve", "Walker", 105, dob1, 5));

    ShrdPtr<Sequence<int>> yearsOfStudy(new ArraySequence<int>());
    yearsOfStudy = yearsOfStudy->Append(s1->getYearOfStudy());
    yearsOfStudy = yearsOfStudy->Append(s2->getYearOfStudy());
    yearsOfStudy = yearsOfStudy->Append(s3->getYearOfStudy());
    yearsOfStudy = yearsOfStudy->Append(s4->getYearOfStudy());
    yearsOfStudy = yearsOfStudy->Append(s5->getYearOfStudy());

    histogram->add(yearsOfStudy);

    const auto& result = histogram->getData();

    assert(result.Get(Interval<int>(1, 2)) == 2);
    assert(result.Get(Interval<int>(3, 4)) == 2);
    assert(result.Get(Interval<int>(5, 5)) == 1);

    TestHashTable();

    std::cout << "All tests passed!" << std::endl;
}



