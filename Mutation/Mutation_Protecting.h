#pragma once
#include <vector>
#include <capstone/capstone.h>
#include "PE.h"
using namespace std;
using namespace asmjit;


typedef struct _imm {
	DWORD address;
	DWORD imm_value;
	uint8_t imm_offset;
	uint8_t imm_size;


}x86_imm, *P_x86_imm;
typedef struct _mem {
	DWORD address;
	uint8_t disp_offset;
	uint8_t disp_size;
	x86_reg base;
	x86_reg index;
	int scale;
	int64_t disp;
	uint8_t mem_size;

}x86_mem, *P_x86_mem;
typedef struct _jcc {
	DWORD address;
	DWORD Target_JumpAddr;
	uint8_t imm_offset;
	uint8_t imm_size;

}x86_jcc, *P_x86_jcc;
typedef struct _jcc_FixOffset
{
	DWORD address;
	DWORD Target_JumpAddr;
	uint8_t imm_offset;

} FixOffset, *P_FixOffset;
//static vector<FixOffset> Fix_Offset;



class Mutation;
class x86Insn_Mutation;
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
		LPBYTE Start;
		LPBYTE End;
	} Mark, *PMark;
	vector<Mark> Mut_Mark;
	CPE	objPE; 


public:
	//�ʼ�ĵط�
	void Start(CString filepath);
	//Ѱ��Mutation��־
	UINT Find_MutationMark(LPBYTE pFinalBuf, DWORD size, OUT vector<Mark> *Mark);
	//��ʼ����
	void Start_Mutation(x86Insn_Mutation& code);
	//jmp������β
	void link_jmp(int flag, x86Insn_Mutation& code, CPE& objPE, LPBYTE Addr);
	//���ԭ����
	void ClearCode(LPBYTE Start_Addr, LPBYTE End_Addr);

	
	
	//�������ռӿǺ���ļ�
	BOOL SaveFinalFile(LPBYTE pFinalBuf, DWORD pFinalBufSize, CString strFilePath);
};

class x86Insn_Mutation: public Mutation
{
public:
	x86Insn_Mutation();
	~x86Insn_Mutation();
	//��ʼ������
	void InitValue();

public:
	csh handle;
	cs_insn insn;
	BOOL again_flag;


	//���ڱ�������ض�λ�Ļ���ַ
	//void* BaseAddress;

	//����ָ��ı������
	CodeHolder Mut_Code;
	//����ָ��ı������Ĵ�С
	//size_t Mut_CodeSize;

	//���б���������ڵ��ڴ�
	void* Final_MutMemory;
	//���б�����������ڴ�Ĵ�С
	size_t FinalMem_Size;
	//���б�����������ڴ��ʣ���С
	size_t FinalRemainMem_Size;

	//���б��������ܴ�С
	size_t Final_CodeSize;


	typedef struct _CodeSection
	{
		//ԭָ���ַ
		DWORD Raw_CodeAddr;
		//����������ʼ��ַ
		DWORD Mut_CodeStartAddr;
		//���������С
		size_t Mut_CodeSize;
		//��������β������һ�������������ʼ����
		DWORD Mut_CodeEndAddr;
		//�ض�λ����ַ
		DWORD BaseAddr;
	} CodeSection, *PCodeSection;
	vector<CodeSection> code_section;
	vector<FixOffset> Fix_Offset;

public:
	//���ÿ�δ�����з����
	BOOL Disassemble(LPBYTE Start_Addr, LPBYTE End_Addr);
	//��Ե���ָ�ʼ����
	UINT Mutation_SingleCode();
	//�ж�ָ������
	UINT Analyze_InsnType();
	//����δ֪��ָ��
	UINT Resolve_UnknownInsn();
	//������ָ��ı������д��Final�ռ�
	UINT Copy_MutCodes_to_FinalMem();
	



	//�ض�λimm��mem(disp)
	BOOL RelocData_imm_mem(DWORD DataAddr, IN OUT x86::Gp base_reg, IN OUT UINT* offset);

	BOOL DealWithReloc(DWORD DataAddr, DWORD NeedtoReloActuAddr);

