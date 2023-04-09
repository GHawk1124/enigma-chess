
// Define the C++ functions you want to export to WebAssembly
extern "C" {

int add(int a, int b) { return a + b; }

int multiply(int a, int b) { return a * b; }

int subtract(int a, int b) { return a - b; }
}