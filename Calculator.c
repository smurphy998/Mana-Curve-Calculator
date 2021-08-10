#include<stdio.h>
#include<mysql/mysql.h>
#include<string.h>
#include<math.h>
#include"Defs.h"
#include<stdlib.h>

/* A lot of the commented codes are failed implementation and debugger helpers, as they were either attmepts to resolve an issue or detecting where the problem was
 * This is a forestatement as it is will not be individually commented on all parts of the st
 */

/*typedef struct Card{
	//The entry style is name/cardtype/manacost/colorless/white/green/red/black/blue/manaproduced/numberplayed\n
	char 	name[50];
	//Enter the name and cardtypes to ensure it works properly.
	char 	cardtype[40];
	int  	ManaCost;
	These five are getting the mana requirements
	 *which will help with color idenity for commander,
	 as well as doubles for discerning the supply of each mana
	 that the deck both needs as well as gives itself
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
	//Abilities may be added at some point.
	//The definition go moved into Defs.h
}Card;*/

FILE *FP;
FILE *Analyst;
void emptyscanner();
Card Deck[100];
char destination[300];
void Manacurve(int cards,int lands);
void analysis(int cards);

void identitycheck(Card c,int cards);
void cardcreate(char *line, int cards);
void printdetails(Card c);
void tooManyWarning(Card c,char *type);
void coloridentitycheck(char *ci,Card c);
int main(int argc, char *argv[]){
	char *Type;
	int check;
	printf("The rules for this are rather simple you shall give your decklist and it will be taken into the system and analyzed\nThere are different ways of doing this, one is providing a textfile with all of the information about the deck and cards\nNext is using the database where you can put together a deck list and get the analysis on the decklist\nThe first two methods are primarily for admins to be able to debug the system, meanwhile the primary method\nWill be a file set up with the names to read from the database\n");
	check=listinitialize();
	for(int t=0;t<100;t++){
		Card Blank;
		//Creating dummy cards became more effective than mallocing for the array
		Blank=(Card){"NoName","NoTYPE",0,0,0,0,0,0,0,0,0,{0,0,0,0,0}};
		Deck[t]=Blank;
		//used to test the reference and maker sure it was properly accessible
		//We found its an issue with the card counter
		//printf("%s\n",Deck[t].name);
	}
		//Different formats have different deck rules
	if (strcmp(argv[1],"Commander")!=0&&strcmp(argv[1],"Modern")!=0){
		//This will reject the improper input
		printf("Give Deck Type with File.");
		return 0;
	}
	//printf("%i\n",cards);
	//printf("%s\n",argv[1]);
	char *coloridentity;
	char filename[300];
	strcpy(destination,strtok(argv[2],"."));
	strcpy(filename,destination);
	strcat(filename,".txt");
	strcat(destination,argv[1]);
	//printf("Please state the name of the deck with .txt extension\n");
	//scanf("%s",destination);
	//printf("%s",destination);
	//printf("%i",argc);
	if (strcmp(argv[3],"textfile")==0){
		char Decklist[300];
		strcpy(Decklist,destination);
		strcat(Decklist,"-Decklist.txt");
		FP=fopen(Decklist,"w");
		//printf("%s\n",Decklist);
		FILE *fp= fopen(filename,"r");
		if(fp==NULL){
			exit(EXIT_FAILURE);
		}
		if(fp!=NULL){
			char line[300];
			int cards=0;
			while(fgets(line,sizeof(line),fp)!=NULL){
			//printf("%s",line);
			//printf("%i\n",cards);
			cardcreate(line,cards);
			cardlister(Deck[cards]);
			if(strcmp(argv[1],"Commander")==0){
				//printf("Commander confirmed\n");
				if(cards==0){
					//Declares the first card as the commander
					coloridentity=Deck[cards].cidentity;
					//printf("%s is the commander.",Deck[cards].name);
				      	fprintf(FP,"%s is the Commander",Deck[cards].name);	
				}else{
					coloridentitycheck(coloridentity,Deck[cards]);			
				}
			}
				//Checks the number of copies against the legal limit
				tooManyWarning(Deck[cards],argv[2]);
			cards++;
			}
			analysis(100);
			fclose(fp);
		}
	}
	if(strcmp(argv[3],"database")==0){
		char Decklist[300];
		strcpy(Decklist,destination);
		strcat(Decklist,"-Decklist.txt");
		FP=fopen(Decklist,"w");
		char line[300];
		int numberplayed;
		char numkeep[3];
		int cards;
		cards=0;

		printf("please enter first card name\n");
		fgets(line,sizeof(line),stdin);
		//printf("%s\n",line);
		strcpy(line,strtok(line,"\n"));
		grabber(line);
		//printf("%s\n",line);
		printf("Please enter number of copies\n");
		scanf("%d",&numberplayed);
		if(numberplayed==0){
			numberplayed=1;
		}
		strcat(line,"/");
		sprintf(numkeep,"%d",numberplayed);	
		strcat(line,numkeep);
		cardcreate(line,cards);
		tooManyWarning(Deck[cards],argv[2]);
		cards++;
		printf("please enter next card name\n");
		while(1){
		emptyscanner();
		//The line won't become blank if you don't enter anything so it needs to be a space to end it
		scanf("%50[^\n]",line);
		printf("%s\n",line);
		if(strcmp(line," ")==0){
			fclose(FP);
			analysis(100);
			exit(0);
		}
		//printf("%s\n",line);
		strcpy(line,strtok(line,"\n"));
		grabber(line);
		//printf("%s\n",line);
		printf("Please enter number of copies\n");
		scanf("%d",&numberplayed);
		if(numberplayed==0){
			numberplayed=1;
		}
		strcat(line,"/");
		sprintf(numkeep,"%d",numberplayed);	
		strcat(line,numkeep);
		cardcreate(line,cards);
		cards++;
		tooManyWarning(Deck[cards],argv[2]);
		printf("please enter next card name\n");
		}
	}


fclose(FP);
return 0;
}
void cardcreate(char *line, int cards){
	//This function tokenizes the line and converts it into a card object within the array

	char split[]="/";
	//Malloc became too complicated for the structure at hand
	strcpy(Deck[cards].name,strtok(line,split));
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
	//printf("Returned from card creation\n");
	return;
}

