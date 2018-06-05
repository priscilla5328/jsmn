//#define JSMN_PARENT_LINKS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../myproduct.h"


/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */


char* readJSONFile() {
	long Fsize;
	char* buffer;
	size_t result;
	FILE* fp;


	fp = fopen("myproduct.json", "rt");
	if( fp == NULL ) {
		printf("Error!!!!!\n");
		exit(1);
	}

	while(!feof(fp)){
		printf("%c", fgetc(fp));
	}

	fseek(fp, 0, SEEK_END);
	Fsize = ftell(fp)+1;
	rewind(fp); //save file size

	buffer = (char*)malloc(sizeof(char)*Fsize);
	if(buffer == NULL){
		fputs("Memory error", stderr);
		exit(2);
	}

	fread(buffer, 1, Fsize, fp);
	result = buffer;



	if(strlen(result) != Fsize-1){
		printf("Reading error");
		exit(3);
	}

	fclose(fp);
	free(buffer);
	return (void *)result;
}

void notjsonNameList(char* jsonstr, jsmntok_t *t, int tokcount, NametokenInfo *nametokenInfo){
  int a=0, b=0, parentnum=1;

  for (a=0; a<tokcount; a++){
		if(t[a+1].type == JSMN_ARRAY || t[a+1].type == JSMN_OBJECT){
			parentnum++;
		} else if(t[a+1].type != JSMN_ARRAY || t[a+1].type != JSMN_OBJECT){
			break;
		}
	}

	for(a=1, b=0; a<tokcount; a++){
    if(t[t[a].parent].parent == parentnum){
      b++;
    }

	}

}

void printObjectList(char* jsonstr, jsmntok_t *t, int tokcount, int *nameTokIndex){
	printf("***** Object List *****\n");

	int a=0, b=0, c=0, parentnum=1, oidx=0;
		for (a=2; a<tokcount; a++){
			if(t[a+1].type == JSMN_ARRAY || t[a+1].type == JSMN_OBJECT){
				parentnum++;
			} else if(t[a+1].type != JSMN_ARRAY || t[a+1].type != JSMN_OBJECT){
				oidx = t[a].size;
				break;
			}
		}

	for(a=0, b=0; a<tokcount; a++){
		if(t[a].size == oidx){
			nameTokIndex[b]=a+1;

			printf("object[%d] %.*s\n", b+1, t[a+2].end-t[a+2].start, jsonstr+t[a+2].start);
			b++;
		}
	}

}

int makeProduct(const char *jsonstr, jsmntok_t *t, int tokcount, product_t* p[]){
	int i=0, parent=-1, parentnum=0, a=0, b=0;

		for(i=0; i<tokcount; i++){
			if(t[i+1].type == JSMN_STRING && t[i+2].type == JSMN_ARRAY){
				parent++;
				continue;
			}
			if(t[i+1].type == JSMN_ARRAY || t[i+1].type == JSMN_OBJECT){
				parent++;
			}else if(t[i+1].type != JSMN_ARRAY || t[i+1].type != JSMN_OBJECT){
				break;
			}
		}

		//printf("%d\n", p);
		char price[10], count[10], company[10];

		for(a=1, b=0; a<tokcount; a++){
			p[b] = (product_t*)malloc(sizeof(product_t));
			if(t[a].parent == parent){
				sprintf(p[b]->name, "%.*s", t[a+4].end - t[a+4].start, jsonstr + t[a+4].start);
				sprintf(price, "%.*s", t[a+6].end - t[a+6].start, jsonstr + t[a+6].start);
				sprintf(company, "%.*s", t[a+2].end - t[a+2].start, jsonstr + t[a+2].start);
				sprintf(count, "%.*s", t[a+8].end - t[a+8].start, jsonstr + t[a+8].start);

				p[b]->price = atoi(price);
				p[b]->count = atoi(count);

				if(strcmp(company,"ORION")==0) p[b]->company = ORION;
				if(strcmp(company,"LOTTE")==0) p[b]->company = LOTTE;
				if(strcmp(company,"HAETAE")==0) p[b]->company = HAETAE;
				if(strcmp(company,"NOBRAND")==0) p[b]->company = NOBRAND;

				b++;
			}
		}
		return b;
}

void printProduct(product_t* p[], int pcount){
	int i=0;
	printf("****************************************************************\n");
	printf("번호\t제품명\t\t제조사\t\t가격\t개수\t총가격\n");
	printf("****************************************************************\n");
	for(i=0; i<pcount; i++){
		printf("%d\t", i+1);
		printf("%s\t", p[i]->name);
		if(p[i]->company == ORION) printf("오리온\t\t");
		else if(p[i]->company == LOTTE) printf("\t롯데\t\t");
		else if(p[i]->company == HAETAE) printf("\t해태\t\t");
		else if(p[i]->company == NOBRAND) printf("\t노브랜드\t");
		printf("%d\t", p[i]->price);
		printf("%d\t", p[i]->count);
		printf("%d\n", p[i]->count*p[i]->price);
	}
}

