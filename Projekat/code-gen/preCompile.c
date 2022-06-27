#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int lastIndex(char *string, char x)
{
  int len = strlen(string);
  for (int i = len - 3; i > 0; i--)
  {
    if (string[i] == x)
    {
      return i;
    }
  }
  return -1;
}

int *indexOfLPAREN(char *string)
{
  int len = strlen(string);
  int *a = malloc(100 * sizeof(int));
  int index = 0;
  for (int i = 0; i < len; i++)
  {
    if (string[i] == '(')
    {
      a[index] = i + 1;
      index++;
    }
  }
  return a;
}

int *indexOfRPAREN(char *string)
{
  int len = strlen(string);
  int *a = malloc(100 * sizeof(int));
  int index = 0;
  for (int i = 0; i < len; i++)
  {
    if (string[i] == ')')
    {
      a[index] = i;
      index++;
    }
  }
  return a;
}

int *indexOfMacroCall(char *string, char *name)
{
  int len = strlen(string);
  int macroLen = strlen(name);
  int *a = malloc(100 * sizeof(int));
  int index = 0;
  int niz = 0;
  char *str = malloc(macroLen);
  while (index < len)
  {
    strncpy(str, string + index, macroLen);
    if (strstr(str, name))
    {
      a[niz] = index;
      index += macroLen;
    }
    else
    {
      index++;
    }
  }
  return a;
}

int indexOfBeginingOfMacroFunctionName(char *string)
{
  int len = strlen(string);
  int j = 1;
  for (int i = 0; i < len; i++)
  {
    if (string[i] == ' ')
    {
      if (j == 0)
      {
        return i + 1;
      }
      else
        j--;
    }
  }
  return -1;
}
void printFile()
{
  FILE *Nesto;
  char singleLine[150];
  Nesto = fopen("test-ok2.mc", "r");
  while (!feof(Nesto))
  {
    fgets(singleLine, 100, Nesto);
    printf("%s", singleLine);
  }
  fclose(Nesto);
}

int main()
{
  FILE *fPointer;
  FILE *ZaPisanje;
  fPointer = fopen("test-ok1.mc", "r");
  ZaPisanje = fopen("test-ok2.mc", "w");
  char macro[50] = "macro_call";
  char singleLine[150];
  int index = 0;
  int lenOfSubstring = 0;
  int indexOfPar = 0;
  int indexOfMacroName = 0;
  char *expression;
  char *macro_par;
  char *macro_name;
  char *variable;
  int *arraylparen;
  int *arrayrparen;
  while (!feof(fPointer))
  {
    fgets(singleLine, 100, fPointer);
    if (strstr(singleLine, macro))
    {
      // macro par name
      indexOfPar = indexOfLPAREN(singleLine)[0];
      macro_par = malloc(2);
      strncpy(macro_par, singleLine + indexOfPar, 1);

      // macro expression
      index = lastIndex(singleLine, ' ');
      lenOfSubstring = strlen(singleLine) - 3 - index;
      expression = malloc(lenOfSubstring + 1);
      strncpy(expression, singleLine + index + 1, lenOfSubstring);

      // macro name
      int endMacroName = indexOfPar - 2;
      int startMacroName = indexOfBeginingOfMacroFunctionName(singleLine);
      lenOfSubstring = endMacroName - startMacroName + 1;
      macro_name = malloc(lenOfSubstring + 1);
      strncpy(macro_name, singleLine + startMacroName, lenOfSubstring);
    }
  }
  fclose(fPointer);
  fPointer = fopen("test-ok1.mc", "r");
  while (!feof(fPointer))
  {
    fgets(singleLine, 100, fPointer);
    if (strstr(singleLine, macro_name) && !strstr(singleLine, "#define"))
    {
      char *correctExpression = malloc(200);
      int indexOfCorrectExp = 0;
      int *arrayMacroCallStart = indexOfMacroCall(singleLine, macro_name);

      int len = sizeof(arrayMacroCallStart) / 4;

      int koliko_sam_vec_ispisao = 0;
      for (int i = 0; i < len - 1; i++)
      {
        int start = arrayMacroCallStart[i];
        for (int i = koliko_sam_vec_ispisao; i < start; i++)
        {
          correctExpression[i] = singleLine[i];
          indexOfCorrectExp++;
        }

        int indexLPMacro = start + strlen(macro_name);
        int indexRPMacro = 0;
        for (int i = indexLPMacro; i < strlen(singleLine); i++)
        {
          if (singleLine[i] == ')')
          {
            indexRPMacro = i;
            break;
          }
        }
        lenOfSubstring = indexRPMacro - indexLPMacro;
        char *var = malloc(lenOfSubstring);
        strncpy(var, singleLine + indexLPMacro + 1, lenOfSubstring - 1);
        for (int i = 0; i < strlen(expression); i++)
        {
          if (expression[i] == macro_par[0])
          {
            for (int j = 0; j < strlen(var); j++)
            {
              correctExpression[indexOfCorrectExp] = var[j];
              indexOfCorrectExp++;
            }
          }
          else
          {
            correctExpression[indexOfCorrectExp] = expression[i];
            indexOfCorrectExp++;
          }
        }
        koliko_sam_vec_ispisao = indexRPMacro + 1;
      }
      for (int i = koliko_sam_vec_ispisao; i < strlen(singleLine); i++)
      {
        correctExpression[indexOfCorrectExp] = singleLine[i];
        indexOfCorrectExp++;
      }
      fprintf(ZaPisanje, "%s", correctExpression);
    }
    else
    {
      fprintf(ZaPisanje, "%s", singleLine);
    }
    singleLine[0]='\n';
  }
  fclose(ZaPisanje);
  fclose(fPointer);
  printFile();
  return 0;
}
