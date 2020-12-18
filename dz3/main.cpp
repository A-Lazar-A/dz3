#include <iostream>
#include <stack>
#include <cmath>
#include <map>
#include <algorithm>

//ошибки которые можно инициализировать сразу
bool error(std::string s) {
    if (std::count(s.begin(), s.end(), '(') != std::count(s.begin(), s.end(), ')')) {
        return true;
    }
    if (s.find("*)") != std::string::npos || s.find("/)") != std::string::npos || s.find("-)") != std::string::npos ||
        s.find("+)") != std::string::npos || s.find("^)") != std::string::npos) {
        return true;
    }
}

bool isSeparator(char s) {
    std::string seps = " ";
    if (seps.find(s) == std::string::npos) {
        return false;
    } else {
        return true;
    }
}

int priority(char s) {
    switch (s) {
        case '+':
            return 1;
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 2;
        case '^':
            return 3;
    }
    return 0;
}

bool isOperator(char s) {
    std::string seps = "-+/*^()";
    if (seps.find(s) == std::string::npos) {
        return false;
    } else {
        return true;
    }
}

float doIt(char s, float a, float b) {
    switch (s) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        case '^':
            return std::pow(a, b);
    }
    return 0;
}

std::string trigonometry(std::string input, int buf) {
    int open = 1;
    int close = 0;
    int i = buf + 4;
    while (close != open) {
        if (input[i] == '(') {
            open += 1;
        } else if (input[i] == ')') {
            close += 1;
        }
        i += 1;
    }
    std::string sub = input.substr(buf + 3, i - buf - 3);
    return sub;
}

float counter(std::string input) {
    std::map<std::string, float> constants{{"pi", M_PI},
                                           {"e", exp(1)}};
    std::stack<float> numbers;
    std::stack<char> operations;
    bool unMinus = true;//костыль но я не знаю как еще это написать
    for (int i = 0; i < static_cast<int>(input.length()); i++) {
        if (isSeparator(input[i])) {
            continue;
        }

        if (isOperator(input[i])) {
            if (operations.empty()) {
                if (numbers.empty() && unMinus) {
                    numbers.push(0);
                }
                operations.push(input[i]);
                unMinus = false;
            } else if (input[i] == '(') {
                operations.push(input[i]);
                unMinus = true;
            } else if (input[i] == ')') {
                while (operations.top() != '(') {
                    float second = numbers.top();
                    numbers.pop();
                    float first = numbers.top();
                    numbers.pop();
                    float result = doIt(operations.top(), first, second);
                    if (std::isinf(result)) {
                        std::cout << "Division by zero";
                        return 0;
                    }
                    numbers.push(result);
                    operations.pop();
                }
                operations.pop();

            } else if (operations.top() == '(' && unMinus) {
                numbers.push(0);
                operations.push(input[i]);
            } else if (priority(input[i]) > priority(operations.top()) || operations.top() == '(') {
                operations.push(input[i]);
            } else {
                float second = numbers.top();
                numbers.pop();
                float first = numbers.top();
                numbers.pop();
                float result = doIt(operations.top(), first, second);
                if (std::isinf(result)) {
                    std::cout << "Division by zero";
                    return 0;
                }
                numbers.push(result);
                operations.pop();
                operations.push(input[i]);
            }
            continue;
        }

        if (isdigit(input[i])) {
            unMinus = false;
            std::string longNumber;
            while (!isSeparator(input[i]) && !isOperator(input[i]) && i < static_cast<int>(input.length())) {
                longNumber += input[i];
                i++;
            }
            i--;
            numbers.push(std::stof(longNumber));
        } else {
            unMinus = false;
            std::string cons;
            while (!isSeparator(input[i]) && !isOperator(input[i]) && i < static_cast<int>(input.length())) {
                cons += input[i];
                i++;
            }
            i--;
            numbers.push(constants[cons]);
        }
    }

    while (!operations.empty()) {
        float second = numbers.top();
        numbers.pop();
        float first = numbers.top();
        numbers.pop();
        float result = doIt(operations.top(), first, second);
        if (std::isinf(result)) {
            std::cout << "Division by zero";
            return 0;
        }
        numbers.push(result);
        operations.pop();
    }
    return numbers.top();
}


int main() {
    std::string input;

    std::cout << "Constants" << std::endl;
    std::cout << "pi = 3.14...." << std::endl;
    std::cout << "e = 2.7...." << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "Capabilities" << std::endl;
    std::cout << "sqr(), sin(), cos(), tan(), ctg()" << std::endl;
    std::cout << "Variable x (only lowercase)" << std::endl;
    std::cout << "Float with dot NOT comma" << std::endl;
    getline(std::cin, input);

    if (error(input)) {
        std::cout << "Syntax error";
        return 0;
    }
    int buf = input.find("sqr");
    while (buf != std::string::npos) {
        std::string sub = trigonometry(input, buf);
        float res = counter(sub);
        if (res<0){
            std::cout << "Error";
            return 0;
        }
        input.replace(buf, sub.length() + 3, std::to_string(std::sqrt(res)));
        buf = input.find("sqr");
    }
    buf = input.find("sin");
    while (buf != std::string::npos) {
        std::string sub = trigonometry(input, buf);
        input.replace(buf, sub.length() + 3, '('+std::to_string(std::sin(counter(sub)))+')');
        buf = input.find("sin");
    }
    buf = input.find("tan");
    while (buf != std::string::npos) {
        std::string sub = trigonometry(input, buf);
        input.replace(buf, sub.length() + 3, '('+std::to_string(std::sin(counter(sub)))+')');
        buf = input.find("tan");
    }
    buf = input.find("cos");
    while (buf != std::string::npos) {
        std::string sub = trigonometry(input, buf);
        input.replace(buf, sub.length() + 3, '('+std::to_string(std::sin(counter(sub)))+')');
        buf = input.find("cos");
    }
    buf = input.find("ctg");
    while (buf != std::string::npos) {
        std::string sub = trigonometry(input, buf);
        input.replace(buf, sub.length() + 3, '('+std::to_string(std::tan(M_PI / 2 - counter(sub)))+')');
        buf = input.find("ctg");
    }
    buf = input.find('x');
    float x;
    if (buf != std::string::npos) {
        std::cout << "Введите Х" << std::endl;
        std::cin >> x;
    }
    while (buf != std::string::npos) {
        input.replace(buf, 1, '('+std::to_string(x)+')');
        buf = input.find('x');
    }
    std::cout << counter(input);
}

