#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
	//define object type
	int kind; // 1 camera, 2 sphere, 3 plane
	
	//camera attributes
	float width;
	float height;

	//sphere attributes
	float position[3];//[x, y, z]
	float color[3];//[r, g, b]
	double radius;

	//plane  attributes
	float normal[3];//[x, y, z]
} Object;

//Global Variables
Object objects[128];//A list of 128 objects
FILE *output;
int numObjects;

//Modified from Project 1
// This function writes a header in the desired format w/o comment
void writeHeader(int magicNum){
    char magicBuffer[2];
    char wbuffer[8];
    char hbuffer[8];
    char colorBuffer[8];
	
    sprintf(magicBuffer, "%d", magicNum);
    sprintf(wbuffer, "%d", objects->width);
    sprintf(hbuffer, "%d", objects->height);
    sprintf(colorBuffer, "%d", objects->color);

	//Puts header elments into output
    fputc('P', output);
    fputs(magicBuffer, output);
    fputc(' ', output);
    fputs(wbuffer, output);
    fputc(' ', output);
    fputs(hbuffer, output);
    fputc(' ', output);
    fputs(colorBuffer, output);
    fputc('\n', output);
}

//This function write to P6(Binary)
void writeP6(){
	fwrite( objects, sizeof(Object), objects->width * objects->height, output);
}

//This function sets the basic type(cam. sphere,or plane) of a single object
int parseType(char *line, int objPointer, Object *object){
	char str[7];//Trimmed because output was giving parenthesis(see plane)
	int strPointer = 0;
	
	while(line[strPointer] != ','){
		str[strPointer] = line[strPointer];//Fills str character by character(ex. plane IF)
		strPointer++;
	}
	
	if(strcmp(str, "camera") == 0){
		(object + strPointer)->kind = 1; //1 is camera
		return 0;
	}
	else if(strcmp(str, "sphere") == 0){
		(object + strPointer)->kind = 2; //2 is sphere
		return 0;
	}
	else if(strcmp(str, "plane(") == 0){
		(object + strPointer)->kind = 3; //3 is plane
		return 0;
	} else{
		fprintf(stderr,"Enter a BASIC object type or populate document\n");
		return -1;
	}
}

//This function sets the fields of a single object
int parseField(char *line, int objPointer, Object *objects){
	char str[32];
	int strPointer = 0;

	//Scans an entire line of the input file
	while(strPointer < strlen(line)){
		sscanf((line + strPointer), "%[^:]", str);//Scan till ':'
		//printf(str); //Prints contents of line in input file
		
		//Check each condition since multiple fields.
		if(strstr(str, "width")){
			float temp;
			sscanf((line+strPointer+strlen(str) + 1), "%f", &temp);
            (objects+objPointer)->width = temp;
		}
		if(strstr(str, "height")){
			float temp;
			sscanf((line+strPointer+strlen(str) + 1), "%f", &temp);
            (objects+objPointer)->height = temp;
		}
		if(strstr(str, "radius")){
			float temp;
			sscanf((line+strPointer+strlen(str) + 1), "%f", &temp);
            (objects+objPointer)->radius = temp;
		}
		if(strstr(str, "color")){
			float x, y, z;
			sscanf((line+strPointer+strlen(str) + 3), "%f, %f, %f", &x, &y, &z);
            (objects+objPointer)->color[0] = x;
			(objects+objPointer)->color[1] = y;
			(objects+objPointer)->color[2] = z;
		}
		if(strstr(str, "position")){
			float x, y, z;
			sscanf((line+strPointer+strlen(str) + 3), "%f, %f, %f", &x, &y, &z);
            (objects+objPointer)->position[0] = x;
			(objects+objPointer)->position[1] = y;
			(objects+objPointer)->position[2] = z;
		}
		if(strstr(str, "normal")){
			float x, y, z;
			sscanf((line+strPointer+strlen(str) + 3), "%f, %f, %f", &x, &y, &z);
            (objects+objPointer)->normal[0] = x;
			(objects+objPointer)->normal[1] = y;
			(objects+objPointer)->normal[2] = z;
		}
		strPointer = strPointer + strlen(str) + 1;//Increment by size of chunk + 1
	}
	return 0;
}

//This Function reads the input file. Saving the type and fields of each object.
int read(FILE *input){
	int lineLength = 1024;//Assuming 1024 characters is enough
	int objPointer = 0;
	char line[lineLength];//a character list of size 1000
	
	if(!input){	//When input file DNE
		fprintf(stderr,"Error:Input file does not exist\n");
		return -1;
	}
	
	//Reads a single line from file at a time. This would also be a single object.
	while(fgets(line, lineLength, input) != NULL){
		parseType(line, objPointer, objects);
		parseField(line, objPointer, objects);
		objPointer++;
		numObjects++;
	}
	return 0;
}

//This function returns the Dor product of two matrices with 3 values
float dot_product(float x[3], float y[3]){
	return x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
}

//This function returns the input squared
float squared(float x){ return x * x;}

//This function returns the distance to a point
float distanceToPoint(float x[2], float y[2]){
	return sqrt(squared(x[0]-y[0]) + squared(x[1]-y[1]));
}

//Render
//Raycast to what?
//Place objects read in where and how large
//
//Possibilities:
//Plane Intersection
//Sphere Intersection

int main (int argc, char *argv[]){
	if(argc != 5) {//When incorrect number of arguments
		fprintf(stderr,"Error:Incorrect Number of Arugments\n");
		return -1;
	}
	
	//Command line Information ("raycast width height input.json output.ppm")
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	FILE *inputFile = fopen(argv[3], "r"); //Opens input file
	FILE *output = fopen(argv[4], "w");
	
	if(width <= 0 || height <= 0){	//When no reasonable dimensions given
		fprintf(stderr,"Error: Invalid height or width parameters");
		return -1;
	}
	
	read(inputFile);//Read input file
	fclose(inputFile);//Close file
	
	//'Render' Image
	
	//write header for output file Project1
	//Write contents to ppm format Project1
	fclose(output);
	
	return 0;
}