# U8: Šablonai — Sprendimas

---

## 1 žingsnis: `swap<T>`

**U8/01/main.cpp:**

```cpp
#include <iostream>
#include <string>

// --- Senas būdas ---
void swapInt(int& x, int& y) {
    int temp = x; x = y; y = temp;
}
void swapDouble(double& x, double& y) {
    double temp = x; x = y; y = temp;
}
void swapString(std::string& x, std::string& y) {
    std::string temp = x; x = y; y = temp;
}

// --- Naujas būdas ---
template<typename T>
void swap(T& x, T& y) {
    T temp = x;
    x = y;
    y = temp;
}

int main() {
    std::cout << "=== 1 žingsnis: swap<T> ===\n\n";

    // Senas būdas:
    int a = 1, b = 2;
    swapInt(a, b);
    std::cout << "swapInt:    a=" << a << " b=" << b << "\n";

    double x = 1.5, y = 2.5;
    swapDouble(x, y);
    std::cout << "swapDouble: x=" << x << " y=" << y << "\n";

    std::string s1 = "hello", s2 = "world";
    swapString(s1, s2);
    std::cout << "swapString: s1=" << s1 << " s2=" << s2 << "\n\n";

    // Naujas būdas:
    int c = 10, d = 20;
    swap(c, d);
    std::cout << "swap<T> int:    c=" << c << " d=" << d << "\n";

    double p = 3.14, q = 2.71;
    swap(p, q);
    std::cout << "swap<T> double: p=" << p << " q=" << q << "\n";

    std::string r = "foo", s = "bar";
    swap(r, s);
    std::cout << "swap<T> string: r=" << r << " s=" << s << "\n";

    return 0;
}
```

**U8/01/Makefile:**

```makefile
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = main
SRC    = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
```

---

## 2 žingsnis: `min<T>` ir `max<T>`

**U8/02/minmax.h:**

```cpp
#pragma once

template<typename T>
const T& min(const T& a, const T& b) {
    return (a < b) ? a : b;
}

template<typename T>
const T& max(const T& a, const T& b) {
    return (a > b) ? a : b;
}
```

**U8/02/main.cpp:**

```cpp
#include <iostream>
#include <string>
#include "minmax.h"

int main() {
    std::cout << "=== 2 žingsnis: min<T> ir max<T> ===\n\n";

    int a = 3, b = 7;
    std::cout << "int:    min(" << a << ", " << b << ") = " << min(a, b)
              << "   max(" << a << ", " << b << ") = " << max(a, b) << "\n";

    double x = 3.14, y = 2.71;
    std::cout << "double: min(" << x << ", " << y << ") = " << min(x, y)
              << "   max(" << x << ", " << y << ") = " << max(x, y) << "\n";

    char c1 = 'a', c2 = 'z';
    std::cout << "char:   min('" << c1 << "', '" << c2 << "') = " << min(c1, c2)
              << "   max('" << c1 << "', '" << c2 << "') = " << max(c1, c2) << "\n";

    std::string s1 = "apple", s2 = "banana";
    std::cout << "string: min(\"" << s1 << "\", \"" << s2 << "\") = " << min(s1, s2)
              << "   max(\"" << s1 << "\", \"" << s2 << "\") = " << max(s1, s2) << "\n";

    return 0;
}
```

**U8/02/Makefile:**

```makefile
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = main
SRC    = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
```

---

## 3 žingsnis: `Pair<T>`

**U8/03/Pair.h:**

```cpp
#pragma once
#include <iostream>

template<typename T>
class Pair {
    T first;
    T second;
public:
    Pair(const T& f, const T& s) : first(f), second(s) {}

    const T& getFirst()  const { return first; }
    const T& getSecond() const { return second; }

    void swap() {
        T temp = first;
        first  = second;
        second = temp;
    }

    void print() const {
        std::cout << "(" << first << ", " << second << ")\n";
    }
};
```

**U8/03/main.cpp:**

```cpp
#include <iostream>
#include <string>
#include "Pair.h"

int main() {
    std::cout << "=== 3 žingsnis: Pair<T> ===\n\n";

    // Pair<int>
    std::cout << "Pair<int>:\n";
    Pair<int> p1(3, 7);
    std::cout << "  ";  p1.print();
    p1.swap();
    std::cout << "  po swap: "; p1.print();

    // Pair<std::string>
    std::cout << "\nPair<string>:\n";
    Pair<std::string> p2("hello", "world");
    std::cout << "  "; p2.print();
    p2.swap();
    std::cout << "  po swap: "; p2.print();

    // Pair<double>
    std::cout << "\nPair<double>:\n";
    Pair<double> p3(3.14, 2.71);
    std::cout << "  first="  << p3.getFirst()
              << "  second=" << p3.getSecond() << "\n";

    return 0;
}
```

**U8/03/Makefile:**

```makefile
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = main
SRC    = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
```

---

## 4 žingsnis: `Stack<T>`

**U8/04/Stack.h:**

```cpp
#pragma once
#include <stdexcept>

template<typename T>
class Stack {
    T*  data;
    int top;
    int capacity;
public:
    Stack(int cap = 16)
        : data(new T[cap]), top(0), capacity(cap) {}

    ~Stack() { delete[] data; }

    Stack(const Stack&)            = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const T& value) {
        if (top >= capacity)
            throw std::out_of_range("Stack: perpildymas");
        data[top++] = value;
    }

    T pop() {
        if (isEmpty())
            throw std::out_of_range("Stack: tuščias");
        return data[--top];
    }

    const T& peek() const {
        if (isEmpty())
            throw std::out_of_range("Stack: tuščias");
        return data[top - 1];
    }

    bool isEmpty() const { return top == 0; }
    int  size()    const { return top; }
};
```

**U8/04/main.cpp:**

```cpp
#include <iostream>
#include <string>
#include "Stack.h"

int main() {
    std::cout << "=== 4 žingsnis: Stack<T> ===\n\n";

    // --- Stack<int> ---
    std::cout << "--- Stack<int> ---\n";
    Stack<int> si;
    si.push(1);
    si.push(2);
    si.push(3);
    std::cout << "size: " << si.size() << "\n";
    while (!si.isEmpty()) {
        std::cout << "pop: " << si.pop() << "\n";
    }

    // --- Stack<std::string> ---
    std::cout << "\n--- Stack<string> ---\n";
    Stack<std::string> ss;
    ss.push("C");
    ss.push("C++");
    ss.push("Stack<T>");
    while (!ss.isEmpty()) {
        std::cout << "pop: " << ss.pop() << "\n";
    }

    // --- Stack<double> ---
    std::cout << "\n--- Stack<double> ---\n";
    Stack<double> sd;
    sd.push(3.14);
    sd.push(2.71);
    std::cout << "peek: " << sd.peek() << "\n";
    std::cout << "peek: " << sd.peek() << "   <- tas pats, peek nekeičia\n";
    std::cout << "pop:  " << sd.pop()  << "\n";
    std::cout << "peek: " << sd.peek() << "   <- dabar viršus kitas\n";

    return 0;
}
```

**U8/04/Makefile:**

```makefile
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = main
SRC    = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
```
