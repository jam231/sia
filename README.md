				Systemy inteligetnych agentów, 2013
=======

Sprawy techniczne:  
+ Upewnijcie się, że dokumenty które wysyłacie są kodowane w UTF8 (without BOM). Nie jest to zbyt uciążliwe, a to standard.  
+ Przed wysłaniem sprawdzcie, czy pliki które dodajecie kompilują się.  
  
Uwagi dotyczące kodu:  
+ Pamiętajcie, że przed wysyłaniem lub odczytywaniem danych przy pomocy gniazd sieciowych zadbać o ustawienie odpowiedniej kolejności bajtów (BigEndian)!  
+ Zwalniajcie po sobie zasoby np. w destruktorze - od tego są. 
+ Sprawdzajcie w dokumentacji czy obiekt który zwrócił wam jakiś zasób poprzez wskaźnik zwalnia go sam i ew. zadbajcie o jego uprzątnięcie.  
+ Jeżeli stworzona przez Was klasa/metoda/funkcja pobiera jakiś zasób przez wskaźnik to napiszcie w komentarzu (najlepiej wielkimi literami) czy go zwalnia,  
czy też nie
