/**
  ******************************************************************************
  * @file    otp_interface_cli.c
  * @author  MCD Application Team
  * @brief   OTP Interface CLI
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020(-2021) STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "otp_interface_cli.h"
#include "otp_interface_cli_util.h"
#include "otp_interface.h"
#include <limits.h>
#include <errno.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum {
  OTP_CMD_HELP = 0,
  OTP_CMD_DISPL,
  OTP_CMD_WRITE,
  OTP_CMD_LOCK,
  OTP_CMD_MAX,
} otp_cmd_id;

typedef struct {
  char *str;
  uint8_t param_min;
  uint8_t param_max;
} otp_command;

typedef struct {
  uint8_t off;
  uint8_t len;
} otp_arg;

const otp_command otp_cmd[OTP_CMD_MAX] = {
    [OTP_CMD_HELP]         = { "help"        , 0, 0 },
    [OTP_CMD_DISPL]        = { "displ"       , 0, 96 },
    [OTP_CMD_WRITE]        = { "write"       , 2, 194 },
    [OTP_CMD_LOCK]         = { "lock"        , 1, 2 },
};

/* Private define ------------------------------------------------------------*/
#define CMD_MAX_LEN 1024
#define CMD_MAX_ARG 255
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static OPENBL_Otp_TypeDef Otp;
static uint8_t idx;

static char *cmd;
static char *val_str;
static char *end_ptr;
static uint32_t val;
static bool once;
static char entry[CMD_MAX_LEN];
static bool auto_comp;
uint32_t prev_opt_cmd;

/* Private function prototypes -----------------------------------------------*/
static void print_help(void);
static void free_args(int argc, char *argv[]);
static void get_entry_string(char *entry);
static int parse_entry_string(char *entry, size_t size, int *command, char *argv[]);
static void print_displ(int argc, char *argv[]);
static void print_command_error(void);
static void print_outofrange_error(uint32_t num);
static void write_otp(uint32_t word, uint32_t val, bool lock);
static void lock_otp(uint32_t word);
static void print_otp_more_status(OPENBL_Otp_TypeDef Otp, uint32_t word);

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

static void print_help(void)
{
  const char *help = {
    "commands:\n\r\n\r"
    "-help                      : This command allows the user to display help menu\n\r\n\r"
    "-displ                     : This command allows the user to display all or parts of the OTP structure\n\r"
    " [word=<id>]               : {Optional} display a specific OTP registers {values and status} Up to 96 OTP words [0 to 95], id value in dec format\n\r\n\r"
    "-write                     : This command allows to fuse or update OTP words Up to 96 OTP words [0 to 95] at the same command line\n\r"
    " [-y]                      : {Optional} enable auto confirmation\n\r"
    " [lock]                    : {Optional} indicate the operation type, update or permanent lock\n\r"
    " [word=<id>]               : This field contains the OTP word number in dec format\n\r"
    " [value=<value>]           : Value to be written in dec format\n\r"
    "-lock                      : This command allows to fuse permanently OTP words Up to 96 OTP words [0 to 95] at the same command line\n\r"
    " [word=<id>]               : This field contains the OTP word number in dec format\n\r"
  };

  printf(help);
}

static void print_otp_more_status(OPENBL_Otp_TypeDef Otp, uint32_t word)
{
  if ((Otp.OtpPart[word] & OTP_READ_ERROR) == OTP_READ_ERROR)
  {
    printf("\n\r                                   |_[00] Invalid");
  }
  if ((Otp.OtpPart[word] & OTP_LOCK_ERROR) == OTP_LOCK_ERROR)
  {
    printf("\n\r                                   |_[26] Lock error");
  }
  if ((Otp.OtpPart[word] & OTP_STICKY_PROG_LOCK_MASK) == OTP_STICKY_PROG_LOCK_MASK)
  {
    printf("\n\r                                   |_[27] Sticky programming lock");
  }
  if ((Otp.OtpPart[word] & OTP_STICKY_WRITE_LOCK_MASK) == OTP_STICKY_WRITE_LOCK_MASK)
  {
    printf("\n\r                                   |_[28] Shadow write sticky lock");
  }
  if ((Otp.OtpPart[word] & OTP_STICKY_READ_LOCK_MASK) == OTP_STICKY_READ_LOCK_MASK)
  {
    printf("\n\r                                   |_[29] Shadow read sticky lock");
  }
  if ((Otp.OtpPart[word] & OTP_PERM_LOCK_MASK) == OTP_PERM_LOCK_MASK)
  {
    printf("\n\r                                   |_[30] Permanent programming lock");
  }
}

