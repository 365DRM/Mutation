#pragma once
#include <vector>
using namespace std;

class CPE
{
public:
	CPE();
	~CPE();
public:
	HANDLE					m_hFile;			//PE�ļ����
	LPBYTE					m_pFileBuf;			//PE�ļ�������
	DWORD					m_dwFileSize;		//�ļ���С
	DWORD					m_dwImageSize;		//�����С
	PIMAGE_DOS_HEADER		m_pDosHeader;		//Dosͷ
	PIMAGE_NT_HEADERS		m_pNtHeader;		//NTͷ
	PIMAGE_SECTION_HEADER	m_pSecHeader;		//��һ��SECTION�ṹ��ָ��
	DWORD					m_dwImageBase;		//�����ַ
	DWORD					m_dwCodeBase;		//�����ַ
	DWORD					m_dwCodeSize;		//�����С
	DWORD					m_dwPEOEP;			//OEP��ַ
	DWORD					m_dwShellOEP;		//��OEP��ַ
	DWORD					m_dwSizeOfHeader;	//�ļ�ͷ��С
	DWORD					m_dwSectionNum;		//��������

	DWORD					m_dwFileAlign;		//�ļ�����
	DWORD					m_dwMemAlign;		//�ڴ����

	DWORD					m_IATSectionBase;	//IAT���ڶλ�ַ
	DWORD					m_IATSectionSize;	//IAT���ڶδ�С

	IMAGE_DATA_DIRECTORY	m_PERelocDir;		//�ض�λ����Ϣ
	IMAGE_DATA_DIRECTORY	m_PEImportDir;		//�������Ϣ

	typedef struct _TYPEOFFSET
	{
		WORD offset : 12;						//ƫ��ֵ
		WORD Type : 4;							//�ض�λ����(��ʽ)
	}TYPEOFFSET, *PTYPEOFFSET;
	typedef struct _RelocData
	{
		DWORD RelocAddr;						//���ض�λ�������ڴ��еĵ�ַ
		DWORD Offset;							//���ض�λ���ݵ�ƫ��ֵ	��ע�������ݵ�ƫ��ֵ��
	} RelocData, *PRelocData;
	vector<RelocData> m_RelocData;


public:
	BOOL InitPE(CString strFilePath);			//��ʼ��PE����ȡPE�ļ�������PE��Ϣ
	void InitValue();							//��ʼ������
	BOOL OpenPEFile(CString strFilePath);		//���ļ�
	BOOL IsPE();								//�ж��Ƿ�ΪPE�ļ�
	void GetPEInfo();							//��ȡPE��Ϣ
	

	void MergeBuf(LPBYTE pFileBuf, DWORD pFileBufSize, 
		LPBYTE pShellBuf, DWORD pShellBufSize, 
		LPBYTE& pFinalBuf, DWORD& pFinalBufSize);
												//�ϲ�PE�ļ���Shell

	void AddSize_RelocSection();

	void Find_reloc();							//�Ѽ���Ҫreloc�����ݵ��ڴ��ַ�������ݱ����ƫ��ֵ

	//DWORD Raw_RelocDirsize;						//��ʼֵΪԭʼ���ض�λ���С
	BOOL Add_DataToRelocDir(WORD added_offset, DWORD added_VA);
};