	//�޸�jmp��offset
	UINT Fix_JmpOffset();
	//ת��jccĿ����ת��ַΪʵ�ʵ�ַ
	UINT Jcc_ActuAddr(DWORD Target_JumpAddr);
	UINT reloc();
	
	

	UINT _mov();
	UINT _mov_reg_reg(x86_reg op0, x86_reg op1);
	UINT _mov_reg_imm(x86_reg op0, x86_imm* imm1);
	UINT _mov_reg_mem(x86_reg op0, x86_mem* mem1);
	UINT _mov_mem_reg(x86_mem* mem0, x86_reg op1);
	UINT _mov_mem_imm(x86_mem* mem0, x86_imm* imm1);
	UINT _mov_reg_reg_16_8(x86_reg op0, x86_reg op1);
	UINT _mov_reg_imm_16_8(x86_reg op0, x86_imm* imm1);
	UINT _mov_reg_mem_16_8(x86_reg op0, x86_mem* mem1);
	UINT _mov_mem_reg_16_8(x86_mem* mem0, x86_reg op1);
	UINT _mov_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1);

	UINT _add();
	UINT _add_reg_reg(x86_reg op0, x86_reg op1);
	UINT _add_reg_imm(x86_reg op0, x86_imm* imm1);
	UINT _add_reg_mem(x86_reg op0, x86_mem* mem1);
	UINT _add_mem_reg(x86_mem* mem0, x86_reg op1);
	UINT _add_mem_imm(x86_mem* mem0, x86_imm* imm1);
	UINT _add_reg_reg_16_8(x86_reg op0, x86_reg op1);
	UINT _add_reg_imm_16_8(x86_reg op0, x86_imm* imm1);
	UINT _add_reg_mem_16_8(x86_reg op0, x86_mem* mem1);
	UINT _add_mem_reg_16_8(x86_mem* mem0, x86_reg op1);
	UINT _add_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1);

	UINT _sub();
	UINT _sub_reg_reg(x86_reg op0, x86_reg op1);
	UINT _sub_reg_imm(x86_reg op0, x86_imm* imm1);
	UINT _sub_reg_mem(x86_reg op0, x86_mem* mem1);
	UINT _sub_mem_reg(x86_mem* mem0, x86_reg op1);
	UINT _sub_mem_imm(x86_mem* mem0, x86_imm* imm1);
	UINT _sub_reg_reg_16_8(x86_reg op0, x86_reg op1);
	UINT _sub_reg_imm_16_8(x86_reg op0, x86_imm* imm1);
	UINT _sub_reg_mem_16_8(x86_reg op0, x86_mem* mem1);
	UINT _sub_mem_reg_16_8(x86_mem* mem0, x86_reg op1);
	UINT _sub_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1);

	UINT _xor();
	UINT _xor_reg_reg(x86_reg op0, x86_reg op1);
	UINT _xor_reg_imm(x86_reg op0, x86_imm* imm1);
	UINT _xor_reg_mem(x86_reg op0, x86_mem* mem1);
	UINT _xor_mem_reg(x86_mem* mem0, x86_reg op1);
	UINT _xor_mem_imm(x86_mem* mem0, x86_imm* imm1);
	UINT _xor_reg_reg_16_8(x86_reg op0, x86_reg op1);
	UINT _xor_reg_imm_16_8(x86_reg op0, x86_imm* imm1);
	UINT _xor_reg_mem_16_8(x86_reg op0, x86_mem* mem1);
	UINT _xor_mem_reg_16_8(x86_mem* mem0, x86_reg op1);
	UINT _xor_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1);

	UINT _and();
	UINT _and_reg_reg(x86_reg op0, x86_reg op1);
	UINT _and_reg_imm(x86_reg op0, x86_imm* imm1);
	UINT _and_reg_mem(x86_reg op0, x86_mem* mem1);
	UINT _and_mem_reg(x86_mem* mem0, x86_reg op1);
	UINT _and_mem_imm(x86_mem* mem0, x86_imm* imm1);
	UINT _and_reg_reg_16_8(x86_reg op0, x86_reg op1);
	UINT _and_reg_imm_16_8(x86_reg op0, x86_imm* imm1);
	UINT _and_reg_mem_16_8(x86_reg op0, x86_mem* mem1);
	UINT _and_mem_reg_16_8(x86_mem* mem0, x86_reg op1);
	UINT _and_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1);

	UINT _or();
	UINT _or_reg_reg(x86_reg op0, x86_reg op1);
	UINT _or_reg_imm(x86_reg op0, x86_imm* imm1);
	UINT _or_reg_mem(x86_reg op0, x86_mem* mem1);
	UINT _or_mem_reg(x86_mem* mem0, x86_reg op1);
	UINT _or_mem_imm(x86_mem* mem0, x86_imm* imm1);
	UINT _or_reg_reg_16_8(x86_reg op0, x86_reg op1);
	UINT _or_reg_imm_16_8(x86_reg op0, x86_imm* imm1);
	UINT _or_reg_mem_16_8(x86_reg op0, x86_mem* mem1);
	UINT _or_mem_reg_16_8(x86_mem* mem0, x86_reg op1);
	UINT _or_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1);

	UINT _rcl();
	UINT _rcl_reg_imm(x86_reg op0, x86_imm* imm1);

	UINT _rcr();
	UINT _rcr_reg_imm(x86_reg op0, x86_imm* imm1);

	UINT _lea();
	UINT _lea_reg_mem(x86_reg op0, x86_mem* mem1);
	UINT _lea_reg_mem_16(x86_reg op0, x86_mem* mem1);

	UINT _cmp();
	UINT _cmp_reg_reg(x86_reg op0, x86_reg op1);
	UINT _cmp_reg_imm(x86_reg op0, x86_imm* imm1);
	UINT _cmp_reg_mem(x86_reg op0, x86_mem* mem1);
	UINT _cmp_mem_reg(x86_mem* mem0, x86_reg op1);
	UINT _cmp_mem_imm(x86_mem* mem0, x86_imm* imm1);
	UINT _cmp_reg_reg_16_8(x86_reg op0, x86_reg op1);
	UINT _cmp_reg_imm_16_8(x86_reg op0, x86_imm* imm1);
	UINT _cmp_reg_mem_16_8(x86_reg op0, x86_mem* mem1);
	UINT _cmp_mem_reg_16_8(x86_mem* mem0, x86_reg op1);
	UINT _cmp_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1);

	UINT _test();
	UINT _test_reg_reg(x86_reg op0, x86_reg op1);
	UINT _test_reg_imm(x86_reg op0, x86_imm* imm1);
	UINT _test_mem_reg(x86_mem* mem0, x86_reg op1);
	UINT _test_mem_imm(x86_mem* mem0, x86_imm* imm1);
	UINT _test_reg_reg_16_8(x86_reg op0, x86_reg op1);
	UINT _test_reg_imm_16_8(x86_reg op0, x86_imm* imm1);
	UINT _test_mem_reg_16_8(x86_mem* mem0, x86_reg op1);
	UINT _test_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1);

	UINT _push();
	UINT _push_reg(x86_reg op0);
	UINT _push_imm(x86_imm* imm0);
	UINT _push_mem(x86_mem* mem0);
	UINT _push_reg_16(x86_reg op0);
	UINT _push_mem_16(x86_mem* mem0);

	UINT _pop();
	UINT _pop_reg(x86_reg op0);
	UINT _pop_mem(x86_mem* mem0);
	UINT _pop_reg_16(x86_reg op0);
	UINT _pop_mem_16(x86_mem* mem0);

	UINT _call();
	UINT _call_reg(x86_reg op0);
	UINT _call_imm(x86_jcc* jcc0);
	UINT _call_mem(x86_mem* mem0);

	UINT _jcc_jmp();
	UINT _jmp_reg(x86_reg op0);
	UINT _jmp_imm(x86_jcc* jcc0);
	UINT _jmp_mem(x86_mem* mem0);

	UINT _je(x86_jcc* jcc0);
	UINT _jne(x86_jcc* jcc0);
	UINT _ja(x86_jcc* jcc0);
	UINT _jae(x86_jcc* jcc0);
	UINT _jb(x86_jcc* jcc0);
	UINT _jbe(x86_jcc* jcc0);
	UINT _jc(x86_jcc* jcc0);
	UINT _jecxz(x86_jcc* jcc0);
	UINT _jg(x86_jcc* jcc0);
	UINT _jge(x86_jcc* jcc0);
	UINT _jl(x86_jcc* jcc0);
	UINT _jle(x86_jcc* jcc0);
	UINT _jna(x86_jcc* jcc0);
	UINT _jnae(x86_jcc* jcc0);
	UINT _jnb(x86_jcc* jcc0);
	UINT _jnbe(x86_jcc* jcc0);
	UINT _jnc(x86_jcc* jcc0);
	UINT _jng(x86_jcc* jcc0);
	UINT _jnge(x86_jcc* jcc0);
	UINT _jnl(x86_jcc* jcc0);
	UINT _jnle(x86_jcc* jcc0);
	UINT _jno(x86_jcc* jcc0);
	UINT _jnp(x86_jcc* jcc0);
	UINT _jns(x86_jcc* jcc0);
	UINT _jnz(x86_jcc* jcc0);
	UINT _jo(x86_jcc* jcc0);
	UINT _jp(x86_jcc* jcc0);
	UINT _jpe(x86_jcc* jcc0);
	UINT _jpo(x86_jcc* jcc0);
	UINT _js(x86_jcc* jcc0);
	UINT _jz(x86_jcc* jcc0);

};

