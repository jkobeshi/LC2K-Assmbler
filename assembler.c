/**
 * Project 1 
 * Assembler code fragment for LC-2K 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

int
main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    char SigLab[65536][7]; unsigned int l = 0;
    while (readAndParse(inFilePtr, SigLab[l], opcode, arg0, arg1, arg2)) {
        for (int i = 0; i < l; ++i) {
            if (!strcmp(SigLab[l], SigLab[i])) {
                if (strlen(SigLab[l]) > 0) {
                    exit(1);
                }
            }
        }
        ++l;
    }
    rewind(inFilePtr);
    unsigned int PC = 0;
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        int code = 0, r0 = atoi(arg0), r1 = atoi(arg1), r2 = atoi(arg2);
        if (!strcmp(opcode, "add")) {
            code = 0;
        }
        else if (!strcmp(opcode, "nor")) {
            code = 1;
        }
        else if (!strcmp(opcode, "lw")) {
            code = 2; 
            if (isNumber(arg2) != 1) {
                for (int i = 0; i < l; ++i) {
                    if (!strcmp(arg2, SigLab[i])) {
                        r2 = i; break;
                    }
                    if ((i + 1) == l) {
                        exit(1);
                    }
                }
            }
            if ((r2 > 32767) || (r2 < -32768)) {
                exit(1);
            }
            if (r2 < 0) {
                r2 = r2 * -1;
                r2 ^= 65535;
                ++r2;
            }
        }
        else if (!strcmp(opcode, "sw")) {
            code = 3;
            if (isNumber(arg2) != 1) {
                for (int i = 0; i < l; ++i) {
                    if (!strcmp(arg2, SigLab[i])) {
                        r2 = i; break;
                    }
                    if ((i + 1) == l) {
                        exit(1);
                    }
                }
            }
            if ((r2 > 32767) || (r2 < -32768)) {
                exit(1);
            }
            if (r2 < 0) {
                r2 = r2 * -1;
                r2 ^= 65535;
                ++r2;
            }
        }
        else if (!strcmp(opcode, "beq")) {
            if (isNumber(arg2) != 1) {
                for (int i = 0; i < l; ++i) {
                    if (!strcmp(arg2, SigLab[i])) {
                        r2 = i; break;
                    }
                    if ((i + 1) == l) {
                        exit(1);
                    }
                }
                r2 = r2 - PC - 1;
            }
            if ((r2 > 32767) || (r2 < -32768)) {
                exit(1);
            }
            if (r2 < 0) {
                r2 = r2 * -1;
                r2 ^= 65535;
                ++r2;
            }
            code = 4; 
            
        }
        else if (!strcmp(opcode, "jalr")) {
            code = 5; r2 = 0;
        }
        else if (!strcmp(opcode, "halt")) {
            code = 6; r0 = r1 = r2 = 0;
        }
        else if (!strcmp(opcode, "noop")) {
            code = 7; r0 = r1 = r2 = 0;
        }
        else if (!strcmp(opcode, ".fill")) {
            code = 0;
            if (isNumber(arg0) != 1) {
                for (int i = 0; i < l; ++i) {
                    if (!strcmp(arg0, SigLab[i])) {
                        r0 = i; break;
                    }
                    if ((i + 1) == l) {
                        exit(1);
                    }
                }
            }
            r2 = r0;
            r1 = r0 = 0;
        }
        else {
            exit(1);
        }
        r1 <<= 16; r0 <<= 19;
        code <<= 22;
        fprintf(outFilePtr, "%d", code + r0 + r1 + r2);
        fprintf(outFilePtr, "\n");
        ++PC;
    }

    //fprintf(outFIlePtr,)

    /* here is an example for how to use readAndParse to read a line from
        inFilePtr */
    if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
        /* reached end of file */
    }

    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);

    /* after doing a readAndParse, you may want to do the following to test the
        opcode */


    return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);
    return(1);
}

int
isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}


