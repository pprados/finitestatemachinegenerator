{
s/@srcdir@/./g
s/@bindir@/\"\/progra~1\"\/fsmg/g
s/@prefix@/\"\/progra~1\/fsmg"/g
s/@exec_prefix@/\$(prefix)/g
s/@SET_MAKE@/MAKE=nmake/g
s/@DEFS@/-DHAVE_CONFIG_H/g
s/@CFLAGS@/-Ox -nologo/g
s/@CPPFLAGS@//g
s/@LDFLAGS@//g
s/@EXEEXT@/.exe/g
s/@YACC@/bison -y/g
s/@LEX@/flex/g
s/@CC@/cl/g
s/@INSTALL@/cp/g
s/@INSTALL_PROGRAM@/$(INSTALL)/g
s/@INSTALL_DATA@/$(INSTALL)/g
s/\.o/\.obj /g
s/if \[ x\"\$(USELEX)\" = x \]; then/if x$(USELEX) == x/g
s/if \[ x\"\$(USELEX)\" = x\"1\" \]; then/if x$(USELEX) == x1/g
s/; fi//g
s/\.\/config\.status/rem/g 
s/\.\/$(FSMG)/$(FSMG)/g
s/DISTBIN_NAME=binfsmg/DISTBIN_NAME=winfsmg/g
}
