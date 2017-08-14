#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <pcsclite.h>
#include <reader.h>
#include <wintypes.h>
#include <winscard.h>

#include "losc.h"

static SCARDCONTEXT context;
static SCARD_READERSTATE rgReaderState;

static struct losc_config *config = NULL;

static bool init(struct losc_config *c)
{
  /* Context */
  LONG ret = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &context);
  if (ret != SCARD_S_SUCCESS)
    LOSC_LOG_EXIT(1, pcsc_stringify_error(ret));
  config = c;

  /* Readers */
  LPSTR mszReaders;
  DWORD dwReaders = SCARD_AUTOALLOCATE;
  ret = SCardListReaders(context, NULL, (LPSTR)&mszReaders, &dwReaders);
  switch (ret)
    {
      case SCARD_E_NO_READERS_AVAILABLE:
        LOSC_LOG_EXIT(1, "You have either no smartcard reader or it is in power saving mode (see BIOS)"); break;
      case SCARD_S_SUCCESS:
        rgReaderState.szReader = mszReaders;
        rgReaderState.dwCurrentState = SCARD_STATE_UNAWARE;
        break;
      default:
        LOSC_LOG_EXIT(1, pcsc_stringify_error(ret)); break;
    }
  return true;
}

static bool losc_connect(SCARDHANDLE *handle)
{
  DWORD dwActiveProtocol;

  LONG ret = SCardConnect(context, rgReaderState.szReader, SCARD_SHARE_SHARED, 
    SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, handle, &dwActiveProtocol);

  if (ret != SCARD_S_SUCCESS)
    {
      syslog(LOG_INFO, pcsc_stringify_error(ret));
      return false;
    }
  return true;
}

static char *retrieve_atr(SCARDHANDLE handle)
{
  unsigned char *pbAttr;
  DWORD dwAttrLen = SCARD_AUTOALLOCATE;

  /* Retrives ATR */
  LONG ret = SCardGetAttrib(handle, SCARD_ATTR_ATR_STRING, (unsigned char *)&pbAttr, &dwAttrLen);
  if (ret != SCARD_S_SUCCESS)
    {
      syslog(LOG_INFO, pcsc_stringify_error(ret));
      return NULL;
    }

  /* Formats ATR */
  int atr_len = 0;
  char t[3];
  char *atr;

  for (DWORD i = 0; i < dwAttrLen; i++)
    {
      atr_len += 2;
      if (dwAttrLen - 1 != i)
        atr_len++;
    }
  atr = calloc(sizeof(char), atr_len + 1);
  for (DWORD i = 0; i < dwAttrLen; i++)
    {
      sprintf(t, "%02X", pbAttr[i]);
      strcat(atr, t);
      if (dwAttrLen - 1 != i)
        strcat(atr, ":");
    }
  return atr;
}

static bool authentificate_card()
{
  SCARDHANDLE handle;
  char *atr;

  if (!losc_connect(&handle))
    return false;
  if (!(atr = retrieve_atr(handle)))
    {
      SCardDisconnect(handle, SCARD_EJECT_CARD);
      return false;
    }
  
  bool f = false;
  for (int i = 0; config->atrs[i]; i++)
    if (!strcmp(config->atrs[i], atr))
      f = true;
  syslog(LOG_INFO, "Smart card authentification %s (%s)", f ? "accepted" : "rejected", atr);
  free(atr);
  SCardDisconnect(handle, SCARD_EJECT_CARD);
  return f;
}

// Unlock is a pretty name for an ugly killall
#define DUPC(fd1, fd2) do { dup2(fd1, fd2); close(fd1); } while (0)
static void unlock()
{
  char * const p[] = { "killall", config->locker, NULL };
  pid_t pid;

  if (!(pid = fork()))
    {
      int dn = open("/dev/null", O_RDWR);
      DUPC(STDIN_FILENO, dn);
      DUPC(STDOUT_FILENO, dn);
      DUPC(STDERR_FILENO, dn);
      execvp(*p, p);
      LOSC_LOG_EXIT(1, strerror(errno));
    }
  else if (pid == -1)
    LOSC_LOG_EXIT(1, strerror(errno));
  else
    wait(NULL);
}
#undef DUPC

bool losc_daemon_loop(struct losc_config *config)
{
  init(config);
  rgReaderState.dwCurrentState = SCARD_STATE_UNAWARE;
  syslog(LOG_INFO, "Initialized successfully");
  while (true)
    {
      SCardGetStatusChange(context, INFINITE, &rgReaderState, 1);
      if (rgReaderState.dwEventState & SCARD_STATE_PRESENT)
        {
          syslog(LOG_INFO, "New smart card detected");
          if (authentificate_card())
            unlock();
        }
      rgReaderState.dwCurrentState = rgReaderState.dwEventState;
    }
  return true;
}

void losc_release()
{
  SCardReleaseContext(context);
}
