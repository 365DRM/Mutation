#include "pch.h"
#include "Mutation_Protecting.h"
#include "auxiliary_function.h"

UINT x86Insn_Mutation::_mov()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };


	//���ж��Ƿ���x16��x8����
	if (x86->operands[0].size != 4 || x86->operands[1].size != 4) {
		//mov reg,reg_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_mov_reg_reg_16_8(x86->operands[0].reg, x86->operands[1].reg));
		//mov reg,imm_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_mov_reg_imm_16_8(x86->operands[0].reg, &imm));
		}
		//mov_reg_mem_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_mov_reg_mem_16_8(x86->operands[0].reg, &mem));
		}
		//mov_mem_reg_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_mov_mem_reg_16_8(&mem, x86->operands[1].reg));
		}
		//mov_mem_imm_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_mov_mem_imm_16_8(&mem, &imm));
		}


	}
	//x32�������û����λ�����жϴ����������������ж��Ƿ���x16��x8����	
	if (x86->op_count == 2) {
		//mov reg,reg
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_mov_reg_reg(x86->operands[0].reg, x86->operands[1].reg));
		//mov reg,imm
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_mov_reg_imm(x86->operands[0].reg, &imm));
		}
		//mov_reg_mem
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_mov_reg_mem(x86->operands[0].reg, &mem));
		}

		//mov_mem_reg
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_mov_mem_reg(&mem, x86->operands[1].reg));
		}
		//mov_mem_imm
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_mov_mem_imm(&mem, &imm));
		}


	}
	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����						
UINT x86Insn_Mutation::_mov_reg_reg(x86_reg op0, x86_reg op1)
{
	//JitRuntime rt;
	//CodeHolder code;
	//Mut_Code.init(rt.codeInfo());
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false || Check_Reg(op1) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		//ֻ��eax��ebx��ecx��edx��8λ�Ĵ���
	} while (
		randreg0 == op0 ||
		randreg0 == op1 ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == randreg0 ||
		randreg1 == op0 ||
		randreg1 == op1 ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);
	auto reg_op1 = to_asmjit_reg(op1);

	a.pushfd();
	a.push(rand1);
	a.mov(rand1, reg_op1);
	a.push(rand0);
	a.mov(rand0, reg_op0);

	a.bswap(rand1);
	a.mov(rand0, rand1);
	a.xchg(Low_reg(randreg0, al), Low_reg(randreg0, ah));
	a.rcl(rand0, 0x10);
	a.rcr(rand1, 0x10);

	a.xor_(Low_reg(randreg0, ax), Low_reg(randreg1, ax));
	a.push(Low_reg(randreg0, ax));
	a.xor_(Low_reg(randreg1, ax), Low_reg(randreg0, ax));
	a.pop(Low_reg(randreg0, ax));
	a.xor_(Low_reg(randreg0, ax), Low_reg(randreg1, ax));

	a.mov(Low_reg(randreg1, al), Low_reg(randreg0, ah));
	a.mov(Low_reg(randreg1, ah), Low_reg(randreg0, al));
	a.xor_(Low_reg(randreg0, ax), Low_reg(randreg0, ax));
	a.add(Low_reg(randreg0, ax), Low_reg(randreg1, ax));

	a.pop(reg_op0);
	a.xchg(reg_op0, rand0);

	a.pop(rand1);
	a.popfd();


	//a.call(0x401000);
	//a.ret();

	//a.call(0x401000);
	//Mut fn;
	//Error err = rt.add(&fn, &Mut_Code);

	//rt.release(fn);
	return mov_reg_reg;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����						
UINT x86Insn_Mutation::_mov_reg_imm(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == op0 || randreg0 == X86_REG_ESP);

	auto rand0 = to_asmjit_reg(randreg0);
	auto reg_op0 = to_asmjit_reg(op0);


	a.pushfd();
	a.push(rand0);
	a.mov(rand0, 0);

	UINT temp = 0;
	UINT randimm1 = rand();
	bool Re_flag = RelocData_imm_mem(address + imm_offset, rand0, &temp);
	//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
	x86::Assembler b(&Mut_Code);
	//�ж��Ƿ���Ҫ�ض�λ
	if (Re_flag) {
		temp -= randimm1;
		b.add(rand0, temp);
	}
	else {
		imm_value -= randimm1;
		b.add(rand0, (UINT)imm_value);
	}

	_mov_reg_reg(op0, randreg0);
	x86::Assembler c(&Mut_Code);
	c.add(reg_op0, randimm1);

	c.pop(rand0);
	c.popfd();

	return mov_reg_imm;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_mov_reg_mem(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1, randreg0;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op0 ||
		randreg0 == randreg1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP
		);

	auto rand1 = to_asmjit_reg(randreg1);
	auto rand0 = to_asmjit_reg(randreg0);


	a.pushfd();
	a.push(rand1);

	a.mov(rand1, 0);
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}

	b.push(rand0);
	_mov_reg_reg(randreg0, op0);
	x86::Assembler c(&Mut_Code);
	c.mov(Low_reg(randreg0, ax), word_ptr(rand1, 2));
	c.rcl(rand0, 0x10);
	c.mov(Low_reg(randreg0, ax), ptr(rand1));
	//��rand0��ֵ��op0
	c.mov(to_asmjit_reg(op0), rand0);

	c.pop(rand0);
	c.pop(rand1);
	c.popfd();

	return mov_reg_mem;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_mov_mem_reg(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op1) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op1 ||
		randreg1 == randreg0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP
		);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);

	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	a.pushfd();
	a.push(rand0);

	a.mov(rand0, 0);
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}

	b.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler c(&Mut_Code);
	//2���ֽ�2���ֽڵظ�ֵ��ѭ��2�Ρ�ÿ��rand1����16λ��rand0 ��2���ֽ�
	for (int i = 0; i < 2; i++) {
		c.mov(word_ptr(rand0), Low_reg(randreg1, ax));
		c.rcr(rand1, 0x10);
		c.inc(rand0);
		c.add(rand0, 1);
	}
	c.pop(rand1);


	c.pop(rand0);
	c.popfd();

	return mov_mem_reg;
}
//memֻ֧��4�ֽڴ�С����֧��esp��
UINT x86Insn_Mutation::_mov_mem_imm(x86_mem* mem0, x86_imm* imm1)
{
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == mem0->base || randreg0 == mem0->index);
	auto rand0 = to_asmjit_reg(randreg0);


	a.pushfd();
	a.push(rand0);
	_mov_reg_imm(randreg0, imm1);
	_mov_mem_reg(mem0, randreg0);
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);
	b.popfd();


	return mov_mem_imm;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_mov_reg_reg_16_8(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	//��1�������op0�ǶμĴ�����op1��ͨ�üĴ���
	if (Check_SReg(op0) == true && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_sreg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	//��2�������op0��ͨ�üĴ�����op1�ǶμĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == true) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_sreg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	//��3�������2��op����ͨ�üĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}

	return mov_reg_reg_16_8;
}
//����x16��x8����ļ���		
UINT x86Insn_Mutation::_mov_reg_imm_16_8(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);

	//����imm����mov���μĴ������������ﲻ���μĴ������жϡ�������imm1���ض�λ�����PS��ʵ���������immҲ���������ض�λ�������PS2��ɾ���ض�λ������
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == Low_reg_Check(op0) ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI);

	auto rand1 = to_asmjit_reg(randreg1);

	a.pushfd();
	a.push(rand1);
	a.mov(rand1, 0);
	//x16
	if (imm_size == 2) {
		a.add(Low_reg(randreg1, ax), (WORD)imm_value);
		a.mov(to_asmjit_reg(op0), Low_reg(randreg1, ax));
	}
	//x8
	if (imm_size == 1) {
		a.add(Low_reg(randreg1, al), (BYTE)imm_value);
		a.mov(to_asmjit_reg(op0), Low_reg(randreg1, al));
	}
	a.pop(rand1);
	a.popfd();

	return mov_reg_imm_16_8;
}
//����x16��x8����ļ���	
UINT x86Insn_Mutation::_mov_reg_mem_16_8(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t mem_size = mem1->mem_size;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == Low_reg_Check(op0) ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand1 = to_asmjit_reg(randreg1);


	a.pushfd();
	a.push(rand1);

	a.mov(rand1, 0);
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}

	//WORD
	if (mem_size == 2) {
		//���op0�ǶμĴ���
		if (op0 == X86_REG_GS || op0 == X86_REG_FS || op0 == X86_REG_ES || op0 == X86_REG_DS || op0 == X86_REG_CS || op0 == X86_REG_SS) {
			b.mov(to_asmjit_sreg(op0), word_ptr(rand1));
		}
		//���op0���ǶμĴ���
		else {
			b.mov(to_asmjit_reg(op0), word_ptr(rand1));
		}
	}
	//BYTE
	if (mem_size == 1) {
		b.mov(to_asmjit_reg(op0), byte_ptr(rand1));
	}

	b.pop(rand1);
	b.popfd();

	return mov_reg_mem_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_mov_mem_reg_16_8(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t mem_size = mem0->mem_size;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == Low_reg_Check(op1) ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand0 = to_asmjit_reg(randreg0);


	a.pushfd();
	a.push(rand0);

	a.mov(rand0, 0);
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}

	//WORD
	if (mem_size == 2) {
		//���op0�ǶμĴ���
		if (op1 == X86_REG_GS || op1 == X86_REG_FS || op1 == X86_REG_ES || op1 == X86_REG_DS || op1 == X86_REG_CS || op1 == X86_REG_SS) {
			b.mov(word_ptr(rand0), to_asmjit_sreg(op1));
		}
		//���op0���ǶμĴ���
		else {
			b.mov(word_ptr(rand0), to_asmjit_reg(op1));
		}
	}
	//BYTE
	if (mem_size == 1) {
		b.mov(byte_ptr(rand0), to_asmjit_reg(op1));
	}


	b.pop(rand0);
	b.popfd();

	return mov_mem_reg_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_mov_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1)
{
	uint8_t mem_size = mem0->mem_size;
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == mem0->base ||
		randreg0 == mem0->index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI);
	auto rand0 = to_asmjit_reg(randreg0);


	a.pushfd();
	a.push(rand0);
	//x16
	if (mem_size == 2) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, ax), imm1);
		_mov_mem_reg_16_8(mem0, Low_reg_2(randreg0, ax));
	}
	//x8
	if (mem_size == 1) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, al), imm1);
		_mov_mem_reg_16_8(mem0, Low_reg_2(randreg0, al));
	}
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);
	b.popfd();


	return mov_mem_imm_16_8;
}


UINT x86Insn_Mutation::_add()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };


	//���ж��Ƿ���x16��x8����
	if (x86->operands[0].size != 4 || x86->operands[1].size != 4) {
		//add reg,reg_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_add_reg_reg_16_8(x86->operands[0].reg, x86->operands[1].reg));
		//add reg,imm_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_add_reg_imm_16_8(x86->operands[0].reg, &imm));
		}
		//add_reg_mem_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_add_reg_mem_16_8(x86->operands[0].reg, &mem));
		}
		//add_mem_reg_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_add_mem_reg_16_8(&mem, x86->operands[1].reg));
		}
		//add_mem_imm_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_add_mem_imm_16_8(&mem, &imm));
		}
	}

	//x32�������û����λ�����жϴ����������������ж��Ƿ���x16��x8����	
	if (x86->op_count == 2) {
		//add reg,reg
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_add_reg_reg(x86->operands[0].reg, x86->operands[1].reg));
		//add reg,imm
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_add_reg_imm(x86->operands[0].reg, &imm));
		}
		//add_reg_mem
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_add_reg_mem(x86->operands[0].reg, &mem));
		}
		//add_mem_reg
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_add_mem_reg(&mem, x86->operands[1].reg));
		}
		//add_mem_imm
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_add_mem_imm(&mem, &imm));
		}
	}

	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����
UINT x86Insn_Mutation::_add_reg_reg(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false || Check_Reg(op1) == false)
		throw "�����reg����";

	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == op1 ||
		randreg1 == X86_REG_ESP);

	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler b(&Mut_Code);
	b.add(reg_op0, rand1);
	b.pop(rand1);

	return add_reg_reg;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����						
