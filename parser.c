#include "parser.h"

// TODO: 1. Inicializace
//      2. Cteni tokenu
//      3. Implementace pravidel

// PROMENNE
// Vstupni bod = Neoznačená nesouvislá sekvence příkazů, to znamena hlavni telo programu (treba jako v pythonu)
// Promenne = 1. globalni ... definovane v hlavnim tele programu, rozsah v celem programu
//            2. lokalni ... rozsah - od pocatku bloku do konce 
//                           2.1. v zanorenych blocich hlavniho tela
//                           2.2. v uzivatelskych funkcich 
// Rozsah platnosti
// Blok: Sekvence příkazů v těle funkce, v rámci větve podmíněného příkazu nebo příkazu cyklu.
// Proměnná je dostupná v bloku a jeho podblokách, pokud není překryta jinou proměnnou se stejným názvem.                 

// FUNKCE
// Funkce v IFJ23 nemusí být nutně definovány před jejich použitím, mohou být lexikálně umístěny i za kódem, kde jsou volány.
// Parametry funkce jsou předávány hodnotou a nelze je měnit.
// Návratová hodnota funkce je dána příkazem return; v případě chybějící nebo špatného typu návratové hodnoty dochází k chybě.
// V jazyce IFJ23 není třeba podporovat definice vnořených funkcí.

// CO JE PARSER? 
// program kt generuje PARSE TREE pro dany RETEZEC pokud RETEZEC muze byt generovan definovanou LL-GRAMATIKOU

// VSTUP - potok tokenu
// VYSTUP - syntakticky strom 
