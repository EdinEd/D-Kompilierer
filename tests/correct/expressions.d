funk haupt() {
    zahl a = 2 + 1;
    zahl add = a + 1;
    zahl multiply_const = a * 2;
    zahl multiply_var = a * a;
    zahl divide = a / 2;
    zahl modulo = a % 2;
    zahl complex = (a+2)*2-(a%2 * 3);
    zahl add_with_funk = 2 + add(1,2);
}

funk add(zahl a, zahl b) -> zahl{
    a = a * 2;
    b = a +b;
    zurück a + b;
}