#ifndef LEYBUF_H
#define LEYBUF_H
#pragma once

#include <memory.h>
#include <string.h>


class leybuf
{

private:
	char*buffer;
	unsigned int size_of_buffer;
	unsigned int curbyte;

public:

	~leybuf()
	{
		buffer = 0;
		size_of_buffer = 0;
		curbyte = 0;
	}

	leybuf(char*buf, unsigned int size, unsigned int byte = 0)
	{
		buffer = buf;
		size_of_buffer = size;
		curbyte = byte;
	}

	//status funcs
	bool IsOverflowed()
	{
		if (curbyte >= size_of_buffer)
			return true;

		return false;
	}

	//set funcs
	bool SetPos(unsigned int byte)
	{
		if (byte > size_of_buffer)
			return false;

		curbyte = byte;
		return true;
	}

	//get funcs

	int GetSize()
	{
		return size_of_buffer;
	}

	int GetPos()
	{
		return curbyte;
	}

	unsigned int GetNumBytesLeft()
	{
		return size_of_buffer - curbyte;
	}

	char* GetData()
	{
		return buffer;
	}

	//write
	bool WriteChar(char ch)
	{

		if (IsOverflowed())
			return false;

		buffer[curbyte] = ch;
		curbyte++;

		if (curbyte > size_of_buffer)
			curbyte = size_of_buffer;

		return true;
	}

	bool WriteString(const char *str, bool removenull=false)
	{
		
		if (IsOverflowed()||!str)
			return false;
	
		if (removenull)
		{
			for (unsigned int i = 0; i < strlen(str); i++)
			{
				buffer[curbyte] = str[i];
				curbyte++;
			}

			return true;
		}
		strcpy(buffer + curbyte, str);
		curbyte += strlen(str)+1;

		if (curbyte > size_of_buffer)
			curbyte = size_of_buffer;

		return true;
	}

	int WriteInt32(int num)
	{
		if (IsOverflowed())
			return 0;

		memcpy(&buffer[curbyte], &num, 4);

		curbyte += 4;

		if (curbyte > size_of_buffer)
			curbyte = size_of_buffer;

		return num;
	}

	bool WriteBytes(void*src, int num)
	{

		if (IsOverflowed())
			return false;

		memcpy(buffer + curbyte, src,  num);
		curbyte += num;

		if (curbyte > size_of_buffer)
			curbyte = size_of_buffer;

		return true;
	}

	//read
	char ReadChar()
	{
		if (IsOverflowed())
			return 0;

		char ret = buffer[curbyte];
		curbyte++;

		if (curbyte > size_of_buffer)
			curbyte = size_of_buffer;

		return ret;
	}

	bool ReadString(char*buf, unsigned int buflen)
	{
		if (IsOverflowed())
			return false;

		if (strlen(buffer + curbyte) > buflen)
			return false;

		strcpy(buf, buffer+curbyte);
		curbyte += strlen(buffer+curbyte)+1;

		if (curbyte > size_of_buffer)
			curbyte = size_of_buffer;

		return true;
	}

	int ReadInt32()
	{
		if (IsOverflowed())
			return 0;

		int ret = 0;
		memcpy(&ret, &buffer[curbyte], 4);

		curbyte+=4;

		if (curbyte > size_of_buffer)
			curbyte = size_of_buffer;

		return ret;
	}

	bool ReadBytes(void*buf, unsigned int buflen)
	{
		if (IsOverflowed())
			return false;

		memcpy(buf, buffer+curbyte, buflen);

		if (curbyte > size_of_buffer)
			curbyte = size_of_buffer;

		return true;
	}

};

#endif