/*
 *  Multi2Sim
 *  Copyright (C) 2012  Rafael Ubal (ubal@ece.neu.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <math.h>

#include <southern-islands-emu.h>
#include <mem-system.h>
#include <x86-emu.h>


char *err_si_isa_note =
	"\tThe AMD Southern Islands instruction set is partially supported by\n" 
	"\tMulti2Sim. If your program is using an unimplemented instruction,\n"
        "\tplease email development@multi2sim.org' to request support for it.\n";

#define NOT_IMPL() fatal("GPU instruction '%s' not implemented\n%s", \
	si_isa_inst->info->name, err_si_isa_note)

#define INST SI_INST_SMRD
void si_isa_S_BUFFER_LOAD_DWORD_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	union si_reg_t value;
	uint32_t m_offset;
	uint32_t m_base;
	//uint32_t m_size;
	struct si_buffer_resource_t buf_desc;
	int sbase;

	assert(INST.imm);

	sbase = INST.sbase << 1;

	/* sbase holds the first of 4 registers containing the buffer resource descriptor */
	si_isa_read_buf_res(work_item, &buf_desc, sbase);

	/* sgpr[dst] = read_dword_from_kcache(m_base, m_offset, m_size) */
	m_base = buf_desc.base_addr;
	m_offset = INST.offset * 4;
	//m_size = (buf_desc.stride == 0) ? 1 : buf_desc.num_records;
	
	mem_read(si_emu->global_mem, m_base+m_offset, 4, &value);

	/* Store the data in the destination register */
	si_isa_write_sreg(work_item, INST.sdst, value);

	if (debug_status(si_isa_debug_category))
		si_isa_debug("S%u<=(%d,%gf)", INST.sdst, value.as_uint, value.as_float);
}
#undef INST

#define INST SI_INST_SMRD
void si_isa_S_LOAD_DWORDX4_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	union si_reg_t value[4];
	uint32_t m_base;
	uint32_t m_offset;
	uint32_t m_addr;
	struct si_mem_ptr_t mem_ptr;
	int sbase;
	int i;

        assert(INST.imm);

	sbase = INST.sbase << 1;

	si_isa_read_mem_ptr(work_item, &mem_ptr, sbase);

	/* assert(uav_table_ptr.addr < UINT32_MAX) */

	m_base = mem_ptr.addr;
	m_offset = INST.offset * 4;
	m_addr = m_base + m_offset; 

	assert(!(m_addr & 0x3));

	for (i = 0; i < 4; i++) 
	{
		mem_read(si_emu->global_mem, m_base + m_offset + i * 4, 4, &value[i]);
		si_isa_write_sreg(work_item, INST.sdst+i, value[i]);
	}	

	if (debug_status(si_isa_debug_category))
		for (i = 0; i < 4; i++) 
			si_isa_debug("S%u<=(%d,%gf) ", INST.sdst+i, value[i].as_uint, 
				value[i].as_float);
}
#undef INST

#define INST SI_INST_SMRD
void si_isa_S_BUFFER_LOAD_DWORDX2_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	union si_reg_t value[2];

	uint32_t m_base;
	uint32_t m_offset;
	uint32_t m_addr;

	struct si_mem_ptr_t mem_ptr;

	int sbase;
	int i;

	assert(INST.imm);

	sbase = INST.sbase << 1;

	si_isa_read_mem_ptr(work_item, &mem_ptr, sbase);

	/* assert(uav_table_ptr.addr < UINT32_MAX) */

	m_base = mem_ptr.addr;
        m_offset = INST.offset * 4;
	m_addr = m_base + m_offset; 

	assert(!(m_addr & 0x3));

	for (i = 0; i < 2; i++) 
	{
		mem_read(si_emu->global_mem, m_base+m_offset+i*4, 4, &value[i]);
		si_isa_write_sreg(work_item, INST.sdst+i, value[i]);
	}	

	if (debug_status(si_isa_debug_category))
	{
		for (i = 0; i < 2; i++) 
		{
			si_isa_debug("S%u<=(%d,%gf) ", INST.sdst+i, value[i].as_uint, 
				value[i].as_float);
		}
	}
}
#undef INST

/* D.u = S0.i + S1.i. scc = overflow. */
#define INST SI_INST_SOP2
void si_isa_S_ADD_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	int s0 = 0;
	int s1 = 0;

	union si_reg_t sum;
	union si_reg_t ovf;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_sreg(work_item, INST.ssrc0).as_int;
	if (INST.ssrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_sreg(work_item, INST.ssrc1).as_int;

	/* Calculate the sum and overflow. */
	sum.as_uint = s0 + s1;
	ovf.as_uint = s0 >> 31 != s1 >> 31 ? 0 : (s0 > 0 && sum.as_int < 0) ||
			(s0 < 0 && sum.as_int > 0);

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, sum);
	si_isa_write_sreg(work_item, SI_SCC, ovf);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d) ", INST.sdst, sum.as_uint);
		si_isa_debug("scc<=(%d) ", ovf.as_uint);
	}
}
#undef INST

/* D.u = (S0.u < S1.u) ? S0.u : S1.u, scc = 1 if S0 is min. */
#define INST SI_INST_SOP2
void si_isa_S_MIN_U32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0 = 0;
	unsigned int s1 = 0;

	union si_reg_t min;
	union si_reg_t s0_min;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_sreg(work_item, INST.ssrc0).as_uint;
	if (INST.ssrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_sreg(work_item, INST.ssrc1).as_uint;

	/* Calculate the minimum operand. */
	if (INST.ssrc0 < INST.ssrc1)
	{
		min.as_uint = s0;
		s0_min.as_uint = 1;
	}
	else
	{
		min.as_uint = s1;
		s0_min.as_uint = 0;
	}

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, min);
	si_isa_write_sreg(work_item, SI_SCC, s0_min);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d) ", INST.sdst, min.as_uint);
		si_isa_debug("scc<=(%d) ", s0_min.as_uint);
	}
}
#undef INST