//The printing to terminal is no longer necessary as it properly prints, so the print to terminal is being removed for the time being to allow easier debugging within the 
void printdetails(Card c){
	if(!strstr(c.cardtype,"Land")){
//	printf("The card is %s of %s cardtype which costs %i mana\nBroken down into %i colorless, %i white, %i green,%i red, %i black, %i blue mana, and produces %i mana\nThe number played within the deck is %i\n",c.name,c.cardtype,c.ManaCost,c.ColorlessMana,c.WhiteMana,c.GreenMana,c.RedMana,c.BlackMana,c.BlueMana,c.ManaProduced,c.NumberPlayed);
		fprintf(FP,"The card is %s of %s cardtype which costs %i mana\nBroken down into %i colorless, %i white, %i green,%i red, %i black, %i blue mana, and produces %i mana\nThe number played within the deck is %i\n",c.name,c.cardtype,c.ManaCost,c.ColorlessMana,c.WhiteMana,c.GreenMana,c.RedMana,c.BlackMana,c.BlueMana,c.ManaProduced,c.NumberPlayed);
	}else{
//		printf("The card is %s of %s cardtype and produces %i mana\nThe number played within the deck is %i\n",c.name,c.cardtype,c.ManaProduced,c.NumberPlayed);
		fprintf(FP,"The card is %s of %s cardtype and produces %i mana\nThe number played within the deck is %i\n",c.name,c.cardtype,c.ManaProduced,c.NumberPlayed);
	}
}
			 
void tooManyWarning(Card c,char *type){
	//This is effectively working as a flag
	//printf("%s has too many copies in the deck\n",c.name);
	if(strcmp(type,"Commander")==0){
	if(c.NumberPlayed>1&&!(strcmp(c.name,"Plains")||strcmp(c.name,"Forest")||strcmp(c.name,"Mountain") ||strcmp(c.name,"Swamp")||strcmp(c.name,"Island"))){
	fprintf(FP,"%s has too many copies in the deck\n",c.name);
	}
	}
	if(strcmp(type,"Modern")==0){
		if(c.NumberPlayed>4&&!(strcmp(c.name,"Plains")||strcmp(c.name,"Forest")||strcmp(c.name,"Mountain") ||strcmp(c.name,"Swamp")||strcmp(c.name,"Island"))){
	fprintf(FP,"%s has too many copies in the deck\n",c.name);
		}
	}
}
void coloridentitycheck(char *ci,Card c){
	if((ci[0]=='-'&&c.cidentity[0]!='W')||(ci[1]=='-'&&c.cidentity[1]!='G')||(ci[2]=='-'&&c.cidentity[2]!='R')||(ci[3]=='-'&&c.cidentity[3]!='B')||(ci[4]=='-'&&c.cidentity[4]!='B')){
		fprintf(FP,"%s is inside of color identity\n", c.name);
	//	printf("%s is inside of the color identity.\n", c.name);
		return;
	
	}
		fprintf(FP,"%s is out of color identity and needs to be removed\n", c.name);
	//	printf("%s is out of color identity\n", c.name);
	}
