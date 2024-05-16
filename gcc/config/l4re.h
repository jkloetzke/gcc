/* The GNU C++ standard library requires that these macros be defined.  */
#undef CPLUSPLUS_CPP_SPEC
#define CPLUSPLUS_CPP_SPEC "-D_GNU_SOURCE %(cpp)"

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC \
  "%{shared:; \
     static:crt1.o%s; \
     static-pie:crt1.p.o%s; \
     " PIE_SPEC ":crt1.s.o%s; \
     :crt1.o%s} " \
   "%{shared:crti.s.o%s;:crti.o%s} " \
   "%{static:crtbeginT.o%s; \
     shared|static-pie|" PIE_SPEC ":crtbeginS.o%s; \
     :crtbegin.o%s} "

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC \
   "%{static:crtend.o%s; \
     shared|static-pie|" PIE_SPEC ":crtendS.o%s; \
     :crtend.o%s} " \
   "%{shared:crtn.s.o%s;:crtn.o%s} "

#undef  LIB_SPEC
#define LIB_SPEC \
  "%{pthread:--whole-archive -lpthread --no-whole-archive} " \
  "--start-group -lc_be_l4refile -lc_be_l4re %{static|static-pie:-ll4re-vfs.o} " \
  "-lc -lc_nonshared -l4re-util -l4re -ll4util -ll4sys -lc_support_misc -lc_be_sig_noop -lc_be_socket_noop -lmount " \
  "%{!shared:%{!static:%{!static-pie:-lld-l4}}} --end-group "

#define LINK_SSP_SPEC "%{fstack-protector|fstack-protector-all" \
		       "|fstack-protector-strong|fstack-protector-explicit" \
		       ":-lssp_nonshared.p}"

#if defined(HAVE_LD_EH_FRAME_HDR)
#define LINK_EH_SPEC "%{!static|static-pie:--eh-frame-hdr} "
#endif

#undef  LINK_SPEC
#define LINK_SPEC \
  "-z max-page-size=0x1000 "                                \
  "-z common-page-size=0x1000 "                             \
  "-gc-sections "                                           \
  "%{static:-Bstatic} "                                     \
  "%{shared:-shared} "                                      \
  "%{symbolic:-Bsymbolic} "                                 \
  "%{!static:%{!static-pie:                                 \
    %{rdynamic:-export-dynamic}                             \
    %{!shared:-dynamic-linker=rom/libld-l4.so}}} "          \
  "%{static-pie:-Bstatic -pie --no-dynamic-linker -z text}"

#define GNU_USER_TARGET_LINK_GCC_C_SEQUENCE_SPEC \
  "%{static|static-pie:--start-group} %G %{!nolibc:%L} \
   %{static|static-pie:--end-group}%{!static:%{!static-pie:%G}}"

#undef LINK_GCC_C_SEQUENCE_SPEC
#define LINK_GCC_C_SEQUENCE_SPEC GNU_USER_TARGET_LINK_GCC_C_SEQUENCE_SPEC

/* Use --as-needed -lgcc_s for eh support.  */
#ifdef HAVE_LD_AS_NEEDED
#define USE_LD_AS_NEEDED 1
#endif

#ifndef TARGET_SUB_OS_CPP_BUILTINS
#define TARGET_SUB_OS_CPP_BUILTINS()
#endif

#undef  TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()		\
  do						\
    {						\
      builtin_define ("__l4re__");		\
      TARGET_SUB_OS_CPP_BUILTINS();		\
    }						\
  while (false)

#define NATIVE_SYSTEM_HEADER_DIR_ARCH NATIVE_SYSTEM_HEADER_DIR "/l4-arch"

/* Add .note.GNU-stack special section */
#define TARGET_ASM_FILE_END file_end_indicate_exec_stack
