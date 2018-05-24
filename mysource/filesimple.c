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
    if((t[a].size == 1) && (t[a].type != JSMN_OBJECT) && (t[a].type != JSMN_ARRAY)){
      nameTokIndex[b]=a;
      //printf("a:%d nameTokIndex[b]:%d\n", a, nameTokIndex[b]);

      count++;
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
    if(nameTokIndex[i] == 0){
      break;
    }
    else if(nameTokIndex[i] != 0){
      printf("name[%d] %.*s\n", i+1, t[nameTokIndex[i]].end - t[nameTokIndex[i]].start, jsonstr + t[nameTokIndex[i]].start);
      //printf("%d %d\n", jsonstr + t[nameTokIndex[i]].start, jsonstr);
      //printf("%.*s\n", t[nameTokIndex[i+1]-1].end-t[nameTokIndex[i+1]-1].start, jsonstr+t[nameTokIndex[i+1]-1].start);
      i++;
      if(jsonstr + t[nameTokIndex[i]].start == jsonstr){
        break;
      }
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
    else if((nameTokIndex[no-1] != 0) && (nameTokIndex[no] != 0)){
      //printf("%d %d\n", nameTokIndex[no]-1, nameTokIndex[no]);

      printf("name[%d] %.*s\n", no, t[nameTokIndex[no-1]].end-t[nameTokIndex[no-1]].start, jsonstr+t[nameTokIndex[no-1]].start);
      printf("%.*s\n", t[nameTokIndex[no]-1].end-t[nameTokIndex[no]-1].start, jsonstr+t[nameTokIndex[no]-1].start);
      //printf("%.*s\n", t[21].end-t[21].start, jsonstr+t[21].start);
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
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

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
		printf("Object expected\n");
		return 1;
	}

  jsonNameList(JSON_STRING, t, r, nameTokIndex);
  printNameList(JSON_STRING, t, nameTokIndex);
  selectNameList(JSON_STRING, t, nameTokIndex);


	return EXIT_SUCCESS;
}