UINT x86Insn_Mutation::_add_reg_imm(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == op0 || randreg0 == X86_REG_ESP);

	auto rand0 = to_asmjit_reg(randreg0);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	UINT temp = 0;
	bool Re_flag = RelocData_imm_mem(address + imm_offset, rand0, &temp);
	//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
	x86::Assembler b(&Mut_Code);
	//�ж��Ƿ���Ҫ�ض�λ
	if (Re_flag) {
		b.add(rand0, temp);
	}
	else {
		b.add(rand0, (UINT)imm_value);
	}
	b.popfd();
	b.add(reg_op0, rand0);

	b.pop(rand0);

	return add_reg_imm;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_add_reg_mem(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1, randreg0;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op0 ||
		randreg0 == randreg1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP
		);

	auto rand1 = to_asmjit_reg(randreg1);
	auto rand0 = to_asmjit_reg(randreg0);



	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand0);
	b.mov(rand0, 0);

	b.pushfd();
	b.mov(Low_reg(randreg0, ax), word_ptr(rand1, 2));
	b.rcl(rand0, 0x10);
	b.mov(Low_reg(randreg0, ax), ptr(rand1));
	b.popfd();

	//��rand0 add ��op0
	b.add(to_asmjit_reg(op0), rand0);

	b.pop(rand0);
	b.pop(rand1);


	return add_reg_mem;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_add_mem_reg(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op1) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op1 ||
		randreg1 == randreg0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP
		);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler c(&Mut_Code);
	c.add(ptr(rand0), rand1);

	c.pop(rand1);
	c.pop(rand0);

	return add_mem_reg;
}
//memֻ֧��4�ֽڴ�С����֧��esp��
UINT x86Insn_Mutation::_add_mem_imm(x86_mem* mem0, x86_imm* imm1)
{
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == mem0->base || randreg0 == mem0->index);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	_mov_reg_imm(randreg0, imm1);
	_add_mem_reg(mem0, randreg0);
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return add_mem_imm;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_add_reg_reg_16_8(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	//addָ���޷��ԶμĴ���������������
	/*
	//��1�������op0�ǶμĴ�����op1��ͨ�üĴ���
	if (Check_SReg(op0) == true && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_sreg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	//��2�������op0��ͨ�üĴ�����op1�ǶμĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == true) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_sreg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	*/
	//��3�������2��op����ͨ�üĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.add(asmjit_op0, asmjit_op1);
	}

	return add_reg_reg_16_8;
}
//����x16��x8����ļ���		
UINT x86Insn_Mutation::_add_reg_imm_16_8(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);

	//����imm����add���μĴ������������ﲻ���μĴ������жϡ���x16��x8��add_reg_imm���������ض�λ���
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == Low_reg_Check(op0) ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	//x16
	if (imm_size == 2) {
		a.pushfd();
		a.add(Low_reg(randreg1, ax), (WORD)imm_value);
		a.popfd();
		a.add(to_asmjit_reg(op0), Low_reg(randreg1, ax));
	}
	//x8
	if (imm_size == 1) {
		a.pushfd();
		a.add(Low_reg(randreg1, al), (BYTE)imm_value);
		a.popfd();
		a.add(to_asmjit_reg(op0), Low_reg(randreg1, al));
	}
	a.pop(rand1);


	return add_reg_imm_16_8;
}
//����x16��x8����ļ���	
UINT x86Insn_Mutation::_add_reg_mem_16_8(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t mem_size = mem1->mem_size;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == Low_reg_Check(op0) ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.add(to_asmjit_reg(op0), word_ptr(rand1));
	}
	//BYTE
	if (mem_size == 1) {
		b.add(to_asmjit_reg(op0), byte_ptr(rand1));
	}

	b.pop(rand1);

	return add_reg_mem_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_add_mem_reg_16_8(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t mem_size = mem0->mem_size;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == Low_reg_Check(op1) ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.add(word_ptr(rand0), to_asmjit_reg(op1));
	}
	//BYTE
	if (mem_size == 1) {
		b.add(byte_ptr(rand0), to_asmjit_reg(op1));
	}

	b.pop(rand0);

	return add_mem_reg_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_add_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1)
{
	uint8_t mem_size = mem0->mem_size;
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI ||
		randreg0 == mem0->base ||
		randreg0 == mem0->index);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	//x16
	if (mem_size == 2) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, ax), imm1);
		_add_mem_reg_16_8(mem0, Low_reg_2(randreg0, ax));
	}
	//x8
	if (mem_size == 1) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, al), imm1);
		_add_mem_reg_16_8(mem0, Low_reg_2(randreg0, al));
	}
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return add_mem_imm_16_8;
}


UINT x86Insn_Mutation::_sub()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };


	//���ж��Ƿ���x16��x8����
	if (x86->operands[0].size != 4 || x86->operands[1].size != 4) {
		//sub reg,reg_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_sub_reg_reg_16_8(x86->operands[0].reg, x86->operands[1].reg));
		//sub reg,imm_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_sub_reg_imm_16_8(x86->operands[0].reg, &imm));
		}
		//sub_reg_mem_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_sub_reg_mem_16_8(x86->operands[0].reg, &mem));
		}
		//sub_mem_reg_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_sub_mem_reg_16_8(&mem, x86->operands[1].reg));
		}
		//sub_mem_imm_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_sub_mem_imm_16_8(&mem, &imm));
		}
	}

	//x32�������û����λ�����жϴ����������������ж��Ƿ���x16��x8����	
	if (x86->op_count == 2) {
		//sub reg,reg
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_sub_reg_reg(x86->operands[0].reg, x86->operands[1].reg));
		//sub reg,imm
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_sub_reg_imm(x86->operands[0].reg, &imm));
		}
		//sub_reg_mem
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_sub_reg_mem(x86->operands[0].reg, &mem));
		}
		//sub_mem_reg
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_sub_mem_reg(&mem, x86->operands[1].reg));
		}
		//sub_mem_imm
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_sub_mem_imm(&mem, &imm));
		}
	}

	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����
UINT x86Insn_Mutation::_sub_reg_reg(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false || Check_Reg(op1) == false)
		throw "�����reg����";

	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == op1 ||
		randreg1 == X86_REG_ESP);

	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler b(&Mut_Code);
	b.sub(reg_op0, rand1);
	b.pop(rand1);

	return sub_reg_reg;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����						
UINT x86Insn_Mutation::_sub_reg_imm(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == op0 || randreg0 == X86_REG_ESP);

	auto rand0 = to_asmjit_reg(randreg0);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	UINT temp = 0;
	bool Re_flag = RelocData_imm_mem(address + imm_offset, rand0, &temp);
	//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
	x86::Assembler b(&Mut_Code);
	//�ж��Ƿ���Ҫ�ض�λ
	if (Re_flag) {
		b.add(rand0, temp);
	}
	else {
		b.add(rand0, (UINT)imm_value);
	}
	b.popfd();
	b.sub(reg_op0, rand0);

	b.pop(rand0);

	return sub_reg_imm;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_sub_reg_mem(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1, randreg0;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op0 ||
		randreg0 == randreg1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP
		);

	auto rand1 = to_asmjit_reg(randreg1);
	auto rand0 = to_asmjit_reg(randreg0);



	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand0);
	b.mov(rand0, 0);

	b.pushfd();
	b.mov(Low_reg(randreg0, ax), word_ptr(rand1, 2));
	b.rcl(rand0, 0x10);
	b.mov(Low_reg(randreg0, ax), ptr(rand1));
	b.popfd();

	//��rand0 sub ��op0
	b.sub(to_asmjit_reg(op0), rand0);

	b.pop(rand0);
	b.pop(rand1);


	return sub_reg_mem;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_sub_mem_reg(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op1) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op1 ||
		randreg1 == randreg0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP
		);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler c(&Mut_Code);
	c.sub(ptr(rand0), rand1);

	c.pop(rand1);
	c.pop(rand0);

	return sub_mem_reg;
}
//memֻ֧��4�ֽڴ�С����֧��esp��
UINT x86Insn_Mutation::_sub_mem_imm(x86_mem* mem0, x86_imm* imm1)
{
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == mem0->base || randreg0 == mem0->index);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	_mov_reg_imm(randreg0, imm1);
	_sub_mem_reg(mem0, randreg0);
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return sub_mem_imm;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_sub_reg_reg_16_8(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	//addָ���޷��ԶμĴ���������������
	/*
	//��1�������op0�ǶμĴ�����op1��ͨ�üĴ���
	if (Check_SReg(op0) == true && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_sreg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	//��2�������op0��ͨ�üĴ�����op1�ǶμĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == true) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_sreg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	*/
	//��3�������2��op����ͨ�üĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.sub(asmjit_op0, asmjit_op1);
	}

	return sub_reg_reg_16_8;
}
//����x16��x8����ļ���		
UINT x86Insn_Mutation::_sub_reg_imm_16_8(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);

	//����imm����add���μĴ������������ﲻ���μĴ������жϡ���x16��x8��add_reg_imm���������ض�λ���
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == Low_reg_Check(op0) ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	//x16
	if (imm_size == 2) {
		a.pushfd();
		a.add(Low_reg(randreg1, ax), (WORD)imm_value);
		a.popfd();
		a.sub(to_asmjit_reg(op0), Low_reg(randreg1, ax));
	}
	//x8
	if (imm_size == 1) {
		a.pushfd();
		a.add(Low_reg(randreg1, al), (BYTE)imm_value);
		a.popfd();
		a.sub(to_asmjit_reg(op0), Low_reg(randreg1, al));
	}
	a.pop(rand1);


	return sub_reg_imm_16_8;
}
//����x16��x8����ļ���	
UINT x86Insn_Mutation::_sub_reg_mem_16_8(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t mem_size = mem1->mem_size;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == Low_reg_Check(op0) ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.sub(to_asmjit_reg(op0), word_ptr(rand1));
	}
	//BYTE
	if (mem_size == 1) {
		b.sub(to_asmjit_reg(op0), byte_ptr(rand1));
	}

	b.pop(rand1);

	return sub_reg_mem_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_sub_mem_reg_16_8(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t mem_size = mem0->mem_size;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == Low_reg_Check(op1) ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.sub(word_ptr(rand0), to_asmjit_reg(op1));
	}
	//BYTE
	if (mem_size == 1) {
		b.sub(byte_ptr(rand0), to_asmjit_reg(op1));
	}

	b.pop(rand0);

	return sub_mem_reg_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_sub_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1)
{
	uint8_t mem_size = mem0->mem_size;
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == mem0->base ||
		randreg0 == mem0->index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	//x16
	if (mem_size == 2) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, ax), imm1);
		_sub_mem_reg_16_8(mem0, Low_reg_2(randreg0, ax));
	}
	//x8
	if (mem_size == 1) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, al), imm1);
		_sub_mem_reg_16_8(mem0, Low_reg_2(randreg0, al));
	}
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return sub_mem_imm_16_8;
}


UINT x86Insn_Mutation::_xor()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };


	//���ж��Ƿ���x16��x8����
	if (x86->operands[0].size != 4 || x86->operands[1].size != 4) {
		//xor reg,reg_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_xor_reg_reg_16_8(x86->operands[0].reg, x86->operands[1].reg));
		//xor reg,imm_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_xor_reg_imm_16_8(x86->operands[0].reg, &imm));
		}
		//xor_reg_mem_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_xor_reg_mem_16_8(x86->operands[0].reg, &mem));
		}
		//xor_mem_reg_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_xor_mem_reg_16_8(&mem, x86->operands[1].reg));
		}
		//xor_mem_imm_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_xor_mem_imm_16_8(&mem, &imm));
		}
	}

	//x32�������û����λ�����жϴ����������������ж��Ƿ���x16��x8����	
	if (x86->op_count == 2) {
		//xor reg,reg
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_xor_reg_reg(x86->operands[0].reg, x86->operands[1].reg));
		//xor reg,imm
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_xor_reg_imm(x86->operands[0].reg, &imm));
		}
		//xor_reg_mem
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_xor_reg_mem(x86->operands[0].reg, &mem));
		}
		//xor_mem_reg
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_xor_mem_reg(&mem, x86->operands[1].reg));
		}
		//xor_mem_imm
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_xor_mem_imm(&mem, &imm));
		}
	}

	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����
UINT x86Insn_Mutation::_xor_reg_reg(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false || Check_Reg(op1) == false)
		throw "�����reg����";

	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == op1 ||
		randreg1 == X86_REG_ESP);

	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler b(&Mut_Code);
	b.xor_(reg_op0, rand1);
	b.pop(rand1);

	return xor_reg_reg;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����						
