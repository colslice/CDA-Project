#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
  if (ALUControl == 0x0) {
	*ALUresult = A + B;
}
else if (ALUControl == 0x1) {
	*ALUresult = A - B;
}
else if (ALUControl == 0x2) {
	if ((int)A < (int)B) {
		*ALUresult = 1;
	}
	else {
		*ALUresult = 0;
	}
}
else if (ALUControl == 0x3) {
	if (A < B) {
		*ALUresult = 1;
	}
	else {
		*ALUresult = 0;
	}
}
else if (ALUControl == 0x4) {
	*ALUresult = A & B;
}
else if (ALUControl == 0x5) {
	*ALUresult = A | B;
}
else if (ALUControl == 0x6) {
	*ALUresult = B << 16;
}
else if (ALUControl == 0x7) {
	*ALUresult = ~A;
}
if (*ALUresult == 0) {
	*Zero = 1;
}
else {
	*Zero = 0;
}
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
  if (PC % 4 != 0) {
	return 1;
}
if (PC < 0x4000) {
	return 1;
}
unsigned index = (PC - 0x4000) >> 2;

if (index >= MEMSIZE) {
	return 1;
}
*instruction = Mem[index];
return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	*op = (instruction >> 26) & 0x3F; //op code
	*r1 = (instruction >> 21) & 0x1F; //rs 
	*r2 = (instruction >> 16) & 0x1F; //rt
	*r3 = (instruction >> 11) & 0x1F; //rd R-type
	*funct = instruction & 0x3F; //function R-type
	*offset = instruction & 0xFFFF; //address/immediate I-type
	*jsec = instruction & 0x3FFFFFF; //target address J-type
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{	
    if (op == 0x0) { // if r type
        controls->RegDst = 1;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 7; 
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 1;
    }
    else if (op == 0x2) { // if jump
        controls->RegDst = 2;
        controls->Jump = 1;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;  
        controls->ALUOp = 0;  
        controls->MemWrite = 0;
        controls->ALUSrc = 1;  
        controls->RegWrite = 0;
    }
    else if (op == 0x4) { // if beq
        controls->RegDst = 2;
        controls->Jump = 0;
        controls->Branch = 1;
        controls->MemRead = 0;
        controls->MemtoReg = 2; 
        controls->ALUOp = 1;  
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 0;
    }
    else if (op == 0x8) { // if addi
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0;  
        controls->MemWrite = 0;
        controls->ALUSrc = 1;  
        controls->RegWrite = 1;
    }
    else if (op == 0xA) { // if slti
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 2;  
        controls->MemWrite = 0;
        controls->ALUSrc = 1; 
        controls->RegWrite = 1;
    }
    else if (op == 0xB) { // if sltiu
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 3;  
        controls->MemWrite = 0;
        controls->ALUSrc = 1;  
        controls->RegWrite = 1;
    }
    else if (op == 0xF) { // if lui
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 6;  
        controls->MemWrite = 0;
        controls->ALUSrc = 1; 
        controls->RegWrite = 1;
    }
    else if (op == 0x23) { // if lw
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->ALUOp = 0;  
        controls->MemWrite = 0;
        controls->ALUSrc = 1;  
        controls->RegWrite = 1;
    }
    else if (op == 0x2B) { // if sw
        controls->RegDst = 2;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;  
        controls->ALUOp = 0;  
        controls->MemWrite = 1;
        controls->ALUSrc = 1;  
        controls->RegWrite = 0;
    }
    else {
        return 1;  // halt
    }

	// printf("ALUOp: %d, ALUSrc: %d, MemtoReg: %d, RegDst: %d, RegWrite: %d, MemRead: %d, MemWrite: %d, Branch: %d, Jump: %d\n", 
    //     controls->ALUOp, controls->ALUSrc, controls->MemtoReg, controls->RegDst, controls->RegWrite, 
    //     controls->MemRead, controls->MemWrite, controls->Branch, controls->Jump);

	return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1]
    *data2 = Reg[r2]
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	// printf("Offset: 8x%08x\n", offset);
	
	unsigned msb = (offset >> 15) & 1; // get the MSB
	// printf("bit_15: %d\n", msb);

	if (msb == 1){
		*extended_value = offset | 0xFFFF0000;  // pad with 1s
	}
	else{
		*extended_value = offset & 0x0000FFFF;  // pad with 0s
	}

	// printf("Extended: 8x%08x\n", *extended_value);
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	// if r type
	if (ALUOp == 7){ 
		if (funct == 0x20) {  // add
        *ALUresult = data1 + data2;
		}
		else if (funct == 0x22) {  // sub
			*ALUresult = data1 - data2;
		}
		else if (funct == 0x24) {  // and
			*ALUresult = data1 & data2;
		}
		else if (funct == 0x25) {  // or
			*ALUresult = data1 | data2;
		}
		else if (funct == 0x2A) {  // slt
			if ((int)data1 < (int)data2)
				*ALUresult = 1;
			else
				*ALUresult = 0;
		}
	}
	// if beq
	else if (ALUOp == 1) { 
        *ALUresult = data1 - data2;  // Compare
    }
	// if lw/sw
    else if (ALUOp == 0) {  
        *ALUresult = data1 + extended_value;  // address calculation
	}
	// if slti 
    else if (ALUOp == 2) {  
        if ((int)data1 < (int)extended_value)
            *ALUresult = 1;
        else
            *ALUresult = 0;
    }
    // if sltiu
    else if (ALUOp == 3) {  
        if (data1 < extended_value)
            *ALUresult = 1;
        else
            *ALUresult = 0;
    }
    // if lui 
    else if (ALUOp == 6) {
        *ALUresult = extended_value << 16;  // Shift the immediate value by 16 bits
    }
	else{
		return 1;
	}

	// set zero flag
	if (*ALUresult == 0) {
		*Zero = 1;
	} else {
		*Zero = 0;
	}

	return 0;
}


/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	// check for permission
	if (MemRead != 1 && MemWrite != 1)
		return 0;

	// halt if not word aligned
	if ((ALUresult % 4) != 0)
		return 1;

	unsigned index = ALUresult >> 2;

	// writing value of data2 to memory location given by ALUresult
	if (MemWrite == 1)
	{
		Mem[index] = data2;
	}

	// reading content of memory location given by ALUresult to memdata
	if (MemRead == 1)
	{
		*memdata = Mem[index];
	}

	return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	// check for permission
	if (RegWrite != 1)
		return;

	unsigned destination;
	
	// if RegDst == 0, the destination r2. if RegDst == 1, destination is r3
	if (RegDst == 1)
		destination = r3;
	else
		destination = r2;
	
	unsigned data;

	// if MemtoReg == 1, bring data from memory. Otherwise, bring data from ALUresult
	if (MemtoReg == 1)
		data = memdata;
	else
		data = ALUresult;

	// write data to register
	Reg[destination] = data;
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

	if(Jump == 1){
		*PC = (jsec << 2);
	}

	else if(Branch == 1){

		if(Zero == 1){
			*PC += extended_value; 
		}

		else{
			*PC += extended_value;
		}
	}

	else{
		*PC += 4;
	}
}
