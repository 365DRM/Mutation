#include "pch.h"
#include "Mutation_Protecting.h"
#include "auxiliary_function.h"
#ifdef _DEBUG
#pragma comment(lib, "asmjit_debug.lib")
#else
#pragma comment(lib, "asmjit.lib")
#endif 

Mutation::Mutation()
{
	InitValue();
}
Mutation::~Mutation(){}
void Mutation::InitValue()
{
	
}
x86Insn_Mutation::x86Insn_Mutation()
{
	InitValue();
}
x86Insn_Mutation::~x86Insn_Mutation() {}
void x86Insn_Mutation::InitValue()
{
	Final_MutMemory = NULL;
	FinalMem_Size = 0;
	FinalRemainMem_Size = 0;
	Final_CodeSize = 0;
	again_flag = false;
}




void Mutation::Start(CString filepath)
{
	//CPE	objPE;
	x86Insn_Mutation code;
	vector<Mark> Mark;
	
	if (filepath.IsEmpty()) {
		MessageBox(NULL, _T("δ�����ļ�·����"), NULL, NULL);
		return;
	}
	if (objPE.InitPE(filepath) == false) {
		MessageBox(NULL, _T("InitPEʧ�ܣ�"), NULL, NULL);
		return;
	}
	//��ʱ�ļ��Ѷ����ڴ������ڴ����
	//1.Ѱ��Mutation������־
	if (Find_MutationMark(objPE.m_pFileBuf, objPE.m_dwImageSize, &Mark) == 0) {
		MessageBox(NULL, _T("δ�ҵ�Mutation������־��"), NULL, NULL);
		VirtualFree(objPE.m_pFileBuf, 0, MEM_DECOMMIT);
		//delete[] objPE.m_pFileBuf;
		return;
	}
	
	//2.��ʼ���б���
	code.objPE = this->objPE;								//objPE�ĳ�ʼ�����ʺϷ��ڹ��캯��������ֱ�Ӹ�ֵ��ȥ
	code.Mut_Mark = this->Mut_Mark;
	Start_Mutation(code);									//����дobjMut.Start_Mutation()

	//3.�ϲ�PE�ļ��ͱ�����뵽�µĻ�����
	LPBYTE pFinalBuf = NULL;
	DWORD dwFinalBufSize = 0;
	objPE.MergeBuf(objPE.m_pFileBuf, objPE.m_dwImageSize,
		(LPBYTE)code.Final_MutMemory, code.Final_CodeSize,
		pFinalBuf, dwFinalBufSize);
	//4.�����ļ���������ɵĻ�������
	SaveFinalFile(pFinalBuf, dwFinalBufSize, filepath);
	//5.�ͷ���Դ
	VirtualFree(objPE.m_pFileBuf, 0, MEM_DECOMMIT);
	VirtualFree(pFinalBuf, 0, MEM_DECOMMIT);
	VirtualFree(code.Final_MutMemory, 0, MEM_DECOMMIT);
	//delete[] objPE.m_pFileBuf;
	//delete[] pFinalBuf;
	//free(code.Final_MutMemory);
}
//������жεĴ�����б���
void Mutation::Start_Mutation(x86Insn_Mutation& code)
{
	//����һ�����������
	srand((unsigned)time(NULL));


	//��ʼ���ض�λ��ַ
	//code.CS_Struct.Mut_CodeStartAddr = (objPE.m_dwImageBase + objPE.m_dwImageSize);
	//��Final�ռ�
	if (code.Final_MutMemory == NULL)
	{
		code.Final_MutMemory = VirtualAlloc(NULL, memory_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		code.FinalMem_Size = memory_size;
		code.FinalRemainMem_Size = memory_size;
		if (code.Final_MutMemory == NULL)
			MessageBox(NULL, _T("Final_MutMemory����ռ�ʧ��"), NULL, NULL);
	}
	//���ÿһ�δ�����б���
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//start_link
		link_jmp(1, code, objPE, iter->Start);
		//�����&&����
		code.Disassemble(iter->Start + strlen((char*)Mutation_Start), iter->End);
		//end_link
		link_jmp(0, code, objPE, iter->End + strlen((char*)Mutation_End));
		//����öε�ԭ����
		ClearCode(iter->Start + 5, iter->End + strlen((char*)Mutation_End));
	}
}
static csh handle;
//���ÿ�δ���
BOOL x86Insn_Mutation ::Disassemble(LPBYTE Start_Addr, LPBYTE End_Addr)
{
	uint64_t address = (uint64_t)Start_Addr;		//��ʼ��ַ
	cs_insn *insn;									//��������ָ����Ϣ
	BOOL result = true;
	size_t count;
	cs_err err = cs_open(CS_ARCH_X86, CS_MODE_32, &handle);
	if (err) {
		MessageBox(NULL, _T("Failed on cs_open()"), NULL, NULL);
		printf("Failed on cs_open() with error returned: %u\n", err);
		abort();
	}

	cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

	//cs_disasm�����ڶ�����������Ҫ������ָ��������0Ϊȫ��
	count = cs_disasm(handle, Start_Addr, End_Addr-Start_Addr, address, 0, &insn);
	if (count) {
		size_t j;
		
		for (j = 0; j < count; j++) {
			//x86Insn_Mutation code;
			this->handle = handle;
			this->insn = insn[j];
			this->Mutation_SingleCode();
		}

		// free memory allocated by cs_disasm()
		cs_free(insn, count);
	}
	else {
		printf("ERROR: Failed to disasm given code!\n");
		abort();
		result = false;
	}

	cs_close(&handle);
	return result;
}
//��Ե���ָ��
UINT x86Insn_Mutation::Mutation_SingleCode()
{
	UINT result = -1;
	Mut_Code.init(CodeInfo(ArchInfo::kIdHost));
	//1.����ǰ���ж� �ô����ַ�Ƿ�Ϊjmp��Ŀ����ת��ַ
	Fix_JmpOffset();
	//1.����ָ�����ͣ����ɱ������
	result = Analyze_InsnType();

	//���ϲ��ܱ����ָ�ֱ��copy��ȥ
	if (result == -1)
	{
		Resolve_UnknownInsn();
		return result;
	}
	//2.������ָ��ı�������ض�λ��д��Final�ռ䣬����дCodeSection�ṹ��
	Copy_MutCodes_to_FinalMem();
	
	//3.�����δ���CodeHolder�Ĵ���
	Mut_Code.reset();

	return result;
}
//�ж�ָ������
UINT x86Insn_Mutation::Analyze_InsnType()
{
	cs_x86 *x86;
	if (insn.detail == NULL)
		return -1;
	x86 = &(insn.detail->x86);
	//1.���ж�reg��mem����û��esp��sp�Ĵ�������������������������죬ֱ������Resolve_UnknownInsn()����
	for (int i = 0; i < x86->op_count; i++) {
		//��ǰop_typeΪreg
		if (x86->operands[i].type == X86_OP_REG ) {
			if (x86->operands[i].reg == X86_REG_ESP || x86->operands[i].reg == X86_REG_SP)
				return -1;
		}
		//��ǰop_typeΪmem
		if (x86->operands[i].type == X86_OP_MEM) {
			if (x86->operands[i].mem.base == X86_REG_ESP || x86->operands[i].mem.base == X86_REG_SP)
				return -1;
			if (x86->operands[i].mem.index == X86_REG_ESP || x86->operands[i].mem.index == X86_REG_SP)
				return -1;
		}
	}

	//2.
	//�ж��ǲ���movָ��
	if (strcmp(insn.mnemonic, "mov") == 0)
		return(_mov());
	//�ж��ǲ���addָ��
	if (strcmp(insn.mnemonic, "add") == 0)
		return(_add());
	//�ж��ǲ���subָ��
	if (strcmp(insn.mnemonic, "sub") == 0)
		return(_sub());
	//�ж��ǲ���xorָ��
	if (strcmp(insn.mnemonic, "xor") == 0)
		return(_xor());
	//�ж��ǲ���andָ��
	if (strcmp(insn.mnemonic, "and") == 0)
		return(_and());
	//�ж��ǲ���orָ��
	if (strcmp(insn.mnemonic, "or") == 0)
		return(_or());
	//�ж��ǲ���rclָ��
	if (strcmp(insn.mnemonic, "rcl") == 0)
		return(_rcl());
	//�ж��ǲ���rcrָ��
	if (strcmp(insn.mnemonic, "rcr") == 0)
		return(_rcr());
	//�ж��ǲ���leaָ��
	if (strcmp(insn.mnemonic, "lea") == 0)
		return(_lea());
	//�ж��ǲ���cmpָ��
	if (strcmp(insn.mnemonic, "cmp") == 0)
		return(_cmp());
	//�ж��ǲ���testָ��
	if (strcmp(insn.mnemonic, "test") == 0)
		return(_test());
	//�ж��ǲ���pushָ��
	if (strcmp(insn.mnemonic, "push") == 0)
		return(_push());
	//�ж��ǲ���popָ��
	if (strcmp(insn.mnemonic, "pop") == 0)
		return(_pop());
	//�ж��ǲ���jcc��jmpָ��
	if (cs_insn_group(handle, &insn, CS_GRP_JUMP) == true)
		return(_jcc_jmp());
	if (cs_insn_group(handle, &insn, CS_GRP_CALL) == true)
		return(_call());


	return -1;
}