UINT x86Insn_Mutation::_xor_reg_imm(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == op0 || randreg0 == X86_REG_ESP);

	auto rand0 = to_asmjit_reg(randreg0);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	UINT temp = 0;
	bool Re_flag = RelocData_imm_mem(address + imm_offset, rand0, &temp);
	//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
	x86::Assembler b(&Mut_Code);
	//�ж��Ƿ���Ҫ�ض�λ
	if (Re_flag) {
		b.add(rand0, temp);
	}
	else {
		b.add(rand0, (UINT)imm_value);
	}
	b.popfd();
	b.xor_(reg_op0, rand0);

	b.pop(rand0);

	return xor_reg_imm;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_xor_reg_mem(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1, randreg0;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op0 ||
		randreg0 == randreg1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP
		);

	auto rand1 = to_asmjit_reg(randreg1);
	auto rand0 = to_asmjit_reg(randreg0);



	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand0);
	b.mov(rand0, 0);

	b.pushfd();
	b.mov(Low_reg(randreg0, ax), word_ptr(rand1, 2));
	b.rcl(rand0, 0x10);
	b.mov(Low_reg(randreg0, ax), ptr(rand1));
	b.popfd();

	b.xor_(to_asmjit_reg(op0), rand0);

	b.pop(rand0);
	b.pop(rand1);


	return xor_reg_mem;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_xor_mem_reg(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op1) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op1 ||
		randreg1 == randreg0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP
		);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler c(&Mut_Code);
	c.xor_(ptr(rand0), rand1);

	c.pop(rand1);
	c.pop(rand0);

	return xor_mem_reg;
}
//memֻ֧��4�ֽڴ�С����֧��esp��
UINT x86Insn_Mutation::_xor_mem_imm(x86_mem* mem0, x86_imm* imm1)
{
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == mem0->base || randreg0 == mem0->index);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	_mov_reg_imm(randreg0, imm1);
	_xor_mem_reg(mem0, randreg0);
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return xor_mem_imm;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_xor_reg_reg_16_8(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	//addָ���޷��ԶμĴ���������������
	/*
	//��1�������op0�ǶμĴ�����op1��ͨ�üĴ���
	if (Check_SReg(op0) == true && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_sreg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	//��2�������op0��ͨ�üĴ�����op1�ǶμĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == true) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_sreg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	*/
	//��3�������2��op����ͨ�üĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.xor_(asmjit_op0, asmjit_op1);
	}

	return xor_reg_reg_16_8;
}
//����x16��x8����ļ���		
UINT x86Insn_Mutation::_xor_reg_imm_16_8(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);

	//����imm����add���μĴ������������ﲻ���μĴ������жϡ���x16��x8��add_reg_imm���������ض�λ���
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == Low_reg_Check(op0) ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	//x16
	if (imm_size == 2) {
		a.pushfd();
		a.add(Low_reg(randreg1, ax), (WORD)imm_value);
		a.popfd();
		a.xor_(to_asmjit_reg(op0), Low_reg(randreg1, ax));
	}
	//x8
	if (imm_size == 1) {
		a.pushfd();
		a.add(Low_reg(randreg1, al), (BYTE)imm_value);
		a.popfd();
		a.xor_(to_asmjit_reg(op0), Low_reg(randreg1, al));
	}
	a.pop(rand1);


	return xor_reg_imm_16_8;
}
//����x16��x8����ļ���	
UINT x86Insn_Mutation::_xor_reg_mem_16_8(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t mem_size = mem1->mem_size;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == Low_reg_Check(op0) ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.xor_(to_asmjit_reg(op0), word_ptr(rand1));
	}
	//BYTE
	if (mem_size == 1) {
		b.xor_(to_asmjit_reg(op0), byte_ptr(rand1));
	}

	b.pop(rand1);

	return xor_reg_mem_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_xor_mem_reg_16_8(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t mem_size = mem0->mem_size;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == Low_reg_Check(op1) ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.xor_(word_ptr(rand0), to_asmjit_reg(op1));
	}
	//BYTE
	if (mem_size == 1) {
		b.xor_(byte_ptr(rand0), to_asmjit_reg(op1));
	}

	b.pop(rand0);

	return xor_mem_reg_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_xor_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1)
{
	uint8_t mem_size = mem0->mem_size;
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == mem0->base ||
		randreg0 == mem0->index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	//x16
	if (mem_size == 2) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, ax), imm1);
		_xor_mem_reg_16_8(mem0, Low_reg_2(randreg0, ax));
	}
	//x8
	if (mem_size == 1) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, al), imm1);
		_xor_mem_reg_16_8(mem0, Low_reg_2(randreg0, al));
	}
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return xor_mem_imm_16_8;
}


UINT x86Insn_Mutation::_and()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };


	//���ж��Ƿ���x16��x8����
	if (x86->operands[0].size != 4 || x86->operands[1].size != 4) {
		//and reg,reg_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_and_reg_reg_16_8(x86->operands[0].reg, x86->operands[1].reg));
		//and reg,imm_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_and_reg_imm_16_8(x86->operands[0].reg, &imm));
		}
		//and_reg_mem_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_and_reg_mem_16_8(x86->operands[0].reg, &mem));
		}
		//and_mem_reg_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_and_mem_reg_16_8(&mem, x86->operands[1].reg));
		}
		//and_mem_imm_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_and_mem_imm_16_8(&mem, &imm));
		}
	}

	//x32�������û����λ�����жϴ����������������ж��Ƿ���x16��x8����	
	if (x86->op_count == 2) {
		//and reg,reg
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_and_reg_reg(x86->operands[0].reg, x86->operands[1].reg));
		//and reg,imm
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_and_reg_imm(x86->operands[0].reg, &imm));
		}
		//and_reg_mem
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_and_reg_mem(x86->operands[0].reg, &mem));
		}
		//and_mem_reg
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_and_mem_reg(&mem, x86->operands[1].reg));
		}
		//and_mem_imm
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_and_mem_imm(&mem, &imm));
		}
	}

	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����
UINT x86Insn_Mutation::_and_reg_reg(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false || Check_Reg(op1) == false)
		throw "�����reg����";

	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == op1 ||
		randreg1 == X86_REG_ESP);

	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler b(&Mut_Code);
	b.and_(reg_op0, rand1);
	b.pop(rand1);

	return and_reg_reg;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����						
UINT x86Insn_Mutation::_and_reg_imm(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == op0 || randreg0 == X86_REG_ESP);

	auto rand0 = to_asmjit_reg(randreg0);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	UINT temp = 0;
	bool Re_flag = RelocData_imm_mem(address + imm_offset, rand0, &temp);
	//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
	x86::Assembler b(&Mut_Code);
	//�ж��Ƿ���Ҫ�ض�λ
	if (Re_flag) {
		b.add(rand0, temp);
	}
	else {
		b.add(rand0, (UINT)imm_value);
	}
	b.popfd();
	b.and_(reg_op0, rand0);

	b.pop(rand0);

	return and_reg_imm;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_and_reg_mem(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1, randreg0;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op0 ||
		randreg0 == randreg1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP
		);

	auto rand1 = to_asmjit_reg(randreg1);
	auto rand0 = to_asmjit_reg(randreg0);



	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand0);
	b.mov(rand0, 0);

	b.pushfd();
	b.mov(Low_reg(randreg0, ax), word_ptr(rand1, 2));
	b.rcl(rand0, 0x10);
	b.mov(Low_reg(randreg0, ax), ptr(rand1));
	b.popfd();

	b.and_(to_asmjit_reg(op0), rand0);

	b.pop(rand0);
	b.pop(rand1);


	return and_reg_mem;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_and_mem_reg(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op1) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op1 ||
		randreg1 == randreg0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP
		);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler c(&Mut_Code);
	c.and_(ptr(rand0), rand1);

	c.pop(rand1);
	c.pop(rand0);

	return and_mem_reg;
}
//memֻ֧��4�ֽڴ�С����֧��esp��
UINT x86Insn_Mutation::_and_mem_imm(x86_mem* mem0, x86_imm* imm1)
{
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == mem0->base || randreg0 == mem0->index);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	_mov_reg_imm(randreg0, imm1);
	_and_mem_reg(mem0, randreg0);
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return and_mem_imm;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_and_reg_reg_16_8(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	//addָ���޷��ԶμĴ���������������
	/*
	//��1�������op0�ǶμĴ�����op1��ͨ�üĴ���
	if (Check_SReg(op0) == true && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_sreg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	//��2�������op0��ͨ�üĴ�����op1�ǶμĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == true) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_sreg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	*/
	//��3�������2��op����ͨ�üĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.and_(asmjit_op0, asmjit_op1);
	}

	return and_reg_reg_16_8;
}
//����x16��x8����ļ���		
UINT x86Insn_Mutation::_and_reg_imm_16_8(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);

	//����imm����add���μĴ������������ﲻ���μĴ������жϡ���x16��x8��add_reg_imm���������ض�λ���
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == Low_reg_Check(op0) ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	//x16
	if (imm_size == 2) {
		a.pushfd();
		a.add(Low_reg(randreg1, ax), (WORD)imm_value);
		a.popfd();
		a.and_(to_asmjit_reg(op0), Low_reg(randreg1, ax));
	}
	//x8
	if (imm_size == 1) {
		a.pushfd();
		a.add(Low_reg(randreg1, al), (BYTE)imm_value);
		a.popfd();
		a.and_(to_asmjit_reg(op0), Low_reg(randreg1, al));
	}
	a.pop(rand1);


	return and_reg_imm_16_8;
}
//����x16��x8����ļ���	
UINT x86Insn_Mutation::_and_reg_mem_16_8(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t mem_size = mem1->mem_size;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == Low_reg_Check(op0) ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.and_(to_asmjit_reg(op0), word_ptr(rand1));
	}
	//BYTE
	if (mem_size == 1) {
		b.and_(to_asmjit_reg(op0), byte_ptr(rand1));
	}

	b.pop(rand1);

	return and_reg_mem_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_and_mem_reg_16_8(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t mem_size = mem0->mem_size;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == Low_reg_Check(op1) ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.and_(word_ptr(rand0), to_asmjit_reg(op1));
	}
	//BYTE
	if (mem_size == 1) {
		b.and_(byte_ptr(rand0), to_asmjit_reg(op1));
	}

	b.pop(rand0);

	return and_mem_reg_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_and_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1)
{
	uint8_t mem_size = mem0->mem_size;
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == mem0->base ||
		randreg0 == mem0->index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	//x16
	if (mem_size == 2) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, ax), imm1);
		_and_mem_reg_16_8(mem0, Low_reg_2(randreg0, ax));
	}
	//x8
	if (mem_size == 1) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, al), imm1);
		_and_mem_reg_16_8(mem0, Low_reg_2(randreg0, al));
	}
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return and_mem_imm_16_8;
}


UINT x86Insn_Mutation::_or()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };


	//���ж��Ƿ���x16��x8����
	if (x86->operands[0].size != 4 || x86->operands[1].size != 4) {
		//or reg,reg_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_or_reg_reg_16_8(x86->operands[0].reg, x86->operands[1].reg));
		//or reg,imm_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_or_reg_imm_16_8(x86->operands[0].reg, &imm));
		}
		//or_reg_mem_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_or_reg_mem_16_8(x86->operands[0].reg, &mem));
		}
		//or_mem_reg_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_or_mem_reg_16_8(&mem, x86->operands[1].reg));
		}
		//or_mem_imm_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_or_mem_imm_16_8(&mem, &imm));
		}
	}

	//x32�������û����λ�����жϴ����������������ж��Ƿ���x16��x8����	
	if (x86->op_count == 2) {
		//or reg,reg
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_or_reg_reg(x86->operands[0].reg, x86->operands[1].reg));
		//or reg,imm
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_or_reg_imm(x86->operands[0].reg, &imm));
		}
		//or_reg_mem
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_or_reg_mem(x86->operands[0].reg, &mem));
		}
		//or_mem_reg
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_or_mem_reg(&mem, x86->operands[1].reg));
		}
		//or_mem_imm
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_or_mem_imm(&mem, &imm));
		}
	}

	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����
