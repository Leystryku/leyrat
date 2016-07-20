
#pragma once

#include "defines.h"
#include "leybuf.h"

#include <Windows.h>
#include <stdlib.h>
#include <cstdio>



struct SEGMENT_INFO
{
	unsigned int fileSegmentOffset;
	unsigned int fileSegmentSize;
	unsigned int memorySegmentOffset;
	unsigned int memorySize;

	unsigned int moduleBase;

	char name[0xFF];


};

unsigned int numsections = 0;

void* TraverseSectionHeaders(unsigned int numberofsegments, unsigned char* section, SEGMENT_INFO* addrInfo, const char *sectionName)
{

	printf("\n\nTraverseSectionHeaders: searching for segment in section headers\n");


	for (unsigned int i = 0; i < numberofsegments; i++)
	{

		PIMAGE_SECTION_HEADER found = (PIMAGE_SECTION_HEADER)(section + (i*sizeof(IMAGE_SECTION_HEADER) ));

		char*cSectionName = (char*)found->Name;

		printf("====================     \n");
		printf("Name:                 %s\n", found->Name);
		printf("Size:                 %x\n", found->SizeOfRawData);
		printf("VSize:                 %x\n", found->Misc.VirtualSize);

		if (strcmp(cSectionName, sectionName) == 0)
		{
		

			if (!addrInfo)
				return section;

			addrInfo->fileSegmentOffset = found->PointerToRawData; /* Location of segment in binary file*/
			addrInfo->fileSegmentSize = found->SizeOfRawData; /* Size of segment */
			addrInfo->memorySegmentOffset = found->VirtualAddress; /* Offset of segment in memory at runtime */
			addrInfo->memorySize = found->Misc.VirtualSize;


			strcpy(addrInfo->name, cSectionName);

			return found;
		}


	}

	printf("\n\nTraverseSectionHeaders: done!\n");

	return 0;
}
/**
* Encrypt .code segment bytes in the given file
*/


void patchStub(char*stubfunc, unsigned int stubfunclen, unsigned long segmentstart_text, unsigned long segmentsize_text, char*baseAddrSignature_text, char*segSizeSignature_text)
{

	int patched = 0;

	bool patched_baseaddr_text = false;
	bool patched_sigsize_text = false;

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


		if (patched == 2)
			break;
	}

	return;
}


void cipherBytes(char* fileName, SEGMENT_INFO* textSection, SEGMENT_INFO* rsrcSection, char* baseAddrSignature_text, char*segSizeSignature_text)
{
/*
	unsigned int cpt = 0;

	//Allocate memory in buffer that will store content of segment
	char *buffer = (char*)malloc(textSection->fileSegmentSize);
	if (buffer == NULL)
	{
		printf("cipherBytes: malloc error \n");
		return;
	}

	//Open binary file
	FILE* fptr = fopen(fileName, "r+b");
	if (fptr == NULL)
	{
		printf("cipherBytes: fopen error \n");
		return;
	}
	//Seek section using calculated offset and copy content into buffer
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

	//Encrypt buffer
	for (unsigned long i = 0; i < textSection->fileSegmentSize; i++)
	{
		int not_match = memcmp(buffer + i + 6, stubstartmask, sizeof(stubstartmask) - 1);

		if (!stubfuncstart&&!not_match)
		{

			stubfuncstart = i;
			stubfuncend = 0;

			for (unsigned int a = i + 3; a < i + 1 + 500; a++)
			{
				if (!memcmp(&buffer[a], stubendmask, sizeof(stubendmask) - 1))
				{
					stubfuncend = a + sizeof(stubendmask);
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



	patchStub(buffer + stubfuncstart, stubfunclen,
		textSection->memorySegmentOffset + textSection->moduleBase,
		textSection->memorySegmentOffset + textSection->moduleBase + textSection->fileSegmentSize,
		baseAddrSignature_text, segSizeSignature_text,
		baseAddrSignature_rsrc, segSizeSignature_rsrc);


	printf(".text: OFFSET: %i __ BYTES: %i __ MEMORYSEG: %x BASE: %x __ %s __ END: %08x\n", textSection->fileSegmentOffset, textSection->memorySize, textSection->memorySegmentOffset, textSection->moduleBase, textSection->name, textSection->memorySegmentOffset + textSection->moduleBase + textSection->fileSegmentSize);

	

	//Replace current .text section in file by encrypted one
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


	free(buffer);

	buffer = (char*)malloc(textSection->fileSegmentSize);

	nItems = fread(buffer, sizeof(char), rsrcSection->fileSegmentSize, fptr);
	if (nItems  <rsrcSection->fileSegmentSize)
	{
		printf("cipherBytes: Trouble reading nItems = %d \n", nItems);
		fclose(fptr);
		return;
	}

	for (unsigned long i = 0; i < rsrcSection->fileSegmentSize; i++)
	{
		buffer[i] ^= (encryptionkey >> 8 * (i % 4)) & 0xff;
	}
	nItems = fwrite(buffer, sizeof(BYTE), rsrcSection->fileSegmentSize, fptr);

	if (nItems  <rsrcSection->fileSegmentSize)
	{
	printf("cipherBytes: Trouble writing nItems = %d \n", nItems);
	fclose(fptr);
	return;
	}

	printf("Successfully ciphered %d bytes\n", rsrcSection->fileSegmentSize + textSection->fileSegmentSize);

	fclose(fptr);

	*/

	return;
}

