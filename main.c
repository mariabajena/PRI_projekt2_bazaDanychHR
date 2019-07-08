#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Osoba {
	char imie[20], nazwisko[20], pesel[12];
	int zarobki, dataZat;
	struct Osoba *next;
};

struct Handler {
	struct Osoba *beg, *end;
};

void dodajOsobe(struct Handler *h, char* imie, char*nazwisko, char*pesel, int zarobki, int dataZat);
void drukujListe (struct Handler *h);
void usunListe(struct Handler *h);
void zapiszDoPliku(struct Handler *h);
struct Osoba *czytajZPliku(struct Handler *h);
void edytujOsobe(struct Handler*h);
void usunOsobe(struct Handler*h);
void sortuj(struct Handler*h);

int main()
{
	int i, zarobki, dataZat;
	char imie[20], nazwisko[20], pesel[12];

	struct Handler *h;
	h=(struct Handler*)malloc(sizeof(struct Handler));
	h->beg=NULL;
	h->end=NULL;

	struct Osoba *wezel;
	wezel=(struct Osoba*)malloc(sizeof(struct Osoba));
	wezel->zarobki=NULL;
	wezel->dataZat=NULL;
	wezel->next=NULL;

	while(1)
    {
	    printf("\n\nMozliwe czynnosci:\n");
	    printf("===============\n");
	    printf("1.Dodaj osobe\n");
	    printf("2.Wyswietl liste\n");
	    printf("3.Usun liste\n");
	    printf("4.Zapisz do pliku\n");
	    printf("5.Czytaj z pliku\n");
	    printf("6.Edytuj dane osoby\n");
	    printf("7.Usun osobe\n");
	    printf("8.Sortuj po danym rekordzie\n");
		printf("Wybierz czynnosc: ");
	    if(scanf("%d",&i)<=0){
	        printf("Wpisz tylko liczbe z podanych powyzej\n");
	        exit(0);
	    } else {
	        switch(i)
	        {
	        case 1:
				printf("Podaj imie: ");
				scanf ("%19s", imie);
				printf("Podaj nazwisko: ");
				scanf ("%19s", nazwisko);
				printf("Podaj pesel: ");
				scanf ("%11s", pesel);
				printf("Podaj zarobki: ");
				scanf ("%d", &zarobki);
				printf("Podaj date zatrudnienia: ");
				scanf ("%d", &dataZat);

				dodajOsobe(h, imie, nazwisko, pesel, zarobki, dataZat);
	            break;
	        case 2:
				drukujListe(h);
	            break;
	        case 3:
				usunListe(h);
		        break;
		    case 4:
		    	zapiszDoPliku(h);
				break;
			case 5:
				czytajZPliku(h);
				break;
			case 6:
				edytujOsobe(h);
				break;
			case 7:
				usunOsobe(h);
				break;
			case 8:
				sortuj(h);
				break;
	        default:
				printf("Nie ma mozliwosci o tym numerze\n");
	        }
	    }
    }

	return 0;
}

void dodajOsobe(struct Handler *h, char* imie, char*nazwisko, char*pesel, int zarobki, int dataZat){
	struct Osoba *nowy;
	nowy=(struct Osoba*)malloc(sizeof(struct Osoba));
	strcpy(nowy->imie, imie);
	strcpy(nowy->nazwisko, nazwisko);
	strcpy(nowy->pesel, pesel);
	nowy->zarobki=zarobki;
	nowy->dataZat=dataZat;
	nowy->next=NULL;

	struct Osoba *tmp;
	if(h->beg == NULL) { /*gdy lista jest pusta wstawia jako h->beg*/
		h->beg = nowy;
		return;
	}

	tmp = h->beg;
	while(tmp->next != NULL) { /*przejœcie na koniec listy */
	 tmp = tmp->next;
	}
	tmp->next = nowy; /*wstawienie elementu*/
	h->end=nowy;
}

