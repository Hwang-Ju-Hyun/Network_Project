#include "MemoryBitStream.hpp"
#include <iostream>
#include <cstring>
#include <cassert>

OutputMemoryBitStream::OutputMemoryBitStream()
{
    ReallocBuffer(256);
}

OutputMemoryBitStream::~OutputMemoryBitStream()
{
    std::free(m_Buffer);
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t _inNewBitLength)
{    
    if(m_Buffer==nullptr)
    {
        m_Buffer = static_cast<char*>(std::malloc((_inNewBitLength+7)>>3));
        assert(std::memset(m_Buffer,0,(_inNewBitLength+7)>>3)!=nullptr);
    }
    else
    {
        char* tempBuffer;
        tempBuffer = static_cast<char*>(std::malloc((_inNewBitLength+7)>>3));
        assert(std::memset(tempBuffer,0,(_inNewBitLength+7)>>3)!=nullptr);
        memcpy(tempBuffer,m_Buffer,m_BitCapacity+7>>3);
        std::free(m_Buffer);
        m_Buffer=tempBuffer;
    }
}

void OutputMemoryBitStream::WriteBits(uint8_t _inData,size_t _inBitsCount)
{
    uint32_t nextBitHead=m_BitHead+static_cast<uint32_t>(_inBitsCount);
    if(m_BitCapacity<nextBitHead)
    {
        ReallocBuffer(std::max(m_BitCapacity*2,nextBitHead));
    }

    uint32_t byteOffset = m_BitHead>>3;
    uint8_t bitOffset  = m_BitHead&0x7;

    uint8_t currentMask=~(0xff<<bitOffset);

    m_Buffer[byteOffset]=(currentMask&m_Buffer[byteOffset])|(_inData<<_inBitsCount);

    uint8_t bitsFreeThisBytes=8-bitOffset;
    if(bitsFreeThisBytes<_inBitsCount)
    {
        m_Buffer[byteOffset+1]=_inData>>bitsFreeThisBytes;
    }
    m_BitHead=nextBitHead;
}

void OutputMemoryBitStream:: WriteBits(const void* _inData,uint32_t _inBitCount)
{
    const char* srcByte=static_cast<const char*>(_inData);

    while(_inBitCount>8)
    {
        WriteBits(*srcByte,8);
        srcByte++;
        _inBitCount-=8;
    }
    if(_inBitCount>0)
    {
        WriteBits(*srcByte,_inBitCount);
    }
}


//================================================================


InputMemoryBitStream::InputMemoryBitStream(char* _inBuffer,uint32_t _inBitCount)
    :m_BitHead(0)
    ,m_BitCapacity(_inBitCount)
    ,m_Buffer(_inBuffer)
{

}

void InputMemoryBitStream::ReadBits(uint8_t& _outData,uint32_t _inBitCount)
{    
    uint32_t byteOffset = m_BitHead >> 3;
    uint32_t bitOffset  = m_BitHead & 0x7;

    _outData = static_cast< uint8_t >( m_Buffer[ byteOffset ] ) >> bitOffset;

    uint32_t bitsFreeThisByte = 8 - bitOffset;
    if( bitsFreeThisByte < _inBitCount )
    {	    
	    _outData |= static_cast< uint8_t >( m_Buffer[ byteOffset + 1 ] ) << bitsFreeThisByte;
    }

    //don't forget a mask so that we only read the bit we wanted...
    _outData &= ( ~( 0x00ff << _inBitCount ) );

    m_BitHead += _inBitCount;
}

void InputMemoryBitStream::ReadBits(void* _outData,uint32_t _inBitCount)
{
    uint8_t* destData=static_cast<uint8_t*>(_outData);
    while(_inBitCount>8)
    {
        ReadBits(*destData,8);
        destData++;
        _inBitCount-=8;
    }
    if(_inBitCount>0)
    {
        ReadBits(*destData,_inBitCount);        
    }
}