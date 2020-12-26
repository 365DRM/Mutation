#pragma once
#include <vector>
#include <capstone/capstone.h>
#include "PE.h"
#include "define.h"

using namespace std;
using namespace asmjit;

typedef struct _imm {
	//imm Operandר�ýṹ��
	DWORD		address;
	DWORD		imm_value;
	uint8_t		imm_offset;
	uint8_t		imm_size;

}x86_imm, *P_x86_imm;
typedef struct _mem {
	//mem Operandר�ýṹ��
	DWORD		address;
	uint8_t		disp_offset;
	uint8_t		disp_size;
	x86_reg		base;
	x86_reg		index;
	int			scale;
	int64_t		disp;
	uint8_t		mem_size;

}x86_mem, *P_x86_mem;
typedef struct _jcc {
	//jccָ��ר�ýṹ��
	DWORD		address;
	DWORD		Target_JumpAddr;
	uint8_t		imm_offset;
	uint8_t		imm_size;

}x86_jcc, *P_x86_jcc;
typedef struct _jcc_FixOffset
{
	//��struct�����޸�jcc unknown_address��call unknown_address
	//���ɵı������jcc�ĵ�ַ
	DWORD		address;
	//ԭ����jcc��Ŀ����ת��ַ
	DWORD		Target_JumpAddr;
	uint8_t		imm_offset;
} FixOffset, *P_FixOffset;
typedef struct _CallAdd_FixOffset
{
	//�ض�λ���õ�call��addָ������
	//��struct�����ڶ��α��������⴦�������ض�λ��call��addָ��
	DWORD		Call_Addr;
	DWORD		Add_Addr;
	DWORD		FixedOffset;
}CA_FixOffset, *PCA_FixOffset;
//static vector<FixOffset> Fix_Offset;


class Mutation;
class x86Insn_Mutation;
class x86Insn_Mutation_again;
class Mutation
{
public:
	Mutation();
	~Mutation();
	//��ʼ������
	void InitValue();

public:
	//��־�в�Ҫ��\x00
#define Mutation_Start		"\xEB\x0C\x4C\x58\x5F\x4D\x75\x74\x5F\x53\x74\x61\x72\x74"
#define Mutation_End		"\xEB\x0A\x4C\x58\x5F\x4D\x75\x74\x5F\x45\x6E\x64"
	typedef struct _Mark
	{
		LPBYTE Jmp_Start;
		LPBYTE Protected_Start;
		LPBYTE Protected_End;
		LPBYTE Jmp_End;
	} Mark, *PMark;
	vector<Mark>	Mut_Mark;
	CPE	objPE;


public:
	//�ʼ�ĵط�
	void	Start(CString filepath);
	//Ѱ��Mutation��־
	UINT	Find_MutationMark(LPBYTE pFinalBuf, DWORD size, OUT vector<Mark> *Mark);
	//��ʼ����
	void	Start_Mutation(x86Insn_Mutation& code);
	//jmp������β
	void	link_jmp(int flag, x86Insn_Mutation& code, CPE& objPE, LPBYTE Addr);
	//���ԭ����
	void	ClearCode(LPBYTE Start_Addr, LPBYTE End_Addr);



	//�������ռӿǺ���ļ�
	BOOL SaveFinalFile(LPBYTE pFinalBuf, DWORD pFinalBufSize, CString strFilePath);
};

class x86Insn_Mutation : public Mutation
{
public:
	x86Insn_Mutation();
	~x86Insn_Mutation();
	//��ʼ������
	void InitValue();

public:
	csh			handle;
	cs_insn		insn;
	vector<Mark>Mut_Mark_again;

	//���ڱ�������ض�λ�Ļ���ַ
	//void* BaseAddress;

	//����ָ��ı������
	CodeHolder	Mut_Code;
	//����ָ��ı������Ĵ�С
	//size_t Mut_CodeSize;