UINT x86Insn_Mutation::_or_reg_reg(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false || Check_Reg(op1) == false)
		throw "�����reg����";

	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == op1 ||
		randreg1 == X86_REG_ESP);

	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler b(&Mut_Code);
	b.or_(reg_op0, rand1);
	b.pop(rand1);

	return or_reg_reg;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����						
UINT x86Insn_Mutation::_or_reg_imm(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == op0 || randreg0 == X86_REG_ESP);

	auto rand0 = to_asmjit_reg(randreg0);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	UINT temp = 0;
	bool Re_flag = RelocData_imm_mem(address + imm_offset, rand0, &temp);
	//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
	x86::Assembler b(&Mut_Code);
	//�ж��Ƿ���Ҫ�ض�λ
	if (Re_flag) {
		b.add(rand0, temp);
	}
	else {
		b.add(rand0, (UINT)imm_value);
	}
	b.popfd();
	b.or_(reg_op0, rand0);

	b.pop(rand0);

	return or_reg_imm;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_or_reg_mem(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1, randreg0;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op0 ||
		randreg0 == randreg1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP
		);

	auto rand1 = to_asmjit_reg(randreg1);
	auto rand0 = to_asmjit_reg(randreg0);



	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand0);
	b.mov(rand0, 0);

	b.pushfd();
	b.mov(Low_reg(randreg0, ax), word_ptr(rand1, 2));
	b.rcl(rand0, 0x10);
	b.mov(Low_reg(randreg0, ax), ptr(rand1));
	b.popfd();

	b.or_(to_asmjit_reg(op0), rand0);

	b.pop(rand0);
	b.pop(rand1);


	return or_reg_mem;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_or_mem_reg(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op1) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op1 ||
		randreg1 == randreg0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP
		);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler c(&Mut_Code);
	c.or_(ptr(rand0), rand1);

	c.pop(rand1);
	c.pop(rand0);

	return or_mem_reg;
}
//memֻ֧��4�ֽڴ�С����֧��esp��
UINT x86Insn_Mutation::_or_mem_imm(x86_mem* mem0, x86_imm* imm1)
{
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == mem0->base || randreg0 == mem0->index);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	_mov_reg_imm(randreg0, imm1);
	_or_mem_reg(mem0, randreg0);
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return or_mem_imm;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_or_reg_reg_16_8(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	//addָ���޷��ԶμĴ���������������
	/*
	//��1�������op0�ǶμĴ�����op1��ͨ�üĴ���
	if (Check_SReg(op0) == true && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_sreg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	//��2�������op0��ͨ�üĴ�����op1�ǶμĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == true) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_sreg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	*/
	//��3�������2��op����ͨ�üĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.or_(asmjit_op0, asmjit_op1);
	}

	return or_reg_reg_16_8;
}
//����x16��x8����ļ���		
UINT x86Insn_Mutation::_or_reg_imm_16_8(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);

	//����imm����add���μĴ������������ﲻ���μĴ������жϡ���x16��x8��add_reg_imm���������ض�λ���
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == Low_reg_Check(op0) ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	//x16
	if (imm_size == 2) {
		a.pushfd();
		a.add(Low_reg(randreg1, ax), (WORD)imm_value);
		a.popfd();
		a.or_(to_asmjit_reg(op0), Low_reg(randreg1, ax));
	}
	//x8
	if (imm_size == 1) {
		a.pushfd();
		a.add(Low_reg(randreg1, al), (BYTE)imm_value);
		a.popfd();
		a.or_(to_asmjit_reg(op0), Low_reg(randreg1, al));
	}
	a.pop(rand1);


	return or_reg_imm_16_8;
}
//����x16��x8����ļ���	
UINT x86Insn_Mutation::_or_reg_mem_16_8(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t mem_size = mem1->mem_size;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == Low_reg_Check(op0) ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.or_(to_asmjit_reg(op0), word_ptr(rand1));
	}
	//BYTE
	if (mem_size == 1) {
		b.or_(to_asmjit_reg(op0), byte_ptr(rand1));
	}

	b.pop(rand1);

	return or_reg_mem_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_or_mem_reg_16_8(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t mem_size = mem0->mem_size;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == Low_reg_Check(op1) ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.or_(word_ptr(rand0), to_asmjit_reg(op1));
	}
	//BYTE
	if (mem_size == 1) {
		b.or_(byte_ptr(rand0), to_asmjit_reg(op1));
	}

	b.pop(rand0);

	return or_mem_reg_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_or_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1)
{
	uint8_t mem_size = mem0->mem_size;
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == mem0->base ||
		randreg0 == mem0->index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	//x16
	if (mem_size == 2) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, ax), imm1);
		_or_mem_reg_16_8(mem0, Low_reg_2(randreg0, ax));
	}
	//x8
	if (mem_size == 1) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, al), imm1);
		_or_mem_reg_16_8(mem0, Low_reg_2(randreg0, al));
	}
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return or_mem_imm_16_8;
}


//�޷�����ģ��eflags�����ڼ�С���ȶ���
UINT x86Insn_Mutation::_rcl()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_imm imm = { 0 };

	//ֻ��rcl reg,imm�����������
	if (x86->operands[0].size == 4 && x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
		imm.address = (DWORD)insn.address;
		imm.imm_value = (DWORD)x86->operands[1].imm;
		imm.imm_offset = x86->encoding.imm_offset;
		imm.imm_size = x86->encoding.imm_size;
		return(_rcl_reg_imm(x86->operands[0].reg, &imm));
	}

	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����						
UINT x86Insn_Mutation::_rcl_reg_imm(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == op0 || randreg1 == X86_REG_ESP);

	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);
	UINT randimm1 = rand() % (imm_value + 1);


	a.push(rand1);
	a.mov(rand1, reg_op0);
	a.rcl(rand1, randimm1);
	_mov_reg_reg(op0, randreg1);
	x86::Assembler b(&Mut_Code);
	b.rcl(rand1, (UINT)imm_value - randimm1);
	_mov_reg_reg(op0, randreg1);
	x86::Assembler c(&Mut_Code);
	c.pop(rand1);


	return rcl_reg_imm;
}
//�޷�����ģ��eflags�����ڼ�С���ȶ���
UINT x86Insn_Mutation::_rcr()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_imm imm = { 0 };

	//ֻ��rcr reg,imm�����������
	if (x86->operands[0].size == 4 && x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
		imm.address = (DWORD)insn.address;
		imm.imm_value = (DWORD)x86->operands[1].imm;
		imm.imm_offset = x86->encoding.imm_offset;
		imm.imm_size = x86->encoding.imm_size;
		return(_rcr_reg_imm(x86->operands[0].reg, &imm));
	}

	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����						
UINT x86Insn_Mutation::_rcr_reg_imm(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == op0 || randreg1 == X86_REG_ESP);

	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);
	UINT randimm1 = rand() % (imm_value + 1);


	a.push(rand1);
	a.mov(rand1, reg_op0);
	a.rcr(rand1, randimm1);
	_mov_reg_reg(op0, randreg1);
	x86::Assembler b(&Mut_Code);
	b.rcr(rand1, (UINT)imm_value - randimm1);
	_mov_reg_reg(op0, randreg1);
	x86::Assembler c(&Mut_Code);
	c.pop(rand1);


	return rcr_reg_imm;
}


UINT x86Insn_Mutation::_lea()
{
	/*
	leaָ���Ǹ��ر��ָ�lea eax,[bx]��x32λ������ʹ�ö���������
	��mov�϶����������������x32λһ�ܾͱ��ˣ�
	*/
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };

	//���ж��Ƿ���x16����
	if (x86->operands[0].size != 4 || x86->operands[1].size != 4) {
		//lea_reg_mem_16
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_lea_reg_mem_16(x86->operands[0].reg, &mem));
		}
	}
	//x32�������û����λ�����жϴ����������������ж��Ƿ���x16��x8����	
	if (x86->op_count == 2) {
		//lea_reg_mem
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_lea_reg_mem(x86->operands[0].reg, &mem));
		}
	}
	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����memֻ֧��4�ֽڴ�С��base��index����Ϊ16λ����	
UINT x86Insn_Mutation::_lea_reg_mem(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);
	//���mem������16λ�Ĵ���(bx,bp,si,di)��ֱ�ӵ���δָ֪��
	if (base != X86_REG_INVALID && Check_Reg(base) == false)
		return -1;
	if (index != X86_REG_INVALID && Check_Reg(index) == false)
		return -1;


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1, randreg0;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op0 ||
		randreg0 == randreg1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP
		);

	auto rand1 = to_asmjit_reg(randreg1);
	auto rand0 = to_asmjit_reg(randreg0);


	a.pushfd();
	a.push(rand1);

	a.mov(rand1, 0);
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}

	b.push(rand0);
	_mov_reg_reg(randreg0, randreg1);
	_mov_reg_reg(op0, randreg0);
	x86::Assembler c(&Mut_Code);

	c.pop(rand0);
	c.pop(rand1);
	c.popfd();

	return lea_reg_mem;
}
//����x16����ļ��ݣ�base��index����Ϊ16λ��
UINT x86Insn_Mutation::_lea_reg_mem_16(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t mem_size = mem1->mem_size;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);
	//���mem������16λ�Ĵ���(bx,bp,si,di)��ֱ�ӵ���δָ֪��
	if (base != X86_REG_INVALID && Check_Reg(base) == false)
		return -1;
	if (index != X86_REG_INVALID && Check_Reg(index) == false)
		return -1;


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == Low_reg_Check(op0) ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand1 = to_asmjit_reg(randreg1);


	a.pushfd();
	a.push(rand1);

	a.mov(rand1, 0);
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}

	//WORD
	if (mem_size == 2) {
		b.mov(to_asmjit_reg(op0), Low_reg(randreg1, ax));
	}


	b.pop(rand1);
	b.popfd();

	return lea_reg_mem_16;
}


UINT x86Insn_Mutation::_cmp()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };


	//���ж��Ƿ���x16��x8����
	if (x86->operands[0].size != 4 || x86->operands[1].size != 4) {
		//cmp reg,reg_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_cmp_reg_reg_16_8(x86->operands[0].reg, x86->operands[1].reg));
		//cmp reg,imm_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_cmp_reg_imm_16_8(x86->operands[0].reg, &imm));
		}
		//cmp_reg_mem_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_cmp_reg_mem_16_8(x86->operands[0].reg, &mem));
		}
		//cmp_mem_reg_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_cmp_mem_reg_16_8(&mem, x86->operands[1].reg));
		}
		//cmp_mem_imm_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_cmp_mem_imm_16_8(&mem, &imm));
		}
	}

	//x32�������û����λ�����жϴ����������������ж��Ƿ���x16��x8����	
	if (x86->op_count == 2) {
		//cmp reg,reg
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_cmp_reg_reg(x86->operands[0].reg, x86->operands[1].reg));
		//cmp reg,imm
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_cmp_reg_imm(x86->operands[0].reg, &imm));
		}
		//cmp_reg_mem
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[1].mem.base;
			mem.index = x86->operands[1].mem.index;
			mem.scale = x86->operands[1].mem.scale;
			mem.disp = x86->operands[1].mem.disp;
			mem.mem_size = x86->operands[1].size;
			return(_cmp_reg_mem(x86->operands[0].reg, &mem));
		}
		//cmp_mem_reg
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_cmp_mem_reg(&mem, x86->operands[1].reg));
		}
		//cmp_mem_imm
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_cmp_mem_imm(&mem, &imm));
		}
	}

	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����
UINT x86Insn_Mutation::_cmp_reg_reg(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false || Check_Reg(op1) == false)
		throw "�����reg����";

	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == op1 ||
		randreg1 == X86_REG_ESP);

	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler b(&Mut_Code);
	b.cmp(reg_op0, rand1);
	b.pop(rand1);

	return cmp_reg_reg;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����						
