#include <stdio.h>
#include <math.h>


char stack[20]; // locul temporar unde se pun operatorii pina se reorganizeaza propozitia
int virf = -1; //numarul de elemente din stack , sau pozitia ultimului element , daca e -1 atunci e gol
int x, y, i;
char prop[256]/*propozitia introdusa de la tastatura*/, post[256]/**propozita procesata intr-un mod mai citibil de calculator */, lit[128]/*stackul pentru litere*/, valori[128], posti[256], rez[256];

void instructiuni() {
	printf("Instructiuni:\n\n"
		" Negatia NOT -\n"
		" Conjunctia AND &\n"
		" Disjunctia OR |\n"
		" Sau exclusiv XOR @\n"
		" Implicatia IMPLICA >\n"
		" Echivalenta IFF =\n"
		" Pentru dubla negatie folositi parantezele -(-p)\n\n");
}

void copie(char *din, char in[]) {
	int i = 0;
	while (*din != '\0') {
		in[i] = *din;
		din++;
		i++;
	}
	in[i] = '\0';
}

void in(char *p) {
	printf("\Introdu propozitia logica : ");
	scanf("%s", p);
}

void bagaInStack(char x)
{
	stack[++virf] = x;
}

char scoateDinStack()
{
	if (virf == -1)
		return -1;
	else
		return stack[virf--];
}

int priOper(char x)
{
	if (x == '(')
		return 0;
	if (x == '=')
		return 1;
	if (x == '>')
		return 2;
	if (x == '@')
		return 3;
	if (x == '|')
		return 4;
	if (x == '&')
		return 5;
	if (x == '!')
		return 6;
}

void infixToPostfix(char *p, char *p1) {
	char x;
	while (*p != '\0')
	{
		if (isalnum(*p)) {//functie predefinita care verifica daca caracterul este unul alfanumeric
			*p1 = *p;//daca este cifra sau numar il punem in rezultatul postfix
			p1++;
		}
		else if (*p == '(')//daca e paranteza ( il punem in stack
			bagaInStack(*p);
		else if (*p == ')')//daca e paranteza ) scoatem din stack pina dam de paranteza ( si o stergem din rezultat
		{
			while ((x = scoateDinStack()) != '(')
			{
				*p1 = x;
				p1++;
			}
		}
		else
		{
			while (priOper(stack[virf]) >= priOper(*p))//daca operatorul din stack are prioritate mai mare ca cel analizat
			{
				*p1 = scoateDinStack();//il scoatem din stack si il punem la rezultat pina cind dam de un operator cu prioritate mai mica decit cel analizat
				p1++;
			}
			bagaInStack(*p);
		}
		p++;
	}
	while (virf != -1)//daca am ajuns la capat punem tot ce a ramas in rezultat
	{
		*p1 = scoateDinStack();
		p1++;
	}
	*p1 = '\0';
}

void alegeNumara(char *s, char *p2, int *i) {
	(*i) = 0;
	while (*s != '\0')
	{
		int caracter = (int)(*s);
		if (((caracter > 64) && (caracter < 91)) || ((caracter > 96) && (caracter < 123))) {
			//contor pt litere asemenea
			int c = 0;
			//parcurge tot vectorul de litere pt a verifica daca avem litera
			for (int k = 0; k < *i; k++)
			{
				if (*s == p2[k])
					c++;
			}
			//daca nu a gasit litera o adauga
			if (c == 0) {
				p2[*i] = *s;
				(*i)++;
			}
		}
		s++;
	}		
	p2[*i] = '\0';
}

void creazaTabel(char *s, int l) {
	int p, c = 0;
	for (int j = 0; j < y - 1; j++)//pentru fiecare coloana
	{
	    c = 0;//contor = 0
		p = pow(2, l - j);//numarul de elemente pe coloana,
		for (int i = 0; i < x; i++)//pentru fiecare element din coloana
		{
			if (c < p / 2)//daca elementul e pina la jumatate =0 in caz contrar =1
				s[i*y + j] = '0';
			else
				s[i*y + j] = '1';
			c++;
		}
	}
	for (int i = 0; i < x; i++)
	{
		s[i*y + y - 1] = '#';
	}
}

int pos(char c, char *p2, int l) {
	for (int i = 0; i < l; i++)
		if (c == p2[i])
			return i;
}

int valoarea(char s) {//0 sau 1 din caracter in cifra , scapam de efectul ASCII

	return ((int)s) - 48;
}

void rind(int ind, char *p1, char* pi, char* t) {
	int j = 0;
	while (*p1 != '\0') {
		int caracter = (int)(*p1);
		if (((caracter > 64) && (caracter < 91)) || ((caracter > 96) && (caracter < 123)))
			posti[j] = t[ind*y + pos(*p1, &lit, i)];
		else
			posti[j] = *p1;
		j++;
		p1++;
	}
	pi[j] = '\0';
}

