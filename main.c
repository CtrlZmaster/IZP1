/*---------------------------------------------------------------*\
|  Project: IZP Project 1                                         |
|  File: proj1.c                                                  |
|  Title: Working with text                                       |
|  Description: This program creates a hexdump. Optionally,       |
|               it can convert hexadecimal input to text and      |
|               vice versa or print only printable characters     |
|               from input with minimal length.                   |
|  Author: Michal Pospíšil, xpospi95@vutbr.cz                     |
|  Study group: 1BIB-34                                           |
|  Date: 06/11/2016 (DD/MM/RRRR)                                  |
\*---------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int returnNumber(char *strNumber);
int textToHex();
int textToHexText(int skip, int numberOfChars);
int textToPrintable(int longest);
int hexToText();
void printHelp();
void errorHandling(int errorNum);

int main(int argc, char *argv[])
{
  int errorNum = 0; //this will remain 0 if there is no error
  int xFlag, rFlag, sValue, nValue, capSValue, helpFlag, tooManyArg;
  xFlag = rFlag = sValue = nValue = capSValue = helpFlag = tooManyArg = 0;

  if (argc == 1)
    {
      textToHexText(sValue,nValue);
    }

  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-' && argv[i][2] == '\0')
    {
      switch (argv[i][1])
      {
        case 's':
          if (returnNumber(argv[i+1]) > 0)
          {
            sValue = returnNumber(argv[i+1]);
            i++;
          }
          else
          {
            errorNum = 1;  //next argument is not a number
          }
          break;

        case 'n':
          if (returnNumber(argv[i+1]) > 0)
          {
            nValue = returnNumber(argv[i+1]);
            i++;
          }
          else
          {
            errorNum = 1;  //next argument is not a number
          }
          break;

        case 'S':
          if (returnNumber(argv[i+1]) > 0 && returnNumber(argv[i+1]) < 200)
          {
            capSValue = returnNumber(argv[i+1]);
            ++i;
          }
          else
          {
            errorNum = 2;  //next argument is not a valid number or exceeds the limit of 199
          }
          break;

        case 'r':
          {
            rFlag = 1;
            break;
          }

        case 'x':
          {
            xFlag = 1;
            break;
          }
        default:
          helpFlag = 1;
      }
    }
    else
    {
      helpFlag = 1;
    }
  }

  if (((sValue != 0) || (nValue != 0)) && ((tooManyArg = capSValue + rFlag + xFlag + helpFlag) == 0))
      {
        errorNum = textToHexText(sValue,nValue);
      }
      else
      {
        if (tooManyArg != 0)
          errorNum = 5;
        else
          errorNum = 1;  //next argument is not a valid number
      }

    if ((capSValue != 0) && (nValue + sValue + rFlag + xFlag + helpFlag == 0))
      errorNum = textToPrintable(capSValue);
    else
      errorNum = 5;

    if ((rFlag == 1) && (capSValue + nValue + sValue + xFlag + helpFlag == 0))
      errorNum = hexToText();
    else
      errorNum = 5;

    if ((xFlag == 1) && (capSValue + nValue + sValue + rFlag + helpFlag == 0))
      errorNum = textToHex();
    else
      errorNum = 5; //check if this is error or help should be printed

    if ((helpFlag == 1) && (capSValue + nValue + sValue + rFlag + xFlag == 0))
      {
        errorNum = 0;
        printHelp();
      }

  return errorNum;
}

/*--------------------------------------------*\
|  This function makes number from string, if  |
|  the string is not a number return value is  |
|  -1.                                      |
\*--------------------------------------------*/
int returnNumber(char *strNumber)
{
  int number;
  char *p_nan;

  number = strtol(strNumber,&p_nan,10);

  if (p_nan[0] == '\0')
  {
    return number;
  }
  else
  {
    return -1;
  }
}

/*--------------------------------------------*\
|  This function prints hexadecimal            |
|  representation of characters on stdin.      |
\*--------------------------------------------*/
int textToHex()
{
  int currentChar = 0;
  while ((currentChar = getchar()) != EOF)
  {
    printf("%02x",currentChar);
  }
  printf("\n");
  return 0;
}


/*--------------------------------------------*\
|  This function creates hexdump, arguments    |
|  specify how many bytes to skip and how many |
|  characters to print. When no arguments are  |
|  given, it doesn't affect the function (both |
|  are zeros).                                 |
\*--------------------------------------------*/
int textToHexText(int skip, int numberOfChars)
{
  int currentChar = 0;
  int i,j,skipped;
  char text[19] = {32};
  text[0] = '|';
  text[17] = '|';
  text[18] = '\0';

  //for skipping chars program reads and then forget everything
  for (skipped = 0; skipped < skip; skipped++)
  {
    getchar();
  }

  //beginning
  for (i = 0; (currentChar = getchar()) != EOF; i++)  //linekeeper
  {
    printf("%08x  %02x",i*16 + skipped, currentChar);  //prints address and first byte

    if ((isprint(currentChar) != 0) && (currentChar < 128))
      {
        text[1] = currentChar;
      }
      else
      {
        text[1] = '.';
      }

    for (j = 2; j <= 16; j++)
    {
      if (j == 9)  //additional space in the middle
      {
        putchar(32);
      }

      currentChar = getchar();  //second and every j-th byte


      if ((isprint(currentChar) != 0) && (currentChar < 128)) //assigning the char to array which is printed afterwards
      {
        text[j] = currentChar;
      }
      else
      {
        text[j] = '.';
      }

      if ((currentChar == EOF) || (i * 16 + j == numberOfChars + 1))  //when EOF is reached, function quickly fills the line with spaces and ends
      {
        currentChar = EOF;
        for (int k = j; k <= 16; k++)
          {
            printf("   ");
            text[k] = ' ';

            if (k == 9) putchar(32);
          }
        break;
      }
        printf(" %02x", currentChar);
    }

    printf("  %s\n",text);

    if (currentChar == EOF)
      return 0;

  }

  return 0;
}