typedef enum {
	mov_reg_reg, mov_reg_imm, mov_reg_mem, mov_mem_reg,
	mov_mem_imm, mov_reg_reg_16_8, mov_reg_imm_16_8,
	mov_reg_mem_16_8, mov_mem_reg_16_8, mov_mem_imm_16_8,

	add_reg_reg, add_reg_imm, add_reg_mem, add_mem_reg,
	add_mem_imm, add_reg_reg_16_8, add_reg_imm_16_8,
	add_reg_mem_16_8, add_mem_reg_16_8, add_mem_imm_16_8,

	sub_reg_reg, sub_reg_imm, sub_reg_mem, sub_mem_reg,
	sub_mem_imm, sub_reg_reg_16_8, sub_reg_imm_16_8,
	sub_reg_mem_16_8, sub_mem_reg_16_8, sub_mem_imm_16_8,

	xor_reg_reg, xor_reg_imm, xor_reg_mem, xor_mem_reg,
	xor_mem_imm, xor_reg_reg_16_8, xor_reg_imm_16_8,
	xor_reg_mem_16_8, xor_mem_reg_16_8, xor_mem_imm_16_8,

	and_reg_reg, and_reg_imm, and_reg_mem, and_mem_reg,
	and_mem_imm, and_reg_reg_16_8, and_reg_imm_16_8,
	and_reg_mem_16_8, and_mem_reg_16_8, and_mem_imm_16_8,

	or_reg_reg, or_reg_imm, or_reg_mem, or_mem_reg,
	or_mem_imm, or_reg_reg_16_8, or_reg_imm_16_8,
	or_reg_mem_16_8, or_mem_reg_16_8, or_mem_imm_16_8,

	rcl_reg_imm, rcr_reg_imm,

	lea_reg_mem, lea_reg_mem_16,

	cmp_reg_reg, cmp_reg_imm, cmp_reg_mem, cmp_mem_reg,
	cmp_mem_imm, cmp_reg_reg_16_8, cmp_reg_imm_16_8,
	cmp_reg_mem_16_8, cmp_mem_reg_16_8, cmp_mem_imm_16_8,

	test_reg_reg, test_reg_imm, test_mem_reg,
	test_mem_imm, test_reg_reg_16_8, test_reg_imm_16_8,
	test_mem_reg_16_8, test_mem_imm_16_8,

	push_reg, push_imm, push_mem, push_reg_16, push_mem_16,
	pop_reg, pop_mem, pop_reg_16, pop_mem_16,

	call_reg, call_imm, call_mem,

	jmp_reg, jmp_imm, jmp_mem,
	je, jne, ja, jae, jb, jbe, jc, jecxz,
	jg, jge, jl, jle, jna, jnae, jnb, jnbe,
	jnc, jng, jnge, jnl, jnle, jno, jnp, jns,
	jnz, jo, jp, jpe, jpo, js, jz,
}x86Insn_Class;

#define memory_size 0x100000	//1MB
#define Unknown_Address 0xFFFFFFFF

