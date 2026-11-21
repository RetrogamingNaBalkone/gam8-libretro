"""Translate MAME's NMOS 6502 bus-operation lists to resumable C11.

Usage: python tools/generate_cpu.py path/to/mame/src/devices/cpu/m6502
Normal builds use the checked-in output and need no generator or network.
"""
from pathlib import Path
import re
import sys

upstream=Path(sys.argv[1])
target=Path(__file__).resolve().parents[1]/"src"
banner="// Generated from MAME, copyright Olivier Galibert. SPDX-License-Identifier: BSD-3-Clause\n"


def convert(line):
    line=re.sub(r"\b(u?int8_t)\(",r"(\1)(",line)
    line=re.sub(r"\bm_(\w+)",r"c->\1",line)
    line=re.sub(r"\b(do_\w+|set_nz)\((?!\))",r"\1(c, ",line)
    return re.sub(r"\b(do_\w+)\(\)",r"\1(c)",line)

source=(upstream/"m6502.cpp").read_text()
alu=source[source.index("void m6502_device::do_adc_d"):source.index("offs_t m6502_device::pc_to_external")]
alu+=source[source.index("void m6502_device::set_nz"):source.index("std::unique_ptr<util::disasm_interface>")]
converted=[]
for line in alu.splitlines():
    if "m6502_device::" in line:
        line="static "+line.replace("m6502_device::","").replace("(","(cpu_t *c, ",1).replace(", )",")")
    else:
        line=convert(re.sub(r"\bc\b","carry",line))
    converted.append(line)
(target/"cpu_alu.inc").write_text(banner+"\n".join(converted).rstrip()+"\n",encoding="utf8")

ops={}
for line in (upstream/"om6502.lst").read_text().splitlines():
    if not line.strip() or line.startswith("#"): continue
    if not line[0].isspace():
        name=line.strip()
        ops[name]=[]
    else:
        ops[name].append(line.expandtabs(4))
dispatch=" ".join(l for l in (upstream/"dm6502.lst").read_text().splitlines() if not l.startswith("#")).split()
assert len(dispatch)==257
out=[banner,"// One invocation performs precisely one memory access.",
     "static void cpu_tick(machine_t *m, bool irq)", "{", "    cpu_t *c=&m->cpu;",
     "    for (;;) {", "        switch (c->micro) {"]
entry={}
number=0

for name in dict.fromkeys(dispatch):
    entry[name]=number
    out.append(f"        case {number}: /* {name} */")
    number+=1
    for line in ops[name]:
        code=line.split("//")[0].strip()
        if not code: continue
        if "standard_irq_callback" in code: line="            (void)0;"
        if "m_inst_state = -1;" in code: continue
        if "prefetch();" in code:
            out+=["            c->IR=read_bus(m,c->PC);",f"            c->micro={number}; return;",
                  f"        case {number}:","            if (c->irq_sampled && !(c->P & F_I)) {",
                  "                c->irq_taken=true; c->IR=0;", "            } else ++c->PC;"]
            number+=1
            continue
        access=re.search(r"\b(read(?:_pc|_arg)?(?:_noirq)?|write)\(",code)
        if access and "_noirq" not in code:
            out.append("            c->irq_sampled=irq;")
        line=convert(line)
        line=re.sub(r"\bread_pc(?:_noirq)?\(\)","read_bus(m,c->PC)",line)
        line=re.sub(r"\bread(?:_arg(?:_noirq)?)?\(","read_bus(m,",line)
        line=re.sub(r"\bwrite\(","write_bus(m,",line)
        line=line.replace("dec_SP();","c->SP=set_l(c->SP,c->SP-1);").replace("inc_SP();","c->SP=set_l(c->SP,c->SP+1);")
        out.append("        "+line)
        if access:
            out.extend([f"            c->micro={number}; return;",f"        case {number}:;"])
            number+=1
    out.append("            break;")
out.extend(["        }","        c->micro=decode[c->IR];","    }","}"])
table=",\n".join("    "+", ".join(str(entry[x]) for x in dispatch[i:i+16]) for i in range(0,256,16))
out.insert(1,f"enum {{ CPU_RESET={entry['reset']}, CPU_MICRO_COUNT={number} }};\nstatic const uint16_t decode[256]={{\n{table}\n}};")
(target/"cpu_ops.inc").write_text("\n".join(out)+"\n",encoding="utf8")
