## compilation
SRCSDIR :=	srcs
INCSDIR :=	incs /usr/include/PCSC
OBJSDIR :=	objs
OUTDIR :=	.

CC :=		gcc
CFLAGS :=	-W -Wall -Wextra -Wvla -std=c11 -pedantic -g3
CFLAGS +=	$(addprefix -I,$(INCSDIR))
LIBS :=		lua pcsclite
LDFLAGS :=	$(addprefix -l,$(LIBS))

RELEASE ?= 1

ifeq ($(RELEASE),1)
CFLAGS :=	$(subst -g3,,$(CFLAGS)) -Werror -O1
endif

SRCS :=		main.c config.c sc.c
SRCS :=		$(addprefix $(SRCSDIR)/,$(SRCS))
OBJS :=		$(patsubst $(SRCSDIR)/%.c,$(OBJSDIR)/%.o,$(SRCS))

LOSC :=		loscd
LOSC :=		$(addprefix $(OUTDIR)/,$(LOSC))

all:		$(LOSC)

$(LOSC):	$(OBJSDIR) $(OBJS)
		$(CC) -o $@ $(filter-out $<,$^) $(LDFLAGS)

$(OBJSDIR)/%.o:	$(SRCSDIR)/%.c
		$(CC) $(CFLAGS) -c -o $@ $<

$(OBJSDIR):;	@mkdir $@

re:		clean
		@$(MAKE)

## installation
LOSCD_S :=	./loscd.service
S_DIR :=	/etc/systemd/system
DIST :=		/bin/
install:	$(LOSCD_S)
		@mkdir -p $$HOME/.config/losc/
		@sudo cp $(LOSC) $(DIST)
		@sudo cp $(LOSCD_S) $(S_DIR)
		@sudo systemctl daemon-reload
		@printf "losc daemon successfully installed\n"

## clean
RM :=		@rm -vfr

clean:;		$(RM) $(OBJSDIR) $(LOSC)
mostlyclean:;	$(RM) $(OBJSDIR)

## misc
MAKE +=		--no-print-directory
.PHONY:		all clean mostlyclean re
