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
Mutation::~Mutation() {}
void Mutation::InitValue()
{

}
x86Insn_Mutation::x86Insn_Mutation()
{
	InitValue();

	//��Final�ռ�
	if (Final_MutMemory == nullptr)
	{
		Final_MutMemory = VirtualAlloc(NULL, memory_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		FinalMem_Size = memory_size;
		FinalRemainMem_Size = memory_size;
		if (Final_MutMemory == NULL)
			MessageBox(NULL, _T("Final_MutMemory����ռ�ʧ��"), NULL, NULL);
	}
}
x86Insn_Mutation::~x86Insn_Mutation() 
{
	if (Final_MutMemory == nullptr)
		return;

	VirtualFree(Final_MutMemory, 0, MEM_DECOMMIT);
	
}
void x86Insn_Mutation::InitValue()
{
	Final_MutMemory = nullptr;
	FinalMem_Size = 0;
	FinalRemainMem_Size = 0;
	Final_CodeSize = 0;
	SingMut_Sec = { 0 };
}




void Mutation::Start(CString filepath)
{
	//CPE	objPE;
	bool	again_flag = false;
	x86Insn_Mutation code;
	x86Insn_Mutation_again code_again;
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
	//�̳�objPE��Mut_Mark����
	code = *this;
	//code.objPE = this->objPE;								//objPE�ĳ�ʼ�����ʺϷ��ڹ��캯��������ֱ�Ӹ�ֵ��ȥ
	//code.Mut_Mark = this->Mut_Mark;
	code.Start_Mutation(code);									//����дobjMut.Start_Mutation()
	////////////////////////////////////////////////////////////////////////////////////
	//��ʼ���α���
	again_flag = true;
	code_again = code;
	code_again.Start_Mutation(code_again);


	////////////////////////////////////////////////////////////////////////////////////
	//3.�ϲ�PE�ļ��ͱ�����뵽�µĻ�����
	LPBYTE pFinalBuf = nullptr;
	DWORD dwFinalBufSize = 0;
	if(again_flag)
		objPE.MergeBuf(objPE.m_pFileBuf, objPE.m_dwImageSize,
		(LPBYTE)code_again.Final_MutMemory, code_again.Final_CodeSize,
			pFinalBuf, dwFinalBufSize);
	else
		objPE.MergeBuf(objPE.m_pFileBuf, objPE.m_dwImageSize,
		(LPBYTE)code.Final_MutMemory, code.Final_CodeSize,
			pFinalBuf, dwFinalBufSize);
	//4.�����ļ���������ɵĻ�������
	SaveFinalFile(pFinalBuf, dwFinalBufSize, filepath);
	//5.�ͷ���Դ
	VirtualFree(objPE.m_pFileBuf, 0, MEM_DECOMMIT);
	VirtualFree(pFinalBuf, 0, MEM_DECOMMIT);
	//VirtualFree(code.Final_MutMemory, 0, MEM_DECOMMIT);
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
	
	//���ÿһ�δ�����б���
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//start_link
		link_jmp(1, code, objPE, iter->Jmp_Start);
		//�����&&����
		code.Disassemble(iter->Protected_Start, iter->Protected_End, iter->Jmp_Start, iter->Jmp_End);
		//end_link
		link_jmp(0, code, objPE, iter->Jmp_End + strlen((char*)Mutation_End));
		//����öε�ԭ����
		ClearCode(iter->Jmp_Start + 5, iter->Jmp_End + strlen((char*)Mutation_End));
	}
}
//static csh handle;
//���ÿ�δ���
BOOL x86Insn_Mutation::Disassemble(LPBYTE Protected_Start, LPBYTE Protected_End, LPBYTE Jmp_Start, LPBYTE Jmp_End)
{
	uint64_t	address = (uint64_t)Protected_Start;	//��ʼ��ַ
	cs_insn		*insn;									//��������ָ����Ϣ
	Mark		Mark_Struct = { 0 };
	BOOL result = true;
	size_t count;
	cs_err err = cs_open(CS_ARCH_X86, CS_MODE_32, &handle);
	if (err) {
		MessageBox(NULL, _T("Failed on cs_open()"), NULL, NULL);
		abort();
	}

	cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

	//cs_disasm�����ڶ�����������Ҫ������ָ��������0Ϊȫ��
	count = cs_disasm(handle, Protected_Start, Protected_End - Protected_Start, address, 0, &insn);
	if (count) {
		size_t j;
		//���ÿ�δ������
		for (j = 0; j < count; j++) {
			this->handle = handle;
			this->insn = insn[j];
			this->Mutation_SingleCode();
		}
		//���������β��ַ����vector��Protected_End������jmp end��
		if (Mut_Mark_again.empty()) {
			Mark_Struct.Jmp_Start = Jmp_Start;
			Mark_Struct.Jmp_End = Jmp_End;
			Mark_Struct.Protected_Start = (LPBYTE)Final_MutMemory;
			Mark_Struct.Protected_End = (LPBYTE)(size_t)Final_MutMemory + Final_CodeSize;
			Mut_Mark_again.push_back(Mark_Struct);
		}		
		else {
			Mark_Struct.Jmp_Start = Jmp_Start;
			Mark_Struct.Jmp_End = Jmp_End;
			Mark_Struct.Protected_Start = Mut_Mark_again.back().Protected_End + 5;
			Mark_Struct.Protected_End = (LPBYTE)(size_t)Final_MutMemory + Final_CodeSize;
			Mut_Mark_again.push_back(Mark_Struct);
		}

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
		return -1;
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
		if (x86->operands[i].type == X86_OP_REG) {
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

	//1.����CodeSection��vector
	SingMut_Sec = { 0 };
	SingMut_Sec.Raw_CodeAddr = (DWORD)insn.address;
	SingMut_Sec.Mut_CodeStartAddr = (DWORD)Final_MutMemory + Final_CodeSize;
	SingMut_Sec.BaseAddr = objPE.m_dwImageBase + objPE.m_dwImageSize + Final_CodeSize;
	SingMut_Sec.Mut_CodeEndAddr = SingMut_Sec.Mut_CodeStartAddr + insn.size;
	SingMut_Sec.Mut_CodeSize = insn.size;
	SingMut.push_back(SingMut_Sec);
	//2.���Finalʣ���ڴ�װ�����������ָ��
	if (FinalRemainMem_Size < SingMut_Sec.Mut_CodeSize)
	{
		Update_Mem();
	}
	FinalRemainMem_Size -= SingMut_Sec.Mut_CodeSize;
	//3.��δָ֪��copy��ȥ
	memcpy_s((void*)((size_t)Final_MutMemory + Final_CodeSize), insn.size, (void*)insn.address, insn.size);
	//3.1����Final_CodeSize
	Final_CodeSize += insn.size;

	//3.�ض�λ����
	//�����ָ���mem��disp_sizeΪ4���������ض�λ
	///*
	if (mem.disp_size == 4) {
		DealWithReloc((DWORD)insn.address + mem.disp_offset, SingMut_Sec.BaseAddr + mem.disp_offset);
	}
	//���imm��sizeΪ4���������ض�λ
	if (imm.imm_size == 4) {
		DealWithReloc((DWORD)insn.address + imm.imm_offset, SingMut_Sec.BaseAddr + imm.imm_offset);
	}
	//*/
	return true;
}

//������ָ��ı�������ض�λ��д��Final�ռ䣬����дCodeSection�ṹ��
UINT x86Insn_Mutation::Copy_MutCodes_to_FinalMem()
{
	Mut_Code.flatten();
	Mut_Code.resolveUnresolvedLinks();
	SingMut_Sec = { 0 };
	//1.��дԭָ���ַ
	SingMut_Sec.Raw_CodeAddr = (DWORD)insn.address;
	//1.1��д���������ʼ��ַ
	SingMut_Sec.Mut_CodeStartAddr = (DWORD)Final_MutMemory + Final_CodeSize;
	//1.2��д�ض�λ����ַ��ԭ���λ���ַ+�����ɵ��ܱ�������С��
	SingMut_Sec.BaseAddr = objPE.m_dwImageBase + objPE.m_dwImageSize + Final_CodeSize;
	//1.3�����ض�λ
	Mut_Code.relocateToBase((uint64_t)SingMut_Sec.BaseAddr);
	//1.4��д���������С���ض�λ����ȡCodeSize��CodeSize�������ض�λ��仯��
	SingMut_Sec.Mut_CodeSize = Mut_Code.codeSize();
	//1.5��д��������β������һ�������������ʼ����
	SingMut_Sec.Mut_CodeEndAddr = SingMut_Sec.Mut_CodeStartAddr + SingMut_Sec.Mut_CodeSize;
	//1.6���ṹ��д��vector
	SingMut.push_back(SingMut_Sec);


	//2.���Finalʣ���ڴ�װ�����������ָ��
	if (FinalRemainMem_Size < SingMut_Sec.Mut_CodeSize)
	{
		Update_Mem();
	}
	FinalRemainMem_Size -= SingMut_Sec.Mut_CodeSize;
	//3.���������д��Final�ռ�
	Mut_Code.copyFlattenedData((void*)SingMut.back().Mut_CodeStartAddr, SingMut_Sec.Mut_CodeSize, CodeHolder::kCopyWithPadding);
	//3.1����Final_CodeSize
	Final_CodeSize += SingMut_Sec.Mut_CodeSize;

	return true;
}

UINT x86Insn_Mutation::Update_Mem()
{
	//����2����С�Ŀռ䲢��ԭ�ռ����copy����

	void* temp = Final_MutMemory;
	size_t temp_size = FinalMem_Size;
	FinalMem_Size *= 2;
	Final_MutMemory = VirtualAlloc(NULL, FinalMem_Size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (Final_MutMemory == nullptr)
	{
		MessageBox(NULL, _T("Final_MutMemory����ռ�ʧ��"), NULL, NULL);
	}
	memcpy_s(Final_MutMemory, FinalMem_Size, temp, temp_size);
	FinalRemainMem_Size = FinalMem_Size - temp_size;
	VirtualFree(temp, 0, MEM_DECOMMIT);

	//���¼���vector�����Ҫ�ı�ľ��Ե�ַ
	size_t differ = (size_t)Final_MutMemory - (size_t)temp;
	for (auto &c : Mut_Mark_again) {
		c.Protected_Start += differ;
		c.Protected_End += differ;
	}
	for (auto &c : SingMut) {
		c.Mut_CodeStartAddr += differ;
		c.Mut_CodeEndAddr += differ;
	}
	for (auto &c : Fix_Offset) {
		c.address += differ;
	}
	for (auto &c : CA_Fix_Offset) {
		c.Call_Addr += differ;
		c.Add_Addr += differ;
	}

	return true;
}







//Ѱ��Mutation������־
UINT Mutation::Find_MutationMark(LPBYTE pFinalBuf, DWORD size, OUT vector<Mark> *_Out_Mark)
{
	Mark Mark_Struct = { 0 };
	UINT Mark_Sum = 0;
	LPBYTE Start_Addr = 0, End_Addr = 0;
	if (pFinalBuf == NULL) {
		MessageBox(NULL, _T("pFinalBuf�������쳣��"), NULL, NULL);
		return 0;
	}
	for (DWORD Offset = 0; size - Offset > 0; Offset = End_Addr - pFinalBuf + 1)
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
		Mark_Struct.Jmp_Start = Start_Addr;
		Mark_Struct.Jmp_End = End_Addr;
		Mark_Struct.Protected_Start = Start_Addr + strlen((char*)Mutation_Start);
		Mark_Struct.Protected_End = End_Addr;
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
	for (int i = 0; i < size; i++)
		memset(Start_Addr + i, rand() % 256, 1);
}
//�������ļ�
BOOL Mutation::SaveFinalFile(LPBYTE pFinalBuf, DWORD pFinalBufSize, CString strFilePath)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFinalBuf;
	PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)(pFinalBuf + pDosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
	for (DWORD i = 0; i < pNtHeader->FileHeader.NumberOfSections; i++, pSectionHeader++)
	{
		//����������Ϣ�� �ļ������С���ļ������Сͬ�ڴ�����С�����ļ������ַ��ͬ�ϣ�
		DWORD dwTemp = 0;
		dwTemp = (pSectionHeader->Misc.VirtualSize / 0x1000) * 0x1000;
		if (pSectionHeader->Misc.VirtualSize % 0x1000)
		{
			dwTemp += 0x1000;
		}
		pSectionHeader->SizeOfRawData = dwTemp;
		pSectionHeader->PointerToRawData = pSectionHeader->VirtualAddress;

		//����SizeOfCode
		if (pSectionHeader->VirtualAddress == pNtHeader->OptionalHeader.BaseOfCode) {
			pNtHeader->OptionalHeader.SizeOfCode = pSectionHeader->SizeOfRawData;
		}

	}
	//�����ļ������СΪ0x1000
	pNtHeader->OptionalHeader.FileAlignment = 0x1000;
	
	


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

