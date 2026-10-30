funk haupt() {}

funk test_add() -> zahl {
    zurück 1 + 2;
}

funk test_sub() -> zahl {
    zurück 5 - 3;
}

funk test_mul() -> zahl {
    zurück 4 * 3;
}

funk test_div() -> zahl {
    zurück 10 / 3;
}

funk test_mod() -> zahl {
    zurück 10 % 3;
}

funk test_eq_true() -> zahl {
    zahl ergebnis;
    wenn (5 == 5) {
        ergebnis = 1;
    }
    sonst {
        ergebnis = 0;
    }
    zurück ergebnis;
}

funk test_eq_false() -> zahl {
    zahl ergebnis;
    wenn (5 == 3) {
        ergebnis = 1;
    }
    sonst {
        ergebnis = 0;
    }
    zurück ergebnis;
}

funk test_neq() -> zahl {
    zahl ergebnis;
    wenn (5 != 3) {
        ergebnis = 1;
    }
    sonst {
        ergebnis = 0;
    }
    zurück ergebnis;
}

funk test_less() -> zahl {
    zahl ergebnis;
    wenn (3 < 5) {
        ergebnis = 1;
    }
    sonst {
        ergebnis = 0;
    }
    zurück ergebnis;
}

funk test_greater() -> zahl {
    zahl ergebnis;
    wenn (5 > 3) {
        ergebnis = 1;
    }
    sonst {
        ergebnis = 0;
    }
    zurück ergebnis;
}

funk test_leq() -> zahl {
    zahl ergebnis;
    wenn (5 <= 5) {
        ergebnis = 1;
    }
    sonst {
        ergebnis = 0;
    }
    zurück ergebnis;
}

funk test_geq() -> zahl {
    zahl ergebnis;
    wenn (5 >= 3) {
        ergebnis = 1;
    }
    sonst {
        ergebnis = 0;
    }
    zurück ergebnis;
}

funk test_var_decl_init() -> zahl {
    zahl x = 42;
    zurück x;
}

funk test_var_assign() -> zahl {
    zahl x;
    x = 99;
    zurück x;
}

funk test_while() -> zahl {
    zahl i = 0;
    solange (i < 5) {
        i = i + 1;
    }
    zurück i;
}

funk test_if_then() -> zahl {
    zahl ergebnis;
    wenn (richtig) {
        ergebnis = 1;
    }
    sonst {
        ergebnis = 0;
    }
    zurück ergebnis;
}

funk test_if_else() -> zahl {
    zahl ergebnis;
    wenn (falsch) {
        ergebnis = 1;
    }
    sonst {
        ergebnis = 2;
    }
    zurück ergebnis;
}

funk test_else_if() -> zahl {
    zahl x = 10;
    zahl ergebnis;
    wenn (x < 5) {
        ergebnis = 1;
    }
    sonst wenn (x < 10) {
        ergebnis = 2;
    }
    sonst {
        ergebnis = 3;
    }
    zurück ergebnis;
}

funk test_func_call() -> zahl {
    zurück helper(7);
}

funk helper(zahl x) -> zahl {
    zurück x;
}

funk test_nested_block() -> zahl {
    zahl x = 5;
    {
        zahl x = 10;
    }
    zurück x;
}

funk test_complex_expr() -> zahl {
    zurück (2 + 3) * 4 - 10 / 2;
}

€€ Dies ist ein Kommentar
funk test_comments() -> zahl {
    €€ Ein Kommentar in der Funktion
    zurück 0;
}