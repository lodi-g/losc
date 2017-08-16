## compilation
SRCSDIR :=	srcs
INCSDIR :=	incs /usr/include/PCSC
OBJSDIR :=	objs
OUTDIR :=	.
DESTDIR :=	/bin

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

all:		$(LOSC) service

$(LOSC):	$(OBJSDIR) $(OBJS)
		$(CC) -o $@ $(filter-out $<,$^) $(LDFLAGS)

$(OBJSDIR)/%.o:	$(SRCSDIR)/%.c
		$(CC) $(CFLAGS) -c -o $@ $<

$(OBJSDIR):;	@mkdir $@

re:		clean
		@$(MAKE)

## service configuration
define EOL


endef

GROUP :=	$(shell id -gn)

SERVICE_F :=	loscd.service
SERVICE_T :=	$(SERVICE_F).mk
SERVICE_C :=	$(eval define SERVICE_C$(EOL)$(file <$(SERVICE_T))$(EOL)endef)$(SERVICE_C)

service:	$(SERVICE_T) $(SERVICE_F)
$(SERVICE_F):;	$(file >$@,$(SERVICE_C))

## installation
SYSD_DIR :=	/etc/systemd/system
INSTALL :=	@install

ifeq (,$(filter 0,\
	$(shell test -f $(LOSC); printf $$?),\
	$(shell test -f $(SERVICE_F); printf $$?)))
FILES_EXISTS :=	RunMakeAsNormalUser
endif

install:	$(FILES_EXISTS) install-losc install-srvce
		@systemctl daemon-reload
		@printf "Please create the configuration file (see README.md)\n"
		@printf "Run sudo systemctl enable loscd --now to enable the service\n"

install-losc:;	$(INSTALL) -m755 $(LOSC) $(DESTDIR)/
install-srvce:;	$(INSTALL) -Dm644 $(SERVICE_F) $(SYSD_DIR)/

## clean
RM :=		@rm -vfr

clean:;		$(RM) $(OBJSDIR) $(LOSC) $(SERVICE_F)
mostlyclean:;	$(RM) $(OBJSDIR)

## uninstall
uninstall:;	$(RM) $(SYSD_DIR)/$(SERVICE_F) $(DESTDIR)/$(LOSC)

## misc
MAKE +=		--no-print-directory
.PHONY:		all clean mostlyclean re service install install-losc install-service uninstall
