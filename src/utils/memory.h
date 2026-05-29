#ifndef DF_UTILS_MEMORY_H
#define DF_UTILS_MEMORY_H

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                              \
	((byte) & 0x80 ? '1' : '0'), ((byte) & 0x40 ? '1' : '0'),         \
		((byte) & 0x20 ? '1' : '0'), ((byte) & 0x10 ? '1' : '0'), \
		((byte) & 0x08 ? '1' : '0'), ((byte) & 0x04 ? '1' : '0'), \
		((byte) & 0x02 ? '1' : '0'), ((byte) & 0x01 ? '1' : '0')

#define SHOW_MEMORY(item)                                     \
	do {                                                  \
		unsigned char *p = &(item);                   \
		for (size_t i = 0; i < sizeof((item)); i++) { \
			cprintf(COLOR_GREEN, COLOR_BLACK,     \
				BYTE_TO_BINARY_PATTERN " ",   \
				BYTE_TO_BINARY(p[i]));        \
			if (i % 8 == 7)                       \
				printf("\n");                 \
		}                                             \
		printf("\n");                                 \
	} while (0)

#endif // DF_UTILS_MEMORY_H