/* D.u = S0.u & S1.u. scc = 1 if result is non-zero. */
#define INST SI_INST_SOP2
void si_isa_S_AND_B32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0 = 0;
	unsigned int s1 = 0;

	union si_reg_t result;
	union si_reg_t nonzero;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_sreg(work_item, INST.ssrc0).as_uint;
	if (INST.ssrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_sreg(work_item, INST.ssrc1).as_uint;

	/* Bitwise AND the two operands and determine if the result is non-zero. */
	result.as_uint = s0 & s1;
	nonzero.as_uint = !!result.as_uint;

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, result);
	si_isa_write_sreg(work_item, SI_SCC, nonzero);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d) ", INST.sdst, result.as_uint);
		si_isa_debug("scc<=(%d) ", nonzero.as_uint);
	}
}
#undef INST

/* D.u = S0.u & S1.u. scc = 1 if result is non-zero. */
#define INST SI_INST_SOP2
void si_isa_S_AND_B64_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	/* Assert no literal constants for a 64 bit instruction. */
	assert(!(INST.ssrc0 == 0xFF || INST.ssrc1 == 0xFF));

	unsigned int s0_lo;
	unsigned int s0_hi;
	unsigned int s1_lo;
	unsigned int s1_hi;

	union si_reg_t result_lo;
	union si_reg_t result_hi;
	union si_reg_t nonzero;

	/* Load operands from registers. */
	s0_lo = si_isa_read_sreg(work_item, INST.ssrc0).as_uint;
	s0_hi = si_isa_read_sreg(work_item, INST.ssrc0 + 1).as_uint;
	s1_lo = si_isa_read_sreg(work_item, INST.ssrc1).as_uint;
	s1_hi = si_isa_read_sreg(work_item, INST.ssrc1 + 1).as_uint;

	/* Bitwise AND the two operands and determine if the result is non-zero. */
	result_lo.as_uint = s0_lo & s1_lo;
	result_hi.as_uint = s0_hi & s1_hi;
	nonzero.as_uint = result_lo.as_uint && result_hi.as_uint;

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, result_lo);
	si_isa_write_sreg(work_item, INST.sdst + 1, result_hi);
	si_isa_write_sreg(work_item, SI_SCC, nonzero);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d) ", INST.sdst, result_lo.as_uint);
		si_isa_debug("S%u<=(%d) ", INST.sdst + 1, result_hi.as_uint);
		si_isa_debug("scc<=(%d) ", nonzero.as_uint);
	}
}
#undef INST

/* D.u = S0.u & ~S1.u. scc = 1 if result is non-zero. */
#define INST SI_INST_SOP2
void si_isa_S_ANDN2_B64_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	/* Assert no literal constants for a 64 bit instruction. */
	assert(!(INST.ssrc0 == 0xFF || INST.ssrc1 == 0xFF));

	unsigned int s0_lo;
	unsigned int s0_hi;
	unsigned int s1_lo;
	unsigned int s1_hi;

	union si_reg_t result_lo;
	union si_reg_t result_hi;
	union si_reg_t nonzero;

	/* Load operands from registers. */
	s0_lo = si_isa_read_sreg(work_item, INST.ssrc0).as_uint;
	s0_hi = si_isa_read_sreg(work_item, INST.ssrc0 + 1).as_uint;
	s1_lo = si_isa_read_sreg(work_item, INST.ssrc1).as_uint;
	s1_hi = si_isa_read_sreg(work_item, INST.ssrc1 + 1).as_uint;

	/* Bitwise AND the first operand with the negation of the second
	 * and determine if the result is non-zero. */
	result_lo.as_uint = s0_lo & ~s1_lo;
	result_hi.as_uint = s0_hi & ~s1_hi;
	nonzero.as_uint = result_lo.as_uint && result_hi.as_uint;

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, result_lo);
	si_isa_write_sreg(work_item, INST.sdst + 1, result_hi);
	si_isa_write_sreg(work_item, SI_SCC, nonzero);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d)= ", INST.sdst, result_lo.as_uint);
		si_isa_debug("S%u<=(%d)= ", INST.sdst + 1, result_hi.as_uint);
		si_isa_debug("scc<=(%d)", nonzero.as_uint);
	}
}
#undef INST

/* D.u = S0.u << S1.u[4:0]. scc = 1 if result is non-zero. */
#define INST SI_INST_SOP2
void si_isa_S_LSHL_B32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0 = 0;
	unsigned int s1 = 0;

	union si_reg_t result;
	union si_reg_t nonzero;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_sreg(work_item, INST.ssrc0).as_uint;
	if (INST.ssrc1 == 0xFF)
		s1 = INST.lit_cnst & 0x1F;
	else
		s1 = si_isa_read_sreg(work_item, INST.ssrc1).as_uint & 0x1F;

	/* Left shift the first operand by the second and
	 * determine if the result is non-zero. */
	result.as_uint = s0 << s1;
	nonzero.as_uint = result.as_uint != 0;

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, result);
	si_isa_write_sreg(work_item, SI_SCC, nonzero);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d)= ", INST.sdst, result.as_uint);
		si_isa_debug("scc<=(%d)", nonzero.as_uint);
	}
}
#undef INST

/* D.u = S0.u >> S1.u[4:0]. scc = 1 if result is non-zero. */
#define INST SI_INST_SOP2
void si_isa_S_LSHR_B32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0 = 0;
	unsigned int s1 = 0;

	union si_reg_t result;
	union si_reg_t nonzero;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_sreg(work_item, INST.ssrc0).as_uint;
	if (INST.ssrc1 == 0xFF)
		s1 = INST.lit_cnst & 0x1F;
	else
		s1 = si_isa_read_sreg(work_item, INST.ssrc1).as_uint & 0x1F;

	/* Right shift the first operand by the second and
	 * determine if the result is non-zero. */
	result.as_uint = s0 >> s1;
	nonzero.as_uint = result.as_uint != 0;

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, result);
	si_isa_write_sreg(work_item, SI_SCC, nonzero);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d)= ", INST.sdst, result.as_uint);
		si_isa_debug("scc<=(%d)", nonzero.as_uint);
	}
}
#undef INST

/* D.i = signext(S0.i) >> S1.i[4:0]. scc = 1 if result is non-zero. */
#define INST SI_INST_SOP2
void si_isa_S_ASHR_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	int s0 = 0;
	int s1 = 0;

	union si_reg_t result;
	union si_reg_t nonzero;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_sreg(work_item, INST.ssrc0).as_int;
	if (INST.ssrc1 == 0xFF)
		s1 = INST.lit_cnst & 0x1F;
	else
		s1 = si_isa_read_sreg(work_item, INST.ssrc1).as_int & 0x1F;
	long se_s0 = s0;

	/* Right shift the first operand sign extended by the second and
	 * determine if the result is non-zero. */
	result.as_int = se_s0 >> s1;
	nonzero.as_uint = result.as_uint != 0;

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, result);
	si_isa_write_sreg(work_item, SI_SCC, nonzero);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d)= ", INST.sdst, result.as_int);
		si_isa_debug("scc<=(%d)", nonzero.as_uint);
	}
}
#undef INST

