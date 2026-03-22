# PA&AE_PZPP

## Opis projektu

Pixel Art & Animation Editor to projekt mający na celu stworzenie edytora grafiki pikselowej i prostych animacji klatkowych. Program pozwala na pracę na wielu warstwach, korzystanie z różnych narzędzi do rysowania oraz podgląd efektów w czasie rzeczywistym. Projekt ma być modularny, aby można było łatwo dodawać nowe funkcje i narzędzia bez ingerencji w istniejącą logikę.

## Architektura: Domain, Ports i Adapters

Projekt wykorzystuje architekturę Domain–Ports–Adapters (heksagonalną). Jej celem jest oddzielenie logiki aplikacji od zewnętrznych bibliotek, takich jak SDL3 czy microui.

### Domain (Core)

Warstwa Domain zawiera całą logikę biznesową i nie zależy od żadnych zewnętrznych narzędzi ani frameworków.

- **Layer (Struktura)**  
  Reprezentuje pojedynczą warstwę obrazu. Przechowuje dane pikseli w `std::vector<uint32_t>` oraz metadane, takie jak ID, widoczność i tryb mieszania.

- **Document**  
  Agreguje warstwy i zarządza nimi. Odpowiada za przechowywanie oraz udostępnianie aktualnie aktywnej warstwy.

- **Tool (Interfejs / Port)**  
  Definiuje kontrakt (`ITool`) dla narzędzi operujących na warstwach. Każde narzędzie implementuje metodę `apply(Layer&, x, y, color)`, która modyfikuje dane pikseli.

### Adapters (Infrastructure)

Adaptery odpowiadają za komunikację między światem zewnętrznym a Domain. Tłumaczą dane wejściowe i wyjściowe na formę zrozumiałą dla logiki aplikacji.

- **Input Adapter**  
  Przechwytuje zdarzenia wejścia (np. ruch myszy) i zamienia je na operacje wykonywane na Domain (np. użycie narzędzia w danym miejscu).

- **Render Adapter**  
  Pobiera dane z `Document` i przekształca je do formatu renderowania (np. kopiuje piksele do tekstury), aby można je było wyświetlić na ekranie.

### Język i biblioteki

- C++
- SDL3

### Cel projektu

Przygotowanie programu do tworzenia grafiki pikselowej i prostych animacji klatkowych. Program umożliwia rysowanie, pracę na warstwach oraz wykorzystanie algorytmów do manipulacji pikselami. Projekt może zawierać dodatkowe funkcje pokazujące zastosowanie algorytmów i przetwarzania obrazu.

## Wymagania funkcjonalne

### 1. Tworzenie i edycja grafiki pikselowej

1.1 Program musi umożliwiać tworzenie nowego projektu grafiki pikselowej.  
1.2 Program musi udostępniać obszar roboczy w postaci dwuwymiarowej siatki pikseli.  
1.3 Każdy piksel w siatce musi być możliwy do edycji poprzez kliknięcie lub przeciąganie kursora myszy.  
1.4 Program musi umożliwiać zmianę koloru pojedynczego piksela.  

### 2. Podstawowe narzędzia rysowania

2.1 Pędzel – umożliwia rysowanie poprzez kolorowanie pikseli.  
2.2 Gumka – usuwa kolor z wybranych pikseli.  
2.3 Wiaderko (Flood Fill) – wypełnia obszar jednolitego koloru.  
2.4 Linia – umożliwia rysowanie prostych linii.  
2.5 Okrąg – umożliwia rysowanie okręgów.  

### 3. Wybór koloru

3.1 Program musi umożliwiać wybór koloru w przestrzeniach RGB, HSV lub HSL.  
3.2 Program musi umożliwiać zapis wybranych kolorów w próbniku kolorów.  

### 4. Operacje na warstwach

4.1 Każda warstwa posiada własną siatkę pikseli.  
4.2 Program umożliwia: dodawanie i usuwanie warstw, zmianę kolejności, włączanie/wyłączanie widoczności. Użytkownik może rysować tylko na aktywnej warstwie.  

### 5. Animacje klatkowe (opcjonalnie)

5.1 Możliwość tworzenia animacji, dodawania/usuwania klatek, zmiany kolejności.  
5.2 Podgląd animacji w pętli z możliwością ustawienia szybkości klatek.  

### 6. Algorytmy

6.1 Flood Fill – wypełnianie obszarów kolorem.  
6.2 Bresenham Line Algorithm – rysowanie linii.  
6.3 Midpoint Circle Algorithm – rysowanie okręgów.  
6.4 Blur / Smoothing – filtr wygładzający obraz.  
6.5 Edge Detection – wykrywanie konturów.  

### 7. Funkcje dodatkowe (opcjonalne)

7.1 Smart Fill – inteligentne wypełnianie brakujących pikseli.  
7.2 Proceduralne generowanie animacji – generowanie kolejnych klatek według reguł.  
7.3 Efekty warstwowe – fade, multiply, overlay.  
7.4 Analiza kolorów – optymalizacja palety obrazu lub animacji.  

### 8. System zapisu i eksportu

8.1 Możliwość zapisu projektu do pliku.  
8.2 Zapis zawiera warstwy, klatki animacji i ustawienia projektu.  
8.3 Możliwość ponownego otwarcia zapisanego projektu.  

### 9. Interfejs użytkownika

9.1 Graficzny interfejs umożliwiający dostęp do funkcji programu:  
- paleta kolorów  
- pasek narzędzi  
- lista warstw  
- panel efektów warstwowych  
- podgląd animacji  

## Wymagania niefunkcjonalne

### 1. Wydajność

1.1 Płynna edycja grafiki w czasie rzeczywistym.  
1.2 Operacje na pikselach bez zauważalnych opóźnień.  
1.3 Płynny podgląd animacji.  

### 2. Stabilność

2.1 Brak awarii podczas normalnego użytkowania.  
2.2 Poprawna obsługa błędów użytkownika (brak warstwy, brak klatki).  

### 3. Użyteczność

3.1 Intuicyjny i czytelny interfejs.  
3.2 Łatwy dostęp do wszystkich narzędzi z GUI.  
3.3 Szybkie przełączanie między narzędziami.  

### 4. Testowalność

4.1 Algorytmy grafiki komputerowej możliwe do przetestowania.  
4.2 Możliwość sprawdzenia poprawności działania funkcji rysowania i filtrów.