void identitycheck(Card c,int cards){
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
	return;
} 
void analysis(int cards){
//Names will also be introduced into the mixture at a later point within incarnations, as it will allow the system to determine the Meta of certain decks, like if Modern deck contains Amulets of Vigor, alongside multiple Primeval Titans, it can be inferred that it might be an Amulet Titan Deck
int creatures=0;
//Implementation of detecting creature types will be added at a later date, this will be used for identifying if the deck is tribal
int enchantments=0;
int artifacts=0;
int lands=0;
int instants=0;
int sorcereries=0;
int planeswalkers=0;
char analysisresults[100];
int slotsused=0;
strcat(analysisresults,destination);
strcat(analysisresults,"-analysis.txt");
printdetails(Deck[1]);
Analyst=fopen(analysisresults,"w"); 
for(int i=0;i<cards;i++){
	if(strcmp(Deck[i].name,"NoName")==0){
	break;
	}
	if(strstr(Deck[i].cardtype,"Creature")){
		creatures+=Deck[i].NumberPlayed;
		//slotsused+=Deck[i].NumberPlayed;
		//printf("i",creatures);
		//creaturetype analysis will go here.
	}
	if(strstr(Deck[i].cardtype,"Enchantment")){
		enchantments+=Deck[i].NumberPlayed;	
		//slotsused+=Deck[i].NumberPlayed;
	}		
	if(strstr(Deck[i].cardtype,"Artifact")){
		artifacts+=Deck[i].NumberPlayed;
		//slotsused+=Deck[i].NumberPlayed;
	}
	if(strstr(Deck[i].cardtype,"Land")){
		lands+=Deck[i].NumberPlayed;
		//slotsused+=Deck[i].NumberPlayed;
	}
	if(strstr(Deck[i].cardtype,"Instant")){
		instants+=Deck[i].NumberPlayed;
		//slotsused+=Deck[i].NumberPlayed;
	}
	if(strstr(Deck[i].cardtype,"Sorcery")){
		sorcereries+=Deck[i].NumberPlayed;
		//slotsused+=Deck[i].NumberPlayed;
	}
	if(strstr(Deck[i].cardtype,"Planeswalker")){
		planeswalkers+=Deck[i].NumberPlayed;
		//slotsused+=Deck[i].NumberPlayed;
	}
	slotsused+=Deck[i].NumberPlayed;
}
//printf("The number of creatures is: %i\nThe number of enchantments is: %i\nThe number of Artifacts is: %i\nThe number of lands is %i\nThe number of instants is: %i\nThe number of Sorcereries is: %i\nThe Number of Planewalkers is %i\n",creatures,enchantments,artifacts,lands,instants,sorcereries,planeswalkers);
fprintf(Analyst,"The number of creatures is: %i\nThe number of enchantments is %i\nThe number of Artifacts is: %i\nThe number of lands is %i\nThe number of instants is: %i\nThe number of Sorcereries is: %i\nThe Number of Planewalkers is: %i\n",creatures,enchantments,artifacts,lands,instants,sorcereries,planeswalkers);
Manacurve(slotsused,lands);
fclose(Analyst);
}
void Manacurve(int cards,int lands){
int manaproducers=0;
int manarocksdorks=0;
//These could have been tuples, however it would've been messy for the print statement
int manarange[17]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
int manadrops[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int manatotal=0;
for(int i=0;i<17;i++){
	//goes through the deck with each possible manacost looking for matches to add to the tally
	for(int u=0;u<cards;u++){
	if(Deck[u].ManaProduced>0&&i==0){
		manaproducers+=Deck[u].NumberPlayed;
		if(Deck[u].ManaCost>0){
			manarocksdorks+=Deck[u].NumberPlayed;
			//Later this will be expanded
		}
	}
	if(Deck[u].ManaCost==manarange[i]&&!strstr(Deck[u].cardtype,"Land")){
		manadrops[i]+=Deck[u].NumberPlayed;
		manatotal+=(Deck[u].NumberPlayed*Deck[u].ManaCost);
	}
	}
	//printf("There are %i %i drops within the deck\n",manadrops[i],manarange[i]);
	fprintf(Analyst,"There are %i %i drops within the deck\n",manadrops[i],manarange[i]);

}
//printf("There is a mixture of %i ManaDorks and ManaRocks\nThere is %i total ManaProducers\n",manarocksdorks,manaproducers);
fprintf(Analyst,"There is a mixture of %i ManaDorks and ManaRocks\nThere is %i total ManaProducers\n",manarocksdorks,manaproducers);
//printf("The average cost of cards within the Deck is %i\n",(manatotal/(cards-lands)));
fprintf(Analyst,"The average cost of cards within the Deck is %i",(manatotal/(cards-lands)));
//printf("%i\n", manatotal);
//printf("%i\n",(cards-lands));
}
void emptyscanner(){
	int c=getchar();
	for(;c!='\n'&&c!=EOF;c=getchar()){}
}
