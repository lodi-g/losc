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

ifeq ($(RELEASE),1)
CFLAGS :=	$(subst -g3,,$(CFLAGS))
endif

SRCS :=		main.c config.c
SRCS :=		$(addprefix $(SRCSDIR)/,$(SRCS))
OBJS :=		$(patsubst $(SRCSDIR)/%.c,$(OBJSDIR)/%.o,$(SRCS))

LOSC :=		losc
LOSC :=		$(addprefix $(OUTDIR)/,$(LOSC))

all:		$(LOSC)

$(LOSC):	$(OBJSDIR) $(OBJS)
		$(CC) -o $@ $(filter-out $<,$^) $(LDFLAGS)

$(OBJSDIR)/%.o:	$(SRCSDIR)/%.c
		$(CC) $(CFLAGS) -c -o $@ $<

$(OBJSDIR):;	@mkdir $@

re:		clean
		@$(MAKE)

## clean
RM :=		@rm -vfr

clean:;		$(RM) $(OBJSDIR) $(LOSC)
mostlyclean:;	$(RM) $(OBJSDIR)

## misc
MAKE +=		--no-print-directory
.PHONY:		all clean mostlyclean re
