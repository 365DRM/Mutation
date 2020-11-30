#include "pch.h"
#include "PE.h"



CPE::CPE()
{
	InitValue();
}


CPE::~CPE()
{
}

//************************************************************
// ��������:	InitValue
// ����˵��:	��ʼ������
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// �� ��	ֵ:	void
//************************************************************
void CPE::InitValue()
{
	m_hFile				= NULL;
	m_pFileBuf			= NULL;
	m_pDosHeader		= NULL;
	m_pNtHeader			= NULL;
	m_pSecHeader		= NULL;
	m_dwFileSize		= 0;
	m_dwImageSize		= 0;
	m_dwImageBase		= 0;
	m_dwCodeBase		= 0;
	m_dwCodeSize		= 0;
	m_dwPEOEP			= 0;
	m_dwShellOEP		= 0;
	m_dwSizeOfHeader	= 0;
	m_dwSectionNum		= 0;
	m_dwFileAlign		= 0;
	m_dwMemAlign		= 0;
	m_PERelocDir		= { 0 };
	m_PEImportDir		= { 0 };
	m_IATSectionBase	= 0;
	m_IATSectionSize	= 0;

	//Raw_RelocDirsize	= 0;
}

//************************************************************
// ��������:	InitPE
// ����˵��:	��ʼ��PE����ȡPE�ļ�������PE��Ϣ
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// ��	��:	CString strFilePath
// �� ��	ֵ:	BOOL
//************************************************************
BOOL CPE::InitPE(CString strFilePath)
{
	//���ļ�
	if (OpenPEFile(strFilePath) == FALSE)
		return FALSE;

	//��PE���ļ��ֲ���ʽ��ȡ���ڴ�
	m_dwFileSize = GetFileSize(m_hFile, NULL);

	m_pFileBuf = (LPBYTE)VirtualAlloc(NULL, m_dwFileSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//m_pFileBuf = new BYTE[m_dwFileSize];
	DWORD ReadSize = 0;
	if (ReadFile(m_hFile, m_pFileBuf, m_dwFileSize, &ReadSize, NULL) == FALSE)
		return FALSE;
	CloseHandle(m_hFile);
	m_hFile = NULL;

	//�ж��Ƿ�ΪPE�ļ�
	if (IsPE() == FALSE)
		return FALSE;

	//��PE���ڴ�ֲ���ʽ��ȡ���ڴ�
	//���������Сû�ж�������
	m_dwImageSize = m_pNtHeader->OptionalHeader.SizeOfImage;
	m_dwMemAlign = m_pNtHeader->OptionalHeader.SectionAlignment;
	m_dwSizeOfHeader = m_pNtHeader->OptionalHeader.SizeOfHeaders;
	m_dwSectionNum = m_pNtHeader->FileHeader.NumberOfSections;

	if (m_dwImageSize % m_dwMemAlign)
		m_dwImageSize = (m_dwImageSize / m_dwMemAlign + 1) * m_dwMemAlign;
	//��������2���ڴ���Ϊ�˷��������ض�λ���δ�С
	LPBYTE pFileBuf_New = (LPBYTE)VirtualAlloc(NULL, m_dwImageSize * 2, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	memset(pFileBuf_New, 0, m_dwImageSize * 2);
	//�����ļ�ͷ
	memcpy_s(pFileBuf_New, m_dwSizeOfHeader, m_pFileBuf, m_dwSizeOfHeader);
	//��������
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader);
	for (DWORD i = 0; i < m_dwSectionNum; i++, pSectionHeader++)
	{
		memcpy_s(pFileBuf_New + pSectionHeader->VirtualAddress,
			pSectionHeader->SizeOfRawData,
			m_pFileBuf+pSectionHeader->PointerToRawData,
			pSectionHeader->SizeOfRawData);
	}
	VirtualFree(m_pFileBuf, 0, MEM_RELEASE);
	//delete[] m_pFileBuf;
	m_pFileBuf = pFileBuf_New;
	pFileBuf_New = NULL;
	
	//���������ض�λ���δ�С
	AddSize_RelocSection();
	//��ȡPE��Ϣ
	GetPEInfo();
	
	return TRUE;
}

//************************************************************
// ��������:	OpenPEFile
// ����˵��:	���ļ�
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// ��	��:	CString strFilePath
// �� ��	ֵ:	BOOL
//************************************************************
BOOL CPE::OpenPEFile(CString strFilePath)
{
	m_hFile = CreateFile(strFilePath,
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, _T("�����ļ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
		m_hFile = NULL;
		return FALSE;
	}
	return TRUE;
}

//************************************************************
// ��������:	IsPE
// ����˵��:	�ж��Ƿ�ΪPE�ļ�
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// �� ��	ֵ:	BOOL
//************************************************************
BOOL CPE::IsPE()
{
	//�ж��Ƿ�ΪPE�ļ�
	m_pDosHeader = (PIMAGE_DOS_HEADER)m_pFileBuf;
	if (m_pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		//����PE
		MessageBox(NULL, _T("������Ч��PE�ļ���"), _T("��ʾ"), MB_OK);
		VirtualFree(m_pFileBuf, 0, MEM_RELEASE);
		//delete[] m_pFileBuf;
		InitValue();
		return FALSE;
	}
	m_pNtHeader = (PIMAGE_NT_HEADERS)(m_pFileBuf + m_pDosHeader->e_lfanew);
	if (m_pNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		//����PE�ļ�
		MessageBox(NULL, _T("������Ч��PE�ļ���"), _T("��ʾ"), MB_OK);
		VirtualFree(m_pFileBuf, 0, MEM_RELEASE);
		//delete[] m_pFileBuf;
		InitValue();
		return FALSE;
	}
	return TRUE;
}

//************************************************************
// ��������:	GetPEInfo
// ����˵��:	��ȡPE��Ϣ
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// �� ��	ֵ:	void
//************************************************************
void CPE::GetPEInfo()
{
	m_pDosHeader	= (PIMAGE_DOS_HEADER)m_pFileBuf;
	m_pNtHeader		= (PIMAGE_NT_HEADERS)(m_pFileBuf + m_pDosHeader->e_lfanew);

	m_dwFileAlign	= m_pNtHeader->OptionalHeader.FileAlignment;
	m_dwMemAlign	= m_pNtHeader->OptionalHeader.SectionAlignment;
	m_dwImageBase	= m_pNtHeader->OptionalHeader.ImageBase;
	m_dwPEOEP		= m_pNtHeader->OptionalHeader.AddressOfEntryPoint;
	m_dwCodeBase	= m_pNtHeader->OptionalHeader.BaseOfCode;
	m_dwCodeSize	= m_pNtHeader->OptionalHeader.SizeOfCode;
	m_dwSizeOfHeader= m_pNtHeader->OptionalHeader.SizeOfHeaders;
	m_dwSectionNum	= m_pNtHeader->FileHeader.NumberOfSections;
	m_pSecHeader	= IMAGE_FIRST_SECTION(m_pNtHeader);
	m_dwImageSize = m_pNtHeader->OptionalHeader.SizeOfImage;
	if (m_dwImageSize % m_dwMemAlign)
		m_dwImageSize = (m_dwImageSize / m_dwMemAlign + 1) * m_dwMemAlign;
	m_pNtHeader->OptionalHeader.SizeOfImage = m_dwImageSize;

	//�����ض�λĿ¼��Ϣ
	m_PERelocDir = 
		IMAGE_DATA_DIRECTORY(m_pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
	//�Ѽ���Ҫreloc�����ݵ��ڴ��ַ�������ݱ����ƫ��ֵ
	if(m_PERelocDir.VirtualAddress)
		Find_reloc();


	//����IAT��ϢĿ¼��Ϣ
	m_PEImportDir =
		IMAGE_DATA_DIRECTORY(m_pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);

	//��ȡIAT���ڵ����ε���ʼλ�úʹ�С��PS:PE�ﲻ����IAT��ƫ�Ƽ����С�𣿣�
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader);
	for (DWORD i = 0; i < m_dwSectionNum; i++, pSectionHeader++)
	{
		if (m_PEImportDir.VirtualAddress >= pSectionHeader->VirtualAddress&&
			m_PEImportDir.VirtualAddress <= pSectionHeader[1].VirtualAddress)
		{
			//��������ε���ʼ��ַ�ʹ�С
			m_IATSectionBase = pSectionHeader->VirtualAddress;
			m_IATSectionSize = pSectionHeader[1].VirtualAddress - pSectionHeader->VirtualAddress;
			break;
		}
	}
}


//************************************************************
// ��������:	MergeBuf
// ����˵��:	�ϲ�PE�ļ���Shell
// ��	��:	cyxvc
// ʱ	��:	2015/12/25
// ��	��:	LPBYTE pFileBuf
// ��	��:	DWORD pFileBufSize
// ��	��:	LPBYTE pShellBuf
// ��	��:	DWORD pShellBufSize
// ��	��:	LPBYTE & pFinalBuf
// �� ��	ֵ:	void
//************************************************************
void CPE::MergeBuf(LPBYTE pFileBuf, DWORD pFileBufSize,
	LPBYTE pShellBuf, DWORD pShellBufSize, 
	LPBYTE& pFinalBuf, DWORD& pFinalBufSize)
{
	//��ȡ���һ�����ε���Ϣ
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileBuf;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(pFileBuf + pDosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
	PIMAGE_SECTION_HEADER pLastSection =
		&pSectionHeader[pNtHeader->FileHeader.NumberOfSections - 1];

	//1.�޸���������
	pNtHeader->FileHeader.NumberOfSections += 1;

	//2.�༭���α�ͷ�ṹ����Ϣ
	PIMAGE_SECTION_HEADER AddSectionHeader =
		&pSectionHeader[pNtHeader->FileHeader.NumberOfSections - 1];
	memcpy_s(AddSectionHeader->Name, 8, ".Mut", 5);

	//VOffset(1000����)
	DWORD dwTemp = 0;
	dwTemp = (pLastSection->Misc.VirtualSize / m_dwMemAlign) * m_dwMemAlign;
	if (pLastSection->Misc.VirtualSize % m_dwMemAlign)
	{
		dwTemp += 0x1000;
	}
	AddSectionHeader->VirtualAddress = pLastSection->VirtualAddress + dwTemp;

	//Vsize��ʵ����ӵĴ�С��
	AddSectionHeader->Misc.VirtualSize = pShellBufSize;

	//ROffset�����ļ���ĩβ��
	AddSectionHeader->PointerToRawData = pFileBufSize;

	//RSize(200����)
	dwTemp = (pShellBufSize / m_dwFileAlign) * m_dwFileAlign;
	if (pShellBufSize % m_dwFileAlign)
	{
		dwTemp += m_dwFileAlign;
	}
	AddSectionHeader->SizeOfRawData = dwTemp;

	//��־
	AddSectionHeader->Characteristics = 0XE0000040;

	//3.�޸�PEͷ�ļ���С���ԣ������ļ���С
	dwTemp = (pShellBufSize / m_dwMemAlign) * m_dwMemAlign;
	if (pShellBufSize % m_dwMemAlign)
	{
		dwTemp += m_dwMemAlign;
	}
	pNtHeader->OptionalHeader.SizeOfImage += dwTemp;


	//4.����ϲ�����Ҫ�Ŀռ�
	pFinalBuf = (LPBYTE)VirtualAlloc(NULL, pFileBufSize + dwTemp, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//pFinalBuf = new BYTE[pFileBufSize + dwTemp];
	pFinalBufSize = pFileBufSize + dwTemp;
	memset(pFinalBuf, 0, pFileBufSize + dwTemp);
	memcpy_s(pFinalBuf, pFileBufSize, pFileBuf, pFileBufSize);
	memcpy_s(pFinalBuf + pFileBufSize, dwTemp, pShellBuf, dwTemp);
}


//�Ѽ���Ҫreloc�����ݵ��ڴ��ַ�������ݱ����ƫ��ֵ
void CPE::Find_reloc()
{
	//1.��ȡ�ض�λ��ṹ��ָ��
	PIMAGE_BASE_RELOCATION	pPEReloc =
		(PIMAGE_BASE_RELOCATION)(m_pFileBuf + m_PERelocDir.VirtualAddress);

	//2.��ʼ�ض�λ
	while (pPEReloc->VirtualAddress)
	{
		//
		PTYPEOFFSET pTypeOffset = (PTYPEOFFSET)(pPEReloc + 1);
		DWORD dwNumber = (pPEReloc->SizeOfBlock - 8) / 2;
		for (DWORD i = 0; i < dwNumber; i++)
		{
			if (*(PWORD)(&pTypeOffset[i]) == NULL)
				continue;
			//���ض�λ���ݵ�ƫ�Ƶ�ַ����Ի���ַ
			DWORD dwRVA = pTypeOffset[i].offset + pPEReloc->VirtualAddress;
			//���ض�λ���ݵ��ڴ��ַ
			DWORD Addr = (DWORD)m_pFileBuf + dwRVA;
			//���ض�λ����
			DWORD DataOfNeedReloc = *(PDWORD)((DWORD)m_pFileBuf + dwRVA);
			//DataOfNeedReloc = DataOfNeedReloc - m_dwImageBase + (DWORD)m_pFileBuf;
			//���ض�λ���ݵ�ƫ��ֵ
			DWORD OffsetData = DataOfNeedReloc - m_dwImageBase;
			
			
			//д���Ա
			RelocData RelocData_Struct = { 0 };
			RelocData_Struct.RelocAddr = Addr;
			RelocData_Struct.Offset = OffsetData;
			m_RelocData.push_back(RelocData_Struct);
		}
		//2.4��һ������
		pPEReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pPEReloc + pPEReloc->SizeOfBlock);
	}
}


BOOL CPE::Add_DataToRelocDir(WORD added_offset, DWORD added_VA)
{
	//�������д�Ļ���Щ���⣺
	//1.�ض�λ�����д�ĺֱܴ�������aslr��ʧЧ��2.�ڼ��������reloc����������Ǻ�������Σ����ڲ��ȶ����
	bool flag = 1;
	//�ж��Ƿ����ض�λ��
	if (m_PERelocDir.VirtualAddress)
	{
		//1.��ȡ�ض�λ��ṹ��ָ��
		PIMAGE_BASE_RELOCATION	pPEReloc =
			(PIMAGE_BASE_RELOCATION)(m_pFileBuf + m_PERelocDir.VirtualAddress);

		//2.�޸��ض�λ���Size��Ա
		m_pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size += 1 * sizeof(WORD) + 8;
		m_PERelocDir.Size += 1 * sizeof(WORD) + 8;
		/*
		//2.1�ж�Ҫ��Ҫ�����ض�λ���εĿռ䣨virtual size��
		PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader);
		for (DWORD i = 0; i < m_pNtHeader->FileHeader.NumberOfSections; i++, pSectionHeader++)
		{	//�����ǰ�������ض�λ����
			if (pSectionHeader->VirtualAddress == m_PERelocDir.VirtualAddress) {
				//���VirtualSize���ڴ�����С
				DWORD dwTemp = 0;
				dwTemp = (pSectionHeader->Misc.VirtualSize / m_dwMemAlign) * m_dwMemAlign;
				if (pSectionHeader->Misc.VirtualSize % m_dwMemAlign)
				{
					//dwTemp += 0x1000;
					dwTemp += m_dwMemAlign;
				}
				//�ض�λ��.size�Ѿ��������ض�λ���οռ�.virtual size�Ķ����С
				if (m_PERelocDir.Size > dwTemp)
				{
					pSectionHeader->Misc.VirtualSize += m_dwMemAlign;
					pSectionHeader->SizeOfRawData += m_dwMemAlign;
					m_pNtHeader->OptionalHeader.SizeOfImage += m_dwMemAlign;
					m_dwImageSize += m_dwMemAlign;
				}
			}
		}
		*/
		//3.��ȡÿ���ض�λ�飬��pPEReloc�ߵ��յ�
		while (pPEReloc->VirtualAddress)
			//3.1��һ������
			pPEReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pPEReloc + pPEReloc->SizeOfBlock);
		//3.2�������ض�λ��
		pPEReloc->VirtualAddress = added_VA;
		pPEReloc->SizeOfBlock = 1 * sizeof(WORD) + 8;
		//3.3д��block
		PTYPEOFFSET pTypeOffset = (PTYPEOFFSET)(pPEReloc + 1);
		*(PWORD)(&pTypeOffset[0]) = 0x3000 + added_offset;
	}
	else
		flag = 0;
	
	return flag;
}


void CPE::AddSize_RelocSection()
{
	m_pDosHeader = (PIMAGE_DOS_HEADER)m_pFileBuf;
	m_pNtHeader = (PIMAGE_NT_HEADERS)(m_pFileBuf + m_pDosHeader->e_lfanew);
	m_PERelocDir =
		IMAGE_DATA_DIRECTORY(m_pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader);
	

	m_pNtHeader->OptionalHeader.SizeOfImage = 0x1000;
	for (DWORD i = 0; i < m_dwSectionNum; i++, pSectionHeader++)
	{	//�����ǰ���ض�λ���Σ��������һ�����Ρ�
		if (pSectionHeader->VirtualAddress == m_PERelocDir.VirtualAddress && i == (m_dwSectionNum - 1)) 
		{	//�������δ�С��
			pSectionHeader->Misc.VirtualSize += m_PERelocDir.Size * 0x10;
			//��Raw Size
			DWORD dwTemp = 0;
			dwTemp = (pSectionHeader->Misc.VirtualSize / m_dwMemAlign) * m_dwMemAlign;
			if (pSectionHeader->Misc.VirtualSize % m_dwMemAlign)
			{
				//dwTemp += 0x1000;
				dwTemp += m_dwMemAlign;
			}
			//pSectionHeader->SizeOfRawData = dwTemp;
		}
		//��SizeOfImage
		DWORD dwTemp = 0;
		dwTemp = (pSectionHeader->Misc.VirtualSize / m_dwMemAlign) * m_dwMemAlign;
		if (pSectionHeader->Misc.VirtualSize % m_dwMemAlign)
		{
			//dwTemp += 0x1000;
			dwTemp += m_dwMemAlign;
		}
		m_pNtHeader->OptionalHeader.SizeOfImage += dwTemp;
		//������Ҫ��m_dwImageSize�����Ǻ�����õ�GetPEInfo()�Ѿ���������ˡ�
	}
}
