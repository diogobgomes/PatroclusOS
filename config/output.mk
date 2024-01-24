# CONFIGURE FILE FOR MAKEFILES
#
# Used to configure the output of makefiles - to be included by configure script,
# not usefull on its own
#
#
# 2024 Diogo Gomes

ifeq ("$(origin V)", "command line")
	KBUILD_VERBOSE := $(V)
endif
ifndef KBUILD_VERBOSE
	KBUILD_VERBOSE := 0
endif

GREEN:=$(shell tput setaf 2)
RED:=$(shell tput setaf 1)
NC:=$(shell tput sgr0)
YELLOW:=$(shell tput setaf 3)
BLUE:=$(shell tput setaf 4)
CYAN:=$(shell tput setaf 6)

OK_STRING:="[OK]"
ERROR_STRING:="[ERROR]"
WARNING_STRING:="[WARNING]"

define run_command
@printf "%b" "$(BLUE)$(2) $(CYAN)$(@F)$(NC)\r"; \
script -q /dev/null -c "$(1)" -e > $@.log; \
RESULT=$$?; \
if [ $$RESULT -ne 0 ]; then \
	printf "%-40b%b" "$(BLUE)$(2) $(CYAN)$(@)" "$(RED)$(ERROR_STRING)$(NC)\n"; \
elif [ -s $@.log ]; then \
	printf "%-40b%b" "$(BLUE)$(2) $(CYAN)$(@)" "$(YELLOW)$(WARNING_STRING)$(NC)\n"; \
else \
	printf "%-40b%b" "$(BLUE)$(2) $(CYAN)$(@F)" "$(GREEN)$(OK_STRING)$(NC)\n"; \
fi; \
cat $@.log; \
rm -f $@.log; \
if [ "$(KBUILD_VERBOSE)" == "1" ]; then \
	printf "\n$(1)\n" "$(1)"; \
fi; \
exit $$RESULT
endef

# Allows printing a make variable in command line as: make print-VAR
# Good for debugging
print-%  : ; @echo $* = $($*)