UINT x86Insn_Mutation::_cmp_reg_imm(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == op0 || randreg0 == X86_REG_ESP);

	auto rand0 = to_asmjit_reg(randreg0);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	UINT temp = 0;
	bool Re_flag = RelocData_imm_mem(address + imm_offset, rand0, &temp);
	//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
	x86::Assembler b(&Mut_Code);
	//�ж��Ƿ���Ҫ�ض�λ
	if (Re_flag) {
		b.add(rand0, temp);
	}
	else {
		b.add(rand0, (UINT)imm_value);
	}
	b.popfd();
	b.cmp(reg_op0, rand0);

	b.pop(rand0);

	return cmp_reg_imm;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_cmp_reg_mem(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1, randreg0;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op0 ||
		randreg0 == randreg1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP
		);

	auto rand1 = to_asmjit_reg(randreg1);
	auto rand0 = to_asmjit_reg(randreg0);



	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand0);
	b.mov(rand0, 0);

	b.pushfd();
	b.mov(Low_reg(randreg0, ax), word_ptr(rand1, 2));
	b.rcl(rand0, 0x10);
	b.mov(Low_reg(randreg0, ax), ptr(rand1));
	b.popfd();

	//��rand0 cmp op0
	b.cmp(to_asmjit_reg(op0), rand0);

	b.pop(rand0);
	b.pop(rand1);


	return cmp_reg_mem;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_cmp_mem_reg(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op1) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op1 ||
		randreg1 == randreg0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP
		);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler c(&Mut_Code);
	c.cmp(ptr(rand0), rand1);

	c.pop(rand1);
	c.pop(rand0);

	return cmp_mem_reg;
}
//memֻ֧��4�ֽڴ�С����֧��esp��
UINT x86Insn_Mutation::_cmp_mem_imm(x86_mem* mem0, x86_imm* imm1)
{
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == mem0->base || randreg0 == mem0->index);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	_mov_reg_imm(randreg0, imm1);
	_cmp_mem_reg(mem0, randreg0);
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return cmp_mem_imm;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_cmp_reg_reg_16_8(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	//addָ���޷��ԶμĴ���������������
	/*
	//��1�������op0�ǶμĴ�����op1��ͨ�üĴ���
	if (Check_SReg(op0) == true && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_sreg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	//��2�������op0��ͨ�üĴ�����op1�ǶμĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == true) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_sreg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	*/
	//��3�������2��op����ͨ�üĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.cmp(asmjit_op0, asmjit_op1);
	}

	return cmp_reg_reg_16_8;
}
//����x16��x8����ļ���		
UINT x86Insn_Mutation::_cmp_reg_imm_16_8(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);

	//����imm����add���μĴ������������ﲻ���μĴ������жϡ���x16��x8��add_reg_imm���������ض�λ���
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == Low_reg_Check(op0) ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	//x16
	if (imm_size == 2) {
		a.pushfd();
		a.add(Low_reg(randreg1, ax), (WORD)imm_value);
		a.popfd();
		a.cmp(to_asmjit_reg(op0), Low_reg(randreg1, ax));
	}
	//x8
	if (imm_size == 1) {
		a.pushfd();
		a.add(Low_reg(randreg1, al), (BYTE)imm_value);
		a.popfd();
		a.cmp(to_asmjit_reg(op0), Low_reg(randreg1, al));
	}
	a.pop(rand1);


	return cmp_reg_imm_16_8;
}
//����x16��x8����ļ���	
UINT x86Insn_Mutation::_cmp_reg_mem_16_8(x86_reg op0, x86_mem* mem1)
{
	DWORD address = mem1->address;
	uint8_t mem_size = mem1->mem_size;
	uint8_t disp_offset = mem1->disp_offset;
	uint8_t disp_size = mem1->disp_size;
	x86_reg base = mem1->base;
	x86_reg index = mem1->index;
	int scale = mem1->scale;
	int64_t disp = mem1->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == Low_reg_Check(op0) ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand1, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand1, temp);
		else
			b.add(rand1, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand1, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand1, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.cmp(to_asmjit_reg(op0), word_ptr(rand1));
	}
	//BYTE
	if (mem_size == 1) {
		b.cmp(to_asmjit_reg(op0), byte_ptr(rand1));
	}

	b.pop(rand1);

	return cmp_reg_mem_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_cmp_mem_reg_16_8(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t mem_size = mem0->mem_size;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == Low_reg_Check(op1) ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.cmp(word_ptr(rand0), to_asmjit_reg(op1));
	}
	//BYTE
	if (mem_size == 1) {
		b.cmp(byte_ptr(rand0), to_asmjit_reg(op1));
	}

	b.pop(rand0);

	return cmp_mem_reg_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_cmp_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1)
{
	uint8_t mem_size = mem0->mem_size;
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == mem0->base ||
		randreg0 == mem0->index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	//x16
	if (mem_size == 2) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, ax), imm1);
		_cmp_mem_reg_16_8(mem0, Low_reg_2(randreg0, ax));
	}
	//x8
	if (mem_size == 1) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, al), imm1);
		_cmp_mem_reg_16_8(mem0, Low_reg_2(randreg0, al));
	}
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return cmp_mem_imm_16_8;
}


UINT x86Insn_Mutation::_test()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };


	//���ж��Ƿ���x16��x8����
	if (x86->operands[0].size != 4 || x86->operands[1].size != 4) {
		//test reg,reg_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_test_reg_reg_16_8(x86->operands[0].reg, x86->operands[1].reg));
		//test reg,imm_16_8
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_test_reg_imm_16_8(x86->operands[0].reg, &imm));
		}
		//test_mem_reg_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_test_mem_reg_16_8(&mem, x86->operands[1].reg));
		}
		//test_mem_imm_16_8
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_test_mem_imm_16_8(&mem, &imm));
		}
	}

	//x32�������û����λ�����жϴ����������������ж��Ƿ���x16��x8����	
	if (x86->op_count == 2) {
		//test reg,reg
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_REG)
			return(_test_reg_reg(x86->operands[0].reg, x86->operands[1].reg));
		//test reg,imm
		if (x86->operands[0].type == X86_OP_REG && x86->operands[1].type == X86_OP_IMM) {
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_test_reg_imm(x86->operands[0].reg, &imm));
		}
		//test_mem_reg
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_REG) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_test_mem_reg(&mem, x86->operands[1].reg));
		}
		//test_mem_imm
		if (x86->operands[0].type == X86_OP_MEM && x86->operands[1].type == X86_OP_IMM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			imm.address = (DWORD)insn.address;
			imm.imm_value = (DWORD)x86->operands[1].imm;
			imm.imm_offset = x86->encoding.imm_offset;
			imm.imm_size = x86->encoding.imm_size;
			return(_test_mem_imm(&mem, &imm));
		}
	}

	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����
UINT x86Insn_Mutation::_test_reg_reg(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false || Check_Reg(op1) == false)
		throw "�����reg����";

	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op0 ||
		randreg1 == op1 ||
		randreg1 == X86_REG_ESP);

	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler b(&Mut_Code);
	b.test(reg_op0, rand1);
	b.pop(rand1);

	return test_reg_reg;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����						
UINT x86Insn_Mutation::_test_reg_imm(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == op0 || randreg0 == X86_REG_ESP);

	auto rand0 = to_asmjit_reg(randreg0);
	auto reg_op0 = to_asmjit_reg(op0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	UINT temp = 0;
	bool Re_flag = RelocData_imm_mem(address + imm_offset, rand0, &temp);
	//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
	x86::Assembler b(&Mut_Code);
	//�ж��Ƿ���Ҫ�ض�λ
	if (Re_flag) {
		b.add(rand0, temp);
	}
	else {
		b.add(rand0, (UINT)imm_value);
	}
	b.popfd();
	b.test(reg_op0, rand0);

	b.pop(rand0);

	return test_reg_imm;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�����memֻ֧��4�ֽڴ�С����֧��esp����		
UINT x86Insn_Mutation::_test_mem_reg(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op1) == false)
		throw "�����reg����";


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == op1 ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == op1 ||
		randreg1 == randreg0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP
		);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	b.push(rand1);
	_mov_reg_reg(randreg1, op1);
	x86::Assembler c(&Mut_Code);
	c.test(ptr(rand0), rand1);

	c.pop(rand1);
	c.pop(rand0);

	return test_mem_reg;
}
//memֻ֧��4�ֽڴ�С����֧��esp��
UINT x86Insn_Mutation::_test_mem_imm(x86_mem* mem0, x86_imm* imm1)
{
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == mem0->base || randreg0 == mem0->index);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	_mov_reg_imm(randreg0, imm1);
	_test_mem_reg(mem0, randreg0);
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return test_mem_imm;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_test_reg_reg_16_8(x86_reg op0, x86_reg op1)
{
	x86::Assembler a(&Mut_Code);
	//addָ���޷��ԶμĴ���������������
	/*
	//��1�������op0�ǶμĴ�����op1��ͨ�üĴ���
	if (Check_SReg(op0) == true && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_sreg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	//��2�������op0��ͨ�üĴ�����op1�ǶμĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == true) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_sreg(op1);
		a.mov(asmjit_op0, asmjit_op1);
	}
	*/
	//��3�������2��op����ͨ�üĴ���
	if (Check_SReg(op0) == false && Check_SReg(op1) == false) {
		auto asmjit_op0 = to_asmjit_reg(op0);
		auto asmjit_op1 = to_asmjit_reg(op1);
		a.test(asmjit_op0, asmjit_op1);
	}

	return test_reg_reg_16_8;
}
//����x16��x8����ļ���		
UINT x86Insn_Mutation::_test_reg_imm_16_8(x86_reg op0, x86_imm* imm1)
{
	DWORD address = imm1->address;
	DWORD imm_value = imm1->imm_value;
	uint8_t imm_offset = imm1->imm_offset;
	uint8_t imm_size = imm1->imm_size;
	x86::Assembler a(&Mut_Code);

	//����imm����add���μĴ������������ﲻ���μĴ������жϡ���x16��x8��add_reg_imm���������ض�λ���
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg1;
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == Low_reg_Check(op0) ||
		randreg1 == X86_REG_ESP ||
		randreg1 == X86_REG_EBP ||
		randreg1 == X86_REG_ESI ||
		randreg1 == X86_REG_EDI);

	auto rand1 = to_asmjit_reg(randreg1);


	a.push(rand1);
	a.mov(rand1, 0);
	//x16
	if (imm_size == 2) {
		a.pushfd();
		a.add(Low_reg(randreg1, ax), (WORD)imm_value);
		a.popfd();
		a.test(to_asmjit_reg(op0), Low_reg(randreg1, ax));
	}
	//x8
	if (imm_size == 1) {
		a.pushfd();
		a.add(Low_reg(randreg1, al), (BYTE)imm_value);
		a.popfd();
		a.test(to_asmjit_reg(op0), Low_reg(randreg1, al));
	}
	a.pop(rand1);


	return test_reg_imm_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_test_mem_reg_16_8(x86_mem* mem0, x86_reg op1)
{
	DWORD address = mem0->address;
	uint8_t mem_size = mem0->mem_size;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == Low_reg_Check(op1) ||
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);

	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	a.mov(rand0, 0);
	a.pushfd();
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}
	b.popfd();

	//WORD
	if (mem_size == 2) {
		b.test(word_ptr(rand0), to_asmjit_reg(op1));
	}
	//BYTE
	if (mem_size == 1) {
		b.test(byte_ptr(rand0), to_asmjit_reg(op1));
	}

	b.pop(rand0);

	return test_mem_reg_16_8;
}
//����x16��x8����ļ���
UINT x86Insn_Mutation::_test_mem_imm_16_8(x86_mem* mem0, x86_imm* imm1)
{
	uint8_t mem_size = mem0->mem_size;
	x86::Assembler a(&Mut_Code);
	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == mem0->base ||
		randreg0 == mem0->index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI);
	auto rand0 = to_asmjit_reg(randreg0);


	a.push(rand0);
	//x16
	if (mem_size == 2) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, ax), imm1);
		_test_mem_reg_16_8(mem0, Low_reg_2(randreg0, ax));
	}
	//x8
	if (mem_size == 1) {
		_mov_reg_imm_16_8(Low_reg_2(randreg0, al), imm1);
		_test_mem_reg_16_8(mem0, Low_reg_2(randreg0, al));
	}
	x86::Assembler b(&Mut_Code);
	b.pop(rand0);


	return test_mem_imm_16_8;
}




