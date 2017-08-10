## compilation
SRCSDIR :=	srcs
INCSDIR :=	incs
OBJSDIR :=	objs
OUTDIR :=	.

CC :=		gcc
CFLAGS :=	-W -Wall -Wextra -Wvla -std=c11 -pedantic -g3
CFLAGS +=	$(addprefix -I,$(INCSDIR))
LIBS :=		lua
LDFLAGS :=	$(addprefix -l,$(LIBS))

ifeq ($(RELEASE),1)
CFLAGS :=	$(subst -g3,,$(CFLAGS))
endif

SRCS :=		main.c config.c
SRCS :=		$(addprefix $(SRCSDIR)/,$(SRCS))
OBJS :=		$(patsubst %.c,%.o,$(SRCS))
OBJS :=		$(subst $(SRCSDIR),$(OBJSDIR),$(OBJS))

LOSC :=		losc
LOSC :=		$(addprefix $(OUTDIR)/,$(LOSC))

all:		$(LOSC)

$(LOSC):	$(OBJS)
		$(CC) -o $@ $^ $(LDFLAGS)

$(OBJSDIR)/%.o:	$(SRCSDIR)/%.c $(OBJSDIR)
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