#include "pch.h"
#include "rand_order.h"
#define head_maxsize 6
#define tail_maxsize 16
//#define link_jmpsize 0
/*
* 乱序这一步必须接在变异完成之后，作为最后一步。且不能单独用乱序
*/


BOOL rand_order::Disassemble(LPBYTE Protected_Start, LPBYTE Protected_End, LPBYTE Jmp_Start, LPBYTE Jmp_End)
{
	uint64_t	address = (uint64_t)Protected_Start;	//起始地址
	cs_insn*	insn;									//反汇编出的指令信息
	size_t		j = 0;
	size_t		order_sum = 0;
	Mark		Mark_Struct = { 0 };
	BOOL		result = true;
	size_t		count;

	cs_err err = cs_open(CS_ARCH_X86, CS_MODE_32, &handle);
	if (err) {
		MessageBox(NULL, _T("cs_open()失败！"), NULL, NULL);
		abort();
	}

	//解析指令
	cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
	count = cs_disasm(handle, Protected_Start, Protected_End - Protected_Start, address, 0, &insn);
	if (count) {
		//针对每段代码变异
		//每次随机抽1-200条指令作为一个乱序段
		firstcode_flag = true;
		endcode_flag = false;
		while (j != count) {
			if (count - j <= 200) {
				order_sum = count - j;
				endcode_flag = true;
			}
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
		MessageBox(NULL, _T("反汇编代码失败！"), NULL, NULL);
		abort();
		result = false;
	}

	cs_close(&handle);
	return result;
}

UINT rand_order::Update_Mem()
{
	//创建2倍大小的空间并将原空间代码copy过来

	void* temp = Final_MutMemory;
	size_t temp_size = FinalMem_Size;
	FinalMem_Size *= 2;
	Final_MutMemory = VirtualAlloc(NULL, FinalMem_Size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (Final_MutMemory == nullptr)
	{
		MessageBox(NULL, _T("Final_MutMemory申请空间失败"), NULL, NULL);
		return false;
	}
	memcpy_s(Final_MutMemory, FinalMem_Size, temp, temp_size);
	FinalRemainMem_Size = FinalMem_Size - temp_size;
	VirtualFree(temp, 0, MEM_RELEASE);

	//更新几个vector里的需要改变的绝对地址
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
		for (auto& b : c.second) {
			b.address += differ;
		}
	}
	for (auto& c : CA_Fix_Offset) {
		c.Call_Addr += differ;
		c.Add_Addr += differ;
	}

	//重写只是多添加了这3行代码
	Order_FixOffset.address += differ;
	phead_mem = (void*)((size_t)phead_mem + differ);
	ptail_mem = (void*)((size_t)ptail_mem + differ);
	return true;
}

//针对一段指令生成乱序段
UINT rand_order::Order_ManyCode()
{
	DWORD	Ord_CodeHeadAddr;
	size_t	Head_Codesize = 0, Body_Codesize = 0, Tail_Codesize = 0;
	size_t	body_maxsize = 0;
	size_t	Total_MaxCodesize = 0;
	size_t	MadeCodesize = 0;

	for (size_t i = 0; i < Ordered_Insns.nums; i++)
		body_maxsize += Ordered_Insns.order_insn[Ordered_Insns.index + i].size;
	Total_MaxCodesize = head_maxsize + body_maxsize + tail_maxsize ;

	Ord_CodeHeadAddr = (DWORD)GetTargetAddress(Total_MaxCodesize);

	//生成&处理乱序指令
	//乱序段首
	Head_Codesize = MakeOrderHead(Ord_CodeHeadAddr);
	MadeCodesize += Head_Codesize;
	if (Head_Codesize > head_maxsize) {
		MessageBox(NULL, _T("乱序段首部大小错误"), NULL, NULL);
	}

	//乱序段主体
	for (size_t i = 0; i < Ordered_Insns.nums; i++)
	{
		this->insn = Ordered_Insns.order_insn[Ordered_Insns.index + i];
		DWORD Body_CodeStartAddr = Ord_CodeHeadAddr + Head_Codesize + Body_Codesize;
		Body_Codesize += MakeOrderBody(Body_CodeStartAddr);
	}
	MadeCodesize += Body_Codesize;
	if (Body_Codesize > body_maxsize) {
		MessageBox(NULL, _T("乱序段主体大小错误2"), NULL, NULL);
	}

	//乱序段尾
	DWORD Tail_CodeStartAddr = Ord_CodeHeadAddr + MadeCodesize;
	Tail_Codesize = MakeOrderTail(Tail_CodeStartAddr);
	MadeCodesize += Tail_Codesize;
	if (Tail_Codesize > tail_maxsize) {
		MessageBox(NULL, _T("乱序段尾部大小错误"), NULL, NULL);
	}
	
	return true;
}

size_t rand_order::MakeOrderHead(DWORD CodeStartAddr)
{
	Mut_Code.init(CodeInfo(ArchInfo::kIdHost));
	size_t codesize = 0;
	Order_FixJcc lastjcc = { 0 };
	x86::Assembler a(&Mut_Code);

	if (firstcode_flag == false) {
		//纠正上一个乱序段的call/jns/jnp的offsetdata
		lastjcc = Order_FixOffset;
		DWORD jcc_offset = CodeStartAddr - lastjcc.imm_offset - 4 - lastjcc.address;
		memcpy_s((void*)(lastjcc.address + lastjcc.imm_offset), 4, &jcc_offset, 4);
		//恢复因上一个乱序段jns/jnp而改变的eflags，因call而改变的栈平衡
		if (lastjcc.imm_offset == 2)
			a.popfd();
		if (lastjcc.imm_offset == 1)
			a.add(x86::esp, 4);

		
		Mut_Code.flatten();
		Mut_Code.resolveUnresolvedLinks();
		DWORD	CodeOffsetAddr = CodeStartAddr - (DWORD)Final_MutMemory;
		DWORD	BaseAddr = objPE.m_dwImageBase + objPE.m_dwImageSize + CodeOffsetAddr;
		Mut_Code.relocateToBase(BaseAddr);
		Mut_Code.copyFlattenedData((void*)CodeStartAddr, Mut_Code.codeSize(), CodeHolder::kCopyWithPadding);
	}

	codesize = Mut_Code.codeSize();
	if (codesize > head_maxsize) {
		MessageBox(NULL, _T("乱序段头部大小错误"), NULL, NULL);
	}
	Mut_Code.reset();
	return codesize;

}

size_t rand_order::MakeOrderBody(DWORD CodeStartAddr)
{
	Mut_Code.init(CodeInfo(ArchInfo::kIdHost));
	size_t codesize = 0;
	BOOL RelocCheck_flag = false;
	UINT flag = -1;
	SingMut_Sec = { 0 };

	SingMut_Sec.Mut_CodeStartAddr = CodeStartAddr;
	SingMut_Sec.Mut_CodeOffsetAddr = SingMut_Sec.Mut_CodeStartAddr - (DWORD)Final_MutMemory;
	//1.先判断 该代码地址是否为jcc的目标跳转地址
	Fix_JmpOffset();
	//2.特殊处理jcc，jmp，call，add指令，其他指令留给后续函数直接copy他们
	//这里生成的指令大小最大为6
	if (strcmp(insn.mnemonic, "add") == 0)
		flag = _add();
	if (cs_insn_group(handle, &insn, CS_GRP_JUMP) == true)
		flag = _jcc_jmp();
	if (cs_insn_group(handle, &insn, CS_GRP_CALL) == true)
		flag = _call();

	if (flag != -1)
	{
		Mut_Code.flatten();
		Mut_Code.resolveUnresolvedLinks();
		DWORD	CodeOffsetAddr = CodeStartAddr - (DWORD)Final_MutMemory;
		uint64_t BaseAddr = (uint64_t)objPE.m_dwImageBase + objPE.m_dwImageSize + CodeOffsetAddr;
		Mut_Code.relocateToBase(BaseAddr);
		Mut_Code.copyFlattenedData((void*)CodeStartAddr, Mut_Code.codeSize(), CodeHolder::kCopyWithPadding);

		codesize = Mut_Code.codeSize();
		if (codesize > 6) {
			MessageBox(NULL, _T("乱序段主体大小错误1"), NULL, NULL);
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------
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
	if (flag == -1)
	{
		//1.加入CodeSection的vector，写到目标地址
		SingMut_Sec.Raw_CodeAddr = (DWORD)insn.address;
		SingMut_Sec.BaseAddr = objPE.m_dwImageBase + objPE.m_dwImageSize + SingMut_Sec.Mut_CodeOffsetAddr;
		SingMut_Sec.Mut_CodeEndAddr = SingMut_Sec.Mut_CodeStartAddr + insn.size;
		SingMut_Sec.Mut_CodeSize = insn.size;
		SingMut.push_back(SingMut_Sec);

		memcpy_s((void*)SingMut_Sec.Mut_CodeStartAddr, insn.size, (void*)insn.address, insn.size);
		//3.重定位处理：
		do {
			//没有op操作数
			if (x86->op_count == 0) {
				break;
			}
			//带有esp/sp寄存器的可能要处理重定位
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
			//未在之前变异中处理的指令可能要处理重定位
			if (strcmp(insn.mnemonic, "mov") == 0)
				break;
			if (strcmp(insn.mnemonic, "push") == 0)
				break;
			if (strcmp(insn.mnemonic, "pop") == 0)
				break;
			if (strcmp(insn.mnemonic, "add") == 0)
				break;
			if (strcmp(insn.mnemonic, "sub") == 0)
				break;
			if (strcmp(insn.mnemonic, "jnp") == 0)
				break;
			if (strcmp(insn.mnemonic, "jns") == 0)
				break;
			if (strcmp(insn.mnemonic, "jmp") == 0)
				break;
			if (strcmp(insn.mnemonic, "call") == 0)
				break;
			if (strcmp(insn.mnemonic, "lea") == 0)
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
			if (strcmp(insn.mnemonic, "cmp") == 0)
				break;
			if (strcmp(insn.mnemonic, "test") == 0)
				break;
			//jcc
			if (strcmp(insn.mnemonic, "je") == 0)
				break;
			if (strcmp(insn.mnemonic, "jne") == 0)
				break;
			if (strcmp(insn.mnemonic, "ja") == 0)
				break;
			if (strcmp(insn.mnemonic, "jae") == 0)
				break;
			if (strcmp(insn.mnemonic, "jb") == 0)
				break;
			if (strcmp(insn.mnemonic, "jbe") == 0)
				break;
			if (strcmp(insn.mnemonic, "jc") == 0)
				break;
			if (strcmp(insn.mnemonic, "jecxz") == 0)
				break;
			if (strcmp(insn.mnemonic, "jg") == 0)
				break;
			if (strcmp(insn.mnemonic, "jge") == 0)
				break;
			if (strcmp(insn.mnemonic, "jl") == 0)
				break;
			if (strcmp(insn.mnemonic, "jle") == 0)
				break;
			if (strcmp(insn.mnemonic, "jna") == 0)
				break;
			if (strcmp(insn.mnemonic, "jnae") == 0)
				break;
			if (strcmp(insn.mnemonic, "jnb") == 0)
				break;
			if (strcmp(insn.mnemonic, "jnbe") == 0)
				break;
			if (strcmp(insn.mnemonic, "jnc") == 0)
				break;
			if (strcmp(insn.mnemonic, "jng") == 0)
				break;
			if (strcmp(insn.mnemonic, "jnge") == 0)
				break;
			if (strcmp(insn.mnemonic, "jnl") == 0)
				break;
			if (strcmp(insn.mnemonic, "jnle") == 0)
				break;
			if (strcmp(insn.mnemonic, "jno") == 0)
				break;
			if (strcmp(insn.mnemonic, "jnz") == 0)
				break;
			if (strcmp(insn.mnemonic, "jo") == 0)
				break;
			if (strcmp(insn.mnemonic, "jp") == 0)
				break;
			if (strcmp(insn.mnemonic, "jpe") == 0)
				break;
			if (strcmp(insn.mnemonic, "jpo") == 0)
				break;
			if (strcmp(insn.mnemonic, "js") == 0)
				break;
			if (strcmp(insn.mnemonic, "jz") == 0)
				break;
			RelocCheck_flag = true;
		} while (0);

		if (RelocCheck_flag == true) {
			//如果该指令的mem的disp_size为4，可能有重定位
			if (mem.disp_size == 4) {
				DealWithReloc((DWORD)insn.address + mem.disp_offset, SingMut_Sec.BaseAddr + mem.disp_offset);
			}
			//如果imm的size为4，可能有重定位
			if (imm.imm_size == 4) {
				DealWithReloc((DWORD)insn.address + imm.imm_offset, SingMut_Sec.BaseAddr + imm.imm_offset);
			}
		}

		codesize = insn.size;
	}

	Mut_Code.reset();
	return codesize;

}

size_t rand_order::MakeOrderTail(DWORD CodeStartAddr)
{
	Mut_Code.init(CodeInfo(ArchInfo::kIdHost));
	x86::Assembler a(&Mut_Code);
	size_t codesize = 0;
	Order_FixJcc curjcc = { 0 };

	if (endcode_flag == true)
	{
		a.jmp(objPE.m_dwImageBase + objPE.m_dwImageSize + Final_CodeSize);

		Mut_Code.flatten();
		Mut_Code.resolveUnresolvedLinks();
		DWORD	CodeOffsetAddr = CodeStartAddr - (DWORD)Final_MutMemory;
		uint64_t BaseAddr = (uint64_t)objPE.m_dwImageBase + objPE.m_dwImageSize + CodeOffsetAddr;
		Mut_Code.relocateToBase(BaseAddr);
		Mut_Code.copyFlattenedData((void*)CodeStartAddr, Mut_Code.codeSize(), CodeHolder::kCopyWithPadding);

		Mut_Code.reset();
		return Mut_Code.codeSize();
	}
	//------------------------------------------------------------------------------------------------------------
	/*
	* jcc/call连接
	* 概率：call（2/10），jns（4/10），jnp（4/10）
	*/
	//------------------------------------------------------------------------------------------------------------
	int	randsum = rand() % 10;
	if (randsum >= 0 && randsum <= 1) {
		curjcc.address = CodeStartAddr + Mut_Code.codeSize();
		curjcc.imm_offset = 1;
		a.call(Unknown_Address);

	}
	if (randsum >= 2 && randsum <= 5) {
		//保存eflags环境
		a.pushfd();

		a.pushfd();
		a.and_(dword_ptr(x86::esp), 0xF7F);
		a.popfd();
		curjcc.address = CodeStartAddr + Mut_Code.codeSize();
		curjcc.imm_offset = 2;
		a.jns(Unknown_Address);

	}
	if (randsum >= 6 && randsum <= 9) {
		//保存eflags环境
		a.pushfd();

		a.pushfd();
		a.and_(dword_ptr(x86::esp), 0xFFB);
		a.popfd();
		curjcc.address = CodeStartAddr + Mut_Code.codeSize();
		curjcc.imm_offset = 2;
		a.jnp(Unknown_Address);

	}
	
	Mut_Code.flatten();
	Mut_Code.resolveUnresolvedLinks();
	DWORD	CodeOffsetAddr = CodeStartAddr - (DWORD)Final_MutMemory;
	uint64_t BaseAddr = (uint64_t)objPE.m_dwImageBase + objPE.m_dwImageSize + CodeOffsetAddr;
	Mut_Code.relocateToBase(BaseAddr);
	Mut_Code.copyFlattenedData((void*)CodeStartAddr, Mut_Code.codeSize(), CodeHolder::kCopyWithPadding);

	//将当前乱序段的待修复的call/jns/jnp信息告知下一个段
	Order_FixOffset = curjcc;

	codesize = Mut_Code.codeSize();
	if (codesize > tail_maxsize) {
		MessageBox(NULL, _T("乱序段尾部大小错误"), NULL, NULL);
	}
	Mut_Code.reset();
	

	return codesize;
}

//取得乱序段所在的目标地址
void* rand_order::GetTargetAddress(DWORD codesize)
{
	/*
	* 选一片0x1000的空间。
	* 对于第一个乱序段，将其放入空间的首部；对于第二个乱序段，将其放入空间的尾部
	* 对于第三个乱序段，将其再次放入空间的首部（接在第一个的后面）；
	* 对于第四个乱序段，将其再次放入空间的尾部（接在第二个的后面）；
	* ......
	* ......
	*/
	
	void* result = nullptr;
	if (codesize >= 0x1000)
		MessageBox(NULL, _T("乱序段代码过大"), NULL, NULL);

	//如果剩余内存不够0x1000
	if (FinalRemainMem_Size < 0x1000)
	{
		Update_Mem();
	}
	if (firstcode_flag == true)
	{
		phead_mem = (void*)((DWORD)Final_MutMemory + Final_CodeSize);
		result = phead_mem;
		ptail_mem = (void*)((DWORD)phead_mem + 0x1000);
		phead_mem = (void*)((DWORD)phead_mem + codesize);
		place_flag = 0;
		Final_CodeSize += 0x1000;
		FinalRemainMem_Size -= 0x1000;
		return result;
	}
	//如果乱序内存不够写入本次code，就重新开个0x1000的内存量
	if ((DWORD)phead_mem + codesize > (DWORD)ptail_mem)
	{
		phead_mem = (void*)((DWORD)Final_MutMemory + Final_CodeSize);
		ptail_mem = (void*)((DWORD)phead_mem + 0x1000);
		Final_CodeSize += 0x1000;
		FinalRemainMem_Size -= 0x1000;
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
/*
void rand_order::link_jmp(int flag, x86Insn_Mutation& code, CPE& objPE, LPBYTE Addr)
{	//只要知道其中一个地址和偏移即可
	//	start_link，往下跳
	if (flag == 1)
	{
		//偏移 =   整块镜像内存 - Addr + CodeSize - 5
		DWORD data = (DWORD)objPE.m_pFileBuf + objPE.m_dwImageSize - (DWORD)Addr + code.Final_CodeSize - 5;
		memcpy_s(Addr, 1, "\xE9", 1);
		memcpy_s(Addr + 1, 4, &data, 4);
	}
	else
		//	end_link，往上跳。由于添加了代码，还要修改一些成员变量
	{
		//偏移 = -(整块镜像内存 - Addr + jmp相对自己区段的偏移) - 5
		//DWORD data = (DWORD)Addr - (DWORD)objPE.m_pFileBuf - objPE.m_dwImageSize - code.Final_CodeSize - 5;
		DWORD data = (DWORD)Addr - ((DWORD)objPE.m_pFileBuf + objPE.m_dwImageSize) - ((DWORD)plink_jmp - (DWORD)Final_MutMemory) - 5;
		memcpy_s(plink_jmp, 1, "\xE9", 1);
		memcpy_s((void*)((size_t)plink_jmp + 1), 4, &data, 4);
	}
}
*/