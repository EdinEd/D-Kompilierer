funk haupt(){

}

funk condition(zahl a, zahl b) -> zahl{
    zahl ergebnis;
    wenn(a > b){
        ergebnis =  1;
    }
    sonst wenn (a < b){
        ergebnis = 2;
    }
    sonst{
        ergebnis = 3;
    }
    zurück ergebnis;
}

funk schleife(zahl i) -> zahl{
    zahl j = 0;
    solange(j < i){
        j = j + 1;
        {
           zahl j = 100000;
        }
    }
    condition(i, j);
    zurück j;
}