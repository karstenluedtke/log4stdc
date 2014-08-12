
#define BFC_PUT_UTF8(cp,limit,unicode)					    \
    do {								    \
	if ((unicode) <= 0x7FuL) {					    \
	    if (cp < limit) {						    \
		cp[0] = (unsigned char) (unicode);			    \
	    }								    \
	    cp += 1;							    \
	} else if ((unicode) <= 0x7FFuL) {				    \
	    if (cp+1 < limit) {						    \
		cp[0] = (unsigned char) (0xC0 | ((unicode) >> 6));	    \
		cp[1] = (unsigned char) (0x80 | ((unicode)        & 0x3F)); \
	    }								    \
	    cp += 2;							    \
	} else if ((unicode) <= 0xFFFFuL) {				    \
	    if (cp+2 < limit) {						    \
		cp[0] = (unsigned char) (0xE0 | ((unicode) >> 12));	    \
		cp[1] = (unsigned char) (0x80 |(((unicode) >>  6) & 0x3F)); \
		cp[2] = (unsigned char) (0x80 | ((unicode)        & 0x3F)); \
	    }								    \
	    cp += 3;							    \
	} else if ((unicode) <= 0x1FFFFFuL) {				    \
	    if (cp+3 < limit) {						    \
		cp[0] = (unsigned char) (0xF0 | ((unicode) >> 18));	    \
		cp[1] = (unsigned char) (0x80 |(((unicode) >> 12) & 0x3F)); \
		cp[2] = (unsigned char) (0x80 |(((unicode) >>  6) & 0x3F)); \
		cp[3] = (unsigned char) (0x80 | ((unicode)        & 0x3F)); \
	    }								    \
	    cp += 4;							    \
	} else if ((unicode) <= 0x3FFFFFFuL) {				    \
	    if (cp+4 < limit) {						    \
		cp[0] = (unsigned char) (0xF8 | ((unicode) >> 24));	    \
		cp[1] = (unsigned char) (0x80 |(((unicode) >> 18) & 0x3F)); \
		cp[2] = (unsigned char) (0x80 |(((unicode) >> 12) & 0x3F)); \
		cp[3] = (unsigned char) (0x80 |(((unicode) >>  6) & 0x3F)); \
		cp[4] = (unsigned char) (0x80 | ((unicode)        & 0x3F)); \
	    }								    \
	    cp += 5;							    \
	} else {							    \
	    if (cp+5 < limit) {						    \
		cp[0] = (unsigned char) (0xFC |(((unicode) >> 30) & 0x01)); \
		cp[1] = (unsigned char) (0x80 |(((unicode) >> 24) & 0x3F)); \
		cp[2] = (unsigned char) (0x80 |(((unicode) >> 18) & 0x3F)); \
		cp[3] = (unsigned char) (0x80 |(((unicode) >> 12) & 0x3F)); \
		cp[4] = (unsigned char) (0x80 |(((unicode) >>  6) & 0x3F)); \
		cp[5] = (unsigned char) (0x80 | ((unicode)        & 0x3F)); \
	    }								    \
	    cp += 6;							    \
	}								    \
    } while (0 /*just once*/)

#if 0
int
decode_utf8_codepoint(long *codebuf, const char *src, int len)
{
	const unsigned char *buf = (const unsigned char *) src;
	unsigned long unicode = *buf;
	int consumed = 1;

	if (unicode < 0xC0) {
		consumed = 1;
	} else if (unicode < 0xE0) {
		unicode = ((unicode & 0x1F) << 6)
			| ( buf[1]  & 0x3F);
		consumed = 2;
	} else if (unicode < 0xF0) {
		unicode = ((unicode & 0x0F) << 12)
			| (((unsigned long) buf[1] & 0x3F) <<  6)
			| (((unsigned long) buf[2] & 0x3F)      );
		consumed = 3;
	} else if (unicode < 0xF8) {
		unicode = ((unicode & 0x07) << 18)
			| (((unsigned long) buf[1] & 0x3F) << 12)
			| (((unsigned long) buf[2] & 0x3F) <<  6)
			| (((unsigned long) buf[3] & 0x3F)      );
		consumed = 4;
	} else if (unicode < 0xFC) {
		unicode = ((unicode & 0x03) << 24)
			| (((unsigned long) buf[1] & 0x3F) << 18)
			| (((unsigned long) buf[2] & 0x3F) << 12)
			| (((unsigned long) buf[3] & 0x3F) <<  6)
			| (((unsigned long) buf[4] & 0x3F)      );
		consumed = 5;
	} else {
		unicode = ((unicode & 0x01) << 30)
			| (((unsigned long) buf[1] & 0x3F) << 24)
			| (((unsigned long) buf[2] & 0x3F) << 18)
			| (((unsigned long) buf[3] & 0x3F) << 12)
			| (((unsigned long) buf[4] & 0x3F) <<  6)
			| (((unsigned long) buf[5] & 0x3F)      );
		consumed = 6;
	}
	if (consumed > len) {
		return (-EINVAL);
	}
	*codebuf = (long) unicode;
	return (consumed);
}
#endif

#define BFC_GET_UTF16(unicode,wp,limit)					    \
do {									    \
	unicode = *(wp++) & 0xFFFFu;					    \
	if ((unicode >= 0xD800) && (unicode < 0xDC00) && (wp < (limit))) {  \
		unsigned _low = *(wp) & 0xFFFF;				    \
		if ((_low >= 0xDC00) && (_low < 0xDF00)) {		    \
			unicode = 0x10000uL + ((unicode & 0x3FF) << 10)	    \
					    + (_low & 0x3FF);		    \
			wp++;						    \
		}							    \
	}								    \
} while (0 /*just once*/)

