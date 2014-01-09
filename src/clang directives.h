  // see ..\Debug\CC_options.txt for example    
  const char gkCCOptions[]  = " \ 
  -triple i386-pc-win32 -fno-caret-diagnostics -fno-gnu-keywords -fms-extensions -mms-bitfields  -fmath-errno -mdisable-fp-elim -Wno-microsoft -Werror=gnu -Werror=pointer-arith                                    \        
  -Wpointer-sign -Wunreachable-code -Wparentheses -Wunused-variable -Wuninitialized -Wdiv-by-zero -Warray-bounds -Wnull-dereference -Wignored-qualifiers -Wreturn-type -Wpanel-handle                               \
  -Wtautological-compare -Wempty-body -Wmissing-braces -Woverlength-strings -Wcast-align -Wmissing-noreturn -Winvalid-noreturn -Wshadow -Wswitch -Wswitch-enum -Wunused-function                                    \
  -Wunused-label -Wunused-parameter -Wunused-value -Wused-but-marked-unused -Wconversion -Wsign-compare -Wchar-subscripts -Wdeclaration-after-statement -Wtypecheck-convert -Wfloat-equal                           \
  -Wvla -Wattributes -Wunknown-attributes -Wdeprecated-declarations -Wpacked -Wpadded -Wcomment -Wformat -Wimplicit-int -Wimplicit-function-declaration -Wbuiltin-implicit-decl                                     \
  -Wmissing-declarations -Wmissing-field-initializers -Wtrigraphs -Wmultichar -Wlong-long -Wunknown-pragmas -Wignored-pragmas                                                                                       \
  -Wpragma-reset -Wpragma-on-off-switch -Wpragma-unused -Wend-of-directive -Wpragma-not-supported -Wunexpected-token -Wfour-char-constants -Wdiscard-qual -Wsign-conversion -Wvariadic-macros                       \
  -Wmacro-name -Wmacro-poisoning -Wmacro-redefined -Wbuiltin-macro-redefined -Wbuiltin-macro-undefined -Wunused-macros -Wgnu-designator -Wnonnull -Wconstant-conversion -Wout-of-scope-declarations                 \
  -Wstatic-non-static -Wconstant-too-large -W#warnings -Wundef -Winclude-next -Wextra-tokens -Wline-number -Wweak -Wmember-redeclare -Wexpected-semi -Wtoo-many-args -Wliteral-range                                \
  -Wunknown-escape -Wshift -Wchar-init -Wbraces-init -Wincomplete-field -Wtentative -Wordered-comparison -Wpointer-compare -Wstack-memory -Wexcess-elements -Wmissing-terminating -Willegal-storage                 \
  -Wclang -Wbackslash -Wdecl-param -Wnegative-to-unsigned -Wextern -Wconstant-logical-operand -Winitializer-overrides -Wvariadic-call-conv -Wmain -Womp-nested-parallel -Womp-repeated-ordered                      \
  -Wpointer-int-conv -Wint-conversion -Wpointer-conv -fdiagnostics-show-option -cvi-debugging=extended -Werror=implicit-function-declaration -Wuninitialized-runtime                                                \
";
  
  // see ..\Debug\CallCC.bat for example
  // -D__LIBPROT__                               
  const char gkClangDefine[] = "         \
-D_CVI_=1300                           \
-D_NI_i386_=1                          \ 
-D_NI_mswin_=1                         \ 
-D_NI_mswin32_=1                       \  
-D_CVI_DEBUG_=1                        \
-D_CVI_EXE_=1                          \
-D_LINK_CVIRTE_=1                      \
-D_CVI_FDS_=1                          \
-D_CVI_USE_FUNCS_FOR_VARS_=1           \  
-D__DEFALIGN=8                         \
-D_NI_VC_=220                          \
-D_WIN32=1                             \
-DWIN32=1                              \
-D__WIN32__=1                          \
-D_WINDOWS=1                           \
-D__NT__=1                             \
-D_M_IX86=400                          \
-D__FLAT__=1                           \
-D_PUSHPOP_SUPPORTED=1                 \
-D_CVI_C99_EXTENSIONS_=1               \
-D_CVI_PROFILE_LEVEL_=0                \
-D_TARGET_FILE_VERSION_=\"1.0.0.0\"    \
-D_TARGET_PRODUCT_VERSION_=\"1.0.0.0\" \
-D_OPENMP=200505                       \
-D_CVI_CONFIGURATION_NAME_=\"Debug\"   \
-D_CVI_RTE_SHARED_=1                   \
-DWIN32_LEAN_AND_MEAN                  \
";
  
  // TODO : Find a way to build the paths below using the CVI installation directory
  // see ..\Debug\CC_options.txt for example    
  const char gkClangIncludes[] ="\  
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\include\"                                \
-I \"C:\\Program Files (x86)\\National Instruments\\Shared\\CVI\\include\"                            \
-I \"C:\\ProgramData\\National Instruments\\CVI2013\\include\"                                        \
-I \"C:\\ProgramData\\National Instruments\\CVI\\include\"                                            \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\include\\ansi\"                          \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\include\\clang\\2.9\"                    \
-I \"C:\\Program Files (x86)\\IVI Foundation\\IVI\\Include\"                                          \
-I \"C:\\Program Files (x86)\\IVI Foundation\\VISA\\winnt\\include\"                                  \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\sdk\\include\"                           \
-I \"c:\\Users\\Public\\Documents\\National Instruments\\CVI2013\\samples\\udp\"                      \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\activex\\word\"                \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\activex\\excel\"               \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\custctrl\"                     \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\custctrl\\custsupp\"           \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\cvirtsup\"                     \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\datasock\"                     \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\daqUtil\"                      \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\printing\"                     \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\toolbox\"                      \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\reportgen\"                    \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\toolslib\\localui\"                      \
-I \"c:\\program files (x86)\\national instruments\\cvi2013\\instr\"                                  \
-I \"C:\\Program Files (x86)\\National Instruments\\Shared\\CVI\\toolslib\\custctrl\"                 \
-I \"C:\\Program Files (x86)\\National Instruments\\Shared\\CVI\\toolslib\\custctrl\\custsupp\"       \
-I \"C:\\Program Files (x86)\\National Instruments\\Shared\\CVI\\instr\"                              \
-I \"C:\\ProgramData\\National Instruments\\CVI2013\\instr\"                                          \
-I \"c:\\Program Files (x86)\\National Instruments\\CVI2013\\bin\"                                    \
-I \"c:\\ProgramData\\National Instruments\\CVI2013\\bin\"                                            \
-I \"c:\\Program Files (x86)\\National Instruments\\Shared\\CVI\\Bin\"                                \
-I \"C:\\Users\\Public\\Documents\\National Instruments\\CVI2013\\samples\\dotnet\"                   \  
";