void* getSegmentsInfo(char* baseAddress, SEGMENT_INFO* segmentInfo, const char*section)
{
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)baseAddress;
	PIMAGE_NT_HEADERS peHeader = (PIMAGE_NT_HEADERS)(baseAddress + dosHeader->e_lfanew);
	IMAGE_OPTIONAL_HEADER32 optionalHeader;

	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		printf("Dos signature not matched: %x\n", dosHeader->e_magic);
		return 0;
	}

	if (peHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		printf("PE signature not matched: %x\n", peHeader->Signature);
		return 0;
	}

	optionalHeader = peHeader->OptionalHeader;

	if (optionalHeader.Magic != 0x10B)
	{
		printf("Optional header magic number does not match: %x\n", optionalHeader.Magic);
	}
	else {
		segmentInfo->moduleBase = optionalHeader.ImageBase;
	}

	

	printf("NumberOfSections: %d\n", peHeader->FileHeader.NumberOfSections);

	numsections = peHeader->FileHeader.NumberOfSections;

	return TraverseSectionHeaders(numsections, (unsigned char*)IMAGE_FIRST_SECTION(peHeader), segmentInfo, section);
}


bool readfile(char*filename, char**filedosheader, int* filedosheader_size)
{

	//Open binary file
	FILE* fptr = fopen(filename, "r+b");
	if (fptr == NULL)
	{
		printf("getdosheader: fopen error\n");
		return false;
	}

	fseek(fptr, 0, SEEK_END);

	int filesize = ftell(fptr);
	char*filebuffer = (char*)malloc(filesize);



	rewind(fptr);

	fread(filebuffer, sizeof(char), filesize, fptr);

	fclose(fptr);


	if (*(unsigned short*)filebuffer != IMAGE_DOS_SIGNATURE)
	{
		free(filebuffer);
		return false;
	}

	*filedosheader_size = filesize;
	*filedosheader = filebuffer;

	return true;

}

bool encryptfile(unsigned int encryptionkey, char*filename, char**filedosheader, int* filedosheader_size)
{

	//Open binary file
	FILE* fptr = fopen(filename, "r+b");
	if (fptr == NULL)
	{
		printf("encryptfile: fopen error\n");
		return false;
	}

	fseek(fptr, 0, SEEK_END);

	int filesize = ftell(fptr);
	char*filebuffer = (char*)malloc(filesize);



	rewind(fptr);

	fread(filebuffer, sizeof(char), filesize, fptr);

	fclose(fptr);


	if (*(unsigned short*)filebuffer != IMAGE_DOS_SIGNATURE)
	{
		free(filebuffer);
		return false;
	}

	for (int i = 0; i < filesize; i++)
	{
		filebuffer[i] ^= (encryptionkey >> 8 * (i % 4)) & 0xff;
	}

	*filedosheader_size = filesize;
	*filedosheader = filebuffer;

	return true;

}

