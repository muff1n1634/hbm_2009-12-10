# basic makefile

# paths (can also pass via command line)
MKDIR ?= mkdir # should support POSIX '-p'
WINE ?= wine # wine path here if not building on Windows natively
MWERKS ?= # mwcc path here (Wii 1.0; should also have mwld in the same directory)

# targets
.PHONY: help homebuttonLib homebuttonLibD clean

help:
	@echo 'Make sure "HBM_APP_TYPE" is configured before building.'
	@echo
	@echo 'options:'
	@echo '    homebuttonLib          creates homebuttonLib.a (release)'
	@echo '    homebuttonLibD         creates homebuttonLibD.a (debug)'
	@echo
	@echo '    build/release/<file>.o compiles <file>.o (release)'
	@echo '    build/debug/<file>.o   compiles <file>.o (debug)'
	@echo
	@echo '    clean                  cleans up build artifacts'

homebuttonLib: lib/homebuttonLib.a
homebuttonLibD: lib/homebuttonLibD.a
clean:; -@rm -rf build/ lib/

# system include search directories
export MWCIncludes = include:include/stdlib

# flags
flags_main = -proc gekko -fp hardware -lang c99 -enum int -cpp_exceptions off -cwd include -enc UTF-8 -flag no-cats
flags_main += -sdata 0 -sdata2 0 # specific to this library

flags_opt_debug = -opt off -inline off -gdwarf-2
flags_opt_release = -O4,p -ipa file -DNDEBUG

# source files
hbm_src :=	\
	homebutton/HBMFrameController.cpp		\
	homebutton/HBMAnmController.cpp			\
	homebutton/HBMGUIManager.cpp			\
	homebutton/HBMController.cpp			\
	homebutton/HBMRemoteSpk.cpp				\
	homebutton/HBMAxSound.cpp				\
	homebutton/HBMCommon.cpp				\
	homebutton/HBMBase.cpp					\
											\
	nw4hbm/lyt/lyt_animation.cpp			\
	nw4hbm/lyt/lyt_arcResourceAccessor.cpp	\
	nw4hbm/lyt/lyt_bounding.cpp				\
	nw4hbm/lyt/lyt_common.cpp				\
	nw4hbm/lyt/lyt_drawInfo.cpp				\
	nw4hbm/lyt/lyt_group.cpp				\
	nw4hbm/lyt/lyt_layout.cpp				\
	nw4hbm/lyt/lyt_material.cpp				\
	nw4hbm/lyt/lyt_pane.cpp					\
	nw4hbm/lyt/lyt_picture.cpp				\
	nw4hbm/lyt/lyt_resourceAccessor.cpp		\
	nw4hbm/lyt/lyt_textBox.cpp				\
	nw4hbm/lyt/lyt_window.cpp				\
											\
	nw4hbm/math/math_arithmetic.cpp			\
	nw4hbm/math/math_equation.cpp			\
	nw4hbm/math/math_triangular.cpp			\
											\
	nw4hbm/ut/ut_binaryFileFormat.cpp		\
	nw4hbm/ut/ut_CharStrmReader.cpp			\
	nw4hbm/ut/ut_CharWriter.cpp				\
	nw4hbm/ut/ut_Font.cpp					\
	nw4hbm/ut/ut_LinkList.cpp				\
	nw4hbm/ut/ut_list.cpp					\
	nw4hbm/ut/ut_ResFont.cpp				\
	nw4hbm/ut/ut_ResFontBase.cpp			\
	nw4hbm/ut/ut_TagProcessorBase.cpp		\
	nw4hbm/ut/ut_TextWriterBase.cpp			\
											\
	sound/mix.cpp							\
	sound/syn.cpp							\
	sound/synctrl.cpp						\
	sound/synenv.cpp						\
	sound/synmix.cpp						\
	sound/synpitch.cpp						\
	sound/synsample.cpp						\
	sound/synvoice.cpp						\
	sound/seq.cpp

.PHONY: envcheck

envcheck:
ifeq ($(strip ${MWERKS}),)
	$(error MWERKS not set)
endif

# object files
build/debug/%.o: source/%.cpp envcheck
	@${MKDIR} -p build/debug/$(dir $*)
	${WINE} ${MWERKS} ${flags_main} ${flags_opt_debug} -o $@ -c $<

build/release/%.o: source/%.cpp envcheck
	@${MKDIR} -p build/release/$(dir $*)
	${WINE} ${MWERKS} ${flags_main} ${flags_opt_release} -o $@ -c $<

# library archives
.NOTPARALLEL: lib/homebuttonLibD.a lib/homebuttonLib.a

lib/homebuttonLibD.a: $(foreach f,$(basename ${hbm_src}),build/debug/$f.o)
	@${MKDIR} -p lib/
	${WINE} ${MWERKS} ${flags_main} -o $@ -library $^

lib/homebuttonLib.a: $(foreach f,$(basename ${hbm_src}),build/release/$f.o)
	@${MKDIR} -p lib/
	${WINE} ${MWERKS} ${flags_main} -o $@ -library $^

