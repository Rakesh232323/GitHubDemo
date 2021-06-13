// ConsoleApplication3.cpp : main project file.

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

using namespace System;
void  unzipfile(char *zippedfile);
void zip(char *zippedfile);

int main(array<System::String ^> ^args)
{
    Console::WriteLine(L"Hello World");

    unzipfile("test.zip");
    zip("test.zip");

    
    return 0;
}


void  unzipfile(char *zippedfile)
{

//condition to check
//delte if already extracted
// check the condition
  char cmd[100];
  sprintf(cmd, "7za x C:/Users/z003w8tx/Desktop/testing/%s -o%s", zippedfile, "D:/delete/");
  printf("command is %s\n", cmd);
  system(cmd);
  return;
}

void zip(char *zippedfile)
{

  char cmd[100];
  sprintf(cmd, "7za a D:/test1.zip C:/Users/z003w8tx/Desktop/testing/");
  printf("command is %s\n", cmd);
  system(cmd);
  return;

}
