
#pragma once

#include "defines.h"
#include "leybuf.h"

#include <Windows.h>
#include <stdlib.h>
#include <cstdio>


unsigned int encryptionkey = 0;

struct SEGMENT_INFO
{
	DWORD fileSegmentOffset;
	DWORD fileSegmentSize;
	DWORD memorySegmentOffset;
	DWORD memorySize;

	DWORD moduleBase;

	BYTE name[0xFF];


};


void TraverseSectionHeaders( PIMAGE_SECTION_HEADER section, DWORD nSections, SEGMENT_INFO* addrInfo, const char *sectionName )
{
	DWORD i;
	/* Copy pointer to initial section (so this function can be called several times) */
	PIMAGE_SECTION_HEADER localSection = section;

	printf("\n\nTraverseSectionHeaders: searching for segment in section headers\n");
	for (i = 0; i<nSections; i++)
	{
		printf("     ====================     \n");
		printf("\tName:                 %s\n", section->Name);

		BYTE*bname = section->Name;

		if (strcmp((char*)bname, sectionName) == 0)
		{
			printf("FOUND SECTION: %s\n", bname);

			addrInfo->fileSegmentOffset = section->PointerToRawData; /* Location of segment in binary file*/
			addrInfo->fileSegmentSize = section->SizeOfRawData; /* Size of segment */
			addrInfo->memorySegmentOffset = section->VirtualAddress; /* Offset of segment in memory at runtime */
			addrInfo->memorySize = section->Misc.VirtualSize;


			strcpy((char*)addrInfo->name, (char*)bname);
			break;
		}
		section = section + 1;
	}	
	return;
}
/**
* Encrypt .code segment bytes in the given file
*/


void patchStub(char*stubfunc, unsigned int stubfunclen, unsigned long segmentstart_text, unsigned long segmentsize_text, unsigned long segmentstart_rdata, unsigned long segmentsize_rdata, char*baseAddrSignature_text, char*segSizeSignature_text, char*baseAddrSignature_rdata, char*segSizeSignature_rdata)
{

	int patched = 0;

	bool patched_baseaddr_text = false;
	bool patched_sigsize_text = false;

	bool patched_baseaddr_rdata = false;
	bool patched_sigsize_rdata = false;


	for (unsigned int i = 0; i < stubfunclen; i++)
	{
		if (!patched_baseaddr_text&&!memcmp(stubfunc + i, baseAddrSignature_text, strlen(baseAddrSignature_text)))
		{
			memcpy(stubfunc + i, &segmentstart_text, 4);

			printf(".text: REPLACED BASE ADDR SIG!\n");
			patched++;
			patched_baseaddr_text = true;

		}

		if (!patched_sigsize_text&&!memcmp(stubfunc + i, segSizeSignature_text, strlen(segSizeSignature_text)))
		{
			memcpy(stubfunc + i, &segmentsize_text, 4);

			printf(".text: REPLACED SEG SIZE SIG!\n");
			patched++;

			patched_sigsize_text = true;
		}


		if (!patched_baseaddr_rdata&&!memcmp(stubfunc + i, baseAddrSignature_rdata, strlen(baseAddrSignature_rdata)))
		{
			memcpy(stubfunc + i, &segmentstart_rdata, 4);

			printf(".text: REPLACED BASE ADDR SIG 2!\n");
			patched++;
			patched_baseaddr_rdata = true;

		}

		if (!patched_sigsize_rdata&&!memcmp(stubfunc + i, segSizeSignature_rdata, strlen(segSizeSignature_rdata)))
		{
			memcpy(stubfunc + i, &segmentsize_rdata, 4);

			printf(".text: REPLACED SEG SIZE SIG 2!\n");
			patched++;

			patched_sigsize_rdata = true;
		}

		if (patched == 4)
			break;
	}

	return;
}