//����δ֪��ָ��
UINT x86Insn_Mutation::Resolve_UnknownInsn()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };
	mem.disp_offset = x86->encoding.disp_offset;
	mem.disp_size = x86->encoding.disp_size;
	imm.imm_offset = x86->encoding.imm_offset;
	imm.imm_size = x86->encoding.imm_size;

	//1.��δָ֪��copy��ȥ
	memcpy_s((void*)((size_t)Final_MutMemory + Final_CodeSize), insn.size, (void*)insn.address, insn.size);
	//2.����CodeSection��vector
	CodeSection CS_Struct = { 0 };
	CS_Struct.Raw_CodeAddr = (DWORD)insn.address;
	CS_Struct.Mut_CodeStartAddr = (DWORD)Final_MutMemory + Final_CodeSize;
	CS_Struct.BaseAddr = objPE.m_dwImageBase + objPE.m_dwImageSize + Final_CodeSize;
	CS_Struct.Mut_CodeEndAddr = CS_Struct.Mut_CodeStartAddr + insn.size;
	CS_Struct.Mut_CodeSize = insn.size;
	code_section.push_back(CS_Struct);
	Final_CodeSize += insn.size;	
	//3.�ض�λ����
	//�����ָ���mem��disp_sizeΪ4���������ض�λ
	///*
	if (mem.disp_size == 4) {
		DealWithReloc((DWORD)insn.address + mem.disp_offset, CS_Struct.BaseAddr + mem.disp_offset);
	}
	//���imm��sizeΪ4���������ض�λ
	if (imm.imm_size == 4) {
		DealWithReloc((DWORD)insn.address + imm.imm_offset, CS_Struct.BaseAddr + imm.imm_offset);
	}
	//*/
	return true;
}

