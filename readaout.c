/*
 *  Copyright 2015 Geyslan G. Bem
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this name except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 *  readaout - An simple study about the oldish a.out format
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/a.out.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	struct exec *header;
	int fd, retval = 0;
	struct stat st;
	unsigned char *aoutbuffer;
	char *bintype, *machtype;

	if (argc < 2) {
		printf("Usage: ./readaout 'binary'\n");
		return EAGAIN;
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("ERROR");
		return errno;
	}
        fstat(fd, &st);

        aoutbuffer = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (aoutbuffer == MAP_FAILED) {
		retval = errno;
		perror("ERROR");
		goto out_open;
	}
        header = (struct exec *) aoutbuffer;

	switch N_MAGIC(*header) {
		case OMAGIC:
			bintype = "OMAGIC (object file or impure object)";
			break;
		case ZMAGIC:
			bintype = "ZMAGIC (demand-paged executable)";
			break;
		case QMAGIC:
			bintype = "QMAGIC (ZMAGIC with header in the text section)";
			break;
		case NMAGIC:
			bintype = "NMAGIC (pure executable)";
			break;
		case CMAGIC:
			bintype = "CMAGIC (Core file)";
			break;
		default:
			printf("The loaded file '%s' isn't a a.out or has a "
			       "malformed header (a.out type).\n", argv[1]);
			retval = EINVAL;
			goto out;
		}

	switch N_MACHTYPE(*header) {
		case M_OLDSUN2:
			machtype = "OLDSUN2";
			break;
		case M_68010:
			machtype = "68010";
			break;
		case M_68020:
			machtype = "68020";
			break;
		case M_SPARC:
			machtype = "SPARC";
			break;
		case M_386:
			machtype = "386";
			break;
		case M_MIPS1:
			machtype = "MIPS1";
			break;
		case M_MIPS2:
			machtype = "MIPS2";
			break;
		default:
			printf("The loaded file '%s' isn't a a.out or has a "
			       "malformed header (machine type).\n", argv[1]);
			retval = EINVAL;
			goto out;
		}

        printf("a.out Header: %s\n", argv[1]);

	printf("  Type:             %s\n", bintype);
        printf("   Magic:           %#o\n", N_MAGIC(*header));
	printf("  Machine Type:     %s\n", machtype);
        printf("  Flags:            %x\n", N_FLAGS(*header));
        printf("  Header size:      %lu (bytes)\n\n", sizeof(struct exec));

        printf("* OFFSETS *\n");
	printf("  Entry Point:      0x%08x\n", header->a_entry);
        printf("  Text Section:     0x%08x\n", N_TXTOFF(*header));

	printf("  Data Section:     ");
	if (header->a_data)
		printf("0x%08x\n", N_DATOFF(*header));
	else
		printf("\n");

	printf("  Text Relocation:  ");
	if (header->a_trsize)
	        printf("0x%08x\n", N_TRELOFF(*header));
	else
		printf("\n");

	printf("  Data Relocation:  ");
	if (header->a_drsize)
		printf("0x%08x\n", N_DRELOFF(*header));
	else
		printf("\n");

	printf("  Symbol Table:     ");
	if (header->a_syms)
		printf("0x%08x\n", N_SYMOFF(*header));
	else
		printf("\n");

	printf("\n");
	printf("* SECTIONS SIZES (in bytes) *\n");
        printf("  Text:             %d\n", header->a_text);
        printf("  Data:             %d\n", header->a_data);
        printf("  Text Relocation:  %d\n", header->a_trsize);
        printf("  Data Relocation:  %d\n\n", header->a_drsize);
        printf("  Symbols           %d\n", header->a_syms);
	printf("  Bss:              %d\n", header->a_bss);

        printf("\nWritten by Geyslan G. Bem\n"
               "http://hackingbits.com\n\n");

        printf("For more info about the a.out format, "
               "read /usr/include/uapi/linux/a.out.h "
               "and /usr/include/asm/a.out.h\n");

out:
        munmap(aoutbuffer, st.st_size);
out_open:
        close(fd);

        return retval;
}