bool injectstubcode(unsigned int encryptionkey, const char* injectfile_encrypted, int injectfile_encrypted_size, char* injectstub, int injectstub_size, SEGMENT_INFO* textSegmentInfo)
{


	unsigned int newfilesize = injectstub_size + injectfile_encrypted_size ;


	char*buffer = new char[injectfile_encrypted_size];

	for (int i = 0; i < injectfile_encrypted_size; i++)
		buffer[i] = 'T';

	char*newinjectstub = new char[newfilesize];
	memset(newinjectstub, 0, newfilesize);


	leybuf writefile(newinjectstub, newfilesize);
	writefile.WriteBytes(injectstub, textSegmentInfo->fileSegmentOffset + textSegmentInfo->fileSegmentSize);
	
	//start add res

	writefile.WriteBytes((void*)buffer, injectfile_encrypted_size);
	//end add res

	writefile.WriteBytes(injectstub+ textSegmentInfo->fileSegmentOffset + textSegmentInfo->fileSegmentSize, injectstub_size - textSegmentInfo->fileSegmentOffset- textSegmentInfo->fileSegmentSize);

	newfilesize = writefile.GetPos();

	//start write loader

	//end write loader

	//inject stub
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)newinjectstub;
	PIMAGE_NT_HEADERS peHeader = (PIMAGE_NT_HEADERS)(newinjectstub + dosHeader->e_lfanew);
	
	PIMAGE_SECTION_HEADER text = (PIMAGE_SECTION_HEADER)TraverseSectionHeaders(numsections, (unsigned char*)IMAGE_FIRST_SECTION(peHeader), 0, ".text");
	text->SizeOfRawData = textSegmentInfo->fileSegmentSize+ injectfile_encrypted_size;
	text->Misc.VirtualSize = textSegmentInfo->fileSegmentSize + injectfile_encrypted_size;


	printf("ENTRY: %x\nSTART: %x\n", textSegmentInfo->fileSegmentOffset + peHeader->OptionalHeader.AddressOfEntryPoint - textSegmentInfo->memorySegmentOffset, textSegmentInfo->fileSegmentOffset);

	writefile.SetPos(textSegmentInfo->fileSegmentOffset + peHeader->OptionalHeader.AddressOfEntryPoint - textSegmentInfo->memorySegmentOffset);

	writefile.WriteString("\xC3\xC3\xC3\xC3");//shellcode




	FILE* shit = fopen("encrypted.exe", "w+b");
	fwrite(newinjectstub, sizeof(char), newfilesize, shit);

	fclose(shit);
	free(buffer);

	return true;

}


int main(int argc, char** argv)
{

	if (argc <4)
	{
		printf("Args: injectstub injectfile key");
		getchar();
		return 0;
	}


	unsigned int encryptionkey = (unsigned int)atoi(argv[1]);

	char ainjectstub[0xFF];
	strcpy(ainjectstub, argv[2]);

	char ainjectfile[0xFF];
	strcpy(ainjectfile, argv[3]);



	printf("Going to inject %s into %s\nKey: %lu\n", ainjectfile, ainjectstub, encryptionkey);


	int injectstub_size = 0;
	char *injectstub = 0;

	if (!readfile(ainjectstub, &injectstub, &injectstub_size))
	{
		printf("couldn't find file %s", ainjectstub);
		getchar();

		return 0;
	}

	SEGMENT_INFO textSegmentInfo = { 0 };

	//Fill segments information
	getSegmentsInfo(injectstub, &textSegmentInfo, ".text");


	printf("\n\n=======================\n");
	printf("segment information: .text\n");
	printf("RAM image base          =0x%08X\n", textSegmentInfo.moduleBase);
	printf("RAM segment offset      =0x%08X\n", textSegmentInfo.memorySegmentOffset);
	printf("File offset =0x%X\n", textSegmentInfo.fileSegmentOffset);
	printf("File size      =0x%X\n", textSegmentInfo.fileSegmentSize);
	printf("\n\n=======================\n");

	char *injectfile_encrypted = 0;
	int injectfile_encrypted_size = 0;

	if (!encryptfile(encryptionkey, ainjectfile, &injectfile_encrypted, &injectfile_encrypted_size))
	{
		printf("couldn't find file %s", injectstub);
		getchar();

		return 0;
	}


	if (!injectstubcode(encryptionkey, injectfile_encrypted, injectfile_encrypted_size, injectstub, injectstub_size, &textSegmentInfo))
	{
		printf("couldn't inject stub code!");

		getchar();

		return 0;
	}

	printf("injected stub code successfully!\n");
	
	getchar();

	return 0;
}

