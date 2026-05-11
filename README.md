# Compiler Projekt

Dieses Repository enthält einen benutzerdefinierten Compiler, der mithilfe von Flex und Bison entwickelt wurde. Das Programm parst den Quellcode, generiert einen abstrakten Syntaxbaum (AST) und baut eine Symboltabelle für die weitere semantische Analyse auf.

## Voraussetzungen

Um das Projekt lokal zu kompilieren, müssen folgende Werkzeuge auf dem System installiert sein:
* **Bison** (neueste Version)
* **Flex** (neueste Version)
* **CMake**
* **C++ Compiler** (z. B. GCC oder Clang)
* *(Optional)* **Docker** & **Docker Compose** (für isolierte Builds)

---

## Kompilieren

Du hast zwei Möglichkeiten, den Compiler zu bauen: lokal auf deinem Rechner oder vollständig isoliert über Docker.

* ### Option 1: Lokal kompilieren (via CMake)

    Wenn Abhängigkeiten alle auf PC:

    ```bash
    mkdir build
    cd build
    cmake ..
    make

* ### Option 2: Kompilieren mit Docker Compose
    Schnelles compilieren ohne lokale abhängigkeiten:

    ```Bash
    docker compose up --build

* ### Lokal ausführen:
    ```Bash
    ./compiler pfad/zur/datei.txt

* ### Mit docker ausführen
    ```Bash
    docker compose run --rm compiler pfad/zur/datei.txt