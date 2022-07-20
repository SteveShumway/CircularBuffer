/******************************************************************************
 *
 * Copyright(c) 2022 Steve Shumway. All rights reserved.
 * email: sshumway@neo.rr.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
					    
#ifdef debug_circular_buffer
 #include <iostream>
 #include <fstream>
#endif

#include <vector> // Used to hold data in CircularBuffer 

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

template < typename T > class CircularBuffer
{
public:
  // NOTE: We need a bunch of testing for various values of mDataSize 
  CircularBuffer() :
    mDataSize(2),
    mIn(0),
    mOut(0),
    mEmptyFlag(true),
    mFullFlag(false),
    mDataQty(0)
  {
    mVec.resize( mDataSize );
  }

#ifdef debug_circular_buffer
void Dump() const
  {
    unsigned index;

    if ( mFullFlag )
        index = mIn;
    else
        index = mOut;

    std::cout << __PRETTY_FUNCTION__ << " mDataQty = " << mDataQty << std::endl;

    for ( int i = 0; i < mDataQty; ++i )
      {
        std::cout << "[" << i << "]=" << (unsigned)mVec[index] << " ";
        index++;
        
        if (index >= mDataSize)
          index = 0;
      }
    std::cout << std::endl;

    if ( mEmptyFlag )
      std::cout << "mEmptyFlag: true   ";
    else
      std::cout << "mEmptyFlag: false  ";

    if ( mFullFlag )
      std::cout << "mFullFlag: true" << std::endl;
    else
      std::cout << "mFullFlag: false" << std::endl;

    std::cout << std::endl;
  }
 #endif

  void SetBufferSize( unsigned howBig )
  {
    mDataSize = howBig;
    mVec.resize( mDataSize );
    mIn = mOut = 0;
    mEmptyFlag = true;
    mDataQty = 0;
  }

  void InsertData( T item )
  {
    // The input index ALWAYS tells us where the next data element is
    // to be stashed.
    mVec[ mIn ] = item;
    bumpInputIndex();
    mEmptyFlag = false;
    
    mDataQty++;
    if ( mDataQty > mDataSize )
        mDataQty = mDataSize;
  }

  bool ExtractData( T & item )
  {
    unsigned index;
    bool rv = true; // Assume failure.

    // If the buffer is full, the oldest
    // piece of data is indexed by: 'mIn'
    //
    // If the buffer is NOT full, the oldest
    // piece of data is indexed by: 'mOut'
    if ( mFullFlag )
        index = mIn;
    else
        index = mOut;
    
    if ( mEmptyFlag != true )
      {
        rv = false; // Non-failure return (we got data for the caller)
        item = mVec[ index ];
        bumpOutputIndex();
      }
    
    return rv;
  }

  unsigned GetBufferSize() const { return mDataSize; }
  unsigned GetElementCount() const { return mDataQty; }
  
private:
  void bumpInputIndex()
  {
    mIn++;
    if ( mIn >= mDataSize )
      mIn = 0;

    if ( mIn == mOut )
      {
        // The input index has caught up with the output index
        // The ring is full. We need to adjust the output index
        // so that it refers to the new oldest element in the ring.
        mOut++;
        if ( mOut >= mDataSize )
            mOut = 0;
        
        mFullFlag = true;
      }
  }
  
  void bumpOutputIndex()
  {
    if ( mEmptyFlag != true )
      {
        // If the buffer is full, the input and output indices will be
        // the same. We don't need to bump the output index in this one
        // special case.
        if ( mFullFlag == false )
          mOut++;

        // We do however need to insure that the buffer full flag now
        // indicates that the buffer is in the non-full state.
        mFullFlag = false;
        
        if ( mOut >= mDataSize )
          mOut = 0;
        
        if ( mOut == mIn )
          {
            // Output index has caught up with input index.
            // The ring is now empty.
            mEmptyFlag = true;
            mDataQty = 0;
          }
      }
  }
  
  std::vector<T> mVec;

  unsigned mDataSize;
  unsigned mIn;
  unsigned mOut;
  bool mEmptyFlag;
  bool mFullFlag;

  unsigned mDataQty;
}; // template < typename T > class CircularBuffer

#endif
