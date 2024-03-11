# Image-Compression
//BOARI Ioana-Ruxandra 312CD					README - TEMA2 SDA
    Pentur implementarea temei am folosit un fisier "tema2.h" in care
am definit structurile folosite si functiile necesare pentru indeplinirea
cerintelor. In fisierul "tema2.c" am implementat rezolvarea temei cu ajutorul
functiilor din fisierul "tema2.h". Alt fisier a fost "Makefile"-ul folosit pentru
compilarea surselor, crearea executabilului, rularea acestuia si stergerea lui.

			Descrierea fisierului tema2.h:

DESPRE STRCTURI:

    Am folosit o strctura "Pixel" in care am retinut cele 3 componente RGB
prezenta in componenta unui pixel. Am folosit stuctura "Zone" pentru a retine
o zona din matricea de pixeli de dimensiune "size", ce are ca punct de pornire
punctul de coordonate "x" si "y" din strctura. In campurile "red_ma", "green_ma"
"blue_ma" retin valorile medii de culoare pentru o zona. In campul "mean"
retin un scor al similaritatii in functie de care se stabileste daca o zona din 
imagine ar mai trebui divizata sau nu. Strctura "TreeNode" o folosesc pentru
a reprezenta nodurile arborelui cuaternar necesar pentru implementare. In campul
"type" retin tipul nodului (0 pentru nod intern si 1 pentru frunza). Fiind un 
arbore cuaternar, fiecare nod are 4 pointeri catre copii acestuia.
Strcturile QueueNode si Queue sunt folosite pentru a implementa cozi la cerinta 3.

DESPRE FUNCTII:

Functia "calculate_mean" calculeaza culorile medii pentru zona primita ca
parametru si mean-ul corespunzator si initializeaza campurile aferente din 
strctura pentru zona. Formulele folosite sunt cele indicate in enunt.

Functia "divizare" stabileste daca o zona mai trebuie sau nu divizata in 
functie de factorul primit.

Functia "createNode_1" primeste ca paremtru o zona din matrice si initializeaza
un nod de tip frunza cu valorile acelei zone;

Functia "createNode_0" primeste ca parametru o zona din matrice ce trebuie
impartita deoarece nodul curent este intern, asadar zona pentru nodul curent
va fi NULL. Zona este imparita in 4 subzone, iar pentru fiecare in parte 
se verifica daca mai trebuie divizata sau nu. Daca zona nu mai trebuie 
divizata atunci nodul copil va fi unul frunza si se apeleaza functia 
"createNode_1", altfel se apeleaza recursiv functia "createNode_0" pentru 
a se crea un alt nod intern, iar subzona urmeaza sa fie din nou divizata.

Functia "createTree" primeste matricea de pixeli ca parametru si, in functie de 
factorul dat, creeaza arborele prin apelarea functiilor "createNode_1" si
"createNode_0", dandu-le ca parametru zona intregii matrici.

Functiile "createQueueNode", "createQueue", "isQueueEmpty", "enqueue",
"front", "dequeue", "destroyQueue" reprezinta functiile clasice si bine cunoscute
pentru cozi.

Functiile "max" si "min" returneaza maximul, respectiv minimul dintre 2 numere
date ca parametru.

Functia "maxDepth" calculeaza adancimea maxima din arbore. Aceasta functie
este folosita pentru a determina numarul de niveluri din arbore.

Functiile "printLevel" si "printLevelOrder" parcurg arborele pe nivel si scriu
informatiile dorite in fisierul binar de la cerinta 2. Implementare luata din curs.

Functia "nrFrunze" calculeaza numarul de frunze din arbore. Aceasta functie este
utilizata la cerinta 1 pentru a scrie numarul de blocuri din imagine pentru
care scorul similaritatii pixelilor este mai mic sau egal decat factorul furnizat.

Functia "firstLeafLevel" determina primul nivel din arborele cuaternar ce
contine un nod frunza. Aceasta functie o folosesc pentru a calcula ultima 
informatie necesara la cerinta 1.

Functia "recreateNode_1" creeaza un nod frunza pentru arborele de decompresie.
Primeste ca parametru dimensiunea zonei, coordonatele punctului de pornire si
cele 3 culori RGB.

Functia "recreateNode_0" creeaza nod intern pentru arborele de decompresie.
Din coada queue_type se extrag tipurile pentru cei 4 copii ai nodului curent.
In functie de aceste tipuri se creeaza noduri frunza pe baza culorilor extrase
din queue_colors sau se apeleaza recursiv functia pentru a se crea alte noduri interne.

Functia "recreateTree" primeste cele 2 cozi (queue_type si queue_colors)
ca parametrii si in functie de tipul primului nod creeaza ori un 
nod frunza ori un nod intern ce urmeaza sa creeze restul arborelui prin apeluri
recursive.

Functia "fill_matrix" parcurge arborele si initializeaza matricea de pixeli cu 
valorile crespunzatoare

Functia "destroyTree" elibereaza memoria alocata pentru un arbore cuaternar

			Descriere fisierului tema2.c:

    Cu ajutorul functiilor din fisierul tema2.h am implementat in acest fisier
rezolvarea temei.
Daca numarul de argumente din linia de comanda este 5 atunci se vor rezolva
cerintele 1 sau 2, acestea avand o rezolvare identica pana intr-un punct.
Se citeste antetul fisierului de input pentru a determina dimensiunile imaginii,
iar apoi se citeste matricea de pixeli. Cu ajutorul functiei createTree se
creeaza arborele cuaternar. Apoi pentru a rezolva prima cerinta se determina
numarul de nivele si numarul de frunze si se scriu aceste informatii pe primele
2 linii din fisierul text. Pentru a determina dimensiunea laturii
aflu mai intai primul nivel pe care se gaseste o frunza in arbore. Apoi
pentru fiecare nivel injumatatesc dimensiunea laturii setata initial ca fiind
latura imaginii. Apoi afisez acest nr in fisierul text pe a 3 a linie.
Pentru cerinta 2 se apeleaza functia "printLevelOrder" pentru a parcurge
arborele pe nivel si pentru a scrie informatiile dorite in fisierul binar.
Pentru cerinta 3 citesc din fisierul binar dimensiunea imaginii si scriu in 
fisierul de out antetul corespunzator. Apoi determin numarul de elemente de tip 
unsigned char ce reprezinta tipuri de noduri si culorile pentru nodurile frunza.
Folosesc vectorul "read_vec" pentru a citi toate informatiile din fisier in el.
Apoi creez si initializez cozile queue_type si queue_colors.
queue_type contine doar 0 si 1, reprezentand tipurile de noduri, iar queue_colors
contine doar valori pentru culorile nodurilor frunza.
Cu ajutorul functiei "recreateTree" se creeaza arborele cuaternar pentru decompresie.
Se reface matricea de pixeli cu functia "fill_matrix" si se scrie in fisierul de out.
In final se elibereaza memoria pentru matrice, vector, queue_type, queue_colors si arbore.
///////////////////////////////////////////////////////////////////////////////
Punctajul obtinut la teste la rularea pe calculatorul propriu este:
Total: 59.0p/80p
Valgrind: 16/20
