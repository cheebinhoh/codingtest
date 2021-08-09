/* Copyright © 2021 Chee Bin HOH. All rights reserved.
 *
 * Given two strings A and B, the task is to convert A to B if possible.
 * The only operation allowed is to put any character from A and insert it at front.
 * Find if it’s possible to convert the string. If yes, then output minimum no. of
 * operations required for transformation.
 *
 * A friend shares above program via the following link
 * https://www.geeksforgeeks.org/transform-one-string-to-another-using-minimum-number-of-given-operation
 *
 * I found it interesting enough that I would like to solve it, here is my answer to it,
 * unfornately, my answer does not give the minimum move in all cases as it is only looking
 * at one character at source and its matching character in target than consider a group of characters
 * at a time.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int   debug = 0;
char *programName = NULL;


/* This method will return distance of matching pivot character from
 * the start of the string.
 *
 * E.g. if the string is "abc", and pivot is "b", then answer is 1.
 *
 * if string is "abbc", and pivot is "b", then it is 2 as that is
 * further character "b" on "abbc" string if numPivotPrior is 0, else
 * it is 1.
 */
int distanceFromPivot(char string[], char pivot, int numPrecedingPivot)
{
    int i = 0;


    while ( '\0' != string[i] )
    {
        if ( string[i] == pivot )
        {
           if ( numPrecedingPivot > 0 )
           {
               break;
           }
           else
           {
               int hasOther = 0;
               int j        = i + 1;


               while ( '\0' != string[j] )
               {
                   if ( string[j] == pivot )
                   {
                       hasOther = 1;
                   }

                   j++;
               }

               if ( ! hasOther  )
               {
                   break;
               }
           }
        }

        i++;
    }

    return i;
}

/* It returns 1 if source can be translated into target, else 0.
 */
int isTransformable(char source[], char target[])
{
    int i       = 0;
    int j       = 0;
    int sH[256] = { 0 };
    int tH[256] = { 0 };


    while ( '\0' != source[i] )
    {
        sH[source[i]]++;

        i++;
    }

    while ( '\0' != target[j] )
    {
        tH[target[j]]++;

        j++;
    }

    if ( j != i )
    {
        return 0;
    }

    for ( i = 0; i < ( sizeof( sH ) / sizeof( sH[0] ) ); i++ )
    {
        if ( sH[i] != tH[i] )
        {
            return 0;
        }
    }

    return 1;
}

int transform(char source[], char target[])
{
    int i;
    int move  = 0;
    int count = 0;


    if ( ! isTransformable(source, target) )
    {
        return -1;
    }

    i = 0;
    while ( '\0' != source[i] && count < 50000 ) // a cut over to prevent infinite loop due to coding mistake
    {
        if ( source[i] == target[i] )
        {
            i++;
        }
        else
        {
           // So that source[i] != target[i] and we want to decide which of the character after
           // ith position in source that we want to move to the front.
           //
           // It will go through all characters after ith position in source and look up same
           // character position in target ith position or after that.
           //
           // if a character is in jth position in source, but it is in position before jth in target, 
           // then it is a potential character to move to the front in source (as it is supposed to be
           // in front position according to target string), and we pick the jth position in source that
           // the gap between its position in source and position in target is the largest one.
           //
           // A couple of tricks are important:
           // - if source is "ACBB" and target is "ABCB",
           // 

           int j     = i + 1;
           int pivot = i + 1;
           int gap   = 1;
           int tmp;
           int pos;
           int k;
           int numPivotPrior;


           move++;

           while ( '\0' != source[j] )
           {
               k             = i;
               numPivotPrior = 0;


               while ( k < j )
               {
                   if ( target[k] == source[j] )
                   {
                       numPivotPrior++;
                   }

                   if ( source[k] == source[j] )
                   {
                       numPivotPrior--;
                   }

                   k++;
               }

               pos = distanceFromPivot(target + i, source[j], numPivotPrior) + i;

               if ( pos < j)
               {
                   if ( ( j - pos ) >= gap )
                   {
                       gap = j - pos;
                       pivot = j;
                   }
               }

               j++;
           } //  while ( '\0' != source[j] )

           if ( debug )
           {
               printf("---- Move %2d, pick %c (%2d) and insert at the front, %s => ",
                      move, source[pivot], pivot, source);
           }


           tmp = source[pivot];


           /* ccomment out: in flavor of memmove where large block copy can be efficient as
            * in some case memmove can be implemented in SIMD (single instruction multiple data)
            * instruction.
            *
            * j   = pivot;
            *
            * while ( j > 0 )
            * {
            *     source[j] = source[j - 1];
            *     j--;
            * }
            */

           memmove(source + 1, source, pivot);
           source[0] = tmp;
           i = 0;

           if ( debug )
           {
               printf("%s\n", source);
           }
        } // if ( source[i] == target[i] ) ... else

        count++;
    } //  while ( '\0' != source[i] && count < 50000 )

    return move;
}

