#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

extern const char qemu_start, qemu_end;
extern const char uefi_start, uefi_end;
extern const char artos_start, artos_end;

int GetTempPathA(int, char *);

char * qemu_cmd = "qemu\\qemu-system-aarch64.exe "
	               "-M virt "
		            "-cpu cortex-a57 "
		            "-m 128M "
		            "-display none "
		            "-serial stdio "
		            "-bios firmware\\uefi_code.fd "
		            "-drive file=fat:rw:disk,if=virtio,format=raw "
                  "-icount shift=1,align=off,sleep=off "
                  "";

/* Parse an octal number, ignoring leading and trailing nonsense. */
static int parseoct(const char *p, size_t n)
{
	int i = 0;

	while ((*p < '0' || *p > '7') && n > 0) {
		++p;
		--n;
	}
	while (*p >= '0' && *p <= '7' && n > 0) {
		i *= 8;
		i += *p - '0';
		++p;
		--n;
	}
	return (i);
}

/* Returns true if this is 512 zero bytes. */
static int is_end_of_archive(const char *p)
{
	int n;
	for (n = 511; n >= 0; --n)
		if (p[n] != '\0')
			return (0);
	return (1);
}

/* Create a directory, including parent directories as necessary. */
static void create_dir(const char *pathname)
{
	_mkdir(pathname);
}

/* Create a file, including parent directory as necessary. */
static FILE *create_file(const char *pathname)
{
	FILE *f;
	f = fopen(pathname, "wb+");
	if (f == NULL) {
		/* Try creating parent dir and then creating file. */
		char *p = strrchr(pathname, '/');
		if (p != NULL) {
			*p = '\0';
			create_dir(pathname);
			*p = '/';
			f = fopen(pathname, "wb+");
		}
	}
	return (f);
}

/* Extract a tar archive. */
static void untar(const char *buf)
{
	const char *buff = buf;
	FILE *f = NULL;
	size_t bytes_read;
	int filesize;

	for (;;) {
		if (is_end_of_archive(buff)) {
			return;
		}
		filesize = parseoct(buff + 124, 12);
		switch (buff[156]) {
		case '1':
			break;
		case '2':
			break;
		case '3':
			break;
		case '4':
			break;
		case '5':
			create_dir(buff);
			filesize = 0;
			break;
		case '6':
			break;
		default:
			f = create_file(buff);
			break;
		}
		while (filesize > 0) {
         buff += 512;
			if (filesize < 512)
				bytes_read = filesize;
         else
            bytes_read = 512;
			if (f != NULL) {
				if (fwrite(buff, 1, bytes_read, f) != bytes_read)
				{
					fprintf(stderr, "Failed write\n");
					fclose(f);
					f = NULL;
				}
			}
			filesize -= bytes_read;
		}
		if (f != NULL) {
			fclose(f);
			f = NULL;
		}
      buff += 512;
	}
}

int main() {
  char TempDirPath[512];
  char CWDPath[512];
  FILE *fd;
  const char *ptr_start;
  const char *ptr_end;

  setvbuf(stdout, NULL, _IONBF, 0);

  printf("Executing artos emulator...\n");

  GetTempPathA(sizeof(TempDirPath)-1, TempDirPath);
  _chdir(TempDirPath);
  _mkdir("TempARTOS");
  _chdir("TempARTOS");
  _getcwd(CWDPath, sizeof(CWDPath));
  printf("Work Directory: %s\n", CWDPath);

  printf("Writing out 'bootaa64.efi'...\n");
  _mkdir("disk");
  _mkdir("disk\\EFI");
  _mkdir("disk\\EFI\\boot");
  fd = fopen("disk\\EFI\\boot\\bootaa64.efi", "wb");
  ptr_start = &artos_start;
  ptr_end   = &artos_end;
  fwrite(ptr_start, ptr_end-ptr_start, 1, fd);
  fclose(fd);

  printf("Writing out 'uefi_code.fd'...\n");
  _mkdir("firmware");
  fd = fopen("firmware\\uefi_code.fd", "wb");
  ptr_start = &uefi_start;
  ptr_end   = &uefi_end;
  fwrite(ptr_start, ptr_end-ptr_start, 1, fd);
  fclose(fd);

  printf("Extracting 'qemu-win.tar'...\n");
  ptr_start = &qemu_start;
  untar(ptr_start);
  fclose(fd);

  printf("Done...\n");

  printf("Booting up...\n");
  system(qemu_cmd);

  return 0;
}