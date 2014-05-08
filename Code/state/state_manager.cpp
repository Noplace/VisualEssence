#include "../ve.h"
#include "../../libs/rapidjson/document.h"
#include <stdio.h>
#include <stdlib.h>













char* ReadFile(const char *filename)
{
   char *buffer = NULL;
   int string_size,read_size;
   FILE *handler = nullptr;
   fopen_s(&handler,filename,"r");

   if (handler)
   {
       //seek the last byte of the file
       fseek(handler,0,SEEK_END);
       //offset from the first to the last byte, or in other words, filesize
       string_size = ftell (handler);
       //go back to the start of the file
       rewind(handler);

       //allocate a string that can hold it all
       buffer = (char*) malloc (sizeof(char) * (string_size + 1) );
       //read it all in one operation
       read_size = fread(buffer,sizeof(char),string_size,handler);
       //fread doesnt set it so put a \0 in the last position
       //and buffer is now officialy a string
       buffer[string_size] = '\0';

       if (string_size != read_size) {
           //something went wrong, throw away the memory and set
           //the buffer to NULL
           free(buffer);
           buffer = NULL;
        }
    }

    return buffer;
}

void test_json() {
  //const char json[] = "{ \"hello\" : \"world\" }";

  auto json = ReadFile("D:\\Personal\\Projects\\GameDemo2\\Solution\\GameDemo2\\Resources\\test.json");
                       

  rapidjson::Document d;
  d.Parse<0>(json);
  if (d.IsArray())  {
    for (auto i = d.Begin(); i != d.End(); ++i) {

      //auto obj = (*i);
      auto str = ((*i)["name"]).GetString();
      auto str2 = ((*i)["DepthEnable"]).GetBool();
      int a = 1;
    }

  }

  free(json);
}