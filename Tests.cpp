#include <cassert>
#include <iostream>
#include "Student.h"
#include "Histogram.h"
#include "ArraySequence.h"
#include "Interval.h"
#include "ShrdPtr.h"
#include "Tests.h"

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

    std::cout << "All tests passed!" << std::endl;
}