static void print_displ(int argc, char *argv[])
{
  errno = 0;
  once = true;

  /* Read otp */
  Otp = OPENBL_OTP_Read();

  if (argc >= 2)
  {
    for (int idx = 0; idx < argc - 1; idx++)
    {
      cmd = strtok(argv[idx], "=");
      val_str = strtok(NULL, "=");

      if (!strcmp(cmd, "word"))
      {
        val = strtol(val_str, &end_ptr, 10);
        if (end_ptr != val_str)
        {
          if ((val == 0 || val == ULONG_MAX) && errno == ERANGE)
          {
            print_command_error();
          }

          if ((val >= 0) && (val <= 95))
          {
            if (once)
            {
              printf("\n\rOTP DATA");
              printf("\n\r---------------------------------------------------------------------");
              printf("\n\r    ID    |      VALUE       |     STATUS     ");
              once = false;
            }
            printf("\n\r---------------------------------------------------------------------");
            printf("\n\r    %02ld    |    0x%08lX    |    0x%08lX    ", val, Otp.OtpPart[val*2], Otp.OtpPart[val*2+1]);
            print_otp_more_status(Otp, val*2+1);
          }
          else
          {
            print_outofrange_error(val);
          }
        }
      }
      else
      {
        print_command_error();
      }
    }
  }
  else
  {
    printf("\n\rOTP GLOBAL STATE");
    if (Otp.GlobalState == BSEC_SEC_OTP_OPEN)
    {
      printf("\n\r |_ State            : Secure Open");
    }
    else if (Otp.GlobalState == BSEC_SEC_OTP_CLOSED)
    {
      printf("\n\r |_ State            : Secure Closed");
    }
    else
    {
      printf("\n\r |_ State            : Invalid");
    }

    printf("\n\r |_ Hardware Key Set : N");
    printf("\n\r |_ Encrypted data   : N\n\r");

    printf("\n\rOTP DATA");
    printf("\n\r---------------------------------------------------------------------");
    printf("\n\r    ID    |      VALUE       |     STATUS     ");
    printf("\n\r---------------------------------------------------------------------");
    for (idx = 0; idx < OTP_PART_SIZE; idx+=2)
    {
      printf("\n\r    %02d    |    0x%08lX    |    0x%08lX    ", (idx/2), Otp.OtpPart[idx], Otp.OtpPart[idx+1]);
      print_otp_more_status(Otp, idx+1);
    }
    printf("\n\r---------------------------------------------------------------------\n\r");
  }
}

static void print_command_error(void)
{
  printf("\n\rError: Wrong command");
  printf("\n\rPlease refer to help for the supported commands\n\r");
  return;
}

static void print_outofrange_error(uint32_t num)
{
  printf("\n\rError: word=0x%08lX is out of range, it will be ignored", num);
  printf("\n\rPlease refer to help for the supported ranges\n\r");
  return;
}

static void write_otp(uint32_t word, uint32_t val, bool lock)
{
  /* Read otp */
  Otp = OPENBL_OTP_Read();
  /* Change otp */
  Otp.OtpPart[word * 2] = val;

  if (lock)
  {
    Otp.OtpPart[word * 2 + 1] |= OTP_PERM_LOCK_MASK;
  }
  /* Apply the change */
  Otp.OtpPart[word * 2 + 1] |= OTP_REQUEST_UPDATE_MASK;
  /* Write otp */
  if (!OPENBL_OTP_Write(Otp))
  {
    printf("\n\rSUCCESS\n\r");
  }
  else
  {
    printf("\n\rFAIL\n\r");
  }
}

static void lock_otp(uint32_t word)
{
  /* Read otp */
  Otp = OPENBL_OTP_Read();
  /* Change otp */
  Otp.OtpPart[word * 2 + 1] |= OTP_PERM_LOCK_MASK;
  /* Apply the change */
  Otp.OtpPart[word * 2 + 1] |= OTP_REQUEST_UPDATE_MASK;
  /* Write otp */
  if (!OPENBL_OTP_Write(Otp))
  {
    printf("\n\rSUCCESS\n\r");
  }
  else
  {
    printf("\n\rFAIL\n\r");
  }
}