void drukujListe (struct Handler *h) {
	struct Osoba *temp=(struct Osoba*)malloc(sizeof(struct Osoba));
	temp=h->beg;
	if(temp==NULL) printf("nie ma nic do wyswietlenia");
	while (temp !=NULL) {
	printf("Imie: %s\nNazwisko: %s\nPesel: %s\nZarobki: %d\nData zatrudnienia: %d\n\n",temp->imie, temp->nazwisko, temp->pesel, temp->zarobki, temp->dataZat);
	temp = temp->next;
 }
}

void usunListe(struct Handler *h) {
	struct Osoba *pom;
	while (h->beg != NULL) {
	 pom=h->beg;
	 h->beg=h->beg->next;
	 free(pom);
	 }
}

void zapiszDoPliku(struct Handler *h) {
	char nazwaPliku[128];

	printf("Wpisz nazwe pliku wraz z rozszerzeniem:\n");
	scanf("%123s",nazwaPliku);
	FILE *file;
	file=fopen(nazwaPliku,"w");

	if(file != NULL) {
		struct Osoba *temp=(struct Osoba*)malloc(sizeof(struct Osoba));
		temp=h->beg;

		while(temp != NULL) {
			struct Osoba *tempNext=(struct Osoba*)malloc(sizeof(struct Osoba));
			tempNext = temp->next;

			temp->next = NULL;

			fseek(file, 0, SEEK_END);
			fwrite(temp, sizeof(struct Osoba), 1, file);

			printf("Writing:%19s to file\n",temp->nazwisko);

			temp->next = tempNext;

			tempNext = NULL;

			temp = temp->next;
		}
		fclose(file);
		file = NULL;
	} else {
		printf("BLAD W ODCZYCIE PLIKU\n");
	}

}

struct Osoba *czytajNastZPliku(struct Handler *h, FILE *file) {
	size_t returnValue;

	if(h->beg == NULL) {
		h->beg = (struct Osoba*)malloc(sizeof(struct Osoba));
		returnValue = fread(h->beg, sizeof(struct Osoba), 1, file);
		h->beg->next = NULL;
	} else {
		struct Osoba *temp = h->beg;
		struct Osoba *nowaOsoba = (struct Osoba*)malloc(sizeof(struct Osoba));
		while(temp->next != NULL) {
			temp = temp->next;
		}
		returnValue = fread(nowaOsoba, sizeof(struct Osoba), 1, file);
		temp->next = nowaOsoba;
		nowaOsoba->next = NULL;
	}
	return h->beg;
}

struct Osoba *czytajZPliku(struct Handler *h) {
	char nazwaPliku[128];
	char bufor[30 + 1];
	printf("Wpisz nazwe pliku, z ktorego chcesz czytac, wraz z rozszerzeniem:\n");
	scanf("%123s",nazwaPliku);
	FILE *file;
	file=fopen(nazwaPliku,"r");

	if(file != NULL) {

		usunListe(h);
		h->beg = NULL;

		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file);

		rewind(file);

		int ileElem = (int)(fileSize / (sizeof(struct Osoba)));

		printf("ILEELEM: %d", ileElem);

		int i;
		for(i = 0; i < ileElem; ++i) {
			fseek(file, (sizeof(struct Osoba) * i), SEEK_SET);
			h->beg = czytajNastZPliku(h, file);
		}
	}  else {
		printf("Blad Odczytu Pliku\n");
	}

	return h->beg;

}

