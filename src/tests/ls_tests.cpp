#include "gtest/gtest.h"
#include "../elfparser.hpp"
#include "../abstract_programheader.hpp"
#include "../abstract_sectionheader.hpp"
#include "../abstract_segments.hpp"
#include "../structures/sectionheader.hpp"
#include "../segment_types/segment_type.hpp"
#include "../dynamicsection.hpp"
#include "../symbols.hpp"

#include <boost/foreach.hpp>

class LSTest : public testing::Test
{
protected:

    virtual void SetUp()
    {
    }

    ELFParser m_parser;
};

TEST_F(LSTest, Sixtyfour_Intel_ls)
{
    m_parser.parse("../test_files/64_intel_ls");
    EXPECT_TRUE(m_parser.getElfHeader().is64());
    EXPECT_TRUE(m_parser.getElfHeader().isLE());
    EXPECT_STREQ("7f 45 4c 46", m_parser.getElfHeader().getMagic().c_str());
    EXPECT_EQ(0x404888, m_parser.getElfHeader().getEntryPoint());
    EXPECT_EQ("0x404888", m_parser.getElfHeader().getEntryPointString());
    EXPECT_EQ(64, m_parser.getElfHeader().getProgramOffset());
    EXPECT_EQ(108296, m_parser.getElfHeader().getSectionOffset());
    EXPECT_EQ(56, m_parser.getElfHeader().getProgramSize());
    EXPECT_EQ(9, m_parser.getElfHeader().getProgramCount());
    EXPECT_EQ(64, m_parser.getElfHeader().getSectionSize());
    EXPECT_EQ(28, m_parser.getElfHeader().getSectionCount());
    EXPECT_EQ(27, m_parser.getElfHeader().getStringTableIndex());
    EXPECT_EQ(9, m_parser.getProgramHeaders().getProgramHeaders().size());
    EXPECT_EQ(28, m_parser.getSectionHeaders().getSections().size());
    EXPECT_EQ(27, m_parser.getSectionHeaders().getStringTableIndex());
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[1].getName().c_str(), ".interp");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[2].getName().c_str(), ".note.ABI-tag");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[3].getName().c_str(), ".note.gnu.build-id");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[4].getName().c_str(), ".gnu.hash");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[5].getName().c_str(), ".dynsym");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[6].getName().c_str(), ".dynstr");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[7].getName().c_str(), ".gnu.version");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[8].getName().c_str(), ".gnu.version_r");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[9].getName().c_str(), ".rela.dyn");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[10].getName().c_str(), ".rela.plt");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[11].getName().c_str(), ".init");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[12].getName().c_str(), ".plt");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[13].getName().c_str(), ".text");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[14].getName().c_str(), ".fini");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[15].getName().c_str(), ".rodata");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[16].getName().c_str(), ".eh_frame_hdr");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[17].getName().c_str(), ".eh_frame");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[18].getName().c_str(), ".init_array");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[19].getName().c_str(), ".fini_array");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[20].getName().c_str(), ".jcr");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[21].getName().c_str(), ".dynamic");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[22].getName().c_str(), ".got");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[23].getName().c_str(), ".got.plt");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[24].getName().c_str(), ".data");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[25].getName().c_str(), ".bss");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[26].getName().c_str(), ".gnu_debuglink");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[27].getName().c_str(), ".shstrtab");
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[0].getVirtAddress(), 0);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[1].getVirtAddress(), 0x400238);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[2].getVirtAddress(), 0x400254);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[3].getVirtAddress(), 0x400274);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[4].getVirtAddress(), 0x400298);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[5].getVirtAddress(), 0x400300);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[6].getVirtAddress(), 0x400f18);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[7].getVirtAddress(), 0x4014ac);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[8].getVirtAddress(), 0x4015b0);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[9].getVirtAddress(), 0x401650);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[10].getVirtAddress(), 0x4016f8);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[11].getVirtAddress(), 0x402178);
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[0].getName().c_str(),"PT_PHDR");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[1].getName().c_str(),"PT_INTERP");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[2].getName().c_str(),"PT_LOAD");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[3].getName().c_str(),"PT_LOAD");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[4].getName().c_str(),"PT_DYNAMIC");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[5].getName().c_str(),"PT_NOTE");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[6].getName().c_str(),"GNU_EH_FRAME");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[7].getName().c_str(),"GNU_STACK");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[8].getName().c_str(),"GNU_RELRO");
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[0].getVirtualAddress(), 0x400040);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[1].getVirtualAddress(), 0x400238);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[2].getVirtualAddress(), 0x400000);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[3].getVirtualAddress(), 0x619dd8);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[4].getVirtualAddress(), 0x619df0);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[5].getVirtualAddress(), 0x400254);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[6].getVirtualAddress(), 0x41759c);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[7].getVirtualAddress(), 0);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[8].getVirtualAddress(), 0x619dd8);
    EXPECT_EQ(0x400000, m_parser.getSegments().getBaseAddress());
    EXPECT_EQ(105968, m_parser.getSegments().getDynamicSection().getOffset());
    EXPECT_EQ(4198168, m_parser.getSegments().getDynamicSection().getStringTableVirtualAddress());
    EXPECT_EQ(1427, m_parser.getSegments().getDynamicSection().getStringTableSize());
    EXPECT_EQ(4195072, m_parser.getSegments().getDynamicSection().getSymbolTableVirtAddress());
    EXPECT_EQ(114, m_parser.getSegments().getDynamicSection().getSymbolTableSize());
    EXPECT_EQ(129, m_parser.getSegments().getDynamicSymbols().getSymbols().size());
}

TEST_F(LSTest, Thirtytwo_Intel_ls)
{
    m_parser.parse("../test_files/32_intel_ls");
    EXPECT_FALSE(m_parser.getElfHeader().is64());
    EXPECT_TRUE(m_parser.getElfHeader().isLE());
    EXPECT_STREQ("7f 45 4c 46", m_parser.getElfHeader().getMagic().c_str());
    EXPECT_EQ(0x804c070, m_parser.getElfHeader().getEntryPoint());
    EXPECT_EQ("0x804c070", m_parser.getElfHeader().getEntryPointString());
    EXPECT_EQ(52, m_parser.getElfHeader().getProgramOffset());
    EXPECT_EQ(107588, m_parser.getElfHeader().getSectionOffset());
    EXPECT_EQ(32, m_parser.getElfHeader().getProgramSize());
    EXPECT_EQ(9, m_parser.getElfHeader().getProgramCount());
    EXPECT_EQ(40, m_parser.getElfHeader().getSectionSize());
    EXPECT_EQ(28, m_parser.getElfHeader().getSectionCount());
    EXPECT_EQ(27, m_parser.getElfHeader().getStringTableIndex());
    EXPECT_EQ(9, m_parser.getProgramHeaders().getProgramHeaders().size());
    EXPECT_EQ(28, m_parser.getSectionHeaders().getSections().size());
    EXPECT_EQ(27, m_parser.getSectionHeaders().getStringTableIndex());
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[1].getName().c_str(), ".interp");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[2].getName().c_str(), ".note.ABI-tag");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[3].getName().c_str(), ".note.gnu.build-id");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[4].getName().c_str(), ".gnu.hash");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[5].getName().c_str(), ".dynsym");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[6].getName().c_str(), ".dynstr");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[7].getName().c_str(), ".gnu.version");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[8].getName().c_str(), ".gnu.version_r");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[9].getName().c_str(), ".rel.dyn");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[10].getName().c_str(), ".rel.plt");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[11].getName().c_str(), ".init");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[12].getName().c_str(), ".plt");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[13].getName().c_str(), ".text");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[14].getName().c_str(), ".fini");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[15].getName().c_str(), ".rodata");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[16].getName().c_str(), ".eh_frame_hdr");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[17].getName().c_str(), ".eh_frame");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[18].getName().c_str(), ".init_array");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[19].getName().c_str(), ".fini_array");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[20].getName().c_str(), ".jcr");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[21].getName().c_str(), ".dynamic");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[22].getName().c_str(), ".got");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[23].getName().c_str(), ".got.plt");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[24].getName().c_str(), ".data");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[25].getName().c_str(), ".bss");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[26].getName().c_str(), ".gnu_debuglink");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[27].getName().c_str(), ".shstrtab");
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[0].getVirtAddress(), 0);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[1].getVirtAddress(), 0x8048154);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[2].getVirtAddress(), 0x8048168);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[3].getVirtAddress(), 0x8048188);
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[0].getName().c_str(),"PT_PHDR");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[1].getName().c_str(),"PT_INTERP");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[2].getName().c_str(),"PT_LOAD");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[3].getName().c_str(),"PT_LOAD");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[4].getName().c_str(),"PT_DYNAMIC");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[5].getName().c_str(),"PT_NOTE");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[6].getName().c_str(),"GNU_EH_FRAME");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[7].getName().c_str(),"GNU_STACK");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[8].getName().c_str(),"GNU_RELRO");
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[0].getVirtualAddress(), 0x08048034);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[1].getVirtualAddress(), 0x08048154);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[2].getVirtualAddress(), 0x08048000);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[3].getVirtualAddress(), 0x08062eec);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[4].getVirtualAddress(), 0x08062ef8);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[5].getVirtualAddress(), 0x08048168);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[6].getVirtualAddress(), 0x0805ed88);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[7].getVirtualAddress(), 0);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[8].getVirtualAddress(), 0x08062eec);
    EXPECT_EQ(0x08048000, m_parser.getSegments().getBaseAddress());
    EXPECT_EQ(0x019ef8, m_parser.getSegments().getDynamicSection().getOffset());
    EXPECT_EQ(0x8048a38, m_parser.getSegments().getDynamicSection().getStringTableVirtualAddress());
    EXPECT_EQ(1493, m_parser.getSegments().getDynamicSection().getStringTableSize());
    EXPECT_EQ(0x8048218, m_parser.getSegments().getDynamicSection().getSymbolTableVirtAddress());
    EXPECT_EQ(114, m_parser.getSegments().getDynamicSection().getSymbolTableSize());
    EXPECT_EQ(130, m_parser.getSegments().getDynamicSymbols().getSymbols().size());
}
/*
TEST_F(LSTest, Sixtyfour_Intel_ls_upx)
{
    m_parser.parse("../test_files/64_intel_ls_upx");
    m_parser.evaluate();
    EXPECT_TRUE(0 > m_parser.getScore());
    ASSERT_FALSE(m_parser.m_reasons.empty());

    bool found_probably_packed = false;
    for(std::vector<std::pair<boost::int32_t, std::string> >::const_iterator reasons = m_parser.m_reasons.begin();
        reasons != m_parser.m_reasons.end(); ++reasons)
    {
        if (reasons->second == "Probably packed")
        {
            found_probably_packed = true;
        }
    }
    EXPECT_TRUE(found_probably_packed);
}
*/
TEST_F(LSTest, Thirtytwo_Arm_ls)
{
    m_parser.parse("../test_files/32_arm_ls");
    EXPECT_FALSE(m_parser.getElfHeader().is64());
    EXPECT_TRUE(m_parser.getElfHeader().isLE());
    EXPECT_STREQ("7f 45 4c 46", m_parser.getElfHeader().getMagic().c_str());
    EXPECT_EQ(0xc3f0, m_parser.getElfHeader().getEntryPoint());
    EXPECT_EQ("0xc3f0", m_parser.getElfHeader().getEntryPointString());
    EXPECT_EQ(52, m_parser.getElfHeader().getProgramOffset());
    EXPECT_EQ(95220, m_parser.getElfHeader().getSectionOffset());
    EXPECT_STREQ("0x5000002", m_parser.getElfHeader().getFlags().c_str());
    EXPECT_EQ(32, m_parser.getElfHeader().getProgramSize());
    EXPECT_EQ(9, m_parser.getElfHeader().getProgramCount());
    EXPECT_EQ(40, m_parser.getElfHeader().getSectionSize());
    EXPECT_EQ(28, m_parser.getElfHeader().getSectionCount());
    EXPECT_EQ(27, m_parser.getElfHeader().getStringTableIndex());
    EXPECT_EQ(9, m_parser.getProgramHeaders().getProgramHeaders().size());
    EXPECT_EQ(28, m_parser.getSectionHeaders().getSections().size());
    EXPECT_EQ(27, m_parser.getSectionHeaders().getStringTableIndex());
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[1].getName().c_str(), ".interp");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[2].getName().c_str(), ".note.ABI-tag");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[3].getName().c_str(), ".note.gnu.build-id");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[4].getName().c_str(), ".hash");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[5].getName().c_str(), ".gnu.hash");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[6].getName().c_str(), ".dynsym");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[7].getName().c_str(), ".dynstr");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[8].getName().c_str(), ".gnu.version");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[9].getName().c_str(), ".gnu.version_r");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[10].getName().c_str(), ".rel.dyn");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[11].getName().c_str(), ".rel.plt");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[12].getName().c_str(), ".init");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[13].getName().c_str(), ".plt");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[14].getName().c_str(), ".text");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[15].getName().c_str(), ".fini");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[16].getName().c_str(), ".rodata");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[17].getName().c_str(), ".ARM.exidx");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[18].getName().c_str(), ".eh_frame");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[19].getName().c_str(), ".init_array");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[20].getName().c_str(), ".fini_array");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[21].getName().c_str(), ".jcr");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[22].getName().c_str(), ".dynamic");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[23].getName().c_str(), ".got");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[24].getName().c_str(), ".data");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[25].getName().c_str(), ".bss");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[26].getName().c_str(), ".ARM.attributes");
    EXPECT_STREQ(m_parser.getSectionHeaders().getSections()[27].getName().c_str(), ".shstrtab");
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[0].getVirtAddress(), 0);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[1].getVirtAddress(), 0x8154);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[2].getVirtAddress(), 0x8170);
    EXPECT_EQ(m_parser.getSectionHeaders().getSections()[3].getVirtAddress(), 0x8190);
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[0].getName().c_str(),"EXIDX");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[1].getName().c_str(),"PT_PHDR");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[2].getName().c_str(),"PT_INTERP");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[3].getName().c_str(),"PT_LOAD");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[4].getName().c_str(),"PT_LOAD");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[5].getName().c_str(),"PT_DYNAMIC");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[6].getName().c_str(),"PT_NOTE");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[7].getName().c_str(),"GNU_STACK");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[8].getName().c_str(),"GNU_RELRO");
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[0].getVirtualAddress(), 0x0001e0c0);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[1].getVirtualAddress(), 0x00008034);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[2].getVirtualAddress(), 0x00008154);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[3].getVirtualAddress(), 0x00008000);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[4].getVirtualAddress(), 0x00026edc);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[5].getVirtualAddress(), 0x00026ee8);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[6].getVirtualAddress(), 0x00008170);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[7].getVirtualAddress(), 0);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[8].getVirtualAddress(), 0x00026edc);
    EXPECT_EQ(0x00008000, m_parser.getSegments().getBaseAddress());
    EXPECT_EQ(0x016ee8, m_parser.getSegments().getDynamicSection().getOffset());
    EXPECT_EQ(0x90fc, m_parser.getSegments().getDynamicSection().getStringTableVirtualAddress());
    EXPECT_EQ(1426, m_parser.getSegments().getDynamicSection().getStringTableSize());
    EXPECT_EQ(0x892c, m_parser.getSegments().getDynamicSection().getSymbolTableVirtAddress());
    EXPECT_EQ(125, m_parser.getSegments().getDynamicSection().getSymbolTableSize());
    EXPECT_EQ(125, m_parser.getSegments().getDynamicSymbols().getSymbols().size());
}

TEST_F(LSTest, mips_be_ping)
{
    m_parser.parse("../test_files/32_mips_be_ping");
    EXPECT_FALSE(m_parser.getElfHeader().is64());
    EXPECT_FALSE(m_parser.getElfHeader().isLE());
    EXPECT_STREQ("7f 45 4c 46", m_parser.getElfHeader().getMagic().c_str());
    EXPECT_EQ(0x401440, m_parser.getElfHeader().getEntryPoint());
    EXPECT_EQ("0x401440", m_parser.getElfHeader().getEntryPointString());
    EXPECT_EQ(52, m_parser.getElfHeader().getProgramOffset());
    EXPECT_EQ(0, m_parser.getElfHeader().getSectionOffset());
    EXPECT_STREQ("0x50001005", m_parser.getElfHeader().getFlags().c_str());
    EXPECT_EQ(32, m_parser.getElfHeader().getProgramSize());
    EXPECT_EQ(8, m_parser.getElfHeader().getProgramCount());
    EXPECT_EQ(0, m_parser.getElfHeader().getSectionSize());
    EXPECT_EQ(0, m_parser.getElfHeader().getSectionCount());
    EXPECT_EQ(0, m_parser.getElfHeader().getStringTableIndex());
    EXPECT_EQ(8, m_parser.getProgramHeaders().getProgramHeaders().size());
    EXPECT_EQ(0, m_parser.getSectionHeaders().getSections().size());
    EXPECT_EQ(0, m_parser.getSectionHeaders().getStringTableIndex());
    EXPECT_TRUE(m_parser.getSectionHeaders().getSections().empty());
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[0].getName().c_str(),"PT_PHDR");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[1].getName().c_str(),"PT_INTERP");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[2].getName().c_str(),"REGINFO");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[3].getName().c_str(),"PT_LOAD");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[4].getName().c_str(),"PT_LOAD");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[5].getName().c_str(),"PT_DYNAMIC");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[6].getName().c_str(),"GNU_STACK");
    EXPECT_STREQ(m_parser.getProgramHeaders().getProgramHeaders()[7].getName().c_str(),"PT_NULL");
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[0].getVirtualAddress(), 0x00400034);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[1].getVirtualAddress(), 0x00400134);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[2].getVirtualAddress(), 0x00400148);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[3].getVirtualAddress(), 0x00400000);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[4].getVirtualAddress(), 0x00416bb8);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[5].getVirtualAddress(), 0x00400160);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[6].getVirtualAddress(), 0);
    EXPECT_EQ(m_parser.getProgramHeaders().getProgramHeaders()[7].getVirtualAddress(), 0);
    EXPECT_EQ(0x00400000, m_parser.getSegments().getBaseAddress());
    EXPECT_EQ(0x000160, m_parser.getSegments().getDynamicSection().getOffset());
    EXPECT_EQ(0x4009f4, m_parser.getSegments().getDynamicSection().getStringTableVirtualAddress());
    EXPECT_EQ(758, m_parser.getSegments().getDynamicSection().getStringTableSize());
    EXPECT_EQ(0x4004d4, m_parser.getSegments().getDynamicSection().getSymbolTableVirtAddress());
    EXPECT_EQ(82, m_parser.getSegments().getDynamicSection().getSymbolTableSize());
    EXPECT_EQ(82, m_parser.getSegments().getDynamicSymbols().getSymbols().size());
}