UINT x86Insn_Mutation::_push()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };


	//���ж��Ƿ���x16��x8����
	if (x86->operands[0].size != 4) {
		//push reg_16
		if (x86->operands[0].type == X86_OP_REG)
			return(_push_reg_16(x86->operands[0].reg));
		//push mem_16
		if (x86->operands[0].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_push_mem_16(&mem));
		}
	}

	//push reg
	if (x86->operands[0].type == X86_OP_REG)
		return(_push_reg(x86->operands[0].reg));
	//push imm
	if (x86->operands[0].type == X86_OP_IMM) {
		imm.address = (DWORD)insn.address;
		imm.imm_value = (DWORD)x86->operands[0].imm;
		imm.imm_offset = x86->encoding.imm_offset;
		imm.imm_size = x86->encoding.imm_size;
		return(_push_imm(&imm));
	}
	//push mem
	if (x86->operands[0].type == X86_OP_MEM) {
		mem.address = (DWORD)insn.address;
		mem.disp_offset = x86->encoding.disp_offset;
		mem.disp_size = x86->encoding.disp_size;
		mem.base = x86->operands[0].mem.base;
		mem.index = x86->operands[0].mem.index;
		mem.scale = x86->operands[0].mem.scale;
		mem.disp = x86->operands[0].mem.disp;
		mem.mem_size = x86->operands[0].size;
		return(_push_mem(&mem));
	}

	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����
UINT x86Insn_Mutation::_push_reg(x86_reg op0)
{
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";

	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	x86_mem mem = { 0 };
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == op0 || randreg0 == X86_REG_ESP);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == op0 || randreg1 == X86_REG_ESP || randreg1 == randreg0);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);
	mem.base = randreg1;


	a.push(rand1);						//��ǰ��һ���ռ䣬����[esp-4]��ֵ�õ�
	a.pushfd();
	a.push(rand0);
	a.push(rand1);

	a.lea(rand1, ptr(x86::esp, 16));	//rand1Ϊesp
	a.mov(rand0, reg_op0);
	a.sub(rand1, 4);
	_mov_mem_reg(&mem, randreg0);		//mov [rand1],rand0

	x86::Assembler b(&Mut_Code);
	b.pop(rand1);
	b.pop(rand0);
	b.popfd();

	return push_reg;
}

UINT x86Insn_Mutation::_push_imm(x86_imm* imm0)
{
	DWORD address = imm0->address;
	DWORD imm_value = imm0->imm_value;
	uint8_t imm_offset = imm0->imm_offset;
	uint8_t imm_size = imm0->imm_size;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	x86_mem mem = { 0 };
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);
	mem.base = randreg1;


	a.push(rand1);						//��ǰ��һ���ռ�
	a.pushfd();
	a.push(rand0);
	a.push(rand1);

	a.lea(rand1, ptr(x86::esp, 16));	//rand1Ϊesp
	a.mov(rand0, 0);
	UINT temp = 0;
	UINT randimm1 = rand();
	bool Re_flag = RelocData_imm_mem(address + imm_offset, rand0, &temp);
	//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
	x86::Assembler b(&Mut_Code);
	//�ж��Ƿ���Ҫ�ض�λ
	if (Re_flag) {
		temp -= randimm1;
		b.add(rand0, temp);
	}
	else {
		imm_value -= randimm1;
		b.add(rand0, (UINT)imm_value);
	}
	b.add(rand0, randimm1);

	b.sub(rand1, 4);
	_mov_mem_reg(&mem, randreg0);		//mov [rand1],rand0

	x86::Assembler c(&Mut_Code);
	c.pop(rand1);
	c.pop(rand0);
	c.popfd();

	return push_imm;
}
//memֻ֧��4�ֽڴ�С����֧��esp��
UINT x86Insn_Mutation::_push_mem(x86_mem* mem0)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	x86_mem mem = { 0 };
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == base || randreg0 == index);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == X86_REG_ESP || randreg1 == randreg0 || randreg1 == base || randreg1 == index);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);
	mem.base = randreg1;


	a.push(rand1);								//��ǰ��һ���ռ�
	a.pushfd();
	a.push(rand0);
	a.push(rand1);

	a.lea(rand1, ptr(x86::esp, 16));			//rand1Ϊesp
	a.sub(rand1, 4);
	_mov_reg_mem(randreg0, mem0);				//mov rand0,mem0
	_mov_mem_reg(&mem, randreg0);				//mov [rand1],rand0

	x86::Assembler b(&Mut_Code);
	b.pop(rand1);
	b.pop(rand0);
	b.popfd();

	return push_mem;
}
//����x16����ļ���
UINT x86Insn_Mutation::_push_reg_16(x86_reg op0)
{
	x86::Assembler a(&Mut_Code);
	if (Check_SReg(op0) == true)
		a.push(to_asmjit_sreg(op0));
	else
		a.push(to_asmjit_reg(op0));

	return push_reg_16;
}
//����x16����ļ���
UINT x86Insn_Mutation::_push_mem_16(x86_mem* mem0)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	x86_mem mem = { 0 };
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == base || randreg0 == index);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == X86_REG_ESP || randreg1 == randreg0 || randreg1 == base || randreg1 == index);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);
	mem.base = randreg1;
	mem.mem_size = 2;


	a.push(Low_reg(randreg1, ax));					//��ǰ��һ���ռ�
	a.pushfd();
	a.push(rand0);
	a.push(rand1);
	a.mov(rand0, 0);

	a.lea(rand1, ptr(x86::esp, 14));				//rand1Ϊesp
	a.sub(rand1, 2);								//16λpush��ÿ��ѹջ��2���ֽ�
	_mov_reg_mem_16_8(Low_reg_2(randreg0, ax), mem0);//mov rand0_ax,mem0
	_mov_mem_reg_16_8(&mem, Low_reg_2(randreg0, ax));//mov [rand1],rand0_ax

	x86::Assembler b(&Mut_Code);
	b.pop(rand1);
	b.pop(rand0);
	b.popfd();

	return push_mem_16;
}


UINT x86Insn_Mutation::_pop()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_mem mem = { 0 };
	x86_imm imm = { 0 };


	//���ж��Ƿ���x16��x8����
	if (x86->operands[0].size != 4) {
		//pop reg_16
		if (x86->operands[0].type == X86_OP_REG)
			return(_pop_reg_16(x86->operands[0].reg));
		//pop mem_16
		if (x86->operands[0].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_pop_mem_16(&mem));
		}
	}

	//pop reg
	if (x86->operands[0].type == X86_OP_REG)
		return(_pop_reg(x86->operands[0].reg));
	//pop mem
	if (x86->operands[0].type == X86_OP_MEM) {
		mem.address = (DWORD)insn.address;
		mem.disp_offset = x86->encoding.disp_offset;
		mem.disp_size = x86->encoding.disp_size;
		mem.base = x86->operands[0].mem.base;
		mem.index = x86->operands[0].mem.index;
		mem.scale = x86->operands[0].mem.scale;
		mem.disp = x86->operands[0].mem.disp;
		mem.mem_size = x86->operands[0].size;
		return(_pop_mem(&mem));
	}

	return result;
}
//opֻ֧��eax��ebx��ecx��edx��ebp��x32λ�Ĵ�������֧��esp����
UINT x86Insn_Mutation::_pop_reg(x86_reg op0)
{
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";

	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	x86_mem mem = { 0 };
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == op0 || randreg0 == X86_REG_ESP);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == op0 || randreg1 == X86_REG_ESP || randreg1 == randreg0);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);
	auto reg_op0 = to_asmjit_reg(op0);
	mem.base = randreg1;


	a.pushfd();
	a.push(rand0);
	a.push(rand1);

	a.lea(rand1, ptr(x86::esp, 12));	//rand1Ϊesp
	_mov_reg_mem(randreg0, &mem);		//mov rand0,[rand1]
	x86::Assembler b(&Mut_Code);
	b.mov(reg_op0, rand0);

	b.pop(rand1);
	b.pop(rand0);
	b.popfd();
	b.lea(x86::esp, ptr(x86::esp, 4));	//add esp,4

	return pop_reg;
}
//memֻ֧��4�ֽڴ�С����֧��esp��
UINT x86Insn_Mutation::_pop_mem(x86_mem* mem0)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	x86_mem mem = { 0 };
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == base || randreg0 == index);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == X86_REG_ESP || randreg1 == randreg0 || randreg1 == base || randreg1 == index);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);
	mem.base = randreg1;


	a.pushfd();
	a.push(rand0);
	a.push(rand1);

	a.lea(rand1, ptr(x86::esp, 12));			//rand1Ϊesp
	_mov_reg_mem(randreg0, &mem);				//mov rand0,[rand1]
	_mov_mem_reg(mem0, randreg0);				//mov [mem0],rand0

	x86::Assembler b(&Mut_Code);
	b.pop(rand1);
	b.pop(rand0);
	b.popfd();
	b.lea(x86::esp, ptr(x86::esp, 4));			//add esp,4

	return pop_mem;
}
//����x16����ļ���
UINT x86Insn_Mutation::_pop_reg_16(x86_reg op0)
{
	x86::Assembler a(&Mut_Code);
	if (Check_SReg(op0) == true)
		a.pop(to_asmjit_sreg(op0));
	else
		a.pop(to_asmjit_reg(op0));

	return pop_reg_16;
}
//����x16����ļ���
UINT x86Insn_Mutation::_pop_mem_16(x86_mem* mem0)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	x86_mem mem = { 0 };
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP || randreg0 == base || randreg0 == index);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg1 == X86_REG_ESP || randreg1 == randreg0 || randreg1 == base || randreg1 == index);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);
	mem.base = randreg1;
	mem.mem_size = 2;


	a.pushfd();
	a.push(rand0);
	a.push(rand1);
	a.mov(rand0, 0);

	a.lea(rand1, ptr(x86::esp, 12));				//rand1Ϊesp
	_mov_reg_mem_16_8(Low_reg_2(randreg0, ax), &mem);//mov rand0_ax,[rand1]
	_mov_mem_reg_16_8(mem0, Low_reg_2(randreg0, ax));//mov [mem0],rand0_ax

	x86::Assembler b(&Mut_Code);
	b.pop(rand1);
	b.pop(rand0);
	b.popfd();
	b.lea(x86::esp, ptr(x86::esp, 2));				//add esp,2

	return pop_mem_16;
}


UINT x86Insn_Mutation::_call()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_jcc jcc = { 0 };
	x86_mem mem = { 0 };

	//call reg
	if (x86->operands[0].type == X86_OP_REG)
		return(_call_reg(x86->operands[0].reg));
	//call imm
	if (x86->operands[0].type == X86_OP_IMM) {
		jcc.address = (DWORD)insn.address;
		jcc.imm_offset = x86->encoding.imm_offset;
		jcc.imm_size = x86->encoding.imm_size;
		jcc.Target_JumpAddr = (DWORD)x86->operands[0].imm;
		return(_call_imm(&jcc));
	}
	//call mem
	if (x86->operands[0].type == X86_OP_MEM) {
		mem.address = (DWORD)insn.address;
		mem.disp_offset = x86->encoding.disp_offset;
		mem.disp_size = x86->encoding.disp_size;
		mem.base = x86->operands[0].mem.base;
		mem.index = x86->operands[0].mem.index;
		mem.scale = x86->operands[0].mem.scale;
		mem.disp = x86->operands[0].mem.disp;
		mem.mem_size = x86->operands[0].size;
		return(_call_mem(&mem));
	}

	return result;
}

UINT x86Insn_Mutation::_call_reg(x86_reg op0)
{
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";
	auto reg_op0 = to_asmjit_reg(op0);
	a.call(reg_op0);
	//��ʵ����call ax���������������x32�ﲻ�������ִ��룬���Բ������ˡ�
	return call_reg;
}