/* D.i = S0.i * S1.i. */
#define INST SI_INST_SOP2
void si_isa_S_MUL_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	int s0 = 0;
	int s1 = 0;

	union si_reg_t result;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_sreg(work_item, INST.ssrc0).as_int;
	if (INST.ssrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_sreg(work_item, INST.ssrc1).as_int;

	/* Multiply the two operands. */
	result.as_int = s0 * s1;

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d)= ", INST.sdst, result.as_int);
	}
}
#undef INST

/* D.i = signext(simm16). */
#define INST SI_INST_SOPK
void si_isa_S_MOVK_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	short simm16;

	union si_reg_t result;

	/* Load constant operand from instruction. */
	simm16 = INST.simm16;

	/* Sign extend the short constant to an integer. */
	result.as_int = (int)simm16;

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d)", INST.sdst, result.as_int);
	}
}
#undef INST

/* D.i = D.i + signext(SIMM16). scc = overflow. */
#define INST SI_INST_SOPK
void si_isa_S_ADDK_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	short simm16;
	int se_simm16;
	int dest;

	union si_reg_t sum;
	union si_reg_t ovf;

	/* Load short constant operand from instruction
	 * and sign extend to an integer. */
	simm16 = INST.simm16;
	se_simm16 = simm16;

	/* Load operand from destination register. */
	dest = si_isa_read_sreg(work_item, INST.sdst).as_int;

	/* Add the two operands and determine overflow. */
	sum.as_int = dest + se_simm16;
	ovf.as_uint = dest >> 31 != se_simm16 >> 31 ? 0 :
			(dest > 0 && sum.as_int < 0) || (dest < 0 && sum.as_int > 0);

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, sum);
	si_isa_write_sreg(work_item, SI_SCC, ovf);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d)", INST.sdst, sum.as_int);
		si_isa_debug("scc<=(%d)", ovf.as_uint);
	}
}
#undef INST

/* D.i = D.i * signext(SIMM16). scc = overflow. */
#define INST SI_INST_SOPK
void si_isa_S_MULK_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	short simm16;
	int se_simm16;
	int dest;

	union si_reg_t product;
	union si_reg_t ovf;

	/* Load short constant operand from instruction
	 * and sign extend to an integer. */
	simm16 = INST.simm16;
	se_simm16 = simm16;

	/* Load operand from destination register. */
	dest = si_isa_read_sreg(work_item, INST.sdst).as_int;

	/* Multiply the two operands and determine overflow. */
	product.as_int = dest * se_simm16;
	ovf.as_uint = ((long)dest * (long)se_simm16) > (long)product.as_uint;

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, product);
	si_isa_write_sreg(work_item, SI_SCC, ovf);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d)", INST.sdst, product.as_int);
		si_isa_debug("scc<=(%d)", ovf.as_uint);
	}
}
#undef INST

/* D.u = S0.u. */
#define INST SI_INST_SOP1
void si_isa_S_MOV_B64_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	/* Assert no literal constant with a 64 bit instruction. */
	assert(!(INST.ssrc0 == 0xFF));

	union si_reg_t s0_lo;
	union si_reg_t s0_hi;

	/* Load operand from registers. */
	s0_lo = si_isa_read_sreg(work_item, INST.ssrc0);
	s0_hi = si_isa_read_sreg(work_item, INST.ssrc0 + 1);

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, s0_lo);
	si_isa_write_sreg(work_item, INST.sdst + 1, s0_hi);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d) ", INST.sdst, s0_lo.as_int);
		si_isa_debug("S%u<=(%d)", INST.sdst + 1, s0_hi.as_int);
	}
}
#undef INST

/* D.u = S0.u. */
#define INST SI_INST_SOP1
void si_isa_S_MOV_B32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	union si_reg_t s0;

	/* Load operand from registers or as a literal constant. */
	if (INST.ssrc0 == 0xFF)
		s0.as_uint = INST.lit_cnst;
	else
		s0 = si_isa_read_sreg(work_item, INST.ssrc0);

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, s0);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d) ", INST.sdst, s0.as_int);
	}
}
#undef INST

/* D.u = EXEC, EXEC = S0.u & EXEC. scc = 1 if the new value of EXEC is non-zero. */
#define INST SI_INST_SOP1
void si_isa_S_AND_SAVEEXEC_B64_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	/* Assert no literal constant with a 64 bit instruction. */
	assert(!(INST.ssrc0 == 0xFF));

	union si_reg_t exec_lo;
	union si_reg_t exec_hi;
	unsigned int s0_lo;
	unsigned int s0_hi;

	union si_reg_t exec_new_lo;
	union si_reg_t exec_new_hi;
	union si_reg_t nonzero;

	/* Load operands from registers. */
	exec_lo = si_isa_read_sreg(work_item, SI_EXEC);
	exec_hi = si_isa_read_sreg(work_item, SI_EXEC + 1);
	s0_lo = si_isa_read_sreg(work_item, INST.ssrc0).as_uint;
	s0_hi = si_isa_read_sreg(work_item, INST.ssrc0 + 1).as_uint;

	/* Bitwise AND exec and the first operand and determine if the result is non-zero. */
	exec_new_lo.as_uint = s0_lo & exec_lo.as_uint;
	exec_new_hi.as_uint = s0_hi & exec_hi.as_uint;
	nonzero.as_uint = exec_new_lo.as_uint && exec_new_hi.as_uint;

	/* Write the results. */
	si_isa_write_sreg(work_item, INST.sdst, exec_lo);
	si_isa_write_sreg(work_item, INST.sdst + 1, exec_hi);
	si_isa_write_sreg(work_item, SI_EXEC, exec_new_lo);
	si_isa_write_sreg(work_item, SI_EXEC + 1, exec_new_hi);
	si_isa_write_sreg(work_item, SI_SCC, nonzero);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("S%u<=(%d) ", INST.sdst, exec_lo.as_uint);
		si_isa_debug("S%u<=(%d) ", INST.sdst + 1, exec_hi.as_uint);
		si_isa_debug("exec_lo<=(%d) ", exec_new_lo.as_uint);
		si_isa_debug("exec_hi<=(%d) ", exec_new_hi.as_uint);
		si_isa_debug("scc<=(%d)", nonzero.as_uint);
	}
}
#undef INST

