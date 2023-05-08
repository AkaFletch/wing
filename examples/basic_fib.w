struct ExampleTypes {
    int32 exampleSignedInt;
    uint32 exampleUnSignedInt;
    f32 exampleFloat;
    f64 exampleDouble;
    string exampleString;
    // All types can be pointers
};

int32 fib(int32 x) {
    if(x < 3) {
        return 1;
    }
    return fib(x-1) + fib(x-2);
}

int32 Main(String args[]) {
    Fib(40);
    return 0;
}
