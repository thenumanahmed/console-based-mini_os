#include <iostream>
#include "task.h"
using namespace std;

int main(int argc, char *argv[])
{

    if (argc == 4)
    {
        cout << "Calender" << endl;

        const int ram = 100;
        const int hardisk = 1000;
        const int core = 1;
        const char *name = "Calender";

        Task *st = Task::sharePid(argv[1], argv[2], argv[3], name, ram, hardisk, core);
    }

    // Calulator Code

    while (true)
    {

        char op;
        double num1, num2, result;

        std::cout << "Enter operator (+, -, *, /): ";
        std::cin >> op;

        std::cout << "Enter first number: ";
        std::cin >> num1;

        std::cout << "Enter second number: ";
        std::cin >> num2;

        switch (op)
        {
        case '+':
            result = num1 + num2;
            break;

        case '-':
            result = num1 - num2;
            break;

        case '*':
            result = num1 * num2;
            break;

        case '/':
            if (num2 == 0)
            {
                std::cout << "Error: division by zero\n";
                return 1;
            }
            result = num1 / num2;
            break;

        default:
            std::cout << "Error: invalid operator\n";
            return 1;
        }

        std::cout << "Result: " << result << "\n";
    };

    return 0;
}
