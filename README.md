# PA&AE_PZPP
Projekt na zajęcia PZPP:
Temat wstępny: Pixel Art & Animation Editor 
Jezyk i bibliioteki: C++, SFML
Cel projektu: Przygotować program do tworzenia grafiki pikselowej i prostych animacji klatkowych.
Program ma umożliwiać rysowanie, pracę na warstwach oraz wykorzystanie algorytmów do manipulacji pikselami.
Dodatkowo projekt ma zawierać dodatkowe, pokazujące zastosowanie algorytmów i przetwarzania obrazu.

Wymagania funkcjonalne:
1. Tworzenie i edycja grafiki pikselowej
1.1 Program musi umożliwiać tworzenie nowego projektu grafiki pikselowej.
1.2 Program musi udostępniać obszar roboczy w postaci dwuwymiarowej siatki pikseli.
1.3 Każdy piksel w siatce musi być możliwy do edycji poprzez kliknięcie lub przeciąganie kursora myszy.
1.4 Program musi umożliwiać zmianę koloru pojedynczego piksela.
   
2. Program musi oferować podstawowe narzędzia rysowania:
2.1 Pędzel = umożliwia rysowanie poprzez kolorowanie pikseli.
2.2 Gumka = usuwa kolor z wybranych pikseli.
2.3 Wiaderko (Flood Fill) = wypełnia obszar jednolitego koloru.
2.4 Linia = umożliwia rysowanie prostych linii.
2.5 Okrąg = umożliwia rysowanie okręgów.

3.Program musi umożliwiać wybór koloru w przestrzeniach kolorów.
3.1 Program musi umożliwiać alternatywny wybór koloru w przestrzeni RGB, HSV lub HSL.
3.2 Program musi umożliwiać zapisywanie wybranych kolorów w próbniku kolorów.

4.Program musi umożliwiać operacje na warstwach obrazu.
4.1 Każda warstwa musi posiadać własną siatkę pikseli.
4.2 Program musi umożliwiać: dodawanie warstw, usuwanie warstw, zmianę kolejności warstw, 
włączanie i wyłączanie widoczności warstw, Użytkownik musi mieć możliwość rysowania tylko na aktywnej warstwie.

5.System  asysty animacji klatkowej (opcjonalnie)
5.1 Program powinien umożliwiać tworzenie animacji klatkowych, dodawanie nowych klatek, usuwanie klatek, zmianę kolejności klatek
5.2 Program powinien umożliwiać  edycję podglądu animacji: w pętli, dokładna szybkość klatek.

6. Program musi implementować następujące algorytm:
6.1 Flood Fill – wypełnianie obszarów kolorem.
6.2 Bresenham Line Algorithm – rysowanie linii.
6.3 Midpoint Circle Algorithm – rysowanie okręgów.
6.4 Blur / Smoothing – filtr wygładzający obraz.
6.5 Edge Detection - wykrywanie konturów.

7.Funkcje dodatkowe (zależne od postępu projektu, i możliwości spełnienia deadline'u)
7.1 Smart Fill – inteligentne wypełnianie brakujących pikseli.
7.2 Proceduralne generowanie animacji – generowanie kolejnych klatek według reguł (np. przesunięcie, obrót).
7.3 Efekty warstwowe, nakładanie efektów takich jak: fade, multiply, overlay
7.4 Analiza kolorów – optymalizacja palety kolorów obrazu lub animacji.

8.System zapisu i eksportu
8.1 Program musi umożliwiać zapis projektu do pliku.
8.2 Zapisany projekt musi zawierać: warstwy, klatki animacji, ustawienia projektu
8.3 Program powinien umożliwiać ponowne otwarcie zapisanego projektu.

9.Interfejs użytkownika
9.1 Program musi zawierać graficzny interfejs użytkownika umożliwiający dostęp do funkcji programu:
9.1.1 paletę kolorów
9.1.2 pasek narzędzi
9.1.3 listę warstw
9.1.4 panel efektów warstwowych
9.1.5 podgląd animacji

Wymagania niefunkcjonalne:
1. Wydajność
1.1 Program powinien umożliwiać płynną edycję grafiki w czasie rzeczywistym.
1.2 Operacje na pikselach powinny być wykonywane bez zauważalnych opóźnień.
1.3 Podgląd animacji powinien działać płynnie.

2. Stabilność
2.1 Program nie powinien ulegać awariom podczas normalnego użytkowania.
2.2 Program powinien poprawnie obsługiwać błędy użytkownika (np. brak warstwy, brak klatki).

3. Użyteczność
3.1 Interfejs użytkownika powinien być czytelny i intuicyjny.
3.2 Wszystkie narzędzia powinny być łatwo dostępne z poziomu GUI.
3.3 Użytkownik powinien mieć możliwość szybkiego przełączania między narzędziami.

4. Testowalność
4.1 Algorytmy grafiki komputerowej powinny być możliwe do przetestowania.
4.2 Program powinien umożliwiać sprawdzenie poprawności działania funkcji rysowania i filtrów.

