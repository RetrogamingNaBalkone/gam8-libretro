// Generated from MAME, copyright Olivier Galibert. SPDX-License-Identifier: BSD-3-Clause
static void do_adc_d(cpu_t *c, uint8_t val)
{
	uint8_t carry = c->P & F_C ? 1 : 0;
	c->P &= ~(F_N|F_V|F_Z|F_C);
	uint8_t al = (c->A & 15) + (val & 15) + carry;
	if(al > 9)
		al += 6;
	uint8_t ah = (c->A >> 4) + (val >> 4) + (al > 15);
	if(!(uint8_t)(c->A + val + carry))
		c->P |= F_Z;
	else if(ah & 8)
		c->P |= F_N;
	if(~(c->A^val) & (c->A^(ah << 4)) & 0x80)
		c->P |= F_V;
	if(ah > 9)
		ah += 6;
	if(ah > 15)
		c->P |= F_C;
	c->A = (ah << 4) | (al & 15);
}

static void do_adc_nd(cpu_t *c, uint8_t val)
{
	uint16_t sum;
	sum = c->A + val + (c->P & F_C ? 1 : 0);
	c->P &= ~(F_N|F_V|F_Z|F_C);
	if(!(uint8_t)(sum))
		c->P |= F_Z;
	else if((int8_t)(sum) < 0)
		c->P |= F_N;
	if(~(c->A^val) & (c->A^sum) & 0x80)
		c->P |= F_V;
	if(sum & 0xff00)
		c->P |= F_C;
	c->A = sum;
}

static void do_adc(cpu_t *c, uint8_t val)
{
	if(c->P & F_D)
		do_adc_d(c, val);
	else
		do_adc_nd(c, val);
}

static void do_arr_nd(cpu_t *c)
{
	bool carry = c->P & F_C;
	c->P &= ~(F_N|F_Z|F_C|F_V);
	c->A >>= 1;
	if(carry)
		c->A |= 0x80;
	if(!c->A)
		c->P |= F_Z;
	else if((int8_t)(c->A)<0)
		c->P |= F_N;
	if(c->A & 0x40)
		c->P |= F_V|F_C;
	if(c->A & 0x20)
		c->P ^= F_V;
}

static void do_arr_d(cpu_t *c)
{
	// The adc/ror interaction gives an extremely weird result
	bool carry = c->P & F_C;
	c->P &= ~(F_N|F_Z|F_C|F_V);
	uint8_t a = c->A >> 1;
	if(carry)
		a |= 0x80;
	if(!a)
		c->P |= F_Z;
	else if((int8_t)(a) < 0)
		c->P |= F_N;
	if((a ^ c->A) & 0x40)
		c->P |= F_V;

	if((c->A & 0x0f) >= 0x05)
		a = ((a + 6) & 0x0f) | (a & 0xf0);

	if((c->A & 0xf0) >= 0x50) {
		a += 0x60;
		c->P |= F_C;
	}
	c->A = a;
}

static void do_arr(cpu_t *c)
{
	if(c->P & F_D)
		do_arr_d(c);
	else
		do_arr_nd(c);
}

static void do_cmp(cpu_t *c, uint8_t val1, uint8_t val2)
{
	c->P &= ~(F_N|F_Z|F_C);
	uint16_t r = val1-val2;
	if(!r)
		c->P |= F_Z;
	else if((int8_t)(r) < 0)
		c->P |= F_N;
	if(!(r & 0xff00))
		c->P |= F_C;
}

static void do_sbc_d(cpu_t *c, uint8_t val)
{
	uint8_t carry = c->P & F_C ? 0 : 1;
	c->P &= ~(F_N|F_V|F_Z|F_C);
	uint16_t diff = c->A - val - carry;
	uint8_t al = (c->A & 15) - (val & 15) - carry;
	uint8_t ah = (c->A >> 4) - (val >> 4) - ((int8_t)(al) < 0);
	if(!(uint8_t)(diff))
		c->P |= F_Z;
	else if(diff & 0x80)
		c->P |= F_N;
	if((c->A^val) & (c->A^diff) & 0x80)
		c->P |= F_V;
	if(!(diff & 0xff00))
		c->P |= F_C;
	if((int8_t)(al) < 0)
		al -= 6;
	if((int8_t)(ah) < 0)
		ah -= 6;
	c->A = (ah << 4) | (al & 15);
}

static void do_sbc_nd(cpu_t *c, uint8_t val)
{
	uint16_t diff = c->A - val - (c->P & F_C ? 0 : 1);
	c->P &= ~(F_N|F_V|F_Z|F_C);
	if(!(uint8_t)(diff))
		c->P |= F_Z;
	else if((int8_t)(diff) < 0)
		c->P |= F_N;
	if((c->A^val) & (c->A^diff) & 0x80)
		c->P |= F_V;
	if(!(diff & 0xff00))
		c->P |= F_C;
	c->A = diff;
}

static void do_sbc(cpu_t *c, uint8_t val)
{
	if(c->P & F_D)
		do_sbc_d(c, val);
	else
		do_sbc_nd(c, val);
}

static void do_bit(cpu_t *c, uint8_t val)
{
	c->P &= ~(F_N|F_Z|F_V);
	uint8_t r = c->A & val;
	if(!r)
		c->P |= F_Z;
	if(val & 0x80)
		c->P |= F_N;
	if(val & 0x40)
		c->P |= F_V;
}

static uint8_t do_asl(cpu_t *c, uint8_t v)
{
	c->P &= ~(F_N|F_Z|F_C);
	uint8_t r = v<<1;
	if(!r)
		c->P |= F_Z;
	else if((int8_t)(r) < 0)
		c->P |= F_N;
	if(v & 0x80)
		c->P |= F_C;
	return r;
}

static uint8_t do_lsr(cpu_t *c, uint8_t v)
{
	c->P &= ~(F_N|F_Z|F_C);
	if(v & 1)
		c->P |= F_C;
	v >>= 1;
	if(!v)
		c->P |= F_Z;
	return v;
}

static uint8_t do_ror(cpu_t *c, uint8_t v)
{
	bool carry = c->P & F_C;
	c->P &= ~(F_N|F_Z|F_C);
	if(v & 1)
		c->P |= F_C;
	v >>= 1;
	if(carry)
		v |= 0x80;
	if(!v)
		c->P |= F_Z;
	else if((int8_t)(v)<0)
		c->P |= F_N;
	return v;
}

static uint8_t do_rol(cpu_t *c, uint8_t v)
{
	bool carry = c->P & F_C;
	c->P &= ~(F_N|F_Z|F_C);
	if(v & 0x80)
		c->P |= F_C;
	v <<= 1;
	if(carry)
		v |= 0x01;
	if(!v)
		c->P |= F_Z;
	else if((int8_t)(v)<0)
		c->P |= F_N;
	return v;
}

static uint8_t do_asr(cpu_t *c, uint8_t v)
{
	c->P &= ~(F_N|F_Z|F_C);
	if(v & 1)
		c->P |= F_C;
	v >>= 1;
	if(!v)
		c->P |= F_Z;
	else if(v & 0x40) {
		c->P |= F_N;
		v |= 0x80;
	}
	return v;
}

static void set_nz(cpu_t *c, uint8_t v)
{
	c->P &= ~(F_Z|F_N);
	if(v & 0x80)
		c->P |= F_N;
	if(!v)
		c->P |= F_Z;
}
