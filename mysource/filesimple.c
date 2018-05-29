//#define JSMN_PARENT_LINKS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */


char* readJSONFile() {
	long Fsize;
	char* buffer;
	size_t result;
	FILE* fp;

	fp = fopen("data.json", "rt");
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

	//printf("%lu %lu\n", result+1, Fsize);

	if(strlen(result) != Fsize-1){
		printf("Reading error");
		exit(3);
	}

	fclose(fp);
	free(buffer);
	return (void *)result;
}

void jsonNameList(char* jsonstr, jsmntok_t *t, int tokcount, int *nameTokIndex){
  int a=0, b=0, count=0;

  for (a=0; a<tokcount; a++){
		printf("parent%d ", t[a].parent);
    if(t[a].parent == 0){
      nameTokIndex[b]=a;

      count++;
      b++;
    }
    nameTokIndex[b]=a+1;
    //printf("a:%d nameTokIndex[b]:%d\n", a+1, nameTokIndex[b]);
  }

}

void printObjectList(char* jsonstr, jsmntok_t *t, int tokcount, int *nameTokIndex){
	printf("***** Object List *****\n");

	int a=0, b=0;

  for (a=0; a<tokcount; a++){
    if(t[a].size == 8){
      nameTokIndex[b]=a;

			printf("object[%d] %.*s\n", b+1, t[a+2].end-t[a+2].start, jsonstr+t[a+2].start);
			//printf("%d %d %d\n",   nameTokIndex[0],  nameTokIndex[1],  nameTokIndex[2]);
			b++;
    }
    nameTokIndex[b]=a+1;
    //printf("a:%d nameTokIndex[b]:%d\n", a+1, nameTokIndex[b]);
  }
}

void printNameList(char* jsonstr, jsmntok_t *t, int *nameTokIndex){
  int i=0;

  printf("***** Name List *****\n");
  while(1){
    if((nameTokIndex[i] == 0) || (t[nameTokIndex[i]].start == 0)){
      break;
    }else if(nameTokIndex[i] != 0){
      printf("name[%d] %.*s\n", i+1, t[nameTokIndex[i]].end - t[nameTokIndex[i]].start, jsonstr + t[nameTokIndex[i]].start);
      //printf("%d\n", t[nameTokIndex[i]+1].parent);
      i++;
			//printf("end %d    start %d\n", t[nameTokIndex[i]].end, t[nameTokIndex[i]].start);

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
      //printf("%d \n", nameTokIndex[no]-1);

      printf("name[%d] %.*s\n", no, t[nameTokIndex[no-1]].end-t[nameTokIndex[no-1]].start, jsonstr+t[nameTokIndex[no-1]].start);
      printf("%.*s\n", t[nameTokIndex[no-1]+1].end-t[nameTokIndex[no-1]+1].start, jsonstr+t[nameTokIndex[no-1]+1].start);
      //printf("%d %d\n", t[nameTokIndex[no]-1].start, t[nameTokIndex[no]-1].end);
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

			obj = (char *)realloc(obj, t[nameTokIndex[no-1]].end-t[nameTokIndex[no-1]].start);
			strncpy(obj, jsonstr+t[nameTokIndex[no-1]].start, t[nameTokIndex[no-1]].end-t[nameTokIndex[no-1]].start);

			printf("%.*s\n", t[nameTokIndex[no-1]].end-t[nameTokIndex[no-1]].start, obj);
			//printf("%d\n", t[nameTokIndex[no-1]].start);
			//printf("object[%d] %.*s\n", no, t[nameTokIndex[no-1]].end-t[nameTokIndex[no-1]].start, jsonstr+t[nameTokIndex[no-1]].start);
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
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Array expected\n");
		return 1;
	}

	printObjectList(JSON_STRING, t, r, nameTokIndex);
	//selectObjectList(JSON_STRING, t, nameTokIndex, obj);
  jsonNameList(JSON_STRING, t, r, nameTokIndex);
  printNameList(JSON_STRING, t, nameTokIndex);
  selectNameList(JSON_STRING, t, nameTokIndex);


	return EXIT_SUCCESS;
}