	//���б���������ڵ��ڴ�
	void*		Final_MutMemory;
	//���б�����������ڴ�Ĵ�С
	size_t		FinalMem_Size;
	//���б�����������ڴ��ʣ���С
	size_t		FinalRemainMem_Size;

	//���б��������ܴ�С
	size_t Final_CodeSize;


	typedef struct _Single_MutCode
	{
		//ԭָ���ַ
		DWORD		Raw_CodeAddr;
		//����������ʼ��ַ
		DWORD		Mut_CodeStartAddr;
		//��������ƫ�Ƶ�ַ
		DWORD		Mut_CodeOffsetAddr;
		//���������С
		size_t		Mut_CodeSize;
		//��������β������һ�������������ʼ����
		DWORD		Mut_CodeEndAddr;
		//�ض�λ����ַ
		DWORD		BaseAddr;
	} Single_MutCode, *PSingle_MutCode;
	Single_MutCode SingMut_Sec;
	vector<Single_MutCode> SingMut;
	vector<FixOffset> Fix_Offset;
	vector<CA_FixOffset> CA_Fix_Offset;

	//�̳г�Ա����
	x86Insn_Mutation& operator=(const Mutation& Mut) {
		objPE = Mut.objPE;
		Mut_Mark = Mut.Mut_Mark;
		return *this;
	}
public:
	//���ÿ�δ�����з����
	virtual BOOL	Disassemble(LPBYTE Protected_Start, LPBYTE Protected_End, LPBYTE Jmp_Start, LPBYTE Jmp_End);
	//��Ե���ָ�ʼ����
	UINT	Mutation_SingleCode();
	//�ж�ָ������
	UINT	Analyze_InsnType();
	//����δ֪��ָ��
	UINT	Resolve_UnknownInsn();
	//������ָ��ı������д��Final�ռ�
	UINT	Copy_MutCodes_to_FinalMem();


	//�ض�λimm��mem(disp)
	BOOL	RelocData_imm_mem(DWORD DataAddr, IN OUT x86::Gp base_reg, IN OUT UINT* offset);
	//�޸�jmp��offset
	UINT	Fix_JmpOffset();
	//�����ڴ�����
	UINT	Update_Mem();
	//�ض�λ����
	virtual BOOL	DealWithReloc(DWORD DataAddr, DWORD NeedtoReloActuAddr);
	//ת��jccĿ����ת��ַΪʵ�ʵ�ַ
	virtual UINT	Jcc_ActuAddr(DWORD Target_JumpAddr);
	
	x86_MutationRule
	x86Insn_Class;
};

class x86Insn_Mutation_again : public x86Insn_Mutation
{
public:
	//��д����
	UINT	Jcc_ActuAddr(DWORD Target_JumpAddr);
	BOOL	DealWithReloc(DWORD DataAddr, DWORD NeedtoReloActuAddr);
	UINT	_call();
	UINT	_add();
public:
	void* old_Final_MutMemory;
	vector<CA_FixOffset> old_Fix_Offset;
	
	//�̳г�Ա����
	x86Insn_Mutation_again& operator=(const x86Insn_Mutation& code) {
		//FinalMem_Size = code.FinalMem_Size;
		//FinalRemainMem_Size = code.FinalRemainMem_Size;
		//Final_CodeSize = code.Final_CodeSize;
		//SingMut = code.SingMut;
		old_Final_MutMemory = code.Final_MutMemory;
		old_Fix_Offset = code.CA_Fix_Offset;
		objPE = code.objPE;
	//	Fix_Offset = code.Fix_Offset;
		Mut_Mark = code.Mut_Mark_again;
		return *this;
	}
	x86Insn_Mutation_again& operator=(const x86Insn_Mutation_again& code) {
		old_Final_MutMemory = code.Final_MutMemory;
		old_Fix_Offset = code.CA_Fix_Offset;
		objPE = code.objPE;
		Mut_Mark = code.Mut_Mark_again;
		return *this;
	}
};

#define memory_size 0x100000	//1MB
#define Unknown_Address 0xFFFFFFFF

