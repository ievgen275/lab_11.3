#include "pch.h"
#include "CppUnitTest.h"
#include "../PR_11.3/PR_11.3.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
        TEST_METHOD(TestBinarySearch)
        {
            const int n = 3;
            Student students[n] = {
                {"Петров", 1, KN, 90, 80, 100},
                {"Іванов", 2, IT, 85, 75, 90},
                {"Сидоров", 3, ME, 80, 70, 80}
            };

            int foundIndex = BinarySearch(students, n, "Іванов", 85);
            Assert::AreEqual(1, foundIndex, L"Іванов з оцінкою 85 має бути знайдений на позиції 2");

            foundIndex = BinarySearch(students, n, "Сидоров", 80);
            Assert::AreEqual(2, foundIndex, L"Сидоров з оцінкою 80 має бути знайдений на позиції 3");

            foundIndex = BinarySearch(students, n, "Невідомий", 70);
            Assert::AreEqual(-1, foundIndex, L"Студента з прізвищем 'Невідомий' не існує");
        }
	};
}
