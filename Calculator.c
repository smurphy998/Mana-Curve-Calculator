#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

struct Card{
	//The entry style is name/cardtype/manacost/colorless/white/green/red/black/blue/manaproduced/numberplayed\n
	char 	name[30];
	//Enter the name and cardtypes to ensure it works properly.
	char 	cardtype[25];
	int  	ManaCost;
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
	char	cidentity[5];
	//In commander everything aside from basic lands should be above one
	//else the maximum number is 4
};
FILE *FP;
struct Card Deck[100];
void identitycheck(struct Card c,int cards);
void cardcreate(char line, int cards);
void printdetails(struct Card c);
void tooManyWarning(struct Card c);
void coloridentitycheck(char *ci, struct Card c);
int main(int argc, char *argv[]){
	char *Type;
	//Different formats have different deck rules
	if (argv[1]=="Commander"){
		Type="Commander";
	}else if(argv[1]=="Modern"){
		Type="Modern";
	}else{
		printf("Give Deck Type");
	}
	int cards=0;
	char *coloridentity;
	char destination;
	printf("Please create a destination for the information, and be sure to include the '.txt'");
	scanf("%s",&destination);
	if (argc==3){
		FP=fopen(&destination,"w");
		FILE *fp= fopen(argv[2],"r");
		if(fp !=NULL){
			char line;
			while(fgets(&line,sizeof(line),fp)!=NULL){
			cardcreate(line,cards);
			if(strcmp(Type,"Commander")){
				if(cards==0){
					//Declares the first card as the commander
					coloridentity=Deck[cards].cidentity;
					printf("%s is the commander.",Deck[cards].name);  
				}else{
					coloridentitycheck(coloridentity,Deck[cards]);			
				}
				//Checks the number of copies against the legal limit
				if(Deck[cards].NumberPlayed>1&&(strcmp(Deck[cards].name,"Plains")||strcmp(Deck[cards].name,"Forest")||strcmp(Deck[cards].name,"Mountain") ||strcmp(Deck[cards].name,"Swamp")||strcmp(Deck[cards].name,"Island"))){
					tooManyWarning(Deck[cards]);
				}
			}
			if(strcmp(Type,"Modern")){
				if(Deck[cards].NumberPlayed>4&&(strcmp(Deck[cards].name,"Plains")||strcmp(Deck[cards].name,"Forest")||strcmp(Deck[cards].name,"Mountain") ||strcmp(Deck[cards].name,"Swamp")||strcmp(Deck[cards].name,"Island"))){
					tooManyWarning(Deck[cards]);
					}
				   }
			cards++;
			}
			fclose(fp);
		}
		else{
			fclose(fp);
			printf("give a deck list txtfile or enter deeclist manually\n Type 'Confirm' and return if you wish to enter Manually.\n");
			char concheck[100];
			scanf("%s",concheck);
			//Manual input section
			if(strcmp(concheck,"Confirm")){
				  printf("The entry style is name/cardtype/manacost/colorless/white/green/red/black/blue/manaproduced/numberplayed\n");
				  //To be completed
			}
		}
	}
fclose(FP);
return 0;
}
void cardcreate(char line, int cards){
	//This function tokenizes the line and converts it into a card object within the array
	const char split[2]="/";
	strcpy(Deck[cards].name,strtok(&line,split));
	strcpy(Deck[cards].cardtype,strtok(NULL,split));
	Deck[cards].ManaCost=atoi(strtok(NULL,split));
	Deck[cards].ColorlessMana=atoi(strtok(NULL,split));
	Deck[cards].WhiteMana=atoi(strtok(NULL,split));
	Deck[cards].GreenMana=atoi(strtok(NULL,split));
	Deck[cards].RedMana=atoi(strtok(NULL,split));
	Deck[cards].BlackMana=atoi(strtok(NULL,split));
	Deck[cards].BlueMana=atoi(strtok(NULL,split));
	Deck[cards].ManaProduced=atoi(strtok(NULL,split));
	Deck[cards].NumberPlayed=atoi(strtok(NULL,split));
	identitycheck(Deck[cards],cards);
	printdetails(Deck[cards]);
}
void printdetails(struct Card c){
	if(strstr(c.cardtype,"Land")==NULL){
		printf("The card is %s of %s cardtype which costs %i mana\nBroken down into %i colorless, %i white, %i green,%i red, %i black, %i blue mana, and produces %i mana\nThe number played within the deck is %i\n",c.name,c.cardtype,c.ManaCost,c.ColorlessMana,c.WhiteMana,c.GreenMana,c.RedMana,c.BlackMana,c.BlueMana,c.ManaProduced,c.NumberPlayed);
		fprintf(FP,"The card is %s of %s cardtype which costs %i mana\nBroken down into %i colorless, %i white, %i green,%i red, %i black, %i blue mana, and produces %i mana\nThe number played within the deck is %i\n",c.name,c.cardtype,c.ManaCost,c.ColorlessMana,c.WhiteMana,c.GreenMana,c.RedMana,c.BlackMana,c.BlueMana,c.ManaProduced,c.NumberPlayed);
	}else{
		printf("The card is %s of %s cardtype and produces %i mana\nThe number played within the deck is %i\n",c.name,c.cardtype,c.ManaProduced,c.NumberPlayed);
		fprintf(FP,"The card is %s of %s cardtype and produces %i mana\nThe number played within the deck is %i\n",c.name,c.cardtype,c.ManaProduced,c.NumberPlayed);
	}
}
			 
void tooManyWarning(struct Card c){
	//This is effectively working as a flag
	printf("%s has too many copies in the deck\n",c.name);
	fprintf(FP,"%s has too many copies in the deck\n",c.name);
	}
void coloridentitycheck(char *ci,struct Card c){
	if(!(ci[0]=='-'&&c.cidentity[0]!='W')){
	if(!(ci[1]=='-'&&c.cidentity[1]!='G')){
	if(!(ci[2]=='-'&&c.cidentity[2]!='R')){
	if(!(ci[3]=='-'&&c.cidentity[3]!='B')){
	if(!(ci[4]=='-'&&c.cidentity[4]!='B')){
		fprintf(FP,"%s is inside of color identity\n", c.name);
		printf("%s is inside of the color identity.\n", c.name);
		return;
		}
	}
	}
	}
	}
		fprintf(FP,"%s is out of color identity\n", c.name);
		printf("%s is out of color identity\n", c.name);
	}
void identitycheck(struct Card c,int cards){
	char result[5]={0,0,0,0,0};
	if(c.WhiteMana>0){
		result[0]='W';
	}else{
		result[0]='-';
	}
	if(c.GreenMana>0){
		result[1]='G';
	}else{
		result[1]='-';
	}
	if(c.RedMana>0){
		result[2]='R';
	}else{
		result[2]='-';
	}
	if(c.BlackMana>0){
		result[3]='B';
	}else{
		result[3]='-';
	}
	if(c.BlueMana>0){
		result[4]='B';
	}else{
		result[4]='-';
	}
	strcpy(Deck[cards].cidentity,result);
}





























