void printHelp(void)
{
     fprintf(stderr, "%s [-d] [-h] [-s source1,source2,..] [-t target]\n", programName);
     fprintf(stderr, "\n");
     fprintf(stderr, " -d                     : debug mode to demonstate each step of the moves\n");
     fprintf(stderr, " -h                     : print this help message\n");
     fprintf(stderr, " -s source1,source2,... : list of source strings to be transformed into target\n");
     fprintf(stderr, " -t target              : target string\n");
     fprintf(stderr, "\n");
     fprintf(stderr, " if -t is specified, then source string will be from -s argument or standard input (if -s is not specified)\n");
}

void runTest(char string[], char target[])
{
    int moveCnt;


    // we do not do printf(...., str1, transform(str1, target), str1 );
    // because comma separator on function arguments are not sequencing point, there
    // is no guarantee that 1st str1 is passed to printf before it is transformed by nested method transform

    printf("Transofmr string = %s to %s\n", string, target);

    moveCnt = transform(string, target);

    printf("after %d move, string = %s\n\n", moveCnt, string);
}

void runSampleTest(void)
{
    char str1[]    = "EACBD";
    char str2[]    = "CABED";
    char str3[]    = "EACBD";
    char str4[]    = "ABECD";
    char str5[]    = "ACEBD";
    char str6[]    = "ABCDE";
    char str7[]    = "DEABC";
    char target1[] = "EABCD";
    char str8[]    = "OHH";
    char str9[]    = "HHO";
    char target2[] = "HOH";
    char str10[]   = "HCEE";
    char str11[]   = "HEEC";
    char str12[]   = "ECEH";
    char target3[] = "CHEE";
    char str13[]   = "Crprogamming";
    char str14[]   = "Cproggrammin";
    char target4[] = "Cprogramming";
    char str15[]   = "Hlelo";
    char str16[]   = "Helol";
    char target5[] = "Hello";
    char str17[]   = "ACCBB";
    char target6[] = "ABCBC";
 
    runTest(str1, target1);
    runTest(str2, target1);
    runTest(str3, target1);
    runTest(str4, target1);
    runTest(str5, target1);
    runTest(str6, target1);
    runTest(str7, target1);

    runTest(str8, target2);
    runTest(str9, target2);

    runTest(str10, target3);
    runTest(str11, target3);
    runTest(str12, target3);

    runTest(str13, target4);
    runTest(str14, target4);

    runTest(str15, target5);
    runTest(str16, target5);

    /* TODO: unfortunately, minimum moves is 5, but we give 6 moves.
       target = ABCBC

     ACCBB -> CACBB
     CACBB -> BCACB
     BCACB -> CBCAB
     CBCAB -> BCBCA
     BCBCA -> ABCBC
     */
    runTest(str17, target6);
}


int main(int argc, char *argv[])
{
    int   c;
    char *targetArg = NULL;
    char *sourceArg = NULL;
    char  source[1024]; // maximum hardcode


    programName = argv[0];

    while ( -1 != ( c = getopt(argc, argv, "s:t:dh") ) )
    {
        switch ( c )
        {
            case 'd':
                debug = 1;
                break;

            case 't':
                targetArg = optarg;
                break;

            case 's':
                sourceArg = optarg;
                break;

            case '?':
            case 'h':
                printHelp();
                exit(1);
        }
    }

    if ( NULL != sourceArg
         && NULL == targetArg )
    {
        printHelp();
        exit(1);
    }


    if ( NULL == targetArg )
    {
        runSampleTest();
    }
    else if ( NULL != sourceArg )
    {
        char *token;


        token = strtok(sourceArg, ",");
        while ( NULL != token )
        {
            strncpy(source, token, sizeof( source ) - 1);
            source[sizeof( source ) - 1] = '\0';

            runTest(source, targetArg);

            token = strtok(NULL, ",");
        }
    }
    else
    {
        while ( fgets(source, sizeof( source ) - 1, stdin) != NULL )
        {
            int len = strlen(source);


            if ( '\n' == source[len - 1] )
            {
                source[len - 1] = '\0';
            }

            runTest(source, targetArg);
        }
    }

    return 0;
}