//������ָ��ı�������ض�λ��д��Final�ռ䣬����дCodeSection�ṹ��
UINT x86Insn_Mutation::Copy_MutCodes_to_FinalMem()
{
	Mut_Code.flatten();
	Mut_Code.resolveUnresolvedLinks();
	CodeSection CS_Struct = { 0 };
	//1.��дԭָ���ַ
	CS_Struct.Raw_CodeAddr = (DWORD)insn.address;
	//1.1��д���������ʼ��ַ
	CS_Struct.Mut_CodeStartAddr = (DWORD)Final_MutMemory + Final_CodeSize;
	//1.2��д�ض�λ����ַ��ԭ���λ���ַ+�����ɵ��ܱ�������С��
	CS_Struct.BaseAddr = objPE.m_dwImageBase + objPE.m_dwImageSize + Final_CodeSize;
	//1.3�����ض�λ
	Mut_Code.relocateToBase((uint64_t)CS_Struct.BaseAddr);
	//1.4��д���������С���ض�λ����ȡCodeSize��CodeSize�������ض�λ��仯��
	CS_Struct.Mut_CodeSize = Mut_Code.codeSize();					
	//1.5��д��������β������һ�������������ʼ����
	CS_Struct.Mut_CodeEndAddr = CS_Struct.Mut_CodeStartAddr + CS_Struct.Mut_CodeSize;
	//1.6���ṹ��д��vector
	code_section.push_back(CS_Struct);


	//2.����Final�ռ乻����װ���������ָ��
	FinalRemainMem_Size -= CS_Struct.Mut_CodeSize;
	if (FinalRemainMem_Size < 0)
	{
		//����2����С�Ŀռ䲢��ԭ�ռ����copy����
		void* temp = Final_MutMemory;
		size_t temp_size = FinalMem_Size;
		FinalMem_Size *= 2;
		Final_MutMemory = VirtualAlloc(NULL, FinalMem_Size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (Final_MutMemory == NULL)
			MessageBox(NULL, _T("Final_MutMemory����ռ�ʧ��"), NULL, NULL);
		memcpy_s(Final_MutMemory, FinalMem_Size, temp, temp_size);
		VirtualFree(temp, 0, MEM_DECOMMIT);
	}
	//3.���������д��Final�ռ�
	Mut_Code.copyFlattenedData((void*)CS_Struct.Mut_CodeStartAddr, CS_Struct.Mut_CodeSize, CodeHolder::kCopyWithPadding);
	//3.1����Final_CodeSize
	Final_CodeSize += CS_Struct.Mut_CodeSize;
	
	return true;
}







//Ѱ��Mutation������־
UINT Mutation::Find_MutationMark(LPBYTE pFinalBuf, DWORD size, OUT vector<Mark> *_Out_Mark)
{
	Mark Mark_Struct = {0};
	UINT Mark_Sum = 0;
	LPBYTE Start_Addr = 0, End_Addr = 0;
	if (pFinalBuf == NULL) {
		MessageBox(NULL, _T("pFinalBuf�������쳣��"), NULL, NULL);
		return 0;
	}
	for (DWORD Offset = 0; size - Offset > 0 ; Offset = End_Addr - pFinalBuf + 1)
	{
		Start_Addr = Find_MemoryString(pFinalBuf + Offset, size - Offset, (LPBYTE)Mutation_Start);
		End_Addr = Find_MemoryString(pFinalBuf + Offset, size - Offset, (LPBYTE)Mutation_End);
		if (Start_Addr == NULL || End_Addr == NULL) {
			return Mark_Sum;
		}
		if (Start_Addr > End_Addr) {
			MessageBox(NULL, _T("���ִ����SDK������SDK��־"), NULL, NULL);
			return Mark_Sum;
		}
		Mark_Struct.Start = Start_Addr;
		Mark_Struct.End = End_Addr;
		_Out_Mark->push_back(Mark_Struct);
		Mut_Mark.push_back(Mark_Struct);
		Mark_Sum++;
	}
	return Mark_Sum;
}
//jmp������β�� 1 = Start, 0 = End
void Mutation::link_jmp(int flag, x86Insn_Mutation& code, CPE& objPE, LPBYTE Addr)
{	//ֻҪ֪������һ����ַ��ƫ�Ƽ���
	//	start_link��������
	if (flag == 1)
	{
		//ƫ�� =   ���龵���ڴ� - Addr + CodeSize - 5
		DWORD data = (DWORD)objPE.m_pFileBuf + objPE.m_dwImageSize - (DWORD)Addr + code.Final_CodeSize - 5;
		memcpy_s(Addr, 1, "\xE9", 1);
		memcpy_s(Addr + 1, 4, &data, 4);
	}
	else
	//	end_link������������������˴��룬��Ҫ�޸�һЩ��Ա����
	{
		//ƫ�� = -(���龵���ڴ� - Addr + CodeSize) - 5
		DWORD data = (DWORD)Addr - (DWORD)objPE.m_pFileBuf - objPE.m_dwImageSize - code.Final_CodeSize - 5;
		memcpy_s((void*)((size_t)code.Final_MutMemory + code.Final_CodeSize), 1, "\xE9", 1);
		memcpy_s((void*)((size_t)code.Final_MutMemory + code.Final_CodeSize + 1), 4, &data, 4);
		code.FinalRemainMem_Size -= 5;
		code.Final_CodeSize += 5;
	}
}
//���ԭ����
void Mutation::ClearCode(LPBYTE Start_Addr, LPBYTE End_Addr)
{
	size_t size = End_Addr - Start_Addr;
	memset(Start_Addr,0,size);
}
//�������ļ�
BOOL Mutation::SaveFinalFile(LPBYTE pFinalBuf, DWORD pFinalBufSize, CString strFilePath)
{
	//����������Ϣ�� �ļ������С���ļ������Сͬ�ڴ�����С��
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFinalBuf;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(pFinalBuf + pDosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
	for (DWORD i = 0; i < pNtHeader->FileHeader.NumberOfSections; i++, pSectionHeader++)
	{
		pSectionHeader->PointerToRawData = pSectionHeader->VirtualAddress;
	}


	//��ȡ����·��
	TCHAR strOutputPath[MAX_PATH] = { 0 };
	LPWSTR strSuffix = PathFindExtension(strFilePath);
	wcsncpy_s(strOutputPath, MAX_PATH, strFilePath, wcslen(strFilePath));
	PathRemoveExtension(strOutputPath);
	wcscat_s(strOutputPath, MAX_PATH, L"_Mut");
	wcscat_s(strOutputPath, MAX_PATH, strSuffix);

	//�����ļ�
	HANDLE hNewFile = CreateFile(
		strOutputPath,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hNewFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, _T("�����ļ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
		return FALSE;
	}
	DWORD WriteSize = 0;
	BOOL bRes = WriteFile(hNewFile, pFinalBuf, pFinalBufSize, &WriteSize, NULL);
	if (bRes)
	{
		CloseHandle(hNewFile);
		return TRUE;
	}
	else
	{
		CloseHandle(hNewFile);
		MessageBox(NULL, _T("�����ļ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
		return FALSE;
	}
}


UINT x86Insn_Mutation::reloc()
{
	if (cs_insn_group(handle, &insn, CS_GRP_CALL) == true)
	{

	}
	if (cs_insn_group(handle, &insn, CS_GRP_JUMP) == true)
	{

	}
	return 0;
}