/* scc = (S0.i == S1.i). */
#define INST SI_INST_SOPC
void si_isa_S_CMP_EQ_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	int s0 = 0;
	int s1 = 0;

	union si_reg_t equal;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_sreg(work_item, INST.ssrc0).as_int;
	if (INST.ssrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_sreg(work_item, INST.ssrc1).as_int;

	/* Compare the operands. */
	equal.as_uint = s0 == s1;

	/* Write the results. */
	si_isa_write_sreg(work_item, SI_SCC, equal);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("scc<=(%d) ", equal.as_uint);
	}
}
#undef INST

/* scc = (S0.u <= S1.u). */
#define INST SI_INST_SOPC
void si_isa_S_CMP_LE_U32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0 = 0;
	unsigned int s1 = 0;

	union si_reg_t le;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.ssrc0 == 0xFF && INST.ssrc1 == 0xFF));
	if (INST.ssrc0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_sreg(work_item, INST.ssrc0).as_uint;
	if (INST.ssrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_sreg(work_item, INST.ssrc1).as_uint;

	/* Compare the operands. */
	le.as_uint = s0 <= s1;

	/* Write the results. */
	si_isa_write_sreg(work_item, SI_SCC, le);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("scc<=(%d) ", le.as_uint);
	}
}
#undef INST

/* End the program. */
void si_isa_S_ENDPGM_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	work_item->wavefront->finished = 1;
}

/* PC = PC + signext(SIMM16 * 4) + 4 */
#define INST SI_INST_SOPP
void si_isa_S_BRANCH_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int pc;
	short simm16;
	int se_simm16;

	/* Load the current program counter. */
	pc = work_item->wavefront->inst_buf - work_item->wavefront->inst_buf_start;

	/* Load the short constant operand and sign extend into an integer. */
	simm16 = INST.simm16;
	se_simm16 = simm16;

	/* Determine the program counter to branch to. */
	pc = pc + (se_simm16 * 4) + 4;

	/* Set the new program counter. */
	work_item->wavefront->inst_buf = work_item->wavefront->inst_buf_start + pc;
}
#undef INST

/* if(SCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop. */
#define INST SI_INST_SOPP
void si_isa_S_CBRANCH_SCC0_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int pc;
	short simm16;
	int se_simm16;

	if(!si_isa_read_sreg(work_item, SI_SCC).as_uint)
	{
		/* Load the current program counter. */
		pc = work_item->wavefront->inst_buf - work_item->wavefront->inst_buf_start;

		/* Load the short constant operand and sign extend into an integer. */
		simm16 = INST.simm16;
		se_simm16 = simm16;

		/* Determine the program counter to branch to. */
		pc = pc + (se_simm16 * 4) + 4;

		/* Set the new program counter. */
		work_item->wavefront->inst_buf = work_item->wavefront->inst_buf_start + pc;
	}
}
#undef INST

/* if(SCC == 1) then PC = PC + signext(SIMM16 * 4) + 4; else nop. */
#define INST SI_INST_SOPP
void si_isa_S_CBRANCH_SCC1_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int pc;
	short simm16;
	int se_simm16;

	if(si_isa_read_sreg(work_item, SI_SCC).as_uint)
	{
		/* Load the current program counter. */
		pc = work_item->wavefront->inst_buf - work_item->wavefront->inst_buf_start;

		/* Load the short constant operand and sign extend into an integer. */
		simm16 = INST.simm16;
		se_simm16 = simm16;

		/* Determine the program counter to branch to. */
		pc = pc + (se_simm16 * 4) + 4;

		/* Set the new program counter. */
		work_item->wavefront->inst_buf = work_item->wavefront->inst_buf_start + pc;
	}
}
#undef INST

/* if(VCC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop. */
#define INST SI_INST_SOPP
void si_isa_S_CBRANCH_VCCZ_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int pc;
	short simm16;
	int se_simm16;

	if(si_isa_read_sreg(work_item, SI_VCCZ).as_uint)
	{
		/* Load the current program counter. */
		pc = work_item->wavefront->inst_buf - work_item->wavefront->inst_buf_start;

		/* Load the short constant operand and sign extend into an integer. */
		simm16 = INST.simm16;
		se_simm16 = simm16;

		/* Determine the program counter to branch to. */
		pc = pc + (se_simm16 * 4) + 4;

		/* Set the new program counter. */
		work_item->wavefront->inst_buf = work_item->wavefront->inst_buf_start + pc;
	}
}
#undef INST

/* if(EXEC == 0) then PC = PC + signext(SIMM16 * 4) + 4; else nop. */
#define INST SI_INST_SOPP
void si_isa_S_CBRANCH_EXECZ_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int pc;
	short simm16;
	int se_simm16;

	if(si_isa_read_sreg(work_item, SI_EXECZ).as_uint)
	{
		/* Load the current program counter. */
		pc = work_item->wavefront->inst_buf - work_item->wavefront->inst_buf_start;

		/* Load the short constant operand and sign extend into an integer. */
		simm16 = INST.simm16;
		se_simm16 = simm16;

		/* Determine the program counter to branch to. */
		pc = pc + (se_simm16 * 4) + 4;

		/* Set the new program counter. */
		work_item->wavefront->inst_buf = work_item->wavefront->inst_buf_start + pc;
	}
}
#undef INST

/* Suspend current wavefront at the barrier.
 * If all wavefronts in work-group reached the barrier, wake them up */
