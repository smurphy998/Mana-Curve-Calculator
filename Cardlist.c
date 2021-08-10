#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"

void finish_with_error(MYSQL *list){
	fprintf(stderr,"%s\n",mysql_error(list));
	mysql_close(list);
	exit(1);
	}
int listinitialize();
/*int main(){
	listinitialize();
	exit(0);
}*/
int listinitialize(){
	MYSQL *list= mysql_init(NULL);
	if(list ==NULL){	
	fprintf(stderr,"%s\n",mysql_error(list));
	return(1);
	}
	if(mysql_real_connect(list,"localhost","tester","Smurphy1998!","cardlist",0,NULL,0)==NULL){
	fprintf(stderr,"%s\n",mysql_error(list));
	mysql_close(list);
	return(1);
	}
	//This creates the table if the table is not already created;
	if (mysql_query(list, "CREATE TABLE IF NOT EXISTS cardlist(name VARCHAR(255) PRIMARY KEY, type VARCHAR(255), ManaCost INT,Colorless Int, White INT, Green INT, Red INT, Black INT, Blue INT, ManaProduced INT)")){
	fprintf(stderr,"%s\n",mysql_error(list));
	//if the table is already created then it is possible that there will be an error thrown, however for the purposes of this, it does not really matter
	mysql_close(list);
	return(1);
	}
	mysql_close(list);
	return(0);
}
void cardlister(Card c){
	MYSQL *list= mysql_init(NULL);
	if(list ==NULL){	
	fprintf(stderr,"%s\n",mysql_error(list));
	}
	if(mysql_real_connect(list,"localhost","tester","Smurphy1998!","cardlist",0,NULL,0)==NULL){
	fprintf(stderr,"%s\n",mysql_error(list));
	mysql_close(list);
	}
	/*if (mysql_query(list, "CREATE TABLE IF NOT EXISTS cardlist(name VARCHAR(255), type VARCHAR(255), ManaCost INT,Colorless Int, White INT, Green INT, Red INT, Black INT, Blue INT, ManaProduced INT)")){
	fprintf(stderr,"%s\n",mysql_error(list));
	//if the table is already created then it is possible that there will be an error thrown, however for the purposes of this, it does not really matter
	mysql_close(list);
	exit(1);
	}*/
	char queryCommand[500];
	char carrier[50];
	strcpy(queryCommand,"INSERT INTO cardlist VALUES(");
	strcat(queryCommand,"'");
	strcat(queryCommand,c.name);
	strcat(queryCommand,"'");
	strcat(queryCommand,",");
	strcat(queryCommand,"'");
	strcat(queryCommand,c.cardtype);
	strcat(queryCommand,"'");
	strcat(queryCommand,",");
	sprintf(carrier,"%d",c.ManaCost);
	strcat(queryCommand,carrier);
	strcat(queryCommand,",");
	sprintf(carrier,"%d",c.ColorlessMana);	
	strcat(queryCommand,carrier);
	strcat(queryCommand,",");
	sprintf(carrier,"%d",c.WhiteMana);
	strcat(queryCommand,carrier);
	strcat(queryCommand,",");
	sprintf(carrier,"%d",c.GreenMana);
	strcat(queryCommand,carrier);
	strcat(queryCommand,",");
	sprintf(carrier,"%d",c.RedMana);
	strcat(queryCommand,carrier);
	strcat(queryCommand,",");
	sprintf(carrier,"%d",c.BlackMana);
	strcat(queryCommand,carrier);
	strcat(queryCommand,",");
	sprintf(carrier,"%d",c.BlueMana);	
	strcat(queryCommand,carrier);
	strcat(queryCommand,",");
	sprintf(carrier,"%d",c.ManaProduced);	
	strcat(queryCommand,carrier);
	strcat(queryCommand,")");
	if(mysql_query(list,queryCommand)){
		//fprintf(stderr,"The card addition failed\n");
		//fprintf(stderr,"%s\n",mysql_error(list));
		//fprintf(stderr,"%s\n",queryCommand);
		return;
	}
	//fprintf(stderr,"%s\n",queryCommand);
	strcpy(queryCommand,"");
	mysql_close(list);			
}
void grabber(char *line){
	MYSQL *list= mysql_init(NULL);
	if(list ==NULL){	
	fprintf(stderr,"%s\n",mysql_error(list));
	return;
	}
	if(mysql_real_connect(list,"localhost","tester","Smurphy1998!","cardlist",0,NULL,0)==NULL){
	fprintf(stderr,"%s\n",mysql_error(list));
	mysql_close(list);
	return;
	}
	
	char queryCommand[500];
	strcpy(queryCommand,"SELECT * FROM cardlist WHERE name like '%");
	strcat(queryCommand,line);
	strcat(queryCommand,"%'");
	fprintf(stderr,"%s\n",queryCommand);
	if (mysql_query(list,queryCommand)){ 		
	fprintf(stderr,"%s\n",mysql_error(list));
	mysql_close(list);
	return;
	}
	MYSQL_RES *result= mysql_store_result(list);
	if(result==NULL){	
	fprintf(stderr,"%s\n",mysql_error(list));
	mysql_close(list);
	return;
	}
	int nums=mysql_num_fields(result);
	MYSQL_ROW row;
	row=mysql_fetch_row(result);
	strcpy(line,row[0]);
	for(int i=1;i<nums;i++){
		strcat(line,"/");
		strcat(line,row[i]);
		}
	fprintf(stderr,"%s\n",line);
}

