//#define JSMN_PARENT_LINKS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../productlist.h"


/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */


char* readJSONFile() {
	long Fsize;
	char* buffer;
	size_t result;
	FILE* fp;


	fp = fopen("data4.json", "rt");
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
			//printf("a:%d b%d prent%d\n",a, b, parentnum);
      //nametokenInfo[b].tokindex=a;
			//nametokenInfo[b].objectindex=b;
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

		int beforePrice[10][10];
		int beforeCount[10][10];
		int afterPrice[10];
		int afterCount[10];
		int totalPrice[10];
		int m=0, n=0;

		printf("**********************************************\n");
		printf("번호\t제품명\t제조사\t가격\t개수\t총가격\n");
		printf("**********************************************\n");

		for(a=1, b=0; a<tokcount; a++){
			if(t[a].parent == parent){
				nametokenInfo[b].objectindex = b+1;
				nametokenInfo[b].tokindex = a;

				for(m=0; m<(t[a+6].end-t[a+6].start); m++){
					beforePrice[b][m]=jsonstr[t[a+6].start + m];
				}

				for(n=0; n<(t[a+8].end-t[a+8].start); n++){
					beforeCount[b][n]=jsonstr[t[a+8].start + n];
				}

				afterPrice[b] = atoi(beforePrice[b]);
				afterCount[b] = atoi(beforeCount[b]);
				totalPrice[b] = afterPrice[b]*afterCount[b];

				//printf("Price %s \n", beforePrice[b]);

				printf("%d\t%.*s", b+1, t[a+4].end - t[a+4].start, jsonstr + t[a+4].start);
				printf("\t%.*s", t[a+2].end - t[a+2].start, jsonstr + t[a+2].start);
				printf("\t%.*s", t[a+6].end - t[a+6].start, jsonstr + t[a+6].start);
				printf("\t%.*s", t[a+8].end - t[a+8].start, jsonstr + t[a+8].start);
				printf("\t%d\n", totalPrice[b]);

				b++;
			}
		}
		printf("\n");

		parentnum = parent;
		printf("***** Name List *****\n");
	for(a=0, b=0; a<tokcount; a++){

		if(t[a].parent == parentnum && a>0){

			//nameTokIndex[b]=a;
			printf("name[%d] %.*s\n", b+1, t[a].end-t[a].start, jsonstr+t[a].start);
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
	char* obj;/* We expect no more than 128 tokens */

	char* JSON_STRING;
	JSON_STRING = readJSONFile();

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}



	/* Assume the top-level element is an object */

	jsonNameList(JSON_STRING, t, r, nametokenInfo);
	printObjectList(JSON_STRING, t, r, nameTokIndex);
	selectObjectList(JSON_STRING, t, nameTokIndex, obj);

  //printNameList(JSON_STRING, t, r, nameTokIndex);
  selectNameList(JSON_STRING, t, nameTokIndex);


	return EXIT_SUCCESS;
}