static void print_write(int argc, char *argv[])
{
  uint32_t opt_word;
  bool lock = false;

  auto_comp = false;
  prev_opt_cmd = 0;
  errno = 0;

  if (argc < 3)
  {
    print_command_error();
  }

  if (argc >= 4)
  {
    if (argc % 2 == 0)
    {
      if(!strcmp(argv[0], "-y"))
      {
        auto_comp = true;
        prev_opt_cmd++;
        printf("Warning: Auto confirmation is enabled.\n\r");
      }
      else if (!strcmp(argv[0], "lock"))
      {
        lock = true;
        prev_opt_cmd++;
      }
      else
      {
        print_command_error();
      }
    }
    else
    {
      if(!strcmp(argv[0], "-y"))
      {
        if(!strcmp(argv[1], "lock"))
        {
          lock = true;
        }
        else
        {
          print_command_error();
        }
        auto_comp = true;
        prev_opt_cmd = 2;
        printf("Warning: Auto confirmation is enabled.\n\r");
      }
    }
  }

  for (int idx = 0; idx < argc - 1; idx += 2)
  {
    cmd = strtok(argv[idx + prev_opt_cmd], "=");
    val_str = strtok(NULL, "=");

    if (!strcmp(cmd, "word"))
    {
      val = strtol(val_str, &end_ptr, 10);
      if (end_ptr != val_str)
      {
        if ((val == 0 || val == ULONG_MAX) && errno == ERANGE)
        {
          print_command_error();
        }

        if ((val >= 0) && (val <= 95))
        {
          opt_word = val;

          cmd = strtok(argv[idx + 1 + prev_opt_cmd], "=");
          val_str = strtok(NULL, "=");

          if (!strcmp(cmd, "value"))
          {
            val = strtol(val_str, &end_ptr, 10);
            if (end_ptr != val_str)
            {
              if ((val == 0 || val == ULONG_MAX) && errno == ERANGE)
              {
                print_command_error();
              }
              else
              {
                printf("\n\rOTP Write command:");
                printf("\n\rYou are trying to write OTP with the following inputs:");
                printf("\n\r--------------------------");
                printf("\n\r Word     |  Value        ");
                printf("\n\r--------------------------");
                printf("\n\r %02ld       |  0x%08lX         ", opt_word, val);
                if (lock)
                {
                  printf("\n\r Lock     |  YES          ");
                }
                else
                {
                  printf("\n\r Lock     |  NO          ");
                }
                printf("\n\r--------------------------");
                printf("\n\rWarning: This operation cannot be reverted and may brick your device.");
                if (auto_comp)
                {
                  write_otp(opt_word, val, lock);
                }
                else
                {
                  printf("\n\rWarning: Do you confirm?  [y/n]\n\r");
                  get_entry_string(entry);

                  while ((entry[0] != 'y') && (entry[0] != 'Y') && (entry[0] != 'n') && (entry[0] != 'N'))
                  {
                    printf("Error: type 'y' or 'Y' for yes and type 'n' or 'N' for no\n\r");
                    get_entry_string(entry);
                  }
                  if ((entry[0] == 'y') || (entry[0] == 'Y'))
                  {
                    printf("The operation was confirmed...");
                    write_otp(opt_word, val, lock);
                  }
                  else
                  {
                    printf("Warning: The operation was cancelled...\n\r");
                  }
                }
              }
            }
            else
            {
              print_command_error();
            }
          }
        }
        else
        {
          print_outofrange_error(val);
        }
      }
      else
      {
        print_command_error();
      }
    }
    else
    {
      print_command_error();
    }
  }

}

static void print_lock(int argc, char *argv[])
{
  auto_comp = false;
  prev_opt_cmd = 0;
  errno = 0;

  if (argc == 3)
  {
    if(!strcmp(argv[0], "-y"))
    {
      auto_comp = true;
      prev_opt_cmd++;
      printf("Warning: Auto confirmation is enabled.\n\r");
    }
    else
    {
      print_command_error();
    }
  }

  cmd = strtok(argv[0 + prev_opt_cmd], "=");
  val_str = strtok(NULL, "=");

  if (!strcmp(cmd, "word"))
  {
    val = strtol(val_str, &end_ptr, 10);
    if (end_ptr != val_str)
    {
      if ((val == 0 || val == ULONG_MAX) && errno == ERANGE)
      {
        print_command_error();
      }

      if ((val >= 0) && (val <= 95))
      {
        /* Read otp */
        Otp = OPENBL_OTP_Read();

        printf("OTP Lock command:");
        printf("\n\rYou are trying to lock some OTP words with the following inputs:");
        printf("\n\r--------------------------");
        printf("\n\r Word     |  Value        ");
        printf("\n\r--------------------------");
        printf("\n\r %02ld       |  0x%08lX         ", val, Otp.OtpPart[2 *val]);
        printf("\n\r Lock     |  YES          ");
        printf("\n\r--------------------------");
        printf("\n\rWarning: This operation cannot be reverted and may brick your device.");

        if (auto_comp)
        {
          lock_otp(val);
        }
        else
        {
          printf("\n\rWarning: Do you confirm?  [y/n]\n\r");
          get_entry_string(entry);

          while ((entry[0] != 'y') && (entry[0] != 'Y') && (entry[0] != 'n') && (entry[0] != 'N'))
          {
            printf("Error: type 'y' or 'Y' for yes and type 'n' or 'N' for no\n\r");
            get_entry_string(entry);
          }
          if ((entry[0] == 'y') || (entry[0] == 'Y'))
          {
            printf("The operation was confirmed...");
            lock_otp(val);
          }
          else
          {
            printf("Warning: The operation was cancelled...\n\r");
          }
        }
      }
      else
      {
        print_outofrange_error(val);
      }
    }
    else
    {
      print_command_error();
    }
  }

}

