# Parallel_Processes_App

Implementati un program, in limbajul C, care foloseste apeluri sistem si de biblioteca in mediul UNIX si ofera urmatoarele functionalitati:

1. Se parcurge un director, al carui nume este furnizat ca argument in linia de comanda

2. Se identifica fisierele obisnuite care au extensia “.c”

3. Se afiseaza urmatoarele informatii despre fiecare fisier corespunzator conditiei precedente, in functie de o optiune, data ca un al doilea argument din linia de comanda:

 - numele fisierului (daca optiunea este -n)

 - identificatorul utilizatorului (daca optiunea este -u)

 - drepturile de acces in formatul exemplificat in continuare (daca optiunea este -a)
   - Valorile DA sau NU vor fi cele potrivite fiecarui fisier gasit. Mai sus este oferit doar un exemplu de afisare.
 - dimensiunea in octeti (-d)

contorul de legaturi al fisierului (-c)

Important: In linia de comanda pot fi specificate mai multe optiuni simultan. De exemplu, daca optiunea e -nuc, se afiseaza una sub alta toate informatiile corespunzatoare fiecareia dintre cele trei optiuni

4. Pentru fiecare fisier mai mic de 100 KB, se creeaza o legatura simbolica avand numele fisierului, dar fara extensia “.c”.

5. Daca este prezenta optiunea “-g”, tot ca parte din al doilea argument (ca si celelalte optiuni de la cerinta 3), pentru fiecare fisier cu extensia “.c”, se creeaza un proces care compileaza fisierul prin lansarea in executie a gcc. Apoi, se transforma cerintele 3 si 4 astfel incat sa fie executate in cadrul cate unui proces separat. In total, vor fi create cate trei procese pentru fiecare fisier cu extensia “.c”. Procesul parinte va prelua starea fiecarui proces fiu si o va afisa sub forma “Procesul fiu cu PID <pid_fiu> s-a terminat cu codul <cod>.”, apoi se va termina.

6. Daca este prezenta optiunea “-p”, tot ca parte din al doilea argument (ca si celelalte optiuni de la cerinta 3), in plus fata de optiunea “-g”, pentru fiecare fisier cu extensia “.c”, procesul care compileaza fisierul (lansand gcc) trimite rezultatele compilarii catre un alt proces care filtreaza doar liniile ce contin unul din cuvintele “error” sau “warning” si trimite aceste linii catre procesul parinte.

Parintele verifica aparitiile cuvintelor “error” si “warning” si calculeaza un punctaj, dupa cum urmeaza:

- Daca nu exista niciunul din cuvinte, atunci rezultatul este 10.

- Daca exista cuvantul “error” cel putin o data, atunci rezultatul este 1. 

- Daca nu exista cuvantul “error”, dar exista cuvantul “warning” de mai mult de 10 ori, rezultatul este 2.

- Daca nu exista cuvantul “error”, dar exista cuvantul “warning” de cel mult 10 ori, 

rezultatul este: 2 + 8*(10 – nr_warning)/10.

La final, se afiseaza un mesaj cu punctajul rezultat.

Procesul parinte va prelua starea fiecarui proces fiu si o va afisa sub forma “Procesul fiu cu PID <pid_fiu> s-a terminat cu codul <cod>.”, apoi se va termina.


Procesele fiu si procesul parinte vor rula obligatoriu in paralel (cerintele 5 si 6).
