funk haupt() -> leer {
    zahl test_funk = 2;
    test_funk(test_funk);
}

funk test_funk(zahl a) -> zahl{
    zahl test_funk = 2;
    
    {
        zahl test_funk;
        test_funk = 1 + test_funk;
    }
    zurück a + test_funk;
}