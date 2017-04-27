#include <libepc.h>

void llmultiply(unsigned long long int l1,
                unsigned long long int l2,
                unsigned char *result);

#define RL *(pllr+0)
#define RH *(pllr+1)
#define RLL *(plr+0)
#define RLH *(plr+1)
#define RHL *(plr+2)
#define RHH *(plr+3)
#define AL (unsigned long long int) *(p1+0)
#define AH (unsigned long long int) *(p1+1)
#define BL (unsigned long long int) *(p2+0)
#define BH (unsigned long long int) *(p2+1)
#define ML *(pm+0)
#define MH *(pm+1)

void llmultiply_c(unsigned long long int l1,
                  unsigned long long int l2,
                  unsigned char *result) {

  unsigned long int * p1 = (unsigned long int *)&l1;           //long pointer to long long l2
  unsigned long int * p2 = (unsigned long int *)&l2;           //long pointer to long long l2
  unsigned long long int mult;            //multiply result
  unsigned int carry = 0;                 //carry "bit"
  unsigned long int * pm = (unsigned long int *)&mult;         //long pointer to long long mult result
  unsigned long long int * pllr = (unsigned long long int *)result; //long long pointer to char result
  unsigned long int * plr = (unsigned long int *)result;       //long pointer to char result
  unsigned long int t;

  // Set result to 0
  RL = 0;
  RH = 0;

  // AL * BL
  mult = AL * BL;
  RLL += ML;
  RLH += MH;

  // AL * BH
  mult = AL * BH;
  t = RLH;
  RLH += ML;
  carry = (RLH < t);
  t = RHL;
  RHL += MH + carry;
  carry = (RHL < t);
  RHH += carry;

  // AH * BL
  mult = AH * BL;
  t = RLH;
  RLH += ML;
  carry = (RLH < t);
  t = RHL;
  RHL += MH + carry;
  carry = (RHL < t);
  RHH += carry;

  // AH * BH
  mult = AH * BH;
  t = RHL;
  RHL += ML;
  carry = (RHL < t);
  RHH += MH + carry;

}

struct test_case {
  unsigned long long int a;
  unsigned long long int b;
  unsigned long long int rh;
  unsigned long long int rl;
};

struct test_case cases[6] = {

  { 0x0000111122223333ULL, 0x0000555566667777ULL,
    0x0000000005B061D9ULL, 0x58BF0ECA7C0481B5ULL },

  { 0x3456FEDCAAAA1000ULL, 0xEDBA00112233FF01ULL,
    0x309A912AF7188C57ULL, 0xE62072DD409A1000ULL },

  { 0xFFFFEEEEDDDDCCCCULL, 0xBBBBAAAA99998888ULL,
    0xBBBB9E2692C5DDDCULL, 0xC28F7531048D2C60ULL },

  { 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL,
    0xFFFFFFFFFFFFFFFEULL, 0x0000000000000001ULL },

  { 0x00000001FFFFFFFFULL, 0x00000001FFFFFFFFULL,
    0x0000000000000003ULL, 0xFFFFFFFC00000001ULL },

  { 0xFFFEFFFFFFFFFFFFULL, 0xFFFF0001FFFFFFFFULL,
    0xFFFE0002FFFDFFFEULL, 0x0001FFFE00000001ULL }
};

void PutUnsignedLongLong(unsigned long long int* ulli)
{
  unsigned long int* uli = (unsigned long int*)ulli;
  PutUnsigned(uli[1], 16, 8);
  PutUnsigned(uli[0], 16, 8);
}

void print_test(
  void (*func)(unsigned long long int l1,
              unsigned long long int l2,
              unsigned char *result))
{
  unsigned char result[16];
  int i;

  ClearScreen(0x07);
  SetCursorPosition(0, 0);

  for (i = 0; i < 6; ++i)
  {
    PutString("Test : ");
    PutUnsignedLongLong(&cases[i].a);
    PutString(" * ");
    PutUnsignedLongLong(&cases[i].b);
    PutString("\r\n");

    PutString("    == ");
    PutUnsignedLongLong(&cases[i].rh);
    PutUnsignedLongLong(&cases[i].rl);
    PutString("\r\n");

    func(cases[i].a, cases[i].b, result);

    PutString("Result ");
    PutUnsignedLongLong(&result[8]);
    PutUnsignedLongLong(&result[0]);
    PutString("\r\n");

    PutString("\r\n");
  }
}


void print_performance()
{
  unsigned char result[16];
  int i, j;
  QWORD64 cycles;
  ClearScreen(0x07);

  // Assembler version
  SetCursorPosition(0, 0);
  cycles = CPU_Clock_Cycles();
  for (j=0; j<1000000; j++)
    for (i = 0; i < 6; ++i)
      llmultiply(cases[i].a, cases[i].b, result);
  cycles = CPU_Clock_Cycles() - cycles;
  PutString("Assembler version: ");
  PutUnsigned((DWORD32)(cycles/1000000), 10, 8);
  PutString(" cycles/execution.");

  // C version
  SetCursorPosition(1, 0);
  cycles = CPU_Clock_Cycles();
  unsigned long long int a, b;
  a = cases[0].a;
  b = cases[1].b;
  for (j=0; j<1000000; j++)
    for (i = 0; i < 6; ++i) {
      //llmultiply(cases[i].a, cases[i].b, result);
       ///*
      llmultiply_c(a, b, result);
      a = *((unsigned long long int*)result);
      b = *((unsigned long long int*)result+8);
       //*/
    }
  cycles = CPU_Clock_Cycles() - cycles;
  PutString("        C version: ");
  PutUnsigned((DWORD32)(cycles/1000000), 10, 8);
  PutString(" cycles/execution.");
}


void delay(DWORD32 stopTime) {
  DWORD32 init = Milliseconds();
  while((Milliseconds()-init) < stopTime) {}
}



int main(int argc, char *argv[])
{

  /*print_test(llmultiply);    // Assembler version
  delay(3000);
  ClearScreen(0x07);
  delay(1000);
  print_test(llmultiply_c);  // C version
  delay(3000);
  */
  print_performance();


  return 0;
}
