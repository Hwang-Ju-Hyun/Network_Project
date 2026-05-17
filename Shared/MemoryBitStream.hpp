#pragma once
#include <cstdint>
#include <stddef.h>
#include <vector>

//비트연산은 LSB기준으로 한다.

class OutputMemoryBitStream
{
public:
    OutputMemoryBitStream();
    ~OutputMemoryBitStream();
private:
    void ReallocBuffer(uint32_t _inNewBitLength);
private:
    uint32_t m_BitHead;
    uint32_t m_BitCapacity;
    char* m_Buffer;
public:
    const char* GetBuffer()const{return m_Buffer;}
    uint32_t GetBitLength()const {return m_BitHead;}
    uint32_t GetByteLength()const{return (m_BitHead+7)>>3;}
public:
    void WriteBits(uint8_t _inData,size_t _inBitsCount);
    void WriteBits(const void* _inData,uint32_t _inBitCount);    
    void WriteBytes(const void* _inData,uint32_t _inByteCount){WriteBits(_inData,_inByteCount<<3);}

    void Write( uint32_t _inData, uint32_t _inBitCount = 32 )	{ WriteBits( &_inData, _inBitCount ); }
    void Write( int _inData, uint32_t _inBitCount = 32 )		{ WriteBits( &_inData, _inBitCount ); }
    void Write( float _inData )								{ WriteBits( &_inData, 32 ); }
    void Write( uint16_t _inData, uint32_t _inBitCount = 16 )	{ WriteBits( &_inData, _inBitCount ); }    
    void Write( int16_t _inData, uint32_t _inBitCount = 16 )	{ WriteBits( &_inData, _inBitCount ); }
    void Write( uint8_t _inData, uint32_t _inBitCount = 8 )	{ WriteBits( &_inData, _inBitCount ); }
    void Write( bool _inData)	{ WriteBits( &_inData, 1 ); }    
};



class  InputMemoryBitStream
{
public:
    InputMemoryBitStream(char* _inBuffer,uint32_t _inBitCount);
    ~InputMemoryBitStream();
private:
    uint32_t m_BitHead;
    uint32_t m_BitCapacity;
    char* m_Buffer;
public:
    void ReadBits(uint8_t& _outData,uint32_t _inBitCount);
    void ReadBits(void* _outData,uint32_t _inBitCount);

    void Read( uint8_t& _outData,uint32_t _inBitCount=8)            {ReadBits(&_outData,_inBitCount);}
    void Read( uint32_t& _outData,uint32_t _inBitCount=32)          {ReadBits(&_outData,_inBitCount);}
    void Read( uint16_t& _outData, uint32_t _inBitCount = 16 )		{ ReadBits( &_outData, _inBitCount ); }
    void Read( int16_t& _outData, uint32_t _inBitCount = 16 )		{ ReadBits( &_outData, _inBitCount ); }
    void Read( bool& _outData )									    { ReadBits( &_outData, 1 ); }
    void Read( int& _outData, uint32_t _inBitCount = 32 )			{ ReadBits( &_outData, _inBitCount ); }
public:
    const char* GetBuffer()const{return m_Buffer;}
    uint32_t	GetRemainingBitCount() 	const { return m_BitCapacity - m_BitHead; }
};