void si_isa_S_BARRIER_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	struct si_wavefront_t *wavefront = work_item->wavefront;
	struct si_work_group_t *work_group = work_item->work_group;

	/* Suspend current wavefront at the barrier */
	assert(DOUBLE_LINKED_LIST_MEMBER(work_group, running, wavefront));
	DOUBLE_LINKED_LIST_REMOVE(work_group, running, wavefront);
	DOUBLE_LINKED_LIST_INSERT_TAIL(work_group, barrier, wavefront);
	si_isa_debug("%s (gid=%d) reached barrier (%d reached, %d left)\n",
		wavefront->name, work_group->id, work_group->barrier_list_count,
		work_group->wavefront_count - work_group->barrier_list_count);

	/* If all wavefronts in work-group reached the barrier, wake them up */
	if (work_group->barrier_list_count == work_group->wavefront_count)
	{
		struct si_wavefront_t *wavefront;
		while (work_group->barrier_list_head)
		{
			wavefront = work_group->barrier_list_head;
			DOUBLE_LINKED_LIST_REMOVE(work_group, barrier, wavefront);
			DOUBLE_LINKED_LIST_INSERT_TAIL(work_group, running, wavefront);
		}
		assert(work_group->running_list_count == work_group->wavefront_count);
		assert(work_group->barrier_list_count == 0);
		si_isa_debug("%s completed barrier, waking up wavefronts\n",
			work_group->name);
	}
}

void si_isa_S_WAITCNT_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	/* Nothing to do in emulation */
}

/* D.u = S0.u. */
#define INST SI_INST_VOP1
void si_isa_V_MOV_B32_VOP1_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	union si_reg_t value;

	/* Load operand from register or as a literal constant. */
	if (INST.src0 == 0xFF)
		value.as_uint = INST.lit_cnst;
	else
		value = si_isa_read_reg(work_item, INST.src0);

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, value);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, value.as_int);
	}
}
#undef INST

/* D.f = (float)S0.i. */
#define INST SI_INST_VOP1
void si_isa_V_CVT_F32_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	union si_reg_t value;

	/* Load operand from register or as a literal constant. */
	if (INST.src0 == 0xFF)
		value.as_float = (float)INST.lit_cnst;
	else
		value.as_float = (float)si_isa_read_reg(work_item, INST.src0).as_int;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, value);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%gf) ", work_item->id, INST.vdst, value.as_float);
	}
}
#undef INST

/* D.f = (float)S0.u. */
#define INST SI_INST_VOP1
void si_isa_V_CVT_F32_U32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	union si_reg_t value;

	/* Load operand from register or as a literal constant. */
	if (INST.src0 == 0xFF)
		value.as_float = (float)INST.lit_cnst;
	else
		value.as_float = (float)si_isa_read_reg(work_item, INST.src0).as_uint;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, value);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%gf) ", work_item->id, INST.vdst, value.as_float);
	}
}
#undef INST

/* D.i = (uint)S0.f. */
#define INST SI_INST_VOP1
void si_isa_V_CVT_U32_F32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	union si_reg_t value;

	/* Load operand from register or as a literal constant. */
	if (INST.src0 == 0xFF)
		value.as_uint = (unsigned int)INST.lit_cnst;
	else
		value.as_uint = (unsigned int)si_isa_read_reg(work_item, INST.src0).as_float;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, value);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, value.as_uint);
	}
}
#undef INST

/* D.i = (int)S0.f. */
#define INST SI_INST_VOP1
void si_isa_V_CVT_I32_F32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	union si_reg_t value;

	/* Load operand from register or as a literal constant. */
	if (INST.src0 == 0xFF)
		value.as_int = (int)INST.lit_cnst;
	else
		value.as_int = (int)si_isa_read_reg(work_item, INST.src0).as_float;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, value);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, value.as_int);
	}
}
#undef INST

/* D.f = 1.0 / S0.f. */
#define INST SI_INST_VOP1
void si_isa_V_RCP_F32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	union si_reg_t rcp;

	/* Load operand from register or as a literal constant. */
	if (INST.src0 == 0xFF)
		rcp.as_float = 1.0 / (float)INST.lit_cnst;
	else
		rcp.as_float = 1.0 / si_isa_read_reg(work_item, INST.src0).as_float;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, rcp);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%gf) ", work_item->id, INST.vdst, rcp.as_float);
	}
}
#undef INST

/* D.f = S0.f + S1.f. */
#define INST SI_INST_VOP2
void si_isa_V_ADD_F32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	float s0 = 0;
	float s1 = 0;

	union si_reg_t sum;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_float;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_float;

	/* Calculate the sum. */
	sum.as_float = s0 + s1;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, sum);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%gf) ", work_item->id, INST.vdst, sum.as_float);
	}
}
#undef INST

/* D.f = S0.f * S1.f. */
#define INST SI_INST_VOP2
void si_isa_V_MUL_F32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	float s0 = 0;
	float s1 = 0;

	union si_reg_t product;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_float;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_float;

	/* Calculate the product. */
	product.as_float = s0 * s1;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, product);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%gf) ", work_item->id, INST.vdst, product.as_float);
	}
}
#undef INST

/* D.i = S0.i[23:0] * S1.i[23:0]. */
#define INST SI_INST_VOP2
void si_isa_V_MUL_I32_I24_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{	
	int s0 = 0;
	int s1 = 0;

	union si_reg_t product;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst & 0xFFFFFF;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_int & 0xFFFFFF;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst & 0xFFFFFF;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_int & 0xFFFFFF;

	/* Calculate the product. */
	product.as_int = s0 * s1;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, product);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d)", work_item->id, INST.vdst, product.as_int);
	}
}
#undef INST

/* D.u = S1.u << S0.u[4:0]. */
#define INST SI_INST_VOP2
void si_isa_V_LSHLREV_B32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0 = 0;
	unsigned int s1 = 0;

	union si_reg_t result;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst & 0x1F;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_uint & 0x1F;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_uint;

	/* Left shift s1 by s0. */
	result.as_uint = s1 << s0;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, result.as_uint);
	}
}
#undef INST

/* D.u = S0.u | S1.u. */
#define INST SI_INST_VOP2
void si_isa_V_OR_B32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0 = 0;
	unsigned int s1 = 0;

	union si_reg_t result;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_uint;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_uint;

	/* Bitwise OR the two operands. */
	result.as_uint = s0 | s1;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, result.as_uint);
	}
}
#undef INST

/* D.f = S0.f * S1.f + D.f. */
#define INST SI_INST_VOP2
void si_isa_V_MAC_F32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	float s0 = 0;
	float s1 = 0;

	union si_reg_t result;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_float;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_float;
	float d = si_isa_read_vreg(work_item, INST.vdst).as_float;

	/* Calculate the result. */
	result.as_float = s0 * s1 + d;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%gf) ", work_item->id, INST.vdst, result.as_float);
	}
}
#undef INST

/* D.u = S0.u + S1.u, vcc = carry-out. */
#define INST SI_INST_VOP2
void si_isa_V_ADD_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0 = 0;
	unsigned int s1 = 0;

	union si_reg_t sum;
	union si_reg_t carry;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_uint;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_uint;

	/* Calculate the sum and carry. */
	sum.as_uint = s0 + s1;
	carry.as_uint = !!(((unsigned long long) s0 + (unsigned long long) s1) >> 32);

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, sum);
	si_isa_bitmask_sreg(work_item, SI_VCC, carry);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, sum.as_uint);
		si_isa_debug("wf_id%d: vcc<=(%d) ", work_item->id_in_wavefront, carry.as_uint);
	}
}
#undef INST

/* D.u = S0.u - S1.u; vcc = carry-out. */
#define INST SI_INST_VOP2
void si_isa_V_SUB_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0 = 0;
	unsigned int s1 = 0;

	union si_reg_t dif;
	union si_reg_t carry;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_uint;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_uint;

	/* Calculate the difference and carry. */
	dif.as_uint = s0 - s1;
	carry.as_uint = (s1 > s0);

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, dif);
	si_isa_bitmask_sreg(work_item, SI_VCC, carry);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, dif.as_uint);
		si_isa_debug("wf_id%d: vcc<=(%d) ", work_item->id_in_wavefront, carry.as_uint);
	}
}
#undef INST

/* D.u = VCC[i] ? S1.u : S0.u (i = threadID in wave); VOP3: specify VCC as a scalar GPR in S2. */
#define INST SI_INST_VOP3a
void si_isa_V_CNDMASK_B32_VOP3a_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	union si_reg_t s0;
	union si_reg_t s1;
	int vcci;

	union si_reg_t result;

	/* Load operands from registers. */
	s0 = si_isa_read_reg(work_item, INST.src0);
	s1 = si_isa_read_reg(work_item, INST.src1);
	vcci = si_isa_read_bitmask_sreg(work_item, INST.src2);

	/* Calculate the result. */
	result = (vcci) ? s1 : s0;

	/* Assert no negation modifiers. */
	assert(!INST.neg);

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, result.as_uint);
	}
}
#undef INST

/* D.f = S0.f * S1.f + S2.f. */
#define INST SI_INST_VOP3a
void si_isa_V_MAD_F32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	float s0;
	float s1;
	float s2;

	union si_reg_t result;

	/* Load operands from registers. */
	s0 = si_isa_read_reg(work_item, INST.src0).as_float;
	s1 = si_isa_read_reg(work_item, INST.src1).as_float;
	s2 = si_isa_read_reg(work_item, INST.src2).as_float;

	/* Apply negation modifiers. */
	if(INST.neg & 1)
		s0 = -s0;
	if(INST.neg & 2)
		s1 = -s1;
	if(INST.neg & 4)
		s2 = -s2;

	/* Calculate the result. */
	result.as_float = s0 * s1 + s2;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%gf) ", work_item->id, INST.vdst, result.as_float);
	}
}
#undef INST

/* D.u = S0.u * S1.u. */
#define INST SI_INST_VOP3a
void si_isa_V_MUL_LO_U32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0;
	unsigned int s1;

	union si_reg_t result;

	/* Load operands from registers. */
	s0 = si_isa_read_reg(work_item, INST.src0).as_uint;
	s1 = si_isa_read_reg(work_item, INST.src1).as_uint;

	/* Apply negation modifiers. */
	if(INST.neg & 1)
		s0 = -s0;
	if(INST.neg & 2)
		s1 = -s1;

	/* Calculate the product. */
	result.as_uint = s0 * s1;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, result.as_uint);
	}
}
#undef INST

/* D.u = (S0.u * S1.u)>>32 */
#define INST SI_INST_VOP3a
void si_isa_V_MUL_HI_U32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0;
	unsigned int s1;

	union si_reg_t result;

	/* Load operands from registers. */
	s0 = si_isa_read_reg(work_item, INST.src0).as_uint;
	s1 = si_isa_read_reg(work_item, INST.src1).as_uint;

	/* Apply negation modifiers. */
	if(INST.neg & 1)
		s0 = -s0;
	if(INST.neg & 2)
		s1 = -s1;

	/* Calculate the product and shift right. */
	result.as_uint = ((unsigned long long)s0 * (unsigned long long)s1) >> 32;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, result.as_uint);
	}
}
#undef INST

/* D.i = S0.i * S1.i. */
#define INST SI_INST_VOP3a
void si_isa_V_MUL_LO_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	int s0;
	int s1;

	union si_reg_t result;

	/* Load operands from registers. */
	s0 = si_isa_read_reg(work_item, INST.src0).as_int;
	s1 = si_isa_read_reg(work_item, INST.src1).as_int;

	/* Apply negation modifiers. */
	if(INST.neg & 1)
		s0 = -s0;
	if(INST.neg & 2)
		s1 = -s1;

	/* Calculate the product. */
	result.as_int = s0 * s1;

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, result.as_int);
	}
}
#undef INST

/* vcc = (S0.i < S1.i). */
#define INST SI_INST_VOPC
void si_isa_V_CMP_LT_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	int s0 = 0;
	int s1 = 0;

	union si_reg_t result;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_int;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_int;

	/* Compare the operands. */
	result.as_uint = (s0 < s1);

	/* Write the results. */
	si_isa_bitmask_sreg(work_item, SI_VCC, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("wf_id%d: vcc<=(%d) ", work_item->id_in_wavefront,
			result.as_uint);
	}
}
#undef INST

/* vcc = (S0.i > S1.i). */
#define INST SI_INST_VOPC
void si_isa_V_CMP_GT_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	int s0 = 0;
	int s1 = 0;

	union si_reg_t result;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_int;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_int;

	/* Compare the operands. */
	result.as_uint = (s0 > s1);

	/* Write the results. */
	si_isa_bitmask_sreg(work_item, SI_VCC, result);
	
	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("wf_id%d: vcc<=(%d) ", work_item->id_in_wavefront,
			result.as_uint);
	}
}
#undef INST

/* vcc = (S0.i <> S1.i). */
#define INST SI_INST_VOPC
void si_isa_V_CMP_NE_I32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	int s0 = 0;
	int s1 = 0;

	union si_reg_t result;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_int;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_int;

	/* Compare the operands. */
	result.as_uint = (s0 != s1);

	/* Write the results. */
	si_isa_bitmask_sreg(work_item, SI_VCC, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("wf_id%d: vcc<=(%d) ", work_item->id_in_wavefront,
			result.as_uint);
	}
}
#undef INST

/* vcc = (S0.u <= S1.u). */
#define INST SI_INST_VOPC
void si_isa_V_CMP_LE_U32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0 = 0;
	unsigned int s1 = 0;

	union si_reg_t result;

	/* Load operands from registers or as a literal constant. */
	assert(!(INST.src0 == 0xFF && INST.vsrc1 == 0xFF));
	if (INST.src0 == 0xFF)
		s0 = INST.lit_cnst;
	else
		s0 = si_isa_read_reg(work_item, INST.src0).as_uint;
	if (INST.vsrc1 == 0xFF)
		s1 = INST.lit_cnst;
	else
		s1 = si_isa_read_vreg(work_item, INST.vsrc1).as_uint;

	/* Compare the operands. */
	result.as_uint = (s0 <= s1);

	/* Write the results. */
	si_isa_bitmask_sreg(work_item, SI_VCC, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("wf_id%d: vcc<=(%d) ", work_item->id_in_wavefront,
			result.as_uint);
	}
}
#undef INST

/* D.u = (S0.i > S1.i). */
#define INST SI_INST_VOP3b
void si_isa_V_CMP_GT_I32_VOP3b_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	int s0;
	int s1;

	union si_reg_t result;

	/* Load operands from registers. */
	s0 = si_isa_read_reg(work_item, INST.src0).as_int;
	s1 = si_isa_read_reg(work_item, INST.src1).as_int;

	/* Apply negation modifiers. */
	if(INST.neg & 1)
		s0 = -s0;
	if(INST.neg & 2)
		s1 = -s1;

	/* Compare the operands. */
	result.as_uint = (s0 > s1);

	/* Write the results. */
	si_isa_bitmask_sreg(work_item, INST.vdst, result);
	
	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("wf_id%d: S[%d:+1]<=(%d) ", work_item->id_in_wavefront,
			INST.vdst, result.as_uint);
	}
}
#undef INST

/* D.u = (S0.i <> S1.i). */
#define INST SI_INST_VOP3b
void si_isa_V_CMP_NE_I32_VOP3b_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	int s0;
	int s1;

	union si_reg_t result;

	/* Load operands from registers. */
	s0 = si_isa_read_reg(work_item, INST.src0).as_int;
	s1 = si_isa_read_reg(work_item, INST.src1).as_int;

	/* Apply negation modifiers. */
	if(INST.neg & 1)
		s0 = -s0;
	if(INST.neg & 2)
		s1 = -s1;

	/* Compare the operands. */
	result.as_uint = (s0 != s1);

	/* Write the results. */
	si_isa_bitmask_sreg(work_item, INST.vdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("wf_id%d: S[%d:+1]<=(%d) ", work_item->id_in_wavefront,
			INST.vdst, result.as_uint);
	}
}
#undef INST

/* D.u = (S0.u >= S1.u). */
#define INST SI_INST_VOP3b
void si_isa_V_CMP_GE_U32_VOP3b_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0;
	unsigned int s1;

	union si_reg_t result;

	/* Load operands from registers. */
	s0 = si_isa_read_reg(work_item, INST.src0).as_uint;
	s1 = si_isa_read_reg(work_item, INST.src1).as_uint;

	/* Apply negation modifiers. */
	if(INST.neg & 1)
		s0 = -s0;
	if(INST.neg & 2)
		s1 = -s1;

	/* Compare the operands. */
	result.as_uint = (s0 >= s1);

	/* Write the results. */
	si_isa_bitmask_sreg(work_item, INST.vdst, result);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("wf_id%d: S[%d:+1]<=(%d) ", work_item->id_in_wavefront,
			INST.vdst, result.as_uint);
	}
}
#undef INST

/* D.u = S0.u + S1.u + VCC; VCC=carry-out (VOP3:sgpr=carry-out, S2.u=carry-in). */
#define INST SI_INST_VOP3b
void si_isa_V_ADDC_U32_VOP3b_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int s0;
	unsigned int s1;
	unsigned int carry_in;

	union si_reg_t sum;
	union si_reg_t carry_out;

	/* Load operands from registers. */
	s0 = si_isa_read_reg(work_item, INST.src0).as_uint;
	s1 = si_isa_read_reg(work_item, INST.src1).as_uint;
	carry_in = si_isa_read_bitmask_sreg(work_item, INST.src2);

	/* Apply negation modifiers. */
	if(INST.neg & 1)
		s0 = -s0;
	if(INST.neg & 2)
		s1 = -s1;
	assert(!(INST.neg & 4));

	/* Calculate sum and carry. */
	sum.as_uint = s0 + s1 + carry_in;
	carry_out.as_uint = !!(((unsigned long long)s0 + (unsigned long long)s1 + (unsigned long long)carry_in) >> 32);

	/* Write the results. */
	si_isa_write_vreg(work_item, INST.vdst, sum);
	si_isa_bitmask_sreg(work_item, INST.sdst, carry_out);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, sum.as_uint);
		si_isa_debug("wf_id%d: vcc<=(%d) ", work_item->id_in_wavefront, carry_out.as_uint);
	}
}
#undef INST

/* DS[A] = D0; write a Dword. */
#define INST SI_INST_DS
void si_isa_DS_WRITE_B32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int addr;
	unsigned int data0;

	/* Load address and data from registers. */
	addr = si_isa_read_vreg(work_item, INST.addr).as_uint;
	data0 = si_isa_read_vreg(work_item, INST.data0).as_uint;

	/* Write Dword. */
	if(INST.gds)
	{
		mem_write(si_emu->global_mem, addr, 4, &data0);
	}
	else
	{
		mem_write(work_item->work_group->local_mem, addr, 4, &data0);
	}

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("GDS?:%d DS[%d]<=(%d) ", INST.gds, addr, data0);
	}
}
#undef INST