void cipherBytes(char* fileName, SEGMENT_INFO* textSection, SEGMENT_INFO* rdataSection, char* baseAddrSignature_text, char*segSizeSignature_text, char* baseAddrSignature_rdata, char*segSizeSignature_rdata )
{

	unsigned int cpt = 0;

	/* Allocate memory in buffer that will store content of segment */
	char *buffer = (char*)malloc(textSection->fileSegmentSize);
	if (buffer == NULL)
	{
		printf("cipherBytes: malloc error \n");
		return;
	}

	/* Open binary file */
	FILE* fptr = fopen(fileName, "r+b");
	if (fptr == NULL)
	{
		printf("cipherBytes: fopen error \n");
		return;
	}
	/* Seek section using calculated offset and copy content into buffer*/
	if (fseek(fptr, textSection->fileSegmentOffset, SEEK_SET) != 0)
	{
		printf("cipherBytes: Unable to set file pointer to %ld \n", textSection->fileSegmentOffset);
		fclose(fptr);
		return;
	}

	DWORD nItems = fread(buffer, sizeof(char), textSection->fileSegmentSize, fptr);
	if (nItems  <textSection->fileSegmentSize)
	{
		printf("cipherBytes: Trouble reading nItems = %d \n", nItems);
		fclose(fptr);
		return;
	}

	char stubstartmask[] = "\xE9\x02\x00\x00\x00\xCD\xFF";
	char stubendmask[] = "\xE9\x02\x00\x00\x00\xCD\xFE";

	unsigned int stubfuncstart = 0;
	unsigned int stubfuncend = 0;
	unsigned int stubfunclen = 0;

	/* Encrypt buffer */
	for (unsigned long i = 0; i < textSection->fileSegmentSize; i++)
	{
		int not_match = memcmp(buffer + i+6, stubstartmask, sizeof(stubstartmask)-1);

		if (!stubfuncstart&&!not_match)
		{

			stubfuncstart = i;
			stubfuncend = 0;

			for (unsigned int a = i+3; a < i+1+500; a++)
			{
				if (!memcmp(&buffer[a], stubendmask, sizeof(stubendmask)-1))
				{
					stubfuncend = a+ sizeof(stubendmask);
				}
			}

			if (!stubfuncend)
			{
				stubfuncstart = 0;

				printf("ERROR! STUBFUNCEND NOT FOUND, PLEASE CHANGE SIG!\n");
				return;
			}

			stubfunclen = stubfuncend - stubfuncstart;

			printf("FOUND STUB FUNC: %x __ %x __%x\n", stubfuncstart, stubfuncend, stubfunclen);
		
			i = stubfuncend;

			if (i > textSection->fileSegmentSize)
				i = textSection->fileSegmentSize - 1;

		}
		
		buffer[i] ^= (encryptionkey >> 8 * (i % 4)) & 0xff;
	}



	patchStub( buffer + stubfuncstart, stubfunclen,
		textSection->memorySegmentOffset + textSection->moduleBase,
		textSection->memorySegmentOffset + textSection->moduleBase + textSection->fileSegmentSize,
		rdataSection->memorySegmentOffset + rdataSection->moduleBase,
		rdataSection->memorySegmentOffset + rdataSection->moduleBase + rdataSection->fileSegmentSize,
		baseAddrSignature_text, segSizeSignature_text,
		baseAddrSignature_rdata, segSizeSignature_rdata );


	printf(".text: OFFSET: %i __ BYTES: %i __ MEMORYSEG: %x BASE: %x __ %s __ END: %08x\n", textSection->fileSegmentOffset, textSection->memorySize, textSection->memorySegmentOffset, textSection->moduleBase, textSection->name, textSection->memorySegmentOffset + textSection->moduleBase + textSection->fileSegmentSize);
	
	printf(".rdata: OFFSET: %i __ BYTES: %i __ MEMORYSEG: %x BASE: %x __ %s __ END: %08x\n", textSection->fileSegmentOffset, rdataSection->memorySize, rdataSection->memorySegmentOffset, rdataSection->moduleBase, rdataSection->name, rdataSection->memorySegmentOffset + rdataSection->moduleBase + rdataSection->fileSegmentSize);


	/* Replace current .text section in file by encrypted one */
	if (fseek(fptr, textSection->fileSegmentOffset, SEEK_SET) != 0)
	{
		printf("cipherBytes: Unable to set file pointer to %ld \n", textSection->fileSegmentOffset);
		fclose(fptr);
		return;
	}
	nItems = fwrite(buffer, sizeof(BYTE), textSection->fileSegmentSize, fptr);
	if (nItems  <textSection->fileSegmentSize)
	{
		printf("cipherBytes: Trouble writing nItems = %d \n", nItems);
		fclose(fptr);
		return;
	}

	/*.rdata encryption*/

	if (fseek(fptr, rdataSection->fileSegmentOffset, SEEK_SET) != 0)
	{
		printf("cipherBytes: Unable to set file pointer to %ld \n", textSection->fileSegmentOffset);
		fclose(fptr);
		return;
	}

	free(buffer);

	buffer = (char*)malloc(textSection->fileSegmentSize);

	nItems = fread(buffer, sizeof(char), rdataSection->fileSegmentSize, fptr);
	if (nItems  <rdataSection->fileSegmentSize)
	{
		printf("cipherBytes: Trouble reading nItems = %d \n", nItems);
		fclose(fptr);
		return;
	}

	for (unsigned long i = 0; i < rdataSection->fileSegmentSize; i++)
	{
		buffer[i] ^= (encryptionkey >> 8 * (i % 4)) & 0xff;
	}

	/* Replace current .rdata section in file by encrypted one */

	if (fseek(fptr, rdataSection->fileSegmentOffset, SEEK_SET) != 0)
	{
		printf("cipherBytes: Unable to set file pointer to %ld \n", rdataSection->fileSegmentOffset);
		fclose(fptr);
		return;
	}

	nItems = fwrite(buffer, sizeof(BYTE), rdataSection->fileSegmentSize, fptr);
	
	if (nItems  <rdataSection->fileSegmentSize)
	{
		printf("cipherBytes: Trouble writing nItems = %d \n", nItems);
		fclose(fptr);
		return;
	}

	printf("Successfully ciphered %d bytes\n", rdataSection->fileSegmentSize+textSection->fileSegmentSize);

	fclose(fptr);

	return;
}

