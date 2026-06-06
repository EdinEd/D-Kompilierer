funk main(){
    zahl a = 2 + 1;
    zahl add = a + 1;
    zahl multiply_const = a * 2;
    zahl multiply_var = a * a;
    zahl divide = a / 2;
    zahl modulo = a % 2;
    zahl complex = (a+2)*2-(a%2 * 3);
    zahl add_with_funk = a + add(1,2);
    zurück a;
}

funk add(zahl a, zahl b) -> zahl{
    zurück a + b;
}