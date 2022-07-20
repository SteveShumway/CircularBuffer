/* Test and example code that shows how to employ the CircularBuffer.
 */
#include <iostream>

#include "CircularBuffer.h"

int main()
{
  CircularBuffer<int> cb;
  int x;

  cb.SetBufferSize( 5 );
  for( int i = 0; i < 10; ++i )
    {
      cb.InsertData(i+10);
    }

  while( cb.ExtractData(x) == false )
    {
      std::cout  << x << ", ";
    }
  std::cout << std::endl;
}




