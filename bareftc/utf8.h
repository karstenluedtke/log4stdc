/**
 * @file      utf8.h
 *
 * @brief     Macros for coding UTF-8 and UTF-16
 *
 * @author    Karsten Luedtke
 *
 * @date      2014-08-10
 *
 * Copyright (c)  2014  Karsten Luedtke, Berlin, Germany.
 */

#ifndef _BFC_UTF8_H_
#define _BFC_UTF8_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BFC_PUT_UTF8(cp,limit,unicode)                                      \
    do {                                                                      \
        if ((unicode) <= 0x7FuL) {                                            \
            if (cp < limit) {                                                 \
                cp[0] = (char)(unicode);                                      \
            }                                                                 \
            cp += 1;                                                          \
        } else if ((unicode) <= 0x7FFuL) {                                    \
            if (cp + 1 < limit) {                                             \
                cp[0] = (char)(0xC0 | ((unicode) >> 6));                      \
                cp[1] = (char)(0x80 | ((unicode)&0x3F));                      \
            }                                                                 \
            cp += 2;                                                          \
        } else if ((unsigned long)(unicode) <= 0xFFFFuL) {                    \
            if (cp + 2 < limit) {                                             \
                cp[0] = (char)(0xE0 | ((unicode) >> 12));                     \
                cp[1] = (char)(0x80 | (((unicode) >> 6) & 0x3F));             \
                cp[2] = (char)(0x80 | ((unicode)&0x3F));                      \
            }                                                                 \
            cp += 3;                                                          \
        } else if ((unsigned long)(unicode) <= 0x1FFFFFuL) {                  \
            if (cp + 3 < limit) {                                             \
                cp[0] = (char)(0xF0 | ((unicode) >> 18));                     \
                cp[1] = (char)(0x80 | (((unicode) >> 12) & 0x3F));            \
                cp[2] = (char)(0x80 | (((unicode) >> 6) & 0x3F));             \
                cp[3] = (char)(0x80 | ((unicode)&0x3F));                      \
            }                                                                 \
            cp += 4;                                                          \
        } else if ((unsigned long)(unicode) <= 0x3FFFFFFuL) {                 \
            if (cp + 4 < limit) {                                             \
                cp[0] = (char)(0xF8 | ((unicode) >> 24));                     \
                cp[1] = (char)(0x80 | (((unicode) >> 18) & 0x3F));            \
                cp[2] = (char)(0x80 | (((unicode) >> 12) & 0x3F));            \
                cp[3] = (char)(0x80 | (((unicode) >> 6) & 0x3F));             \
                cp[4] = (char)(0x80 | ((unicode)&0x3F));                      \
            }                                                                 \
            cp += 5;                                                          \
        } else {                                                              \
            if (cp + 5 < limit) {                                             \
                cp[0] = (char)(0xFC | (((unicode) >> 30) & 0x01));            \
                cp[1] = (char)(0x80 | (((unicode) >> 24) & 0x3F));            \
                cp[2] = (char)(0x80 | (((unicode) >> 18) & 0x3F));            \
                cp[3] = (char)(0x80 | (((unicode) >> 12) & 0x3F));            \
                cp[4] = (char)(0x80 | (((unicode) >> 6) & 0x3F));             \
                cp[5] = (char)(0x80 | ((unicode)&0x3F));                      \
            }                                                                 \
            cp += 6;                                                          \
        }                                                                     \
    } while (0 /*just once*/)

#define BFC_GET_UTF8(unicode,cp,limit)                                      \
    do {                                                                      \
        unicode = *(cp++) & 0xFFu;                                            \
        if ((unicode >= 0xC0) && (cp < (limit))) {                            \
            if (unicode < 0xE0) {                                             \
                unicode = ((unicode & 0x1F) << 6) | (cp[0] & 0x3F);           \
                cp += 1;                                                      \
            } else if ((unicode < 0xF0) && (cp + 1 < (limit))) {              \
                unicode = ((unicode & 0x0F) << 12) |                          \
                          (((unsigned)cp[0] & 0x3F) << 6) |                   \
                          (((unsigned)cp[1] & 0x3F));                         \
                cp += 2;                                                      \
                if ((unicode >= 0xD800) && (unicode < 0xDC00) &&              \
                    (cp + 2 < (limit)) &&                                     \
                    (((unsigned)cp[0] & 0xFF) == 0xED)) {                     \
                    unicode = (((unicode & 0x3FF) + 0x40) << 10) |            \
                              (((unsigned)cp[1] & 0x0F) << 6) |               \
                              (((unsigned)cp[2] & 0x3F));                     \
                    cp += 3;                                                  \
                }                                                             \
            } else if ((unicode < 0xF8) && (cp + 2 < (limit))) {              \
                unicode = ((unicode & 0x07) << 12) |                          \
                          (((unsigned)cp[0] & 0x3F) << 6) |                   \
                          (((unsigned)cp[1] & 0x3F));                         \
                unicode = ((unicode << 6) | (cp[2] & 0x3F));                  \
                cp += 3;                                                      \
            } else if ((unicode < 0xFC) && (cp + 3 < (limit))) {              \
                unicode = ((unicode & 0x03) << 12) |                          \
                          (((unsigned)cp[0] & 0x3F) << 6) |                   \
                          (((unsigned)cp[1] & 0x3F));                         \
                unicode = ((unicode << 6) | (cp[2] & 0x3F));                  \
                unicode = ((unicode << 6) | (cp[3] & 0x3F));                  \
                cp += 4;                                                      \
            } else if (cp + 4 < (limit)) {                                    \
                unicode = ((unicode & 0x01) << 12) |                          \
                          (((unsigned)cp[0] & 0x3F) << 6) |                   \
                          (((unsigned)cp[1] & 0x3F));                         \
                unicode = ((unicode << 6) | (cp[2] & 0x3F));                  \
                unicode = ((unicode << 6) | (cp[3] & 0x3F));                  \
                unicode = ((unicode << 6) | (cp[4] & 0x3F));                  \
                cp += 5;                                                      \
            }                                                                 \
        }                                                                     \
    } while (0 /*just once*/)

#define BFC_GET_UTF16(unicode,wp,limit)                                     \
    do {                                                                      \
        unicode = *(wp++) & 0xFFFFu;                                          \
        if ((unicode >= 0xD800) && (unicode < 0xDC00) && (wp < (limit))) {    \
            unsigned _low = *(wp)&0xFFFF;                                     \
            if ((_low >= 0xDC00) && (_low < 0xDF00)) {                        \
                unicode =                                                     \
                    (((unicode & 0x3FF) + 0x40) << 10) + (_low & 0x3FF);      \
                wp++;                                                         \
            }                                                                 \
        }                                                                     \
    } while (0 /*just once*/)

#define BFC_PUT_UTF16(wp,limit,unicode)                                     \
    do {                                                                      \
        if ((unsigned long)(unicode) <= 0xFFFFuL) {                           \
            if (wp < limit) {                                                 \
                wp[0] = (char)(unicode);                                      \
            }                                                                 \
            wp += 1;                                                          \
        } else {                                                              \
            unsigned long u = unicode - 0x10000uL;                            \
            if (wp + 1 < limit) {                                             \
                wp[0] = (unsigned short)(0xD800 | (((u) >> 10) & 0x3FF));     \
                wp[1] = (unsigned short)(0xDC00 | ((u)&0x3FF));               \
            }                                                                 \
            wp += 2;                                                          \
        }                                                                     \
    } while (0 /*just once*/)

#ifdef __cplusplus
} /* C++ */
#endif
#endif /* _BFC_UTF8_H_ */