void edytujOsobe(struct Handler*h){
	int i, zarobki, dataZat;
	int j=1;
	char imie[20], nazwisko[20], pesel[12];
	char szukaneNazwisko[20];
	struct Osoba *temp=(struct Osoba*)malloc(sizeof(struct Osoba));
	temp=h->beg;

	printf("Podaj nazwisko osoby, ktorej dane chcesz zmienic: ");
	scanf("%19s", szukaneNazwisko);

	if(temp==NULL) printf("nie ma nic do wyswietlenia");
		while (temp !=NULL) {
			if(strcmp(temp->nazwisko,szukaneNazwisko)==0) {
				printf("Imie: %s\nNazwisko: %s\nPesel: %s\nZarobki: %d\nData zatrudnienia: %d\n\n",temp->imie, temp->nazwisko, temp->pesel, temp->zarobki, temp->dataZat);
				while (j==1) {
				printf("Ktora dana chcesz zmienic?\n");
				printf("===============\n");
			    printf("1.Imie\n");
			    printf("2.Nazwisko\n");
			    printf("3.Pesel\n");
			    printf("4.Zarobki\n");
			    printf("5.Data zatrudnienia\n");
				printf("Wpisz numer danej, ktora chcesz zmienic: ");

				if(scanf("%d",&i)<=0){
			        printf("Wpisz tylko liczbe z podanych powyzej\n");
			        exit(0);
		    	}
				else {
			        switch(i)
			        {
				        case 1:
							printf("Podaj nowe imie: ");
							scanf ("%19s", imie);
							strcpy(temp->imie, imie);
							break;
				        case 2:
							printf("Podaj nowe nazwisko: ");
							scanf ("%19s", nazwisko);
							strcpy(temp->nazwisko, nazwisko);
				            break;
				        case 3:
							printf("Podaj nowy pesel: ");
							scanf ("%11s", pesel);
							strcpy(temp->pesel, pesel);
					        break;
					    case 4:
					    	printf("Podaj nowe zarobki: ");
							scanf ("%d", &zarobki);
							temp->zarobki=zarobki;
							break;
						case 5:
							printf("Podaj poprawiona date zatrudnienia: ");
							scanf ("%d", &dataZat);
							temp->dataZat=dataZat;
							break;
				        default:
							printf("Nie ma mozliwosci o tym numerze\n");
			        }
		    	}
		    	printf("Czy chcesz zmienic jeszcze jakies dane w tym rekordzie? Tak---->1, Nie----->0");
		    	scanf("%d", &j);
			}
			}
			temp = temp->next;
		}
		printf("W bazie danych nie ma osoby o tym nazwisku");
}