void getSegmentsInfo(LPVOID baseAddress, SEGMENT_INFO* textSegmentInfo, const char*section)
{
	PIMAGE_DOS_HEADER dosHeader;
	PIMAGE_NT_HEADERS peHeader;
	IMAGE_OPTIONAL_HEADER32 optionalHeader;
	
	dosHeader = (PIMAGE_DOS_HEADER)baseAddress;
	if (((*dosHeader).e_magic) != IMAGE_DOS_SIGNATURE)
	{
		printf("getSegmentsInfo: Dos signature not matched\n");
		return;
	}
	printf("getSegmentsInfo: Dos signature=%X\n", (*dosHeader).e_magic);

	peHeader = (PIMAGE_NT_HEADERS)((DWORD)baseAddress + (*dosHeader).e_lfanew);
	if (((*peHeader).Signature) != IMAGE_NT_SIGNATURE)
	{
		printf("getSegmentsInfo: PE signature not matched\n");
		return;
	}
	printf("getSegmentsInfo: PE signature=%X\n", (*peHeader).Signature);

	optionalHeader = (*peHeader).OptionalHeader;
	if ((optionalHeader.Magic) != 0x10B)
	{
		printf("getSegmentsInfo: Optional header magic number does not match\n");
		return;
	}

	(*textSegmentInfo).moduleBase = optionalHeader.ImageBase;

	printf("getSegmentsInfo: # sections=%d\n", (*peHeader).FileHeader.NumberOfSections);
	/* Fill code information with content of code segment */
	TraverseSectionHeaders(IMAGE_FIRST_SECTION(peHeader), (*peHeader).FileHeader.NumberOfSections, textSegmentInfo, section);
	return;
}


