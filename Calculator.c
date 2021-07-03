#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<iostream>
class Card{
	public:
	char 	name;
	//Enter the name and cardtypes without spaces as to ensure it works properly.
	char 	cardtype;
	int  	Manacost;
	/*These five are getting the mana requirements
	 *which will help with color idenity for commander,
	 as well as doubles for discerning the supply of each mana
	 that the deck both needs as well as gives itself*/
	int 	ColorlessMana;
	int	WhiteMana;
	int	GreenMana;
	int	RedMana;
	int	BlackMana;
	int 	BlueMana;
	int	ManaProduced;
	int	NumberPlayed;
	//In commander everything aside from basic lands should be above one
	//else the maximum number is 4
	Card(){}
	Card(char na,char caty, int mac,int cm,int wm,int gm, int rm,int bm,int blm,int mp, int np){
		name=na;
      		cardtype=caty;
  		Manacost=mac;
		ColorlessMana=cm;
		WhiteMana=wm;
		GreenMana=gm;
		RedMana=rm;
		BlackMana=bm;
		BlueMana=blm;
		ManaProduced=mp;
		NumberPlayed=np;		
	void printdetails(){
		cout<<name<<"costs"<<ManaCost<<"/"<<ColorlessMana<<"Colorless/"<<WhiteMana<<"White/"<<GreenMana"Green/"<<RedMana<<"Red/"<<BlackMana<<"Black/"<<BlueMana<<"Blue/"<<ManaProduced<<"ManaGeneration"<<NumberPlayed
	}
	void tooManyWarning(){
		cout<<name<<"Has too many copies in the deck"
	}
	void outofcoloridentity(){
		cout<<name<<"is out of color identity"
	}
}
int main(int argc, char *argv[]){
	char Type;
	if (argv[1]=="Commander"){
		Type=Commander;
	}else{
		Type=Modern;
	}
	Card Deck[100];
	FILE *fp= fopen( argv[2],"r");
	if(fp !=NUll){
		char line[256]
		while(fgets(line,sizeof(line),fp)!=NULL){
		
		}
	}else{
	cout<<"give a deck list"
	}
return 0;
}