/* R = DS[A]; Dword read. */
#define INST SI_INST_DS
void si_isa_DS_READ_B32_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	unsigned int addr;
	union si_reg_t data;

	/* Load address from register. */
	addr = si_isa_read_vreg(work_item, INST.addr).as_uint;

	/* Read Dword. */
	if(INST.gds)
	{
		mem_read(si_emu->global_mem, addr, 4, &data.as_uint);
	}
	else
	{
		mem_read(work_item->work_group->local_mem, addr, 4, &data.as_uint);
	}

	/* Write results. */
	si_isa_write_vreg(work_item, INST.vdst, data);

	/* Print isa debug information. */
	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%d) ", work_item->id, INST.vdst, data.as_uint);
	}
}
#undef INST

#define INST SI_INST_MTBUF
void si_isa_T_BUFFER_LOAD_FORMAT_X_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	assert(!INST.addr64);
	assert(!INST.index);

	unsigned int offset; 
	int elem_size;
	int num_elems;
	int bytes_to_read;
	struct si_buffer_resource_t buf_desc;
	uint32_t buffer_addr;
	union si_reg_t value;

	if (INST.offen)
	{
		offset = si_isa_read_vreg(work_item, INST.vaddr).as_uint;
	}
	else 
	{
		offset = INST.offset;
	}

	elem_size = si_isa_get_elem_size(INST.dfmt);
	num_elems = si_isa_get_num_elems(INST.dfmt);

	/* If num_elems is greater than 1, we need to see how 
	 * the destination register is handled */
	assert(num_elems == 1);

	bytes_to_read = elem_size * num_elems;

	/* srsrc is in units of 4 registers */
	si_isa_read_buf_res(work_item, &buf_desc, INST.srsrc*4);

	buffer_addr = offset;

	mem_read(si_emu->global_mem, buffer_addr, bytes_to_read, &value);

	si_isa_write_vreg(work_item, INST.vdata, value);

	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: V%u<=(%u)(%d,%gf) ", work_item->id, INST.vdata,
			buffer_addr, value.as_uint, value.as_float);
	}
}
#undef INST

#define INST SI_INST_MTBUF
void si_isa_T_BUFFER_LOAD_FORMAT_XYZW_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	assert(!INST.addr64);
	assert(!INST.index);

	unsigned int offset;
	int elem_size;
	int num_elems;
	struct si_buffer_resource_t buf_desc;
	uint32_t buffer_addr;
	union si_reg_t value;

	if (INST.offen)
	{
		offset = si_isa_read_vreg(work_item, INST.vaddr).as_uint;
	}
	else
	{
		offset = INST.offset;
	}

	elem_size = si_isa_get_elem_size(INST.dfmt);
	num_elems = si_isa_get_num_elems(INST.dfmt);

	/* If num_elems is greater than 1, we need to see how
	 * the destination register is handled */
	assert(num_elems == 4);
	assert(elem_size == 4);

	/* srsrc is in units of 4 registers */
	si_isa_read_buf_res(work_item, &buf_desc, INST.srsrc*4);

	for(unsigned int i = 0; i < 4; i++)
	{
		buffer_addr = offset + 4*i;

		mem_read(si_emu->global_mem, buffer_addr, 4, &value);

		si_isa_write_vreg(work_item, INST.vdata + i, value);

		if (debug_status(si_isa_debug_category))
		{
			si_isa_debug("t%d: V%u<=(%u)(%d,%gf) ", work_item->id, INST.vdata + i,
				buffer_addr, value.as_uint, value.as_float);
		}
	}
}
#undef INST

#define INST SI_INST_MTBUF
void si_isa_T_BUFFER_STORE_FORMAT_X_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	assert(!INST.addr64);
	assert(!INST.index);

	unsigned int offset; 
	int elem_size;
	int num_elems;
	int bytes_to_write;
	struct si_buffer_resource_t buf_desc;
	uint32_t buffer_addr;
	union si_reg_t value;

	if (INST.offen)
	{
		offset = si_isa_read_vreg(work_item, INST.vaddr).as_uint;
	}
	else 
	{
		offset = INST.offset;
	}

	elem_size = si_isa_get_elem_size(INST.dfmt);
	num_elems = si_isa_get_num_elems(INST.dfmt);

	/* If num_elems is greater than 1, we need to see how 
	 * the destination register is handled */
	assert(num_elems == 1);

	bytes_to_write = elem_size * num_elems;

	/* srsrc is in units of 4 registers */
	si_isa_read_buf_res(work_item, &buf_desc, INST.srsrc*4);

	buffer_addr = offset;

	value = si_isa_read_vreg(work_item, INST.vdata);

	mem_write(si_emu->global_mem, buffer_addr, bytes_to_write, &value);

	if (debug_status(si_isa_debug_category))
	{
		si_isa_debug("t%d: (%u)<=V%u(%d,%gf) ", work_item->id, buffer_addr,
			INST.vdata, value.as_uint, value.as_float);
	}
}
#undef INST

#define INST SI_INST_MTBUF
void si_isa_T_BUFFER_STORE_FORMAT_XYZW_impl(struct si_work_item_t *work_item, struct si_inst_t *inst)
{
	assert(!INST.addr64);
	assert(!INST.index);

	unsigned int offset;
	int elem_size;
	int num_elems;
	struct si_buffer_resource_t buf_desc;
	uint32_t buffer_addr;
	union si_reg_t value;

	if (INST.offen)
	{
		offset = si_isa_read_vreg(work_item, INST.vaddr).as_uint;
	}
	else
	{
		offset = INST.offset;
	}

	elem_size = si_isa_get_elem_size(INST.dfmt);
	num_elems = si_isa_get_num_elems(INST.dfmt);

	/* If num_elems is greater than 1, we need to see how
	 * the destination register is handled */
	assert(num_elems == 4);
	assert(elem_size == 4);

	/* srsrc is in units of 4 registers */
	si_isa_read_buf_res(work_item, &buf_desc, INST.srsrc*4);

	for(unsigned int i = 0; i < 4; i++)
	{

		buffer_addr = offset + 4*i;

		value = si_isa_read_vreg(work_item, INST.vdata + i);

		mem_write(si_emu->global_mem, buffer_addr, 4, &value);

		if (debug_status(si_isa_debug_category))
		{
			si_isa_debug("t%d: (%u)<=V%u(%d,%gf) ", work_item->id, buffer_addr,
				INST.vdata + i, value.as_uint, value.as_float);
		}
	}
}
#undef INST