static void free_args(int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; i++)
  {
    free(argv[i - 1]);
  }
}

static void get_entry_string(char *entry)
{
  uint8_t user_entry_valid = 0;
  char user_entry_value = 0;
  int i = 0;

  Serial_Putchar(0xd);
  Serial_Putchar('U');
  Serial_Putchar('s');
  Serial_Putchar('e');
  Serial_Putchar('r');
  Serial_Putchar('>');

  while(user_entry_valid == 0)
  {
    /* Scan for user entry */
    user_entry_value = Serial_Scanf(255);
    /* Print the entry character */
    Serial_Putchar(user_entry_value);

    if (user_entry_value == 0xd)
    {
      entry[i] = '\0';
      printf("\r\n");
      user_entry_valid = 1;
    }
    else if (user_entry_value == 0x18)
    {
      i--;
    }
    else
    {
      entry[i++] = user_entry_value;
    }
  }
}

static int parse_entry_string(char *entry, size_t size, int *command, char *argv[])
{
  int i = 0;
  int n = 0;
  otp_arg cmd = {0, 0};
  otp_arg arg = {0, 0};

  /* Identify command and parameters */
  for (i = 0; i < (int)size; i++)
  {
    if (entry[i] == '\0')
    {
      break;
    }
    if (entry[i] == ' ')
    {
      continue;
    }

    if (n == 0)
    {
      cmd.off = i;
    }
    else
    {
      arg.off = i;
    }

    while ((entry[i] != ' ') && (entry[i] != '\0'))
    {
      i++;
    }

    if (n == 0)
    {
      cmd.len = i - cmd.off;
    }
    else
    {
      arg.len = i - arg.off;
      argv[n - 1] = (char *)malloc(arg.len + 1);
      strncpy(argv[n - 1], &entry[arg.off], arg.len);
      strncpy(argv[n - 1] + arg.len, "\0", 1);
    }

    n++;

    if (n > CMD_MAX_ARG + 1)
    {
      printf("Error: too many arguments (max=%d)\n\r", CMD_MAX_ARG);
      printf("\n\rPlease refer to help for the supported commands\n\r");
      return -1;
    }

    i--;
  }

  /* Case of empty string, simple ignore, no error */
  if ((n == 0) && (cmd.len == 0))
  {
    return 0;
  }

  /* Get command */
  for (i = 0; i < OTP_CMD_MAX; i++)
  {
    if (!strncmp(&entry[cmd.off], otp_cmd[i].str, cmd.len)
        && (cmd.len == strlen(otp_cmd[i].str)))
    {
      *command = i;
      break;
    }
  }

  /* Check number of parameters */
  if ((n - 1) < otp_cmd[*command].param_min)
  {
    printf("Error: %s: argument missing (min=%d)\n\r", otp_cmd[*command].str,
                                                otp_cmd[*command].param_min);
    printf("Please refer to help for the supported commands\n\r");
    return -3;
  }
  else if ((n - 1) > otp_cmd[*command].param_max)
  {
    printf("Error: %s: too many arguments (max=%d)\n\r", otp_cmd[*command].str,
                                                  otp_cmd[*command].param_max);
    printf("Please refer to help for the supported commands\n\r");
    return -4;
  }

  /* return number of arguments, including command and parameters */
  return n;
}

void otp_commands_interactive(void)
{
  char buffer[CMD_MAX_LEN];
  char *argv[CMD_MAX_ARG + 1]; /* NULL terminated */
  int argc;
  int cmd;

  /* Init otp */
  OPENBL_OTP_Init();

#ifndef __TERMINAL_IO__
  get_entry_string(buffer);
#else
  printf("\r\nUser>");
  fflush(stdout);
  scanf("%s", buffer);
#endif

  argc = parse_entry_string(buffer, strlen(buffer), &cmd, argv);
  if (argc <= 0)
  {
    return;
  }

  switch (cmd)
  {
  case OTP_CMD_HELP:
    print_help();
    break;

  case OTP_CMD_DISPL:
    print_displ(argc, argv);
    break;

  case OTP_CMD_WRITE:
    print_write(argc, argv);
    break;

  case OTP_CMD_LOCK:
    print_lock(argc, argv);
    break;

  default:
    break;
  }

  free_args(argc, argv);

}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
