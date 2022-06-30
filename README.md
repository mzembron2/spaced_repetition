# ZPR_projekt

Autorzy: Maciej Groszyk, Mateusz Zembroń

# Szkielet aplikacji

Lista wymaganych bibliotek:
- Qt5 `sudo apt install -y qtbase5-dev qt5-qmake cmake`
- Qt5 multimedia: `sudo apt-get build-dep qtmultimedia5-dev`
- gtest (pobierane automatycznie przez cmake)

Przydatne komendy:
`sudo apt-get install build-essential`

Krótki sposób kompilacji i uruchomienia:

1. przejście do folderu głównego aplikacji
2. `cmake CMakeLists.txt`
3. `make`
4. Uruchomienie aplikacji: `./<nazwa_utworzonego_pliku_wykonywalnego>` (najczęściej zpr_projekt)
5. Uruchomienie testów jednostkowych: `ctest`