UINT x86Insn_Mutation::_call_imm(x86_jcc* jcc0)
{
	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}

	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (randreg0 == X86_REG_ESP);
	auto rand0 = to_asmjit_reg(randreg0);


	Label L0 = a.newLabel();
	Label L1 = a.newLabel();
	Label Jump_Success = a.newLabel();
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L0);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���

		//ԭcall
		a.call(Jcc_ActuAddr(Target_JumpAddr));
		CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L0);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.call((UINT)Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.call(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return call_imm;
}
//ת����call eax������ʧeax�Ĵ���
UINT x86Insn_Mutation::_call_mem(x86_mem* mem0)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI ||		//Ҫ�õ�8λ�Ĵ���
		randreg0 == X86_REG_EAX			//Ҫת����call eax�����Բ�����eax
		);

	auto rand0 = to_asmjit_reg(randreg0);

	a.pushfd();
	a.push(rand0);

	a.mov(rand0, 0);
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}

	b.mov(x86::eax, ptr(rand0));
	b.pop(rand0);
	b.popfd();

	b.call(x86::eax);
	return call_mem;

	/*
	UINT x86Insn_Mutation::_call_mem(x86_mem* mem0)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	//mem_disp�������ض�λ
	if (disp_size == 4) {
		size_t Temp_CodeSize = Mut_Code.codeSize();
		DWORD Call_BaseAddr = objPE.m_dwImageBase + objPE.m_dwImageSize + Final_CodeSize + Temp_CodeSize;
		DealWithReloc(address + disp_offset, Call_BaseAddr + disp_offset);
	}

	//call [base]:				call [eax]
	if (base != 0 && disp == 0 && index == 0)
		a.call(ptr(to_asmjit_reg(base)));
	//call [base+index*scale]:	call [eax+ecx]		call [eax+ecx*2]	call [eax+ecx*4]
	if (base != 0 && disp == 0 && index != 0)
		a.call(ptr(to_asmjit_reg(base), to_asmjit_reg(index), scale >> 1));
	//call [disp]:				call [0x401000]
	if (base == 0 && disp != 0 && index == 0)
		a.call(x86::dword_ptr((UINT)disp));
	//call [base+disp]:			call [eax+0x401000]
	if (base != 0 && disp != 0 && index == 0)
		a.call(ptr(to_asmjit_reg(base), (UINT)disp));
	//call [base+index+disp]:	call [eax+ecx*2+0x401000]
	if (base != 0 && disp != 0 && index != 0)
		a.call(ptr(to_asmjit_reg(base), to_asmjit_reg(index), scale >> 1, (UINT)disp));

	return call_mem;
}
	*/
}


UINT x86Insn_Mutation::_jcc_jmp()
{
	UINT result = -1;
	cs_x86 *x86;
	if (insn.detail == NULL)
		return result;
	x86 = &(insn.detail->x86);
	x86_jcc jcc = { 0 };
	x86_mem mem = { 0 };

	//jmp:
	if (strcmp(insn.mnemonic, "jmp") == 0) {
		//jmp reg
		if (x86->operands[0].type == X86_OP_REG)
			return(_jmp_reg(x86->operands[0].reg));
		//jmp imm
		if (x86->operands[0].type == X86_OP_IMM) {
			jcc.address = (DWORD)insn.address;
			jcc.imm_offset = x86->encoding.imm_offset;
			jcc.imm_size = x86->encoding.imm_size;
			jcc.Target_JumpAddr = (DWORD)x86->operands[0].imm;
			return(_jmp_imm(&jcc));
		}
		//jmp mem
		if (x86->operands[0].type == X86_OP_MEM) {
			mem.address = (DWORD)insn.address;
			mem.disp_offset = x86->encoding.disp_offset;
			mem.disp_size = x86->encoding.disp_size;
			mem.base = x86->operands[0].mem.base;
			mem.index = x86->operands[0].mem.index;
			mem.scale = x86->operands[0].mem.scale;
			mem.disp = x86->operands[0].mem.disp;
			mem.mem_size = x86->operands[0].size;
			return(_jmp_mem(&mem));
		}

	}


	//jcc:
	jcc.address = (DWORD)insn.address;
	jcc.imm_offset = x86->encoding.imm_offset;
	jcc.imm_size = x86->encoding.imm_size;
	jcc.Target_JumpAddr = (DWORD)x86->operands[0].imm;
	if (strcmp(insn.mnemonic, "je") == 0)
		return(_je(&jcc));
	if (strcmp(insn.mnemonic, "jne") == 0)
		return(_jne(&jcc));
	if (strcmp(insn.mnemonic, "ja") == 0)
		return(_ja(&jcc));
	if (strcmp(insn.mnemonic, "jae") == 0)
		return(_jae(&jcc));
	if (strcmp(insn.mnemonic, "jb") == 0)
		return(_jb(&jcc));
	if (strcmp(insn.mnemonic, "jbe") == 0)
		return(_jbe(&jcc));
	if (strcmp(insn.mnemonic, "jc") == 0)
		return(_jc(&jcc));
	if (strcmp(insn.mnemonic, "jecxz") == 0)
		return(_jecxz(&jcc));
	if (strcmp(insn.mnemonic, "jg") == 0)
		return(_jg(&jcc));
	if (strcmp(insn.mnemonic, "jge") == 0)
		return(_jge(&jcc));
	if (strcmp(insn.mnemonic, "jl") == 0)
		return(_jl(&jcc));
	if (strcmp(insn.mnemonic, "jle") == 0)
		return(_jle(&jcc));
	if (strcmp(insn.mnemonic, "jna") == 0)
		return(_jna(&jcc));
	if (strcmp(insn.mnemonic, "jnae") == 0)
		return(_jnae(&jcc));
	if (strcmp(insn.mnemonic, "jnb") == 0)
		return(_jnb(&jcc));
	if (strcmp(insn.mnemonic, "jnbe") == 0)
		return(_jnbe(&jcc));
	if (strcmp(insn.mnemonic, "jnc") == 0)
		return(_jnc(&jcc));
	if (strcmp(insn.mnemonic, "jng") == 0)
		return(_jng(&jcc));
	if (strcmp(insn.mnemonic, "jnge") == 0)
		return(_jnge(&jcc));
	if (strcmp(insn.mnemonic, "jnl") == 0)
		return(_jnl(&jcc));
	if (strcmp(insn.mnemonic, "jnle") == 0)
		return(_jnle(&jcc));
	if (strcmp(insn.mnemonic, "jno") == 0)
		return(_jno(&jcc));
	if (strcmp(insn.mnemonic, "jnp") == 0)
		return(_jnp(&jcc));
	if (strcmp(insn.mnemonic, "jns") == 0)
		return(_jns(&jcc));
	if (strcmp(insn.mnemonic, "jnz") == 0)
		return(_jnz(&jcc));
	if (strcmp(insn.mnemonic, "jo") == 0)
		return(_jo(&jcc));
	if (strcmp(insn.mnemonic, "jp") == 0)
		return(_jp(&jcc));
	if (strcmp(insn.mnemonic, "jpe") == 0)
		return(_jpe(&jcc));
	if (strcmp(insn.mnemonic, "jpo") == 0)
		return(_jpo(&jcc));
	if (strcmp(insn.mnemonic, "js") == 0)
		return(_js(&jcc));
	if (strcmp(insn.mnemonic, "jz") == 0)
		return(_jz(&jcc));

	return result;
}

UINT x86Insn_Mutation::_jmp_reg(x86_reg op0)
{
	x86::Assembler a(&Mut_Code);
	if (Check_Reg(op0) == false)
		throw "�����reg����";
	auto reg_op0 = to_asmjit_reg(op0);
	//a.jmp(reg_op0);
	int randsum = rand() % 4 + 1;
	for (int i = 0; i < randsum; i++)
		a.push(reg_op0);
	a.ret(4 * (randsum - 1));

	//��ʵ����jmp ax���������������x32�ﲻ�������ִ��룬���Բ������ˡ�
	return jmp_reg;
}