void usunOsobe(struct Handler *h){
	char szukaneNazwisko[20];
	struct Osoba *temp=(struct Osoba*)malloc(sizeof(struct Osoba));
	temp=h->beg;
	struct Osoba *pom=(struct Osoba*)malloc(sizeof(struct Osoba));

	printf("przed while");

	printf("Podaj nazwisko osoby, ktorej dane chcesz usunac z listy: ");
	scanf("%19s", szukaneNazwisko);

    if(strcmp(temp->nazwisko,szukaneNazwisko)==0) {
		h->beg = h->beg->next;
        free(temp);
		return;
    }

    struct Osoba * current = h->beg->next;
    struct Osoba * previous = h->beg;
    while (current != NULL && previous != NULL) {
        if (strcmp(szukaneNazwisko, current->nazwisko) == 0) {
            temp = current;
            previous->next = current->next;
            free(temp);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("W bazie danych nie ma osoby o tym nazwisku");
    return;
}

int ileElem(struct Handler *h) {
	int i=0;
	struct Osoba *temp=(struct Osoba*)malloc(sizeof(struct Osoba));
	temp=h->beg;

	while (temp !=NULL) {
		i++;
		temp = temp->next;
	}
	 return i;
}

int porownajImie (const void * p, const void * q)
{
    struct Osoba* p1 = *(struct Osoba**)p;
    struct Osoba* q1 = *(struct Osoba**)q;
    if(strcmp(p1->imie, q1->imie)<0) return -1;
    else if(strcmp(p1->imie, q1->imie)==0) return 0;
    else return 1;
}

int porownajNazwisko (const void * p, const void * q)
{
    struct Osoba* p1 = *(struct Osoba**)p;
    struct Osoba* q1 = *(struct Osoba**)q;
    if(strcmp(p1->nazwisko, q1->nazwisko)<0) return -1;
    else if(strcmp(p1->nazwisko, q1->nazwisko)==0) return 0;
    else return 1;
}

int porownajPesel (const void * p, const void * q)
{
    struct Osoba* p1 = *(struct Osoba**)p;
    struct Osoba* q1 = *(struct Osoba**)q;
    if(strcmp(p1->pesel, q1->pesel)<0) return -1;
    else if(strcmp(p1->pesel, q1->pesel)==0) return 0;
    else return 1;
}

int porownajZarobki (const void * p, const void * q)
{
    struct Osoba* p1 = *(struct Osoba**)p;
    struct Osoba* q1 = *(struct Osoba**)q;
    if(p1->zarobki < q1->zarobki) return -1;
    else if(p1->zarobki == q1->zarobki) return 0;
    else return 1;
}

int porownajDateZat (const void * p, const void * q)
{
    struct Osoba* p1 = *(struct Osoba**)p;
    struct Osoba* q1 = *(struct Osoba**)q;
    if(p1->dataZat < q1->dataZat) return -1;
    else if(p1->dataZat == q1->dataZat) return 0;
    else return 1;
}

void sortuj(struct Handler*h){
	int i=ileElem(h);
	int j=0;
	int k;
	struct Osoba *temp=(struct Osoba*)malloc(sizeof(struct Osoba));
	temp=h->beg;

	if(temp==NULL) printf("Baza danych jest pusta");

	else {
		struct Osoba **tab=(struct Osoba**)malloc(i*sizeof(struct Osoba*));
		for (j=0; j<i; j++){
			tab[j]=temp;
			temp=temp->next;
		}

		printf("Wedlug ktorej danej chcesz sortowac?\n");
					printf("===============\n");
				    printf("1.Imie\n");
				    printf("2.Nazwisko\n");
				    printf("3.Pesel\n");
				    printf("4.Zarobki\n");
				    printf("5.Data zatrudnienia\n");
					printf("Wpisz numer danej, wedlug ktorej chcesz sortowac: ");

					if(scanf("%d",&k)<=0){
				        printf("Wpisz tylko liczbe z podanych powyzej\n");
				        exit(0);
			    	}
					else {
				        switch(k)
				        {
					        case 1:
						    	qsort(tab, i, sizeof(struct Osoba*), porownajImie);
						    	printf("\nPosortowane: \n");

								for(j=0; j<i; j++){
									printf("Imie: %s\nNazwisko: %s\nPesel: %s\nZarobki: %d\nData zatrudnienia: %d\n\n",tab[j]->imie, tab[j]->nazwisko, tab[j]->pesel, tab[j]->zarobki, tab[j]->dataZat);
								}

							    free(tab);
								break;
					        case 2:
						    	qsort(tab, i, sizeof(struct Osoba*), porownajNazwisko);

						    	printf("\nPosortowane: \n");

								for(j=0; j<i; j++){
									printf("Imie: %s\nNazwisko: %s\nPesel: %s\nZarobki: %d\nData zatrudnienia: %d\n\n",tab[j]->imie, tab[j]->nazwisko, tab[j]->pesel, tab[j]->zarobki, tab[j]->dataZat);
								}

							    free(tab);
								break;
					        case 3:
						    	qsort(tab, i, sizeof(struct Osoba*), porownajPesel);

						    	for(j=0; j<i; j++){
									printf("Imie: %s\nNazwisko: %s\nPesel: %s\nZarobki: %d\nData zatrudnienia: %d\n\n",tab[j]->imie, tab[j]->nazwisko, tab[j]->pesel, tab[j]->zarobki, tab[j]->dataZat);
								}

							    free(tab);
								break;
						    case 4:
						    	qsort(tab, i, sizeof(struct Osoba*), porownajZarobki);

						    	for(j=0; j<i; j++){
									printf("Imie: %s\nNazwisko: %s\nPesel: %s\nZarobki: %d\nData zatrudnienia: %d\n\n",tab[j]->imie, tab[j]->nazwisko, tab[j]->pesel, tab[j]->zarobki, tab[j]->dataZat);
								}

							    free(tab);
								break;
							case 5:
						    	qsort(tab, i, sizeof(struct Osoba*), porownajDateZat);

						    	for(j=0; j<i; j++){
									printf("Imie: %s\nNazwisko: %s\nPesel: %s\nZarobki: %d\nData zatrudnienia: %d\n\n",tab[j]->imie, tab[j]->nazwisko, tab[j]->pesel, tab[j]->zarobki, tab[j]->dataZat);
								}

							    free(tab);
								break;
					        default:
								printf("Nie ma mozliwosci o tym numerze\n");
				        }
			    	}
	}
}
