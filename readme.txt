Paweł Budniak i Michał Świątek
Analiza algorytmów kolorowania mapy

Program ma 3 tryby wykonywania:
-m1:
Pobranie jednego grafu ze standardowego wejścia i wyświetlenie kolorowania na standardowe wyjście.
Przykład:
map_coloring -m1 -a dsatur <<graph.txt >>coloring.txt

flaga -a wybiera stosowany algorytm, możliwe opcje:
{greedy, dsatur, linear5}

-m2:
Losowa generacja jednego grafu o podanej liczbie wierzchołków, wyświetlenie grafu i kolorowania
na standardowe wyjście.
Przykład:
map_coloring -m2 -n100 >>coloring.txt

-m3:
Przeprowadzony jest proces testowania wszystkich algorytmów dla rosnącej liczby wierzchołków
i porównaniem ze złożonością teoretyczną.
Przykład:
map_coloring -m3 -n 20 -k 15 -r 50 -step 10

Wygeneruje grafy dla k=15 problemów o liczbie wierzchołków: n=20, 30, ..., 160
(zwiększającej się o step=10), dla każdego z nich będzie r=50 losowych grafów.
Dla każdego algorytmu ostanie wygenerowana tabela:

|  n  |   T(n)   |   t(n)   |   q(n)   |   c   |

gdzie:
n - l. wierzchołków
T(n) - teoretyczne oszacowanie czasu działania algorytmu
t(n) - średni czas działania algorytmu
q(n) - współczynnik oszacowania == t(n)*T(mediana(n))/ ( T(n) * t (mediana(n) )
c - średnia liczba kolorów wykorzystanych

Konwencja we/wy:

Wejście:
Dla danych wejściowych z pliku lub od użytkownika program będzie oczekiwał danych w formacie:
n - liczba naturalna dodatnia reprezentująca ilość państw do pokolorowania
m - ilość granic między państwami
m par liczb z zakresu 1...n oddzielonych spacją reprezentujących granicę między dwoma państwami

Wyjście:
Program w pierwszej linijce wypisze liczbę całkowitą dodatnią k określającą ilość użytych kolorów.
Następnie wypisze n linijek zawierających pary liczb 1...n, 1...k reprezentującychpaństwo oraz 
przydzielony mu kolor.

Rozpatrywane algorytmy:
1) Greedy Coloring
1.5) Greedy Coloring + random shuffle (permutacja wyznaczająca kolejność przechodzenia po wierzchołkach jest losowana)
2) DSatur
3) "A Linear Algorithm for Colouring Planar Graphs with Five Colours" - Algorytm Howarda Williamsa
    opublikowany w 1985 roku w "The Computer Journal"

Algorytm 3) wymaga, aby graf był reprezentowany przez listę sąsiedztwa spełniającą następujące warunki:
 - Sąsiedzi wierzchołka muszą być przechowywani w liście, która zapewnia usuwanie elementu ze środka w czasie stałym,
   zakładając, że mamy iterator wskazujący na ten element
 - Wierzchołki przechowywane w listach posiadają iterator wskazujący na analogiczny element
   będący drugim końcem krawędzi, którą tworzą te wierzchołki. Zapewnia to szybkie usuwanie wierzchołków z grafu.
Taki graf jest reprezentowany w naszym programie poprzez typ:
Graph<LinkedVertex, LinkedVertexList>

Pozostałe algorytmy nie mają takich wymagań, mogą przyjąć Graf z dowolnymi wartościami templatkowymi spełniającymi
następujące warunki (dla Graph<V, NeighbourList>):
 - V - typ zapewniający implicite rzutowanie na typ int
 - NeighbourList - Kontener przechowujący typ V

Podział na moduły:

1) Kod dotyczący kolorowania i struktur grafowych w nim wykorzystywanych znajduje się w folderze "coloring":
 - Coloring.h 
   	Definicja funkcji templatkowych odpowiedzialnych za algorytmy 1) - 2)
      	Deklaracja funkcji przyjmujących tylko typ Graph<LinkedVertex, LinkedVertexList> odpowiedzialnych za algorytm 4)
	Funkcje testujące poprawność kolorowania
- Coloring.cpp
	Implementacja algorytmu 3) i klasy pomocniczej Linear5
	Implementacja funkcji testującej poprawność kolorwania grafu G przez wszystkie algorytmy
- Graph.h
	Implementacja klasy Graph oraz jej funkcji specjalizujących dla typu Graph<LinkedVertex, LinkedVertexList>

2) Kod związany z generacją danych, pobieraniem ich od użytkownika oraz mierzeniem czasu znajduje się w folderach:
 - include (pliki .h)
	- CommandLineParser.h
		Deklaracja klasy CommandLineParser przetwarzającej argumenty programu
	- Generator.h
		Deklaracja klasy Generator generującej dane testowe
	- Timer.h
		Implementacja klasy templatkowej Timer odpowiedzialnej za mierzenie czasu działania funkcji
 - src (pliki .cpp)
	- CommandLineParser.cpp
		Implementacja klasy CommandLineParser 
	- Generator.cpp
		Implementacja klasy Generator
3) Kod odpowiedzialny za testowanie jednostkowe - folder unittest
 (zawiera plik catch.hpp - plik nagłówkowy zapewniający funkcjonalność frameworka catch2)
4) Kod odpowiedzialny za wykonanie trybu wybranego przez użytkownika - main.cpp (w folderze głównym)

Projekt wykonany został za pomocą języka C++.
 Program wymaga biblioteki boost/graph do generowania algorytmów w trybie trzecim.
 Należy ustalić link do biblioteki boost w pliku CMakeLists.txt w folderze głównym w taki sposób:
set(BOOST_ROOT <path_to_boost>)

	







