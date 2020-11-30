#include "pch.h"
#include "rand_order.h"
#define head_maxsize 6
#define tail_maxsize 16
/*
* ������һ��������ڱ������֮�󣬲��ܵ���������
*/


BOOL rand_order::Disassemble(LPBYTE Protected_Start, LPBYTE Protected_End, LPBYTE Jmp_Start, LPBYTE Jmp_End)
{
	uint64_t	address = (uint64_t)Protected_Start;	//��ʼ��ַ
	cs_insn*	insn;									//��������ָ����Ϣ
	size_t		j = 0;
	size_t		order_sum = 0;
	Mark		Mark_Struct = { 0 };
	BOOL		result = true;
	size_t		count;

	cs_err err = cs_open(CS_ARCH_X86, CS_MODE_32, &handle);
	if (err) {
		MessageBox(NULL, _T("Failed on cs_open()"), NULL, NULL);
		abort();
	}

	cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

	//cs_disasm�����ڶ�����������Ҫ������ָ��������0Ϊȫ��
	count = cs_disasm(handle, Protected_Start, Protected_End - Protected_Start, address, 0, &insn);
	if (count) {
		//���ÿ�δ������
		firstcode_flag = true;
		phead_mem = nullptr;
		ptail_mem = nullptr;
		while (j != count) {
			if (count - j <= 200)
				order_sum = count - j;
			else
				order_sum = rand() % 200 + 1;
			Ordered_Insns.index = j;
			Ordered_Insns.nums = order_sum;
			Ordered_Insns.order_insn = insn;
			this->handle = handle;
			this->Order_ManyCode();
			firstcode_flag = false;
			j += order_sum;
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


//���һ��ָ�����������
UINT rand_order::Order_ManyCode()
{
	DWORD	Ord_CodeHeadAddr;
	size_t	Body_Codesize = 0;
	size_t	body_maxsize = 0;
	size_t	Total_MaxCodesize = 0;
	size_t	MadeCodesize = 0;
	//���������εĵ�һ�д���
	if (firstcode_flag == true) {
		Ord_CodeHeadAddr = (DWORD)Final_MutMemory + Final_CodeSize;
	}
	else {
		for (size_t i = 0; i < Ordered_Insns.nums; i++)
			body_maxsize += Ordered_Insns.order_insn[Ordered_Insns.index + i].size;
		Total_MaxCodesize = head_maxsize + body_maxsize + tail_maxsize;
		Ord_CodeHeadAddr = (DWORD)GetTargetAddress(Total_MaxCodesize);
	}

	//2.����&��������ָ��
	//�������
	MadeCodesize = MakeOrderHead(Ord_CodeHeadAddr);

	//���������
	for (size_t i = 0; i < Ordered_Insns.nums; i++)
	{
		this->insn = Ordered_Insns.order_insn[Ordered_Insns.index + i];
		DWORD Body_CodeStartAddr = Ord_CodeHeadAddr + MadeCodesize;
		Body_Codesize = MakeOrderBody(Body_CodeStartAddr);
		MadeCodesize += Body_Codesize;
	}
	if (Body_Codesize > body_maxsize) {
		MessageBox(NULL, _T("����������С����2"), NULL, NULL);
	}

	//�����β
	DWORD Tail_CodeStartAddr = Ord_CodeHeadAddr + MadeCodesize;
	MadeCodesize += MakeOrderTail(Tail_CodeStartAddr);
	if (MadeCodesize > Total_MaxCodesize) {
		MessageBox(NULL, _T("ʵ�����ɵĴ����С����Ԥ��Ĵ����С"), NULL, NULL);
	}
	
	//Copy_OrdCodes_to_FinalMem(copy_flag, Total_MaxCodesize);
	return true;
}

size_t rand_order::MakeOrderHead(DWORD CodeStartAddr)
{
	Mut_Code.init(CodeInfo(ArchInfo::kIdHost));
	size_t codesize = 0;
	Order_FixJcc lastjcc = { 0 };
	x86::Assembler a(&Mut_Code);

	if (firstcode_flag == false) {
		//������һ������ε�call/jns/jnp��offsetdata
		lastjcc = Order_FixOffset;
		DWORD jcc_offset = CodeStartAddr - lastjcc.imm_offset - 4 - lastjcc.address;
		memcpy_s((void*)(lastjcc.address + lastjcc.imm_offset), 4, &jcc_offset, 4);
		//�ָ�����һ�������jns/jnp���ı��eflags����call���ı��ջƽ��
		if (lastjcc.imm_offset == 2)
			a.popfd();
		if (lastjcc.imm_offset == 1)
			a.add(x86::esp, 4);

		codesize = Mut_Code.codeSize();
		Mut_Code.flatten();
		Mut_Code.resolveUnresolvedLinks();
		DWORD	CodeOffsetAddr = CodeStartAddr - (DWORD)Final_MutMemory;
		uint64_t BaseAddr = (uint64_t)objPE.m_dwImageBase + objPE.m_dwImageSize + CodeOffsetAddr;
		Mut_Code.relocateToBase(BaseAddr);
		Mut_Code.copyFlattenedData((void*)CodeStartAddr, codesize, CodeHolder::kCopyWithPadding);
	}

	codesize = Mut_Code.codeSize();
	if (codesize > head_maxsize) {
		MessageBox(NULL, _T("�����ͷ����С����"), NULL, NULL);
	}
	Mut_Code.reset();
	return codesize;

}

size_t rand_order::MakeOrderBody(DWORD CodeStartAddr)
{
	Mut_Code.init(CodeInfo(ArchInfo::kIdHost));
	cs_x86* x86;
	if (insn.detail == NULL)
		return -1;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };
	mem.disp_offset = x86->encoding.disp_offset;
	mem.disp_size = x86->encoding.disp_size;
	imm.imm_offset = x86->encoding.imm_offset;
	imm.imm_size = x86->encoding.imm_size;
	size_t codesize = 0;
	BOOL RelocCheck_flag = false;
	SingMut_Sec = { 0 };

	
	SingMut_Sec.Mut_CodeStartAddr = CodeStartAddr;
	SingMut_Sec.Mut_CodeOffsetAddr = SingMut_Sec.Mut_CodeStartAddr - (DWORD)Final_MutMemory;
	//1.���ж� �ô����ַ�Ƿ�Ϊjcc��Ŀ����ת��ַ
	Fix_JmpOffset();
	//2.���⴦��jcc��jmp��call��addָ�����ָ��������������ֱ��copy����
	//�������ɵ�ָ���С���Ϊ6
	//�ж��ǲ���jcc��jmp��call��addָ��
	if (strcmp(insn.mnemonic, "add") == 0)
		_add();
	if (cs_insn_group(handle, &insn, CS_GRP_JUMP) == true)
		_jcc_jmp();
	if (cs_insn_group(handle, &insn, CS_GRP_CALL) == true)
		_call();

	codesize += Mut_Code.codeSize();
	if (codesize > 6) {
		MessageBox(NULL, _T("����������С����1"), NULL, NULL);
	}
		
	//1.����CodeSection��vector��д��Ŀ���ַ
	SingMut_Sec.Raw_CodeAddr = (DWORD)insn.address;
	SingMut_Sec.BaseAddr = objPE.m_dwImageBase + objPE.m_dwImageSize + SingMut_Sec.Mut_CodeOffsetAddr;
	SingMut_Sec.Mut_CodeEndAddr = SingMut_Sec.Mut_CodeStartAddr + insn.size;
	SingMut_Sec.Mut_CodeSize = insn.size;
	SingMut.push_back(SingMut_Sec);
	
	memcpy_s((void*)SingMut_Sec.Mut_CodeStartAddr, insn.size, (void*)insn.address, insn.size);
	//3.�ض�λ����
	do {
		//����esp/sp�Ĵ����Ŀ���Ҫ�����ض�λ
		for (int i = 0; i < x86->op_count; i++) {
			if (x86->operands[i].type == X86_OP_REG) {
				if (x86->operands[i].reg == X86_REG_ESP || x86->operands[i].reg == X86_REG_SP) {
					RelocCheck_flag = true;
					break;
				}
			}
			if (x86->operands[i].type == X86_OP_MEM) {
				if (x86->operands[i].mem.base == X86_REG_ESP || x86->operands[i].mem.base == X86_REG_SP) {
					RelocCheck_flag = true;
					break;
				}
				if (x86->operands[i].mem.index == X86_REG_ESP || x86->operands[i].mem.index == X86_REG_SP) {
					RelocCheck_flag = true;
					break;
				}
			}
		}
		if (RelocCheck_flag == true)
			break;
		//δ��֮ǰ�����д����ָ�����Ҫ�����ض�λ
		if (strcmp(insn.mnemonic, "mov") == 0)
			break;
		if (strcmp(insn.mnemonic, "sub") == 0)
			break;
		if (strcmp(insn.mnemonic, "xor") == 0)
			break;
		if (strcmp(insn.mnemonic, "and") == 0)
			break;
		if (strcmp(insn.mnemonic, "or") == 0)
			break;
		if (strcmp(insn.mnemonic, "rcl") == 0)
			break;
		if (strcmp(insn.mnemonic, "rcr") == 0)
			break;
		if (strcmp(insn.mnemonic, "lea") == 0)
			break;
		if (strcmp(insn.mnemonic, "cmp") == 0)
			break;
		if (strcmp(insn.mnemonic, "test") == 0)
			break;
		if (strcmp(insn.mnemonic, "push") == 0)
			break;
		if (strcmp(insn.mnemonic, "pop") == 0)
			break;
		RelocCheck_flag = true;
	} while (0);
	
	if (RelocCheck_flag == true) {
		//�����ָ���mem��disp_sizeΪ4���������ض�λ
		if (mem.disp_size == 4) {
			DealWithReloc((DWORD)insn.address + mem.disp_offset, SingMut_Sec.BaseAddr + mem.disp_offset);
		}
		//���imm��sizeΪ4���������ض�λ
		if (imm.imm_size == 4) {
			DealWithReloc((DWORD)insn.address + imm.imm_offset, SingMut_Sec.BaseAddr + imm.imm_offset);
		}
	}


	codesize += insn.size;
	Mut_Code.reset();
	return codesize;

}

size_t rand_order::MakeOrderTail(DWORD CodeStartAddr)
{
	Mut_Code.init(CodeInfo(ArchInfo::kIdHost));
	x86::Assembler a(&Mut_Code);
	size_t codesize = 0;
	Order_FixJcc curjcc = { 0 };
	//------------------------------------------------------------------------------------------------------------
	/*
	* jcc/call����
	* call��2/10����jns��4/10����jnp��4/10��
	*/
	//------------------------------------------------------------------------------------------------------------
	int	randsum = rand() % 10;
	if (0 <= randsum <= 1) {
		curjcc.address = CodeStartAddr + Mut_Code.codeSize();
		curjcc.imm_offset = 1;
		a.call(Unknown_Address);

	}
	if (2 <= randsum <= 5) {
		//����eflags����
		a.pushfd();

		a.pushfd();
		a.and_(ptr(x86::esp), 0xF7F);
		a.popfd();
		curjcc.address = CodeStartAddr + Mut_Code.codeSize();
		curjcc.imm_offset = 2;
		a.jns(Unknown_Address);

	}
	if (6 <= randsum <= 9) {
		//����eflags����
		a.pushfd();

		a.pushfd();
		a.and_(ptr(x86::esp), 0xFFB);
		a.popfd();
		curjcc.address = CodeStartAddr + Mut_Code.codeSize();
		curjcc.imm_offset = 2;
		a.jnp(Unknown_Address);

	}
	codesize = Mut_Code.codeSize();
	Mut_Code.flatten();
	Mut_Code.resolveUnresolvedLinks();
	DWORD	CodeOffsetAddr = CodeStartAddr - (DWORD)Final_MutMemory;
	uint64_t BaseAddr = (uint64_t)objPE.m_dwImageBase + objPE.m_dwImageSize + CodeOffsetAddr;
	Mut_Code.relocateToBase(BaseAddr);
	Mut_Code.copyFlattenedData((void*)CodeStartAddr, codesize, CodeHolder::kCopyWithPadding);

	//����ǰ����εĴ��޸���call/jns/jnp��Ϣ��֪��һ����
	Order_FixOffset = curjcc;

	codesize = Mut_Code.codeSize();
	if (codesize > tail_maxsize) {
		MessageBox(NULL, _T("�����β����С����"), NULL, NULL);
	}
	Mut_Code.reset();
	return codesize;

}

/*
UINT rand_order::Copy_OrdCodes_to_FinalMem(BOOL copy_flag , DWORD codesize)
{
	UINT result = -1;
	cs_x86* x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };
	mem.disp_offset = x86->encoding.disp_offset;
	mem.disp_size = x86->encoding.disp_size;
	imm.imm_offset = x86->encoding.imm_offset;
	imm.imm_size = x86->encoding.imm_size;

	
	if (Mut_Code.codeSize() > asmjit_codesize) {
		MessageBox(NULL, _T("���ɵĴ����С����!"), NULL, NULL);
		throw	"asmjit���ɵĴ����С����Ԥ�����ɵĴ����С";
	}
	
	if (copy_flag == true) {
		codesize = insn.address + Mut_Code.codeSize();
	}
	else {
		codesize = Mut_Code.codeSize();
	}
	

	
	




	if (copy_flag == true)
	{

	}



	return 0;
}
*/
/*
UINT rand_order::Update_Mem()
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
	VirtualFree(temp, 0, MEM_RELEASE);

	//���¼���vector�����Ҫ�ı�ľ��Ե�ַ
	size_t differ = (size_t)Final_MutMemory - (size_t)temp;
	for (auto& c : Mut_Mark_again) {
		c.Protected_Start += differ;
		c.Protected_End += differ;
	}
	for (auto& c : SingMut) {
		c.Mut_CodeStartAddr += differ;
		c.Mut_CodeEndAddr += differ;
	}
	for (auto& c : Fix_Offset) {
		c.address += differ;
	}
	//���α����õ�
	for (auto& c : CA_Fix_Offset) {
		c.Call_Addr += differ;
		c.Add_Addr += differ;
	}
	//�����õ�
	Order_FixJcc top = Order_FixOffset.top();
	Order_FixOffset.pop();
	top.address += differ;
	Order_FixOffset.push(top);


	return true;
}
*/

void* rand_order::GetTargetAddress(DWORD codesize)
{
	void* result = nullptr;
	//���ʣ���ڴ治��0x1000
	if (FinalRemainMem_Size < 0x1000)
	{
		Update_Mem();
	}
	//��������ڴ治��д�뱾��code�������¿���0x1000���ڴ���
	if ((DWORD)phead_mem + codesize > (DWORD)ptail_mem)
	{
		phead_mem = (void*)((DWORD)Final_MutMemory + Final_CodeSize);
		ptail_mem = (void*)((DWORD)phead_mem + 0x1000);
		Final_CodeSize += 0x1000;
		FinalRemainMem_Size -= 0x1000;
	}
	if (firstcode_flag == true)
	{
		result = phead_mem;
		phead_mem = (void*)((DWORD)phead_mem + codesize);
		place_flag = 0;
		return result;
	}
	if (firstcode_flag == false) 
	{
		if (place_flag) {
			result = phead_mem;
			phead_mem = (void*)((DWORD)phead_mem + codesize);
			place_flag = 0;
			return result;
		}
		else {
			ptail_mem = (void*)((DWORD)ptail_mem - codesize);
			result = ptail_mem;
			place_flag = 1;
			return result;
		}
	}

	return nullptr;
}

void rand_order::link_jmp(int flag, x86Insn_Mutation& code, CPE& objPE, LPBYTE Addr)
{
	int a = 0b11'1111;
}