char not(char a) {
	if (a == '0')
		return '1';
	else
		return '0';
}

int calcul(char x, int op1, int op2) {
	if (x == '&')
		return(op1 & op2);
	if (x == '|')
		return(op1 | op2);
	if (x == '>')
		return(not(op1) | op2);
	if (x == '=')
		return((not(op1) | op2) & (not(op2) | op1));
	if (x == '@')
		return(not(calcul('=', op1, op2)));
}

char eval(char *s) {
	char tmp2[128];
	int i = 0;
	while (*s != '\0') {
		int caracter = (int)(*s);
		if (caracter == 48 || caracter == 49) {//daca e 0 sau 1
			tmp2[i] = *s;
			i++;
		}
		else {
			if (*s == '-') {//daca e not facem not ultimei litere din stack si o punem inapoi schimbata
				tmp2[i - 1] = not(tmp2[i - 1]);
			}
			else {
				char tmp = (char)(calcul(caracter, (int)tmp2[i - 1], (int)tmp2[i - 2]));
				//daca e alt operator facem caluclul dintre ultimele 2 litere din stack si operatorul care il analizam si punem inapoi 1 litera rezultanta
				tmp2[i - 2] = tmp2[i - 1] = tmp2[i];//golim ultimele 3 locuri din stack
				i -= 2;//pozitia din stack se micsoreaza cu 2 unitati
				tmp2[i] = tmp;// adaugam la stack(sau rezultat) litera obtinuta
				i++;
			}
		}
		s++;

	}
	return(tmp2[0]);//returnam valoarea obtinuta
}

void afiseaza(char *t, char *p, char p2[]) {
	printf("\n");
	for (int i = 0; i < y; i++)
		if (i == y - 1) {
			printf("%s ", p);//afiseaza literele
		}
		else
			printf("%c ", p2[i]);//afiseaza propozitia
	printf("\n");
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			printf("%c ", t[i*y + j]);//afiseaza valorile calculate pt ultima coloana
		}
		printf("\n");
	}
}

void vertip(char *t) {
	int contor1, contor2;
	char val[8], sat[8], nes[8], cont[8];
	contor1 = contor2 = 0;
	for (int i = 0; i < x; i++)
	{
		if (t[i*y + y - 1] == '0')
			contor1++;//daca intilneste un 0 sau F incrementam contorul 1
		else
			contor2++;// in caz contrar incrementam contor 2
	}
	if (contor1 != 0 && contor2 != 0) {//daca este cel putin un F si A
		copie("Nu este\0", val);
		copie("Este\0", sat);
		copie("Nu este\0", nes);
		copie("Este\0", cont);
	}
	if (contor1 == 0 && contor2 != 0) {//daca nu e nici un F si este cel putin 1 A => mereu A
		copie("Este\0", val);
		copie("Este\0", sat);
		copie("Nu este\0", nes);
		copie("Nu este\0", cont);
	}
	if (contor1 != 0 && contor2 == 0) {//daca nu e nici un A si este cel putin 1 F => mereu F
		copie("Nu este\0", val);
		copie("Nu este\0", sat);
		copie("Este\0", nes);
		copie("Este\0", cont);
	}
	printf("\nPropozitia data:\n%s valida\n%s satisfiabila\n%s nesatisfiabila\n%s contingenta\n", val, sat, nes, cont);
}

void main() {
	char *p, *p1, *p2, *t, *val, *pi, *r;
	int j;
	instructiuni();//afisam instructiunile 
	in(prop);//citim de la tastatura propozitia si o atribuim sirului de caractere "prop"
	system("cls");//Stergem ecranul
	p = &prop;
	p1 = &post;
	p2 = &lit;
	val = &valori;
	pi = &posti;
	r = rez;
	infixToPostfix(p, p1);//transformam propozitia introdusa intr-un mod mai usor citibil de calculator
	alegeNumara(p1, p2, &i);//alegem si numaram literele din "p1" care este postfix si le punem in sirul de caractere "p2" si numarul de litere in "i"
	x = pow(2, i); //numarul de elemente pe coloana
	y = i + 1;// numarul de coloane = numarul de litere + rezultatul final
	t = (char *)(malloc(x*y * sizeof(char)));// alocarea memoriei tabelului
	creazaTabel(t, i);
	for (int row = 0; row < x; row++)//pentru fiecare rind calculam valoarea finala
	{
		rind(row, p1, pi, t);//pi=valorile de pe rindul row cu operatiile in ordine postfixa
		char temporal = eval(pi);//calculeaza valoarea
		t[row*y + y - 1] = temporal;//atribuie valoarea din ultima colonita a rindului dat
	}
	afiseaza(t, p, lit);//Afiseaza tabel
	vertip(t);//verifica tip
	getch();
}	