/*----------------------------------------------*\
|  This function prints printable strings longer |
|  than the argument.                            |
\*----------------------------------------------*/
int textToPrintable(int shortest)
{

  int length, currentChar;
  length = currentChar = 0;

  while ((currentChar = getchar()) != EOF)  //1st char of string
  {
    length = 0;
    char finalString[200] = {0};
    while ((currentChar != '\0') && (currentChar != '\n') && (currentChar != EOF) && (length < 200))    //build array if \0 or \n are not the char
    {
      if ((isprint(currentChar) != 0) || (isspace(currentChar) != 0))  //building just from printables and non-empty chars
      {
          finalString[length] = currentChar;
          length++;
      }

      currentChar = getchar(); //get new char
    }

    if (length > shortest)   //if it's long enough it gets printed with a new line
        {
          printf("%s", finalString);
          while ((currentChar != '\0') && (currentChar != '\n') && (currentChar != EOF))
            {
              putchar(currentChar);
              currentChar = getchar();
            }

          printf("\n");
        }

  }

  return 0;
}


/*--------------------------------------------*\
|  This function prints characters indicated   |
|  by their hexadecimal representation in ASCII|
|  table.                                      |
\*--------------------------------------------*/
int hexToText()
{
  int hex1, hex2, dec1, dec2, dec, errorNum;
  hex1 = hex2 = dec1 = dec2 = errorNum = 0;

  for(;;)
  {
    hex1 = getchar();
    if (hex1 == EOF)
    {
      printf("\n");
      return 0;
    }

    dec = 0;

    while ((isspace(hex1) != 0))
    {
      hex1 = getchar();

      if (hex1 == EOF)
        {
          printf("\n");
          return 0;
        }
    }

    hex2 = getchar();
    if (hex2 == EOF)
      {
        printf("\n");
        return 0;
      }

    while ((isspace(hex2) != 0))
    {
      hex2 = getchar();

      if (hex2 == EOF)
        {
          printf("\n");
          return 0;
        }
    }

    hex1 = toupper(hex1);  //converting to uppercase to remove one if statement
    hex2 = toupper(hex2);

    if (hex1 >= 48 && hex1 <= 57) //numbers
    {
      dec1 = hex1 - 48; //difference between ASCII code and decimal
    }
    else
    {
      if (hex1 >= 65 && hex1 <= 70) //A-F
      {
        dec1 = hex1 - 55;  //difference between ASCII code and numerical value of A-F
      }
      else
        errorNum = 3;  //invalid hexadecimal number on input
    }



    if (hex2 >= 48 && hex2 <= 57)
    {
      dec2 = hex2 - 48;
    }
    else
    {
      if (hex2 >= 65 && hex2 <= 70)
      {
        dec2 = hex2 - 55;
      }
      else
        errorNum = 3;
    }

    dec = (dec1 * 16) + dec2;

    printf("%c", dec);
  }

  printf("\n");

  return errorNum;
}

/*----------------------------------------------*\
|  This function prints error codes to stdout.   |
\*----------------------------------------------*/
void printHelp()
{
  printf("\n*****IZP Project 1 HELP*****\n\n");
  printf("This program creates a hexdump. Optionally, it can convert hexadecimal input to text and vice versa or print only printable characters from input with minimal length. \n\n");
  printf("USAGE:\n");
  printf("%s\n%6s\n%s\n","  ./proj1","or","  ./proj1 [options]");
  printf("\nOPTIONS:\n");
  printf("  %-17s %s\n","-s num1 -n num2","Creates hexdump that skips first num1 of characters and prints next num2 characters.");
  printf("  %-17s %s\n","-x","Text from input will be converted to unformatted hexadecimal representation of characters.");
  printf("  %-17s %s\n","-r","Reverse operation, hexadecimal input will be converted to text without white-spaces.");
  printf("  %-17s %s\n","-S len","Returned will be strings with minimal length of len and maximal length of 200 characters that contains printable characters.");
  printf("\nAUTHOR:\n");
  printf("  Michal Pospíšil \n  1BC-IT - Student (group: 1BIB-34) \n\n  Brno University of Technology \n  Faculty of Information Technologies \n\n  michal.pospisil1@vutbr.cz\n");
}

/*----------------------------------------------*\
|  This function prints error codes to stderr.   |
\*----------------------------------------------*/
void errorHandling(int errorNum)
{
  switch (errorNum)
  {
    case '1':
      fprintf(stderr,"Error %d: Argument is not a valid number.",errorNum);
      break;

    case '2':
      fprintf(stderr,"Error %d: Argument is not a valid number or is bigger than 200 and smaller than 1.",errorNum);
      break;

    case '3':
      fprintf(stderr,"Error %d: Invalid hexadecimal number on input.",errorNum);
      break;

    case '4':
      fprintf(stderr,"Error %d: Array overflow - input is longer than 199 characters.",errorNum);
      break;

    case '5':
      fprintf(stderr,"Error %d: Unexpected arguments.",errorNum);
      break;
  }
}
