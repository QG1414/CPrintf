// Printf.cpp - Written simple Printf function

#include <stdio.h>
#include <stdarg.h>

int Printf( const char* sFormat, ...  ); // header of Printf with multiple arguments
int PrintfV( const char* sFormat, va_list args ); // header of PrintfV with list of arguments


void outDec( int );       // znakowo wypisuje liczbe calk
void outChar( char );     // wypisuje znak  // putchar()
void outStr( char* );     // wypisuje zanakowo string
void outDouble( double ); // wypisuje znakowow liczbe double    0.  (do 8 po kropce)
void outNum( int x );     // wypisuje znakowo liczbe int >0     rekurencyjnie


#define DOUBLE_MIN_STOP 1De-6 // when double number is below this we should stop displayin it
#define DOUBLE_MAX_NUMBERS 8 // max number of digits after dot in double number

//----------------------------------
int main( int argc, char* argv[] )
{
  int n  = -0;
  char c = '$';
  double x = 12000000.34500000012;   // nie wiecej jak 8 po pkropce  // .0000012 <=1e-6
  double y = -.12;
  double z = -1.5;
  char*  str = (char*)"to jest string";

  n = Printf( "%s\n%f%c  n=%%d \\ \% /\ny=%f ` ` z=%%f\n%", str, x, c, n, y, z );  // ` -> '
  Printf( "\nilosc formatow=%d\n", n );
  return 0;
}


//-----------------------------------------------
int Printf( const char* sFormat, ... )
{
  va_list args; // create arguments list

  // Initialize variable arguments 
  // set the pointer at the beggining of the arg list and get the first argument from the args list
  va_start( args, sFormat );

  int res = PrintfV( sFormat, args ); // invoke PrintfV funtion and pass args, get result (number of formats)

  // Reset variable arguments
  va_end( args );

  return res; // return number of formats
}

//-----------------------------------------------
int PrintfV( const char* sFormat, va_list args )
{
  char c; // temporary char to hold sFormat chars
  int formats_num = 0; // number of formats that will be done

  while( c = *sFormat++ ) // get next chars in sFormat
  {
    switch( c ) // check for special cases % and `
    {
      case '%': 
        switch( c = *sFormat++ ) // check for format type after %
        {
            case 'd': outDec( va_arg( args, int ) ); formats_num++; break;
            case 'f': outDouble( va_arg( args, double ) ); formats_num++; break;
            case 's': outStr( va_arg( args, char* ) ); formats_num++; break;
            case 'c': outChar( va_arg( args, char ) ); formats_num++; break;
            default: 
              outChar( '%' ); // if format was not d,f,s or c then we had an simple % always
              if( !c || c=='%' )  sFormat--; // in case % have % after or next char is 0, then go to previous char
              else  outChar( c ); // if it was not empty or % then just print it out case like %g
        }
        break;

      case '`': c = '\''; // in case char is ` convert it to ' using \ and then display it using outChar in default
      default: outChar( c );
    }
  }
  return formats_num; // return number of formats
}

//-----------------------------------------------
void outChar( char c ) // displays char in console    
{
  putchar( c );
}

//-----------------------------------------------
void outStr( char* pStr ) // display string in console
{
  while( *pStr )  outChar( *pStr++ ); // we go through all chars in array pStr and we invoke outChar for every one of them
}

//-----------------------------------------------
void outDec( int x ) // display decimal number
{
  if( x < 0 ) // in case number is negative convert to positiv and display minus sign
  {
    outChar( '-' );
    x = -x;
  }
  else if( !x ) // in case number is equal 0, display 0 because function outNum will not show it
    outChar( '0' );

  outNum( x ); // invoke function outNum to display number by number
}

//-----------------------------------------------
void outDouble( double x ) // display floating-point number
{
  if( x < 0 ) // the same case as in outDec when number is negative change and display, we can't use outDec because -0.12 in out dec is 0 without -
  {
    outChar( '-' );
    x = -x;
  }

  outDec( (int)x ); // display natural numbers

  outChar( '.' ); // display dot to represent that number is floating-point

  x = x - (int)x; // cut off natural part of number for example 123.32 => 0.32 to not exceed size of int ???

  for( int i=0; i<DOUBLE_MAX_NUMBERS && ( x - (int)x )>DOUBLE_MIN_STOP; i++ ) // go through each number until we checked 8 numbers or decimal number is too small to recognize
    outChar( ( (int)(x *= 10) % 10 ) + '0' );
}

//-----------------------------------------------
void outNum( int x ) // display numbers one by one
{
  if( x > 0 ) // when x>0 we should display number otherwise stop because dividing 0 by 10 is always 0
  {  
    outNum( x / 10 ); // recursion first then displaying for correct order of digits
    outChar( '0' + (x%10) );
  }
}