UINT x86Insn_Mutation::_jmp_imm(x86_jcc* jcc0)
{
#define jcc jmp

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110			
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110	
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jmp_imm;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jmp_mem(x86_mem* mem0)
{
	DWORD address = mem0->address;
	uint8_t disp_offset = mem0->disp_offset;
	uint8_t disp_size = mem0->disp_size;
	x86_reg base = mem0->base;
	x86_reg index = mem0->index;
	int scale = mem0->scale;
	int64_t disp = mem0->disp;
	x86::Assembler a(&Mut_Code);


	x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
	x86_reg randreg0, randreg1;
	do {
		randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg0 == base ||
		randreg0 == index ||
		randreg0 == X86_REG_ESP ||
		randreg0 == X86_REG_EBP ||
		randreg0 == X86_REG_ESI ||
		randreg0 == X86_REG_EDI			//Ҫ�õ�8λ�Ĵ���
		);
	do {
		randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
	} while (
		randreg1 == randreg0 ||
		randreg1 == base ||
		randreg1 == index ||
		randreg1 == X86_REG_ESP
		);

	auto rand0 = to_asmjit_reg(randreg0);
	auto rand1 = to_asmjit_reg(randreg1);


	a.pushfd();
	a.push(rand0);
	a.push(rand1);

	a.mov(rand0, 0);
	//���immƫ�Ʋ�Ϊ0
	if (disp != 0) {
		UINT temp = 0;
		bool Re_flag = RelocData_imm_mem(address + disp_offset, rand0, &temp);
		//����м���õ���������Ҳ����x86::Assembler��֮���Ҫ��������һ��x86::Assembler����
		x86::Assembler b(&Mut_Code);
		//�ж��Ƿ���Ҫ�ض�λ
		if (Re_flag)
			b.add(rand0, temp);
		else
			b.add(rand0, (UINT)disp);
	}
	x86::Assembler b(&Mut_Code);
	//���base��Ϊ��
	if (base != X86_REG_INVALID) {
		b.add(rand0, to_asmjit_reg(base));
	}
	//���index�Ĵ�����Ϊ�գ�add����
	if (index != X86_REG_INVALID) {
		while (scale--)
			b.add(rand0, to_asmjit_reg(index));
	}

	b.push(rand1);						//��һ���ռ�

	b.mov(rand1, ptr(x86::esp, 4));
	b.mov(ptr(x86::esp), rand1);
	b.mov(rand1, ptr(x86::esp, 8));
	b.mov(ptr(x86::esp, 4), rand1);
	b.mov(rand1, ptr(x86::esp, 12));
	b.mov(ptr(x86::esp, 8), rand1);

	b.mov(ptr(x86::esp, 12), rand0);	//д��jmpĿ���ַ

	b.pop(rand1);
	b.pop(rand0);
	b.popfd();

	b.ret();
	return jmp_mem;
}

UINT x86Insn_Mutation::_je(x86_jcc* jcc0)
{
#define jcc je
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jeת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jeת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jne(x86_jcc* jcc0)
{
#define jcc jne
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jneת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jneת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_ja(x86_jcc* jcc0)
{
#define jcc ja
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.or_(rand0, rand1);		//rand0��ZF��rand1��CF����or���㣬ֻ��ȫΪ0ʱ����������SF������0
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.or_(rand0, rand1);		//rand0��ZF��rand1��CF����or���㣬ֻ��ȫΪ0ʱ����������PF������0
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jae(x86_jcc* jcc0)
{
#define jcc jae
#define eflag_offset 0

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jaeת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jaeת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jb(x86_jcc* jcc0)
{
#define jcc jb
#define eflag_offset 0

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jbת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jbת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jbe(x86_jcc* jcc0)
{
#define jcc jbe
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {				//��ʽ��or(CF,ZF)	xor (rand0,1)
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.or_(rand0, rand1);		//rand0��ZF��rand1��CF����or���㣬�ٶ���������xor 1
			a.xor_(rand0, 1);
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.or_(rand0, rand1);		//rand0��ZF��rand1��CF����or���㣬�ٶ���������xor 1
			a.xor_(rand0, 1);
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jc(x86_jcc* jcc0)
{
#define jcc jc
#define eflag_offset 0

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jcת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jcת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jecxz(x86_jcc* jcc0)
{
#define jcc jecxz
#define eflag_offset 6


	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//����jecxz��offset��1�ֽڣ���������ôԶ�����Ի��ɵȼ۵�jmp�������ܼ򵥵�cmp+je����������ı�ZFλ��
		Label L0 = a.newLabel();

		a.pushfd();							//����eflags
		a.cmp(x86::ecx, 0);					//���ecx==0����jmp
		a.jne(L0);
		a.popfd();
		a.jmp(Jcc_ActuAddr(Target_JumpAddr));

		a.bind(L0);
		a.popfd();
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.cmp(x86::ecx, 0);
			a.pushfd();
			a.pop(rand0);
			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.cmp(x86::ecx, 0);
			a.pushfd();
			a.pop(rand0);
			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jg(x86_jcc* jcc0)
{
#define jcc jg
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {				//��ʽ��xor sf,of	or sf��zf
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand1, 11);			//��ԭrand1_eflags
			a.rcr(rand1, eflag_offset); //ZF�ƶ���CFλ
			a.or_(rand0, rand1);
			a.rcl(rand0, 7);			//CF����7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {							//��ʽ��xor sf,of	or sf��zf	sf��λ��pf
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand1, 11);			//��ԭrand1_eflags
			a.rcr(rand1, eflag_offset); //ZF�ƶ���CFλ
			a.or_(rand0, rand1);
			a.rcl(rand0, 2);			//CF����2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jge(x86_jcc* jcc0)
{
#define jcc jge
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {				//��ʽ��xor sf,of
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand0, 7);			//CF����7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {							//��ʽ��xor sf,of
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand0, 2);			//CF����2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jl(x86_jcc* jcc0)
{
#define jcc jl
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {				//��ʽ��xor sf,of	xor rand0,1
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.xor_(rand0, 1);
			a.rcl(rand0, 7);			//CF����7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {							//��ʽ��xor sf,of
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.xor_(rand0, 1);
			a.rcl(rand0, 2);			//CF����2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jle(x86_jcc* jcc0)
{
#define jcc jle
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {				//��ʽ����jg��ʽ�Ļ����ϣ��Ӹ�a.xor_(rand0, 1);
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand1, 11);			//��ԭrand1_eflags
			a.rcr(rand1, eflag_offset); //ZF�ƶ���CFλ
			a.or_(rand0, rand1);
			a.xor_(rand0, 1);
			a.rcl(rand0, 7);			//CF����7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand1, 11);			//��ԭrand1_eflags
			a.rcr(rand1, eflag_offset); //ZF�ƶ���CFλ
			a.or_(rand0, rand1);
			a.xor_(rand0, 1);
			a.rcl(rand0, 2);			//CF����2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jna(x86_jcc* jcc0)
{
#define jcc jna
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {				//��ʽ��or(CF,ZF)	xor (rand0,1)
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.or_(rand0, rand1);		//rand0��ZF��rand1��CF����or���㣬�ٶ���������xor 1
			a.xor_(rand0, 1);
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.or_(rand0, rand1);		//rand0��ZF��rand1��CF����or���㣬�ٶ���������xor 1
			a.xor_(rand0, 1);
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jnae(x86_jcc* jcc0)
{
#define jcc jnae
#define eflag_offset 0

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jbת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jbת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jnb(x86_jcc* jcc0)
{
#define jcc jnb
#define eflag_offset 0

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jaeת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jaeת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jnbe(x86_jcc* jcc0)
{
#define jcc jnbe
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.or_(rand0, rand1);		//rand0��ZF��rand1��CF����or���㣬ֻ��ȫΪ0ʱ����������SF������0
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.or_(rand0, rand1);		//rand0��ZF��rand1��CF����or���㣬ֻ��ȫΪ0ʱ����������PF������0
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jnc(x86_jcc* jcc0)
{
#define jcc jnc
#define eflag_offset 0

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jaeת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jaeת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jng(x86_jcc* jcc0)
{
#define jcc jng
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {				//��ʽ����jg��ʽ�Ļ����ϣ��Ӹ�a.xor_(rand0, 1);
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand1, 11);			//��ԭrand1_eflags
			a.rcr(rand1, eflag_offset); //ZF�ƶ���CFλ
			a.or_(rand0, rand1);
			a.xor_(rand0, 1);
			a.rcl(rand0, 7);			//CF����7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand1, 11);			//��ԭrand1_eflags
			a.rcr(rand1, eflag_offset); //ZF�ƶ���CFλ
			a.or_(rand0, rand1);
			a.xor_(rand0, 1);
			a.rcl(rand0, 2);			//CF����2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jnge(x86_jcc* jcc0)
{
#define jcc jnge
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {				//��ʽ��xor sf,of	xor rand0,1
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.xor_(rand0, 1);
			a.rcl(rand0, 7);			//CF����7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {							//��ʽ��xor sf,of
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.xor_(rand0, 1);
			a.rcl(rand0, 2);			//CF����2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jnl(x86_jcc* jcc0)
{
#define jcc jnl
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {				//��ʽ��xor sf,of
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand0, 7);			//CF����7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {							//��ʽ��xor sf,of
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand0, 2);			//CF����2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jnle(x86_jcc* jcc0)
{
#define jcc jnle
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0, randreg1;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		do {
			randreg1 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg1 == X86_REG_ESP || randreg1 == randreg0);
		auto rand0 = to_asmjit_reg(randreg0);
		auto rand1 = to_asmjit_reg(randreg1);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//*2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {				//��ʽ��xor sf,of	or sf��zf
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand1, 11);			//��ԭrand1_eflags
			a.rcr(rand1, eflag_offset); //ZF�ƶ���CFλ
			a.or_(rand0, rand1);
			a.rcl(rand0, 7);			//CF����7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {							//��ʽ��xor sf,of	or sf��zf	sf��λ��pf
			a.push(rand1);
			a.push(rand0);
			a.mov(rand1, rand0);		//eflags����rand1

			a.rcr(rand0, 7);			//SF�ƶ���CFλ
			a.rcr(rand1, 11);			//OF�ƶ���CFλ
			a.xor_(rand0, rand1);
			a.rcl(rand1, 11);			//��ԭrand1_eflags
			a.rcr(rand1, eflag_offset); //ZF�ƶ���CFλ
			a.or_(rand0, rand1);
			a.rcl(rand0, 2);			//CF����2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.pop(rand1);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jno(x86_jcc* jcc0)
{
#define jcc jno
#define eflag_offset 11

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//OF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jnoת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//OF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jnoת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jnp(x86_jcc* jcc0)
{
#define jcc jnp
#define eflag_offset 2

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//PF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jnpת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//PF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jnpת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jns(x86_jcc* jcc0)
{
#define jcc jns
#define eflag_offset 7

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//SF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jnsת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//SF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jnsת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jnz(x86_jcc* jcc0)
{
#define jcc jnz
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jneת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jneת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jo(x86_jcc* jcc0)
{
#define jcc jo
#define eflag_offset 11

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//OF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jeת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//OF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jeת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jp(x86_jcc* jcc0)
{
#define jcc jp
#define eflag_offset 2

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//PF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jeת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//PF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jeת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jpe(x86_jcc* jcc0)
{
#define jcc jpe
#define eflag_offset 2

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//PF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jeת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//PF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jeת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jpo(x86_jcc* jcc0)
{
#define jcc jpo
#define eflag_offset 2

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//PF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jnpת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//PF��־λ�Ƶ�CF
			a.xor_(rand0, 0);			//jnpת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_js(x86_jcc* jcc0)
{
#define jcc js
#define eflag_offset 7

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//SF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jsת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//SF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jsת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}

UINT x86Insn_Mutation::_jz(x86_jcc* jcc0)
{
#define jcc jz
#define eflag_offset 6

	DWORD address = jcc0->address;
	uint8_t imm_offset = jcc0->imm_offset;
	uint8_t imm_size = jcc0->imm_size;
	DWORD Target_JumpAddr = jcc0->Target_JumpAddr;


	x86::Assembler a(&Mut_Code);
	bool flag = false;
	//ȡ��jcc��offset��ַ��offset
	DWORD offset_Addr = address + imm_offset;
	DWORD offset = 0;
	memcpy_s(&offset, imm_size, (void*)offset_Addr, imm_size);
	//�ж�Ŀ����ת��ַ�ǲ�����Mutation������־��Χ��
	for (auto iter = Mut_Mark.begin(); iter != Mut_Mark.end(); iter++) {
		//�ڱ�����Χ
		if (Target_JumpAddr >= (DWORD)iter->Protected_Start && Target_JumpAddr <= (DWORD)iter->Protected_End) {
			flag = true;
			break;
		}
	}
	//Ŀ����ת��ַ���ڱ�����Χ��
	if (flag == false)
	{
		//*����Ҫ����ÿ��jccָ���޸ģ�
		a.jcc(Jcc_ActuAddr(Target_JumpAddr));
	}
	CodeBuffer& buffer = Mut_Code.sectionById(0)->buffer();
	//Ŀ����ת��ַ�ڱ�����Χ��
	if (flag == true)
	{
		x86_reg regs[] = { X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_EBP, X86_REG_ESP, X86_REG_ESI, X86_REG_EDI };
		x86_reg randreg0;
		do {
			randreg0 = regs[rand() % (sizeof(regs) / sizeof(regs[0]))];
		} while (randreg0 == X86_REG_ESP);
		auto rand0 = to_asmjit_reg(randreg0);


		Label L0 = a.newLabel();
		Label L1 = a.newLabel();
		Label L2 = a.newLabel();
		Label Jump_Success = a.newLabel();

		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		//1.������֧����
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L0);
			}
			else {
				a.jnp(L0);					//�����ת��ȷ���᲻����L0
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L0);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);

		}
		//2.ԭjccָ��ת��Ϊjns��jnpָ��
		a.bind(L0);
		a.push(rand0);						//�����޸ĵ�SF/PF��ԭ����
		a.popfd();
		//���ѡjns��jnp
		if (rand() & 1) {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jeת����jns
			a.rcl(rand0, 7);			//��CF��7λ��SF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jns(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		else {
			a.push(rand0);

			a.rcr(rand0, eflag_offset);	//ZF��־λ�Ƶ�CF
			a.xor_(rand0, 1);			//jeת����jnp
			a.rcl(rand0, 2);			//��CF��2λ��PF

			a.and_(rand0, 0xEFF);		//��TF���0��������������ģʽ���±���  0xEFF=1110 1111 1111
			a.push(rand0);
			a.popfd();
			a.pop(rand0);
			a.jnp(Jump_Success);		//��ת�ɹ���Jump_Success
		}
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		a.jmp(L2);

		a.bind(Jump_Success);
		a.push(rand0);					//��ԭeflags��rand0�Ĵ���
		a.popfd();
		a.pop(rand0);
		//----------------------------------------------------------------------------------------------------------------
		//2.1�ж��Ƿ��Ѿ�����Ŀ���ַ
		bool flag_2 = false;
		for (auto iter = SingMut.begin(); iter != SingMut.end(); iter++) {
			//�Ѿ����ɣ��޸�Ŀ����ת��ַ
			if (Target_JumpAddr == iter->Raw_CodeAddr) {
				flag_2 = true;
				Target_JumpAddr = iter->BaseAddr;
				break;
			}
		}
		//2.2�Ѿ�����Ŀ���ַ
		if (flag_2 == true)
		{
			a.jmp(Target_JumpAddr);
		}
		//2.3��û������Ŀ���ַ
		if (flag_2 == false)
		{
			a.jmp(Unknown_Address);
			size_t Temp_CodeSize = Mut_Code.codeSize();
			//����ǰjcc��Ϣд��vector����Target_JumpAddr��ַ��ָ�����ǰ���jcc_offset�޸�
			FixOffset FO_Struct = { 0 };
			FO_Struct.address = (DWORD)Final_MutMemory + Final_CodeSize + Temp_CodeSize - 5;
			FO_Struct.Target_JumpAddr = Target_JumpAddr;
			FO_Struct.imm_offset = 1;
			Fix_Offset.push_back(FO_Struct);
		}
		//----------------------------------------------------------------------------------------------------------------
		//3.������֧���ţ�copy��ǰһ����
		a.bind(L2);
		a.push(rand0);						//����rand0�Ĵ���

		a.pushfd();							//eflags��ֵ��rand0
		a.pop(rand0);
		if (rand() & 1) {
			//���ѡjns��jnp
			if (rand() & 1) {
				a.jns(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 7);			//����Ҳû��ϵ����������λSF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�SFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 7);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jns(L1);
			}
			else {
				a.jnp(L1);					//�����ת��ȷ���᲻����L1
				a.push(rand0);
				a.rcr(rand0, 2);			//����Ҳû��ϵ����������λPF����һ��bit���ڲ�Ӱ������eflags(rand0)������¸�PFΪ0
				a.and_(rand0, 0xFFE);		//0xFFE=1111 1111 1110
				a.rcl(rand0, 2);
				a.push(rand0);
				a.popfd();
				a.pop(rand0);
				a.jnp(L1);
			}
			a.mov(rand0, rand0);			//��������
			a.add(x86::esp, rand() & 1);
			a.push(rand0);
			a.sub(x86::esp, rand() & 1);
			a.pop(rand0);
		}

		a.bind(L1);
		a.push(rand0);						//��Ϊ��һ����֧�ı���eflag������Ҫ��ԭ
		a.popfd();

		a.pop(rand0);						//��ԭrand0�Ĵ���
	}
	return jcc;
#undef jcc
#undef eflag_offset
}