void jsonNameList(char* jsonstr, jsmntok_t *t, int tokcount, NametokenInfo *nametokenInfo){
  int i=0, parent=-1, parentnum=0, a=0, b=0;

		for(i=0; i<tokcount; i++){
			if(t[i+1].type == JSMN_STRING && t[i+2].type == JSMN_ARRAY){
				parent++;
				continue;
			}
			if(t[i+1].type == JSMN_ARRAY || t[i+1].type == JSMN_OBJECT){
				parent++;
			}else if(t[i+1].type != JSMN_ARRAY || t[i+1].type != JSMN_OBJECT){
				break;
			}
		}
		nametokenInfo = (NametokenInfo*)malloc(sizeof(NametokenInfo)*parent);

		int beforePrice[15][15];
		int beforeCount[15][15];
		int afterPrice[15];
		int afterCount[15];
		int totalPrice[15];
		int m=0, n=0;

		printf("**********************************************\n");
		printf("번호\t제품명\t제조사\t가격\t개수\t총가격\n");
		printf("**********************************************\n");

		for(a=1, b=0; a<tokcount; a++){
			if(t[a].parent == parent){
				nametokenInfo[b].objectindex = b+1;
				nametokenInfo[b].tokindex = a;



				afterPrice[b] = atoi(beforePrice[b]);
				afterCount[b] = atoi(beforeCount[b]);
				totalPrice[b] = afterPrice[b]*afterCount[b];

				//printf("Price %s \n", beforePrice[b]);

				printf("%.*s",t[a+4].end - t[a+4].start, jsonstr + t[a+4].start);
				printf("%.*s", t[a+2].end - t[a+2].start, jsonstr + t[a+2].start);
				printf("%.*s", t[a+6].end - t[a+6].start, jsonstr + t[a+6].start);
				printf("%.*s", t[a+8].end - t[a+8].start, jsonstr + t[a+8].start);
				printf("%d\n", totalPrice[b]);

				b++;
			}
		}
		printf("\n");

		parentnum = parent;
		printf("***** Name List *****\n");
	for(a=1, b=0; a<tokcount; a++){

		if(t[a].parent == parentnum ){

			nametokenInfo[b].objectindex = b+1;
			nametokenInfo[b].tokindex = a;
			printf("name[%d] %.*s\n", b+1, t[a+1].end-t[a+1].start, jsonstr+t[a+1].start);
			b++;
		}
	}

}

void selectNameList(char* jsonstr, jsmntok_t *t, int* nameTokIndex){
  int no, i;

  while(1){
    printf("Select Name's NO (exit:0) >> ");
    scanf("%d", &no);

    if(no == 0){
      break;
    }
    else if((nameTokIndex[no-1] != 0) ){

      printf("name[%d] %.*s\n", no, t[nameTokIndex[no-1]].end-t[nameTokIndex[no-1]].start, jsonstr+t[nameTokIndex[no-1]].start);
      printf("%.*s\n", t[nameTokIndex[no-1]+1].end-t[nameTokIndex[no-1]+1].start, jsonstr+t[nameTokIndex[no-1]+1].start);
    }
  }
}

void selectObjectList(char* jsonstr, jsmntok_t *t, int* nameTokIndex, char* obj){
	int no, n;

	obj = (char *)malloc(sizeof(char));


	while(1){
		printf("Select Object's NO (exit:0) >> ");
		scanf("%d", &no);
		n = strlen(obj);


		if(no == 0 || (t[nameTokIndex[no-1]].end < t[nameTokIndex[no-1]].start)){
			break;
		}else if(nameTokIndex[no-1] != 0){

			obj = (char *)realloc(obj, t[nameTokIndex[no]].end-t[nameTokIndex[no-1]].start);
			strncpy(obj, jsonstr+t[nameTokIndex[no-1]].start, t[nameTokIndex[no]].end-t[nameTokIndex[no-1]].start);

			printf("%.*s\n", t[nameTokIndex[no]].end-t[nameTokIndex[no-1]].start, obj);
		}
	}
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
  int count;
  int nameTokIndex[100]={0,};
	NametokenInfo *nametokenInfo;
	jsmn_parser p;
	jsmntok_t t[128];
	char* obj;
	product_t *snackList[10];
	int pcount;

	char* JSON_STRING;
	JSON_STRING = readJSONFile();

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}



	/* Assume the top-level element is an object */
	pcount = makeProduct(JSON_STRING, t, r, snackList);
	//jsonNameList(JSON_STRING, t, r, nametokenInfo);
	printObjectList(JSON_STRING, t, r, nameTokIndex);
	//selectObjectList(JSON_STRING, t, nameTokIndex, obj);
	printProduct(snackList, pcount);
  //printNameList(JSON_STRING, t, r, nameTokIndex);
  //selectNameList(JSON_STRING, t, nameTokIndex);


	return EXIT_SUCCESS;
}
