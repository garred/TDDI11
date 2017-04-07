#include <libepc.h>

int main(int argc, char *argv[])
{
   ClearScreen(0x07);

   SetCursorPosition(0, 0);
   PutString(">>>>>>> Empty Floppy Used as Skeleton <<<<<<<\r\n");

   SetCursorPosition(5, 5);
   PutString("Hello world!!!!\r\n");

   SetCursorPosition(8, 5);
   PutString("Alvaro Arco Castillo\r\n");

   SetCursorPosition(9, 5);
   PutString("Alvaro Gonzalez Redondo\r\n");

   SetCursorPosition(10, 5);
   PutString("2017/03/29\r\n");


   return 0;
}
