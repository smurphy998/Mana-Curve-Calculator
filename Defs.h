//Calculator.c
typedef struct Card{
	char name[50];
	char cardtype[40];
	int ManaCost;
	int ColorlessMana;
	int WhiteMana;
	int GreenMana;
	int RedMana;
	int BlackMana;
	int BlueMana;
	int ManaProduced;
	int NumberPlayed;
	char cidentity[5];
}Card;
//Cardlist.c
int listinitialize();
void cardlister(Card c);
void grabber(char *line);
