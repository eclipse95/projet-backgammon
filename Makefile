# -std=c11 -Wall -Wextra -Wshadow -pedantic -pipe -fdiagnostics-color=auto -Og -g
#Flags, Libraries and Includes
export CFLAGS        += -std=c11 -Wall -Wextra -Wshadow -pedantic -pipe 
#-fdiagnostics-color=auto
# -fsanitize=address
export CFLAGS_DEBUG  += -g
#-Og
export CFLAGS_PROD   += -O2 -s -DNDEBUG
export CPPFLAGS      += -MMD -MP -I include
export LDFLAGS       +=
export LDLIBS        +=

.PHONY: interface strategy clean doc

all: strategy interface

interface strategy:
	@$(MAKE) -C $@

clean:
	@$(MAKE) -C strategy clean
	@$(MAKE) -C interface clean