bool getdosheader(char*filename, char*filedosheader, int filedosheader_size)
{
	HANDLE hMapFile;
	LPCTSTR lpFileBase;

	HANDLE hFile = CreateFile(filename, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	int filesize = GetFileSize(hFile, 0);

	hMapFile = CreateFileMapping( hFile, NULL, PAGE_READWRITE, 0, 0, filename);

	if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE)
	{
		printf("Could not create file mapping object (%d).\n", GetLastError());
		return 0;
	}

	lpFileBase = (LPTSTR)MapViewOfFile(hMapFile,  FILE_MAP_ALL_ACCESS,  0, 0, 0);

	if (lpFileBase == NULL)
	{
		printf("Could not map view of file (%d).\n", GetLastError());
		return 0;
	}

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)lpFileBase;


	int size = filedosheader_size;

	if (size > filesize)
		size = filesize;

	memcpy(filedosheader, lpFileBase, size);

	UnmapViewOfFile(lpFileBase);
	CloseHandle(hMapFile);
	CloseHandle(hFile);

	return 1;

}

int main(int argc, char** argv)
{

	if (argc <3)
	{
		printf("Args: filename key");
		getchar();
		return 0;
	}
	

	char filename[0xFF];
	strcpy(filename, argv[1]);
	encryptionkey = (unsigned int)atoi(argv[2]);



	printf("Going to protect: %s\nKey: %lu\n", filename, encryptionkey);

	/* Map target file */


	int filedosheader_size = 0xFFFFFF;
	char *filedosheader = new char[filedosheader_size];

	if (!getdosheader(filename, filedosheader, filedosheader_size))
	{
		printf("couldn't find file %s", filename);
		getchar();

		return 0;
	}

	/* Init structures */
	SEGMENT_INFO textSegmentInfo = { 0 };
	SEGMENT_INFO rdataSegmentInfo = { 0 };

	/* Fill segments information */
	getSegmentsInfo(filedosheader, &textSegmentInfo, ".text");
	getSegmentsInfo(filedosheader, &rdataSegmentInfo, ".rdata");

	printf("\n\n=======================\n");
	printf("segment information: .text\n");
	printf("RAM image base          =0x%08X\n", textSegmentInfo.moduleBase);
	printf("RAM segment offset      =0x%08X\n", textSegmentInfo.memorySegmentOffset);
	printf("File offset =0x%X\n", textSegmentInfo.fileSegmentOffset);
	printf("File size      =0x%X\n", textSegmentInfo.fileSegmentSize);
	printf("\n\n=======================\n");

	printf("\n\n=======================\n");
	printf("segment information: .rdata\n");
	printf("RAM image base          =0x%08X\n", rdataSegmentInfo.moduleBase);
	printf("RAM segment offset      =0x%08X\n", rdataSegmentInfo.memorySegmentOffset);
	printf("File offset =0x%X\n", rdataSegmentInfo.fileSegmentOffset);
	printf("File size      =0x%X\n", rdataSegmentInfo.fileSegmentSize);
	printf("\n\n=======================\n");


	char baseAddrSignature_text[] = { 0x15, 0x15, 0x15, 0x15, 0x00 };
	char segSizeSignature_text[] = { 0x14, 0x14, 0x14, 0x14, 0x00 };

	char baseAddrSignature_rdata[] = { 0x13, 0x13, 0x13, 0x13, 0x00 };
	char segSizeSignature_rdata[] = { 0x12, 0x12, 0x12, 0x12, 0x00 };


	cipherBytes(filename, &textSegmentInfo, &rdataSegmentInfo, baseAddrSignature_text, segSizeSignature_text, baseAddrSignature_rdata, segSizeSignature_rdata);

	getchar();

	return 0;
}

