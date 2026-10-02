int floordiv(int a, int b) { // b>0
    if (b < 0) a = -a, b = -b;
    if (a >= 0) return a / b;
    return - ((-a + b - 1) / b);
}

int ceildiv(int a, int b) { // b>0
    if (b < 0) a = -a, b = -b;
    if (a >= 0) return (a + b - 1) / b;
    return - ((-a) / b);
}