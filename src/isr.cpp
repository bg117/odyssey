#include "low_level/isr.hpp"

#include "misc/log.hpp"

namespace
{
low_level::isr::handler handlers[256] = {};
}

extern "C" void exception_handler(low_level::isr::interrupt_info *int_info)
{
  if (handlers[int_info->vector] != nullptr)
  {
    handlers[int_info->vector](int_info);
  }
}

#pragma region Stub declarations
extern "C" void interrupt_stub_0();
extern "C" void interrupt_stub_1();
extern "C" void interrupt_stub_2();
extern "C" void interrupt_stub_3();
extern "C" void interrupt_stub_4();
extern "C" void interrupt_stub_5();
extern "C" void interrupt_stub_6();
extern "C" void interrupt_stub_7();
extern "C" void interrupt_stub_8();
extern "C" void interrupt_stub_9();
extern "C" void interrupt_stub_10();
extern "C" void interrupt_stub_11();
extern "C" void interrupt_stub_12();
extern "C" void interrupt_stub_13();
extern "C" void interrupt_stub_14();
extern "C" void interrupt_stub_15();
extern "C" void interrupt_stub_16();
extern "C" void interrupt_stub_17();
extern "C" void interrupt_stub_18();
extern "C" void interrupt_stub_19();
extern "C" void interrupt_stub_20();
extern "C" void interrupt_stub_21();
extern "C" void interrupt_stub_22();
extern "C" void interrupt_stub_23();
extern "C" void interrupt_stub_24();
extern "C" void interrupt_stub_25();
extern "C" void interrupt_stub_26();
extern "C" void interrupt_stub_27();
extern "C" void interrupt_stub_28();
extern "C" void interrupt_stub_29();
extern "C" void interrupt_stub_30();
extern "C" void interrupt_stub_31();
extern "C" void interrupt_stub_32();
extern "C" void interrupt_stub_33();
extern "C" void interrupt_stub_34();
extern "C" void interrupt_stub_35();
extern "C" void interrupt_stub_36();
extern "C" void interrupt_stub_37();
extern "C" void interrupt_stub_38();
extern "C" void interrupt_stub_39();
extern "C" void interrupt_stub_40();
extern "C" void interrupt_stub_41();
extern "C" void interrupt_stub_42();
extern "C" void interrupt_stub_43();
extern "C" void interrupt_stub_44();
extern "C" void interrupt_stub_45();
extern "C" void interrupt_stub_46();
extern "C" void interrupt_stub_47();
extern "C" void interrupt_stub_48();
extern "C" void interrupt_stub_49();
extern "C" void interrupt_stub_50();
extern "C" void interrupt_stub_51();
extern "C" void interrupt_stub_52();
extern "C" void interrupt_stub_53();
extern "C" void interrupt_stub_54();
extern "C" void interrupt_stub_55();
extern "C" void interrupt_stub_56();
extern "C" void interrupt_stub_57();
extern "C" void interrupt_stub_58();
extern "C" void interrupt_stub_59();
extern "C" void interrupt_stub_60();
extern "C" void interrupt_stub_61();
extern "C" void interrupt_stub_62();
extern "C" void interrupt_stub_63();
extern "C" void interrupt_stub_64();
extern "C" void interrupt_stub_65();
extern "C" void interrupt_stub_66();
extern "C" void interrupt_stub_67();
extern "C" void interrupt_stub_68();
extern "C" void interrupt_stub_69();
extern "C" void interrupt_stub_70();
extern "C" void interrupt_stub_71();
extern "C" void interrupt_stub_72();
extern "C" void interrupt_stub_73();
extern "C" void interrupt_stub_74();
extern "C" void interrupt_stub_75();
extern "C" void interrupt_stub_76();
extern "C" void interrupt_stub_77();
extern "C" void interrupt_stub_78();
extern "C" void interrupt_stub_79();
extern "C" void interrupt_stub_80();
extern "C" void interrupt_stub_81();
extern "C" void interrupt_stub_82();
extern "C" void interrupt_stub_83();
extern "C" void interrupt_stub_84();
extern "C" void interrupt_stub_85();
extern "C" void interrupt_stub_86();
extern "C" void interrupt_stub_87();
extern "C" void interrupt_stub_88();
extern "C" void interrupt_stub_89();
extern "C" void interrupt_stub_90();
extern "C" void interrupt_stub_91();
extern "C" void interrupt_stub_92();
extern "C" void interrupt_stub_93();
extern "C" void interrupt_stub_94();
extern "C" void interrupt_stub_95();
extern "C" void interrupt_stub_96();
extern "C" void interrupt_stub_97();
extern "C" void interrupt_stub_98();
extern "C" void interrupt_stub_99();
extern "C" void interrupt_stub_100();
extern "C" void interrupt_stub_101();
extern "C" void interrupt_stub_102();
extern "C" void interrupt_stub_103();
extern "C" void interrupt_stub_104();
extern "C" void interrupt_stub_105();
extern "C" void interrupt_stub_106();
extern "C" void interrupt_stub_107();
extern "C" void interrupt_stub_108();
extern "C" void interrupt_stub_109();
extern "C" void interrupt_stub_110();
extern "C" void interrupt_stub_111();
extern "C" void interrupt_stub_112();
extern "C" void interrupt_stub_113();
extern "C" void interrupt_stub_114();
extern "C" void interrupt_stub_115();
extern "C" void interrupt_stub_116();
extern "C" void interrupt_stub_117();
extern "C" void interrupt_stub_118();
extern "C" void interrupt_stub_119();
extern "C" void interrupt_stub_120();
extern "C" void interrupt_stub_121();
extern "C" void interrupt_stub_122();
extern "C" void interrupt_stub_123();
extern "C" void interrupt_stub_124();
extern "C" void interrupt_stub_125();
extern "C" void interrupt_stub_126();
extern "C" void interrupt_stub_127();
extern "C" void interrupt_stub_128();
extern "C" void interrupt_stub_129();
extern "C" void interrupt_stub_130();
extern "C" void interrupt_stub_131();
extern "C" void interrupt_stub_132();
extern "C" void interrupt_stub_133();
extern "C" void interrupt_stub_134();
extern "C" void interrupt_stub_135();
extern "C" void interrupt_stub_136();
extern "C" void interrupt_stub_137();
extern "C" void interrupt_stub_138();
extern "C" void interrupt_stub_139();
extern "C" void interrupt_stub_140();
extern "C" void interrupt_stub_141();
extern "C" void interrupt_stub_142();
extern "C" void interrupt_stub_143();
extern "C" void interrupt_stub_144();
extern "C" void interrupt_stub_145();
extern "C" void interrupt_stub_146();
extern "C" void interrupt_stub_147();
extern "C" void interrupt_stub_148();
extern "C" void interrupt_stub_149();
extern "C" void interrupt_stub_150();
extern "C" void interrupt_stub_151();
extern "C" void interrupt_stub_152();
extern "C" void interrupt_stub_153();
extern "C" void interrupt_stub_154();
extern "C" void interrupt_stub_155();
extern "C" void interrupt_stub_156();
extern "C" void interrupt_stub_157();
extern "C" void interrupt_stub_158();
extern "C" void interrupt_stub_159();
extern "C" void interrupt_stub_160();
extern "C" void interrupt_stub_161();
extern "C" void interrupt_stub_162();
extern "C" void interrupt_stub_163();
extern "C" void interrupt_stub_164();
extern "C" void interrupt_stub_165();
extern "C" void interrupt_stub_166();
extern "C" void interrupt_stub_167();
extern "C" void interrupt_stub_168();
extern "C" void interrupt_stub_169();
extern "C" void interrupt_stub_170();
extern "C" void interrupt_stub_171();
extern "C" void interrupt_stub_172();
extern "C" void interrupt_stub_173();
extern "C" void interrupt_stub_174();
extern "C" void interrupt_stub_175();
extern "C" void interrupt_stub_176();
extern "C" void interrupt_stub_177();
extern "C" void interrupt_stub_178();
extern "C" void interrupt_stub_179();
extern "C" void interrupt_stub_180();
extern "C" void interrupt_stub_181();
extern "C" void interrupt_stub_182();
extern "C" void interrupt_stub_183();
extern "C" void interrupt_stub_184();
extern "C" void interrupt_stub_185();
extern "C" void interrupt_stub_186();
extern "C" void interrupt_stub_187();
extern "C" void interrupt_stub_188();
extern "C" void interrupt_stub_189();
extern "C" void interrupt_stub_190();
extern "C" void interrupt_stub_191();
extern "C" void interrupt_stub_192();
extern "C" void interrupt_stub_193();
extern "C" void interrupt_stub_194();
extern "C" void interrupt_stub_195();
extern "C" void interrupt_stub_196();
extern "C" void interrupt_stub_197();
extern "C" void interrupt_stub_198();
extern "C" void interrupt_stub_199();
extern "C" void interrupt_stub_200();
extern "C" void interrupt_stub_201();
extern "C" void interrupt_stub_202();
extern "C" void interrupt_stub_203();
extern "C" void interrupt_stub_204();
extern "C" void interrupt_stub_205();
extern "C" void interrupt_stub_206();
extern "C" void interrupt_stub_207();
extern "C" void interrupt_stub_208();
extern "C" void interrupt_stub_209();
extern "C" void interrupt_stub_210();
extern "C" void interrupt_stub_211();
extern "C" void interrupt_stub_212();
extern "C" void interrupt_stub_213();
extern "C" void interrupt_stub_214();
extern "C" void interrupt_stub_215();
extern "C" void interrupt_stub_216();
extern "C" void interrupt_stub_217();
extern "C" void interrupt_stub_218();
extern "C" void interrupt_stub_219();
extern "C" void interrupt_stub_220();
extern "C" void interrupt_stub_221();
extern "C" void interrupt_stub_222();
extern "C" void interrupt_stub_223();
extern "C" void interrupt_stub_224();
extern "C" void interrupt_stub_225();
extern "C" void interrupt_stub_226();
extern "C" void interrupt_stub_227();
extern "C" void interrupt_stub_228();
extern "C" void interrupt_stub_229();
extern "C" void interrupt_stub_230();
extern "C" void interrupt_stub_231();
extern "C" void interrupt_stub_232();
extern "C" void interrupt_stub_233();
extern "C" void interrupt_stub_234();
extern "C" void interrupt_stub_235();
extern "C" void interrupt_stub_236();
extern "C" void interrupt_stub_237();
extern "C" void interrupt_stub_238();
extern "C" void interrupt_stub_239();
extern "C" void interrupt_stub_240();
extern "C" void interrupt_stub_241();
extern "C" void interrupt_stub_242();
extern "C" void interrupt_stub_243();
extern "C" void interrupt_stub_244();
extern "C" void interrupt_stub_245();
extern "C" void interrupt_stub_246();
extern "C" void interrupt_stub_247();
extern "C" void interrupt_stub_248();
extern "C" void interrupt_stub_249();
extern "C" void interrupt_stub_250();
extern "C" void interrupt_stub_251();
extern "C" void interrupt_stub_252();
extern "C" void interrupt_stub_253();
extern "C" void interrupt_stub_254();
extern "C" void interrupt_stub_255();
#pragma endregion

namespace
{
low_level::idt::entry *idt;
}

namespace low_level
{
namespace isr
{
void initialize(idt::entry idt[256])
{
  ::idt = idt;
  idt[0].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_0) & 0xFFFF;
  idt[0].offset_hi = reinterpret_cast<virtual_address>(interrupt_stub_0) >> 16;
  idt[0].cs        = 0x28;
  idt[0].type      = 0x8E;
  idt[1].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_1) & 0xFFFF;
  idt[1].offset_hi = reinterpret_cast<virtual_address>(interrupt_stub_1) >> 16;
  idt[1].cs        = 0x28;
  idt[1].type      = 0x8E;
  idt[2].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_2) & 0xFFFF;
  idt[2].offset_hi = reinterpret_cast<virtual_address>(interrupt_stub_2) >> 16;
  idt[2].cs        = 0x28;
  idt[2].type      = 0x8E;
  idt[3].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_3) & 0xFFFF;
  idt[3].offset_hi = reinterpret_cast<virtual_address>(interrupt_stub_3) >> 16;
  idt[3].cs        = 0x28;
  idt[3].type      = 0x8E;
  idt[4].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_4) & 0xFFFF;
  idt[4].offset_hi = reinterpret_cast<virtual_address>(interrupt_stub_4) >> 16;
  idt[4].cs        = 0x28;
  idt[4].type      = 0x8E;
  idt[5].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_5) & 0xFFFF;
  idt[5].offset_hi = reinterpret_cast<virtual_address>(interrupt_stub_5) >> 16;
  idt[5].cs        = 0x28;
  idt[5].type      = 0x8E;
  idt[6].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_6) & 0xFFFF;
  idt[6].offset_hi = reinterpret_cast<virtual_address>(interrupt_stub_6) >> 16;
  idt[6].cs        = 0x28;
  idt[6].type      = 0x8E;
  idt[7].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_7) & 0xFFFF;
  idt[7].offset_hi = reinterpret_cast<virtual_address>(interrupt_stub_7) >> 16;
  idt[7].cs        = 0x28;
  idt[7].type      = 0x8E;
  idt[8].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_8) & 0xFFFF;
  idt[8].offset_hi = reinterpret_cast<virtual_address>(interrupt_stub_8) >> 16;
  idt[8].cs        = 0x28;
  idt[8].type      = 0x8F;
  idt[9].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_9) & 0xFFFF;
  idt[9].offset_hi = reinterpret_cast<virtual_address>(interrupt_stub_9) >> 16;
  idt[9].cs        = 0x28;
  idt[9].type      = 0x8E;
  idt[10].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_10) & 0xFFFF;
  idt[10].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_10) >> 16;
  idt[10].cs   = 0x28;
  idt[10].type = 0x8F;
  idt[11].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_11) & 0xFFFF;
  idt[11].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_11) >> 16;
  idt[11].cs   = 0x28;
  idt[11].type = 0x8F;
  idt[12].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_12) & 0xFFFF;
  idt[12].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_12) >> 16;
  idt[12].cs   = 0x28;
  idt[12].type = 0x8F;
  idt[13].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_13) & 0xFFFF;
  idt[13].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_13) >> 16;
  idt[13].cs   = 0x28;
  idt[13].type = 0x8F;
  idt[14].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_14) & 0xFFFF;
  idt[14].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_14) >> 16;
  idt[14].cs   = 0x28;
  idt[14].type = 0x8F;
  idt[15].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_15) & 0xFFFF;
  idt[15].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_15) >> 16;
  idt[15].cs   = 0x28;
  idt[15].type = 0x8E;
  idt[16].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_16) & 0xFFFF;
  idt[16].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_16) >> 16;
  idt[16].cs   = 0x28;
  idt[16].type = 0x8E;
  idt[17].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_17) & 0xFFFF;
  idt[17].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_17) >> 16;
  idt[17].cs   = 0x28;
  idt[17].type = 0x8F;
  idt[18].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_18) & 0xFFFF;
  idt[18].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_18) >> 16;
  idt[18].cs   = 0x28;
  idt[18].type = 0x8E;
  idt[19].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_19) & 0xFFFF;
  idt[19].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_19) >> 16;
  idt[19].cs   = 0x28;
  idt[19].type = 0x8E;
  idt[20].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_20) & 0xFFFF;
  idt[20].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_20) >> 16;
  idt[20].cs   = 0x28;
  idt[20].type = 0x8E;
  idt[21].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_21) & 0xFFFF;
  idt[21].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_21) >> 16;
  idt[21].cs   = 0x28;
  idt[21].type = 0x8F;
  idt[22].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_22) & 0xFFFF;
  idt[22].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_22) >> 16;
  idt[22].cs   = 0x28;
  idt[22].type = 0x8E;
  idt[23].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_23) & 0xFFFF;
  idt[23].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_23) >> 16;
  idt[23].cs   = 0x28;
  idt[23].type = 0x8E;
  idt[24].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_24) & 0xFFFF;
  idt[24].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_24) >> 16;
  idt[24].cs   = 0x28;
  idt[24].type = 0x8E;
  idt[25].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_25) & 0xFFFF;
  idt[25].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_25) >> 16;
  idt[25].cs   = 0x28;
  idt[25].type = 0x8E;
  idt[26].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_26) & 0xFFFF;
  idt[26].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_26) >> 16;
  idt[26].cs   = 0x28;
  idt[26].type = 0x8E;
  idt[27].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_27) & 0xFFFF;
  idt[27].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_27) >> 16;
  idt[27].cs   = 0x28;
  idt[27].type = 0x8E;
  idt[28].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_28) & 0xFFFF;
  idt[28].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_28) >> 16;
  idt[28].cs   = 0x28;
  idt[28].type = 0x8E;
  idt[29].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_29) & 0xFFFF;
  idt[29].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_29) >> 16;
  idt[29].cs   = 0x28;
  idt[29].type = 0x8E;
  idt[30].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_30) & 0xFFFF;
  idt[30].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_30) >> 16;
  idt[30].cs   = 0x28;
  idt[30].type = 0x8E;
  idt[31].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_31) & 0xFFFF;
  idt[31].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_31) >> 16;
  idt[31].cs   = 0x28;
  idt[31].type = 0x8E;
  idt[32].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_32) & 0xFFFF;
  idt[32].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_32) >> 16;
  idt[32].cs   = 0x28;
  idt[32].type = 0x8E;
  idt[33].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_33) & 0xFFFF;
  idt[33].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_33) >> 16;
  idt[33].cs   = 0x28;
  idt[33].type = 0x8E;
  idt[34].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_34) & 0xFFFF;
  idt[34].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_34) >> 16;
  idt[34].cs   = 0x28;
  idt[34].type = 0x8E;
  idt[35].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_35) & 0xFFFF;
  idt[35].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_35) >> 16;
  idt[35].cs   = 0x28;
  idt[35].type = 0x8E;
  idt[36].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_36) & 0xFFFF;
  idt[36].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_36) >> 16;
  idt[36].cs   = 0x28;
  idt[36].type = 0x8E;
  idt[37].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_37) & 0xFFFF;
  idt[37].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_37) >> 16;
  idt[37].cs   = 0x28;
  idt[37].type = 0x8E;
  idt[38].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_38) & 0xFFFF;
  idt[38].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_38) >> 16;
  idt[38].cs   = 0x28;
  idt[38].type = 0x8E;
  idt[39].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_39) & 0xFFFF;
  idt[39].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_39) >> 16;
  idt[39].cs   = 0x28;
  idt[39].type = 0x8E;
  idt[40].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_40) & 0xFFFF;
  idt[40].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_40) >> 16;
  idt[40].cs   = 0x28;
  idt[40].type = 0x8E;
  idt[41].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_41) & 0xFFFF;
  idt[41].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_41) >> 16;
  idt[41].cs   = 0x28;
  idt[41].type = 0x8E;
  idt[42].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_42) & 0xFFFF;
  idt[42].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_42) >> 16;
  idt[42].cs   = 0x28;
  idt[42].type = 0x8E;
  idt[43].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_43) & 0xFFFF;
  idt[43].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_43) >> 16;
  idt[43].cs   = 0x28;
  idt[43].type = 0x8E;
  idt[44].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_44) & 0xFFFF;
  idt[44].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_44) >> 16;
  idt[44].cs   = 0x28;
  idt[44].type = 0x8E;
  idt[45].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_45) & 0xFFFF;
  idt[45].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_45) >> 16;
  idt[45].cs   = 0x28;
  idt[45].type = 0x8E;
  idt[46].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_46) & 0xFFFF;
  idt[46].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_46) >> 16;
  idt[46].cs   = 0x28;
  idt[46].type = 0x8E;
  idt[47].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_47) & 0xFFFF;
  idt[47].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_47) >> 16;
  idt[47].cs   = 0x28;
  idt[47].type = 0x8E;
  idt[48].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_48) & 0xFFFF;
  idt[48].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_48) >> 16;
  idt[48].cs   = 0x28;
  idt[48].type = 0x8E;
  idt[49].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_49) & 0xFFFF;
  idt[49].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_49) >> 16;
  idt[49].cs   = 0x28;
  idt[49].type = 0x8E;
  idt[50].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_50) & 0xFFFF;
  idt[50].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_50) >> 16;
  idt[50].cs   = 0x28;
  idt[50].type = 0x8E;
  idt[51].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_51) & 0xFFFF;
  idt[51].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_51) >> 16;
  idt[51].cs   = 0x28;
  idt[51].type = 0x8E;
  idt[52].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_52) & 0xFFFF;
  idt[52].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_52) >> 16;
  idt[52].cs   = 0x28;
  idt[52].type = 0x8E;
  idt[53].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_53) & 0xFFFF;
  idt[53].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_53) >> 16;
  idt[53].cs   = 0x28;
  idt[53].type = 0x8E;
  idt[54].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_54) & 0xFFFF;
  idt[54].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_54) >> 16;
  idt[54].cs   = 0x28;
  idt[54].type = 0x8E;
  idt[55].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_55) & 0xFFFF;
  idt[55].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_55) >> 16;
  idt[55].cs   = 0x28;
  idt[55].type = 0x8E;
  idt[56].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_56) & 0xFFFF;
  idt[56].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_56) >> 16;
  idt[56].cs   = 0x28;
  idt[56].type = 0x8E;
  idt[57].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_57) & 0xFFFF;
  idt[57].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_57) >> 16;
  idt[57].cs   = 0x28;
  idt[57].type = 0x8E;
  idt[58].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_58) & 0xFFFF;
  idt[58].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_58) >> 16;
  idt[58].cs   = 0x28;
  idt[58].type = 0x8E;
  idt[59].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_59) & 0xFFFF;
  idt[59].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_59) >> 16;
  idt[59].cs   = 0x28;
  idt[59].type = 0x8E;
  idt[60].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_60) & 0xFFFF;
  idt[60].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_60) >> 16;
  idt[60].cs   = 0x28;
  idt[60].type = 0x8E;
  idt[61].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_61) & 0xFFFF;
  idt[61].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_61) >> 16;
  idt[61].cs   = 0x28;
  idt[61].type = 0x8E;
  idt[62].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_62) & 0xFFFF;
  idt[62].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_62) >> 16;
  idt[62].cs   = 0x28;
  idt[62].type = 0x8E;
  idt[63].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_63) & 0xFFFF;
  idt[63].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_63) >> 16;
  idt[63].cs   = 0x28;
  idt[63].type = 0x8E;
  idt[64].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_64) & 0xFFFF;
  idt[64].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_64) >> 16;
  idt[64].cs   = 0x28;
  idt[64].type = 0x8E;
  idt[65].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_65) & 0xFFFF;
  idt[65].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_65) >> 16;
  idt[65].cs   = 0x28;
  idt[65].type = 0x8E;
  idt[66].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_66) & 0xFFFF;
  idt[66].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_66) >> 16;
  idt[66].cs   = 0x28;
  idt[66].type = 0x8E;
  idt[67].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_67) & 0xFFFF;
  idt[67].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_67) >> 16;
  idt[67].cs   = 0x28;
  idt[67].type = 0x8E;
  idt[68].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_68) & 0xFFFF;
  idt[68].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_68) >> 16;
  idt[68].cs   = 0x28;
  idt[68].type = 0x8E;
  idt[69].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_69) & 0xFFFF;
  idt[69].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_69) >> 16;
  idt[69].cs   = 0x28;
  idt[69].type = 0x8E;
  idt[70].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_70) & 0xFFFF;
  idt[70].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_70) >> 16;
  idt[70].cs   = 0x28;
  idt[70].type = 0x8E;
  idt[71].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_71) & 0xFFFF;
  idt[71].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_71) >> 16;
  idt[71].cs   = 0x28;
  idt[71].type = 0x8E;
  idt[72].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_72) & 0xFFFF;
  idt[72].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_72) >> 16;
  idt[72].cs   = 0x28;
  idt[72].type = 0x8E;
  idt[73].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_73) & 0xFFFF;
  idt[73].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_73) >> 16;
  idt[73].cs   = 0x28;
  idt[73].type = 0x8E;
  idt[74].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_74) & 0xFFFF;
  idt[74].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_74) >> 16;
  idt[74].cs   = 0x28;
  idt[74].type = 0x8E;
  idt[75].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_75) & 0xFFFF;
  idt[75].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_75) >> 16;
  idt[75].cs   = 0x28;
  idt[75].type = 0x8E;
  idt[76].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_76) & 0xFFFF;
  idt[76].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_76) >> 16;
  idt[76].cs   = 0x28;
  idt[76].type = 0x8E;
  idt[77].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_77) & 0xFFFF;
  idt[77].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_77) >> 16;
  idt[77].cs   = 0x28;
  idt[77].type = 0x8E;
  idt[78].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_78) & 0xFFFF;
  idt[78].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_78) >> 16;
  idt[78].cs   = 0x28;
  idt[78].type = 0x8E;
  idt[79].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_79) & 0xFFFF;
  idt[79].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_79) >> 16;
  idt[79].cs   = 0x28;
  idt[79].type = 0x8E;
  idt[80].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_80) & 0xFFFF;
  idt[80].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_80) >> 16;
  idt[80].cs   = 0x28;
  idt[80].type = 0x8E;
  idt[81].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_81) & 0xFFFF;
  idt[81].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_81) >> 16;
  idt[81].cs   = 0x28;
  idt[81].type = 0x8E;
  idt[82].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_82) & 0xFFFF;
  idt[82].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_82) >> 16;
  idt[82].cs   = 0x28;
  idt[82].type = 0x8E;
  idt[83].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_83) & 0xFFFF;
  idt[83].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_83) >> 16;
  idt[83].cs   = 0x28;
  idt[83].type = 0x8E;
  idt[84].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_84) & 0xFFFF;
  idt[84].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_84) >> 16;
  idt[84].cs   = 0x28;
  idt[84].type = 0x8E;
  idt[85].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_85) & 0xFFFF;
  idt[85].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_85) >> 16;
  idt[85].cs   = 0x28;
  idt[85].type = 0x8E;
  idt[86].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_86) & 0xFFFF;
  idt[86].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_86) >> 16;
  idt[86].cs   = 0x28;
  idt[86].type = 0x8E;
  idt[87].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_87) & 0xFFFF;
  idt[87].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_87) >> 16;
  idt[87].cs   = 0x28;
  idt[87].type = 0x8E;
  idt[88].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_88) & 0xFFFF;
  idt[88].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_88) >> 16;
  idt[88].cs   = 0x28;
  idt[88].type = 0x8E;
  idt[89].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_89) & 0xFFFF;
  idt[89].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_89) >> 16;
  idt[89].cs   = 0x28;
  idt[89].type = 0x8E;
  idt[90].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_90) & 0xFFFF;
  idt[90].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_90) >> 16;
  idt[90].cs   = 0x28;
  idt[90].type = 0x8E;
  idt[91].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_91) & 0xFFFF;
  idt[91].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_91) >> 16;
  idt[91].cs   = 0x28;
  idt[91].type = 0x8E;
  idt[92].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_92) & 0xFFFF;
  idt[92].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_92) >> 16;
  idt[92].cs   = 0x28;
  idt[92].type = 0x8E;
  idt[93].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_93) & 0xFFFF;
  idt[93].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_93) >> 16;
  idt[93].cs   = 0x28;
  idt[93].type = 0x8E;
  idt[94].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_94) & 0xFFFF;
  idt[94].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_94) >> 16;
  idt[94].cs   = 0x28;
  idt[94].type = 0x8E;
  idt[95].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_95) & 0xFFFF;
  idt[95].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_95) >> 16;
  idt[95].cs   = 0x28;
  idt[95].type = 0x8E;
  idt[96].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_96) & 0xFFFF;
  idt[96].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_96) >> 16;
  idt[96].cs   = 0x28;
  idt[96].type = 0x8E;
  idt[97].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_97) & 0xFFFF;
  idt[97].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_97) >> 16;
  idt[97].cs   = 0x28;
  idt[97].type = 0x8E;
  idt[98].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_98) & 0xFFFF;
  idt[98].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_98) >> 16;
  idt[98].cs   = 0x28;
  idt[98].type = 0x8E;
  idt[99].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_99) & 0xFFFF;
  idt[99].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_99) >> 16;
  idt[99].cs   = 0x28;
  idt[99].type = 0x8E;
  idt[100].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_100) & 0xFFFF;
  idt[100].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_100) >> 16;
  idt[100].cs   = 0x28;
  idt[100].type = 0x8E;
  idt[101].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_101) & 0xFFFF;
  idt[101].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_101) >> 16;
  idt[101].cs   = 0x28;
  idt[101].type = 0x8E;
  idt[102].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_102) & 0xFFFF;
  idt[102].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_102) >> 16;
  idt[102].cs   = 0x28;
  idt[102].type = 0x8E;
  idt[103].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_103) & 0xFFFF;
  idt[103].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_103) >> 16;
  idt[103].cs   = 0x28;
  idt[103].type = 0x8E;
  idt[104].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_104) & 0xFFFF;
  idt[104].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_104) >> 16;
  idt[104].cs   = 0x28;
  idt[104].type = 0x8E;
  idt[105].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_105) & 0xFFFF;
  idt[105].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_105) >> 16;
  idt[105].cs   = 0x28;
  idt[105].type = 0x8E;
  idt[106].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_106) & 0xFFFF;
  idt[106].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_106) >> 16;
  idt[106].cs   = 0x28;
  idt[106].type = 0x8E;
  idt[107].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_107) & 0xFFFF;
  idt[107].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_107) >> 16;
  idt[107].cs   = 0x28;
  idt[107].type = 0x8E;
  idt[108].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_108) & 0xFFFF;
  idt[108].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_108) >> 16;
  idt[108].cs   = 0x28;
  idt[108].type = 0x8E;
  idt[109].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_109) & 0xFFFF;
  idt[109].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_109) >> 16;
  idt[109].cs   = 0x28;
  idt[109].type = 0x8E;
  idt[110].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_110) & 0xFFFF;
  idt[110].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_110) >> 16;
  idt[110].cs   = 0x28;
  idt[110].type = 0x8E;
  idt[111].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_111) & 0xFFFF;
  idt[111].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_111) >> 16;
  idt[111].cs   = 0x28;
  idt[111].type = 0x8E;
  idt[112].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_112) & 0xFFFF;
  idt[112].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_112) >> 16;
  idt[112].cs   = 0x28;
  idt[112].type = 0x8E;
  idt[113].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_113) & 0xFFFF;
  idt[113].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_113) >> 16;
  idt[113].cs   = 0x28;
  idt[113].type = 0x8E;
  idt[114].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_114) & 0xFFFF;
  idt[114].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_114) >> 16;
  idt[114].cs   = 0x28;
  idt[114].type = 0x8E;
  idt[115].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_115) & 0xFFFF;
  idt[115].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_115) >> 16;
  idt[115].cs   = 0x28;
  idt[115].type = 0x8E;
  idt[116].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_116) & 0xFFFF;
  idt[116].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_116) >> 16;
  idt[116].cs   = 0x28;
  idt[116].type = 0x8E;
  idt[117].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_117) & 0xFFFF;
  idt[117].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_117) >> 16;
  idt[117].cs   = 0x28;
  idt[117].type = 0x8E;
  idt[118].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_118) & 0xFFFF;
  idt[118].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_118) >> 16;
  idt[118].cs   = 0x28;
  idt[118].type = 0x8E;
  idt[119].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_119) & 0xFFFF;
  idt[119].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_119) >> 16;
  idt[119].cs   = 0x28;
  idt[119].type = 0x8E;
  idt[120].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_120) & 0xFFFF;
  idt[120].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_120) >> 16;
  idt[120].cs   = 0x28;
  idt[120].type = 0x8E;
  idt[121].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_121) & 0xFFFF;
  idt[121].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_121) >> 16;
  idt[121].cs   = 0x28;
  idt[121].type = 0x8E;
  idt[122].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_122) & 0xFFFF;
  idt[122].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_122) >> 16;
  idt[122].cs   = 0x28;
  idt[122].type = 0x8E;
  idt[123].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_123) & 0xFFFF;
  idt[123].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_123) >> 16;
  idt[123].cs   = 0x28;
  idt[123].type = 0x8E;
  idt[124].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_124) & 0xFFFF;
  idt[124].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_124) >> 16;
  idt[124].cs   = 0x28;
  idt[124].type = 0x8E;
  idt[125].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_125) & 0xFFFF;
  idt[125].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_125) >> 16;
  idt[125].cs   = 0x28;
  idt[125].type = 0x8E;
  idt[126].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_126) & 0xFFFF;
  idt[126].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_126) >> 16;
  idt[126].cs   = 0x28;
  idt[126].type = 0x8E;
  idt[127].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_127) & 0xFFFF;
  idt[127].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_127) >> 16;
  idt[127].cs   = 0x28;
  idt[127].type = 0x8E;
  idt[128].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_128) & 0xFFFF;
  idt[128].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_128) >> 16;
  idt[128].cs   = 0x28;
  idt[128].type = 0x8E;
  idt[129].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_129) & 0xFFFF;
  idt[129].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_129) >> 16;
  idt[129].cs   = 0x28;
  idt[129].type = 0x8E;
  idt[130].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_130) & 0xFFFF;
  idt[130].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_130) >> 16;
  idt[130].cs   = 0x28;
  idt[130].type = 0x8E;
  idt[131].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_131) & 0xFFFF;
  idt[131].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_131) >> 16;
  idt[131].cs   = 0x28;
  idt[131].type = 0x8E;
  idt[132].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_132) & 0xFFFF;
  idt[132].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_132) >> 16;
  idt[132].cs   = 0x28;
  idt[132].type = 0x8E;
  idt[133].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_133) & 0xFFFF;
  idt[133].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_133) >> 16;
  idt[133].cs   = 0x28;
  idt[133].type = 0x8E;
  idt[134].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_134) & 0xFFFF;
  idt[134].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_134) >> 16;
  idt[134].cs   = 0x28;
  idt[134].type = 0x8E;
  idt[135].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_135) & 0xFFFF;
  idt[135].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_135) >> 16;
  idt[135].cs   = 0x28;
  idt[135].type = 0x8E;
  idt[136].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_136) & 0xFFFF;
  idt[136].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_136) >> 16;
  idt[136].cs   = 0x28;
  idt[136].type = 0x8E;
  idt[137].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_137) & 0xFFFF;
  idt[137].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_137) >> 16;
  idt[137].cs   = 0x28;
  idt[137].type = 0x8E;
  idt[138].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_138) & 0xFFFF;
  idt[138].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_138) >> 16;
  idt[138].cs   = 0x28;
  idt[138].type = 0x8E;
  idt[139].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_139) & 0xFFFF;
  idt[139].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_139) >> 16;
  idt[139].cs   = 0x28;
  idt[139].type = 0x8E;
  idt[140].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_140) & 0xFFFF;
  idt[140].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_140) >> 16;
  idt[140].cs   = 0x28;
  idt[140].type = 0x8E;
  idt[141].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_141) & 0xFFFF;
  idt[141].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_141) >> 16;
  idt[141].cs   = 0x28;
  idt[141].type = 0x8E;
  idt[142].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_142) & 0xFFFF;
  idt[142].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_142) >> 16;
  idt[142].cs   = 0x28;
  idt[142].type = 0x8E;
  idt[143].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_143) & 0xFFFF;
  idt[143].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_143) >> 16;
  idt[143].cs   = 0x28;
  idt[143].type = 0x8E;
  idt[144].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_144) & 0xFFFF;
  idt[144].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_144) >> 16;
  idt[144].cs   = 0x28;
  idt[144].type = 0x8E;
  idt[145].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_145) & 0xFFFF;
  idt[145].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_145) >> 16;
  idt[145].cs   = 0x28;
  idt[145].type = 0x8E;
  idt[146].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_146) & 0xFFFF;
  idt[146].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_146) >> 16;
  idt[146].cs   = 0x28;
  idt[146].type = 0x8E;
  idt[147].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_147) & 0xFFFF;
  idt[147].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_147) >> 16;
  idt[147].cs   = 0x28;
  idt[147].type = 0x8E;
  idt[148].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_148) & 0xFFFF;
  idt[148].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_148) >> 16;
  idt[148].cs   = 0x28;
  idt[148].type = 0x8E;
  idt[149].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_149) & 0xFFFF;
  idt[149].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_149) >> 16;
  idt[149].cs   = 0x28;
  idt[149].type = 0x8E;
  idt[150].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_150) & 0xFFFF;
  idt[150].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_150) >> 16;
  idt[150].cs   = 0x28;
  idt[150].type = 0x8E;
  idt[151].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_151) & 0xFFFF;
  idt[151].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_151) >> 16;
  idt[151].cs   = 0x28;
  idt[151].type = 0x8E;
  idt[152].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_152) & 0xFFFF;
  idt[152].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_152) >> 16;
  idt[152].cs   = 0x28;
  idt[152].type = 0x8E;
  idt[153].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_153) & 0xFFFF;
  idt[153].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_153) >> 16;
  idt[153].cs   = 0x28;
  idt[153].type = 0x8E;
  idt[154].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_154) & 0xFFFF;
  idt[154].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_154) >> 16;
  idt[154].cs   = 0x28;
  idt[154].type = 0x8E;
  idt[155].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_155) & 0xFFFF;
  idt[155].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_155) >> 16;
  idt[155].cs   = 0x28;
  idt[155].type = 0x8E;
  idt[156].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_156) & 0xFFFF;
  idt[156].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_156) >> 16;
  idt[156].cs   = 0x28;
  idt[156].type = 0x8E;
  idt[157].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_157) & 0xFFFF;
  idt[157].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_157) >> 16;
  idt[157].cs   = 0x28;
  idt[157].type = 0x8E;
  idt[158].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_158) & 0xFFFF;
  idt[158].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_158) >> 16;
  idt[158].cs   = 0x28;
  idt[158].type = 0x8E;
  idt[159].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_159) & 0xFFFF;
  idt[159].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_159) >> 16;
  idt[159].cs   = 0x28;
  idt[159].type = 0x8E;
  idt[160].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_160) & 0xFFFF;
  idt[160].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_160) >> 16;
  idt[160].cs   = 0x28;
  idt[160].type = 0x8E;
  idt[161].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_161) & 0xFFFF;
  idt[161].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_161) >> 16;
  idt[161].cs   = 0x28;
  idt[161].type = 0x8E;
  idt[162].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_162) & 0xFFFF;
  idt[162].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_162) >> 16;
  idt[162].cs   = 0x28;
  idt[162].type = 0x8E;
  idt[163].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_163) & 0xFFFF;
  idt[163].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_163) >> 16;
  idt[163].cs   = 0x28;
  idt[163].type = 0x8E;
  idt[164].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_164) & 0xFFFF;
  idt[164].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_164) >> 16;
  idt[164].cs   = 0x28;
  idt[164].type = 0x8E;
  idt[165].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_165) & 0xFFFF;
  idt[165].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_165) >> 16;
  idt[165].cs   = 0x28;
  idt[165].type = 0x8E;
  idt[166].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_166) & 0xFFFF;
  idt[166].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_166) >> 16;
  idt[166].cs   = 0x28;
  idt[166].type = 0x8E;
  idt[167].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_167) & 0xFFFF;
  idt[167].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_167) >> 16;
  idt[167].cs   = 0x28;
  idt[167].type = 0x8E;
  idt[168].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_168) & 0xFFFF;
  idt[168].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_168) >> 16;
  idt[168].cs   = 0x28;
  idt[168].type = 0x8E;
  idt[169].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_169) & 0xFFFF;
  idt[169].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_169) >> 16;
  idt[169].cs   = 0x28;
  idt[169].type = 0x8E;
  idt[170].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_170) & 0xFFFF;
  idt[170].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_170) >> 16;
  idt[170].cs   = 0x28;
  idt[170].type = 0x8E;
  idt[171].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_171) & 0xFFFF;
  idt[171].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_171) >> 16;
  idt[171].cs   = 0x28;
  idt[171].type = 0x8E;
  idt[172].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_172) & 0xFFFF;
  idt[172].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_172) >> 16;
  idt[172].cs   = 0x28;
  idt[172].type = 0x8E;
  idt[173].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_173) & 0xFFFF;
  idt[173].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_173) >> 16;
  idt[173].cs   = 0x28;
  idt[173].type = 0x8E;
  idt[174].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_174) & 0xFFFF;
  idt[174].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_174) >> 16;
  idt[174].cs   = 0x28;
  idt[174].type = 0x8E;
  idt[175].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_175) & 0xFFFF;
  idt[175].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_175) >> 16;
  idt[175].cs   = 0x28;
  idt[175].type = 0x8E;
  idt[176].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_176) & 0xFFFF;
  idt[176].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_176) >> 16;
  idt[176].cs   = 0x28;
  idt[176].type = 0x8E;
  idt[177].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_177) & 0xFFFF;
  idt[177].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_177) >> 16;
  idt[177].cs   = 0x28;
  idt[177].type = 0x8E;
  idt[178].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_178) & 0xFFFF;
  idt[178].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_178) >> 16;
  idt[178].cs   = 0x28;
  idt[178].type = 0x8E;
  idt[179].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_179) & 0xFFFF;
  idt[179].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_179) >> 16;
  idt[179].cs   = 0x28;
  idt[179].type = 0x8E;
  idt[180].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_180) & 0xFFFF;
  idt[180].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_180) >> 16;
  idt[180].cs   = 0x28;
  idt[180].type = 0x8E;
  idt[181].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_181) & 0xFFFF;
  idt[181].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_181) >> 16;
  idt[181].cs   = 0x28;
  idt[181].type = 0x8E;
  idt[182].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_182) & 0xFFFF;
  idt[182].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_182) >> 16;
  idt[182].cs   = 0x28;
  idt[182].type = 0x8E;
  idt[183].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_183) & 0xFFFF;
  idt[183].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_183) >> 16;
  idt[183].cs   = 0x28;
  idt[183].type = 0x8E;
  idt[184].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_184) & 0xFFFF;
  idt[184].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_184) >> 16;
  idt[184].cs   = 0x28;
  idt[184].type = 0x8E;
  idt[185].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_185) & 0xFFFF;
  idt[185].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_185) >> 16;
  idt[185].cs   = 0x28;
  idt[185].type = 0x8E;
  idt[186].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_186) & 0xFFFF;
  idt[186].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_186) >> 16;
  idt[186].cs   = 0x28;
  idt[186].type = 0x8E;
  idt[187].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_187) & 0xFFFF;
  idt[187].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_187) >> 16;
  idt[187].cs   = 0x28;
  idt[187].type = 0x8E;
  idt[188].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_188) & 0xFFFF;
  idt[188].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_188) >> 16;
  idt[188].cs   = 0x28;
  idt[188].type = 0x8E;
  idt[189].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_189) & 0xFFFF;
  idt[189].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_189) >> 16;
  idt[189].cs   = 0x28;
  idt[189].type = 0x8E;
  idt[190].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_190) & 0xFFFF;
  idt[190].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_190) >> 16;
  idt[190].cs   = 0x28;
  idt[190].type = 0x8E;
  idt[191].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_191) & 0xFFFF;
  idt[191].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_191) >> 16;
  idt[191].cs   = 0x28;
  idt[191].type = 0x8E;
  idt[192].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_192) & 0xFFFF;
  idt[192].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_192) >> 16;
  idt[192].cs   = 0x28;
  idt[192].type = 0x8E;
  idt[193].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_193) & 0xFFFF;
  idt[193].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_193) >> 16;
  idt[193].cs   = 0x28;
  idt[193].type = 0x8E;
  idt[194].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_194) & 0xFFFF;
  idt[194].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_194) >> 16;
  idt[194].cs   = 0x28;
  idt[194].type = 0x8E;
  idt[195].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_195) & 0xFFFF;
  idt[195].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_195) >> 16;
  idt[195].cs   = 0x28;
  idt[195].type = 0x8E;
  idt[196].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_196) & 0xFFFF;
  idt[196].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_196) >> 16;
  idt[196].cs   = 0x28;
  idt[196].type = 0x8E;
  idt[197].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_197) & 0xFFFF;
  idt[197].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_197) >> 16;
  idt[197].cs   = 0x28;
  idt[197].type = 0x8E;
  idt[198].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_198) & 0xFFFF;
  idt[198].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_198) >> 16;
  idt[198].cs   = 0x28;
  idt[198].type = 0x8E;
  idt[199].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_199) & 0xFFFF;
  idt[199].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_199) >> 16;
  idt[199].cs   = 0x28;
  idt[199].type = 0x8E;
  idt[200].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_200) & 0xFFFF;
  idt[200].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_200) >> 16;
  idt[200].cs   = 0x28;
  idt[200].type = 0x8E;
  idt[201].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_201) & 0xFFFF;
  idt[201].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_201) >> 16;
  idt[201].cs   = 0x28;
  idt[201].type = 0x8E;
  idt[202].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_202) & 0xFFFF;
  idt[202].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_202) >> 16;
  idt[202].cs   = 0x28;
  idt[202].type = 0x8E;
  idt[203].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_203) & 0xFFFF;
  idt[203].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_203) >> 16;
  idt[203].cs   = 0x28;
  idt[203].type = 0x8E;
  idt[204].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_204) & 0xFFFF;
  idt[204].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_204) >> 16;
  idt[204].cs   = 0x28;
  idt[204].type = 0x8E;
  idt[205].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_205) & 0xFFFF;
  idt[205].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_205) >> 16;
  idt[205].cs   = 0x28;
  idt[205].type = 0x8E;
  idt[206].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_206) & 0xFFFF;
  idt[206].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_206) >> 16;
  idt[206].cs   = 0x28;
  idt[206].type = 0x8E;
  idt[207].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_207) & 0xFFFF;
  idt[207].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_207) >> 16;
  idt[207].cs   = 0x28;
  idt[207].type = 0x8E;
  idt[208].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_208) & 0xFFFF;
  idt[208].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_208) >> 16;
  idt[208].cs   = 0x28;
  idt[208].type = 0x8E;
  idt[209].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_209) & 0xFFFF;
  idt[209].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_209) >> 16;
  idt[209].cs   = 0x28;
  idt[209].type = 0x8E;
  idt[210].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_210) & 0xFFFF;
  idt[210].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_210) >> 16;
  idt[210].cs   = 0x28;
  idt[210].type = 0x8E;
  idt[211].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_211) & 0xFFFF;
  idt[211].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_211) >> 16;
  idt[211].cs   = 0x28;
  idt[211].type = 0x8E;
  idt[212].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_212) & 0xFFFF;
  idt[212].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_212) >> 16;
  idt[212].cs   = 0x28;
  idt[212].type = 0x8E;
  idt[213].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_213) & 0xFFFF;
  idt[213].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_213) >> 16;
  idt[213].cs   = 0x28;
  idt[213].type = 0x8E;
  idt[214].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_214) & 0xFFFF;
  idt[214].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_214) >> 16;
  idt[214].cs   = 0x28;
  idt[214].type = 0x8E;
  idt[215].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_215) & 0xFFFF;
  idt[215].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_215) >> 16;
  idt[215].cs   = 0x28;
  idt[215].type = 0x8E;
  idt[216].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_216) & 0xFFFF;
  idt[216].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_216) >> 16;
  idt[216].cs   = 0x28;
  idt[216].type = 0x8E;
  idt[217].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_217) & 0xFFFF;
  idt[217].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_217) >> 16;
  idt[217].cs   = 0x28;
  idt[217].type = 0x8E;
  idt[218].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_218) & 0xFFFF;
  idt[218].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_218) >> 16;
  idt[218].cs   = 0x28;
  idt[218].type = 0x8E;
  idt[219].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_219) & 0xFFFF;
  idt[219].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_219) >> 16;
  idt[219].cs   = 0x28;
  idt[219].type = 0x8E;
  idt[220].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_220) & 0xFFFF;
  idt[220].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_220) >> 16;
  idt[220].cs   = 0x28;
  idt[220].type = 0x8E;
  idt[221].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_221) & 0xFFFF;
  idt[221].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_221) >> 16;
  idt[221].cs   = 0x28;
  idt[221].type = 0x8E;
  idt[222].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_222) & 0xFFFF;
  idt[222].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_222) >> 16;
  idt[222].cs   = 0x28;
  idt[222].type = 0x8E;
  idt[223].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_223) & 0xFFFF;
  idt[223].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_223) >> 16;
  idt[223].cs   = 0x28;
  idt[223].type = 0x8E;
  idt[224].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_224) & 0xFFFF;
  idt[224].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_224) >> 16;
  idt[224].cs   = 0x28;
  idt[224].type = 0x8E;
  idt[225].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_225) & 0xFFFF;
  idt[225].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_225) >> 16;
  idt[225].cs   = 0x28;
  idt[225].type = 0x8E;
  idt[226].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_226) & 0xFFFF;
  idt[226].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_226) >> 16;
  idt[226].cs   = 0x28;
  idt[226].type = 0x8E;
  idt[227].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_227) & 0xFFFF;
  idt[227].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_227) >> 16;
  idt[227].cs   = 0x28;
  idt[227].type = 0x8E;
  idt[228].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_228) & 0xFFFF;
  idt[228].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_228) >> 16;
  idt[228].cs   = 0x28;
  idt[228].type = 0x8E;
  idt[229].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_229) & 0xFFFF;
  idt[229].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_229) >> 16;
  idt[229].cs   = 0x28;
  idt[229].type = 0x8E;
  idt[230].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_230) & 0xFFFF;
  idt[230].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_230) >> 16;
  idt[230].cs   = 0x28;
  idt[230].type = 0x8E;
  idt[231].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_231) & 0xFFFF;
  idt[231].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_231) >> 16;
  idt[231].cs   = 0x28;
  idt[231].type = 0x8E;
  idt[232].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_232) & 0xFFFF;
  idt[232].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_232) >> 16;
  idt[232].cs   = 0x28;
  idt[232].type = 0x8E;
  idt[233].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_233) & 0xFFFF;
  idt[233].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_233) >> 16;
  idt[233].cs   = 0x28;
  idt[233].type = 0x8E;
  idt[234].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_234) & 0xFFFF;
  idt[234].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_234) >> 16;
  idt[234].cs   = 0x28;
  idt[234].type = 0x8E;
  idt[235].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_235) & 0xFFFF;
  idt[235].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_235) >> 16;
  idt[235].cs   = 0x28;
  idt[235].type = 0x8E;
  idt[236].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_236) & 0xFFFF;
  idt[236].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_236) >> 16;
  idt[236].cs   = 0x28;
  idt[236].type = 0x8E;
  idt[237].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_237) & 0xFFFF;
  idt[237].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_237) >> 16;
  idt[237].cs   = 0x28;
  idt[237].type = 0x8E;
  idt[238].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_238) & 0xFFFF;
  idt[238].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_238) >> 16;
  idt[238].cs   = 0x28;
  idt[238].type = 0x8E;
  idt[239].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_239) & 0xFFFF;
  idt[239].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_239) >> 16;
  idt[239].cs   = 0x28;
  idt[239].type = 0x8E;
  idt[240].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_240) & 0xFFFF;
  idt[240].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_240) >> 16;
  idt[240].cs   = 0x28;
  idt[240].type = 0x8E;
  idt[241].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_241) & 0xFFFF;
  idt[241].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_241) >> 16;
  idt[241].cs   = 0x28;
  idt[241].type = 0x8E;
  idt[242].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_242) & 0xFFFF;
  idt[242].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_242) >> 16;
  idt[242].cs   = 0x28;
  idt[242].type = 0x8E;
  idt[243].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_243) & 0xFFFF;
  idt[243].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_243) >> 16;
  idt[243].cs   = 0x28;
  idt[243].type = 0x8E;
  idt[244].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_244) & 0xFFFF;
  idt[244].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_244) >> 16;
  idt[244].cs   = 0x28;
  idt[244].type = 0x8E;
  idt[245].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_245) & 0xFFFF;
  idt[245].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_245) >> 16;
  idt[245].cs   = 0x28;
  idt[245].type = 0x8E;
  idt[246].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_246) & 0xFFFF;
  idt[246].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_246) >> 16;
  idt[246].cs   = 0x28;
  idt[246].type = 0x8E;
  idt[247].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_247) & 0xFFFF;
  idt[247].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_247) >> 16;
  idt[247].cs   = 0x28;
  idt[247].type = 0x8E;
  idt[248].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_248) & 0xFFFF;
  idt[248].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_248) >> 16;
  idt[248].cs   = 0x28;
  idt[248].type = 0x8E;
  idt[249].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_249) & 0xFFFF;
  idt[249].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_249) >> 16;
  idt[249].cs   = 0x28;
  idt[249].type = 0x8E;
  idt[250].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_250) & 0xFFFF;
  idt[250].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_250) >> 16;
  idt[250].cs   = 0x28;
  idt[250].type = 0x8E;
  idt[251].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_251) & 0xFFFF;
  idt[251].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_251) >> 16;
  idt[251].cs   = 0x28;
  idt[251].type = 0x8E;
  idt[252].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_252) & 0xFFFF;
  idt[252].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_252) >> 16;
  idt[252].cs   = 0x28;
  idt[252].type = 0x8E;
  idt[253].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_253) & 0xFFFF;
  idt[253].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_253) >> 16;
  idt[253].cs   = 0x28;
  idt[253].type = 0x8E;
  idt[254].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_254) & 0xFFFF;
  idt[254].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_254) >> 16;
  idt[254].cs   = 0x28;
  idt[254].type = 0x8E;
  idt[255].offset_low =
      reinterpret_cast<virtual_address>(interrupt_stub_255) & 0xFFFF;
  idt[255].offset_hi =
      reinterpret_cast<virtual_address>(interrupt_stub_255) >> 16;
  idt[255].cs   = 0x28;
  idt[255].type = 0x8E;
}

void set_handler(uint8_t vector, handler fn)
{
  handlers[vector] = fn;
}
} // namespace isr
} // namespace low_level
