
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "des3.h"


static
void
DES_set_key_unchecked(const_DES_cblock * key, DES_key_schedule * schedule)
{
	static const int shifts2[16] = {
		0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0
	};
	register DES_LONG c, d, t, s, t2;
	register const unsigned char * in;
	register DES_LONG * k;
	register int	i;


	k					= &schedule->ks->deslong[0];
	in					= & (*key)[0];

	c2l(in, c);
	c2l(in, d);

	/*
	 * do PC1 in 47 simple operations :-) Thanks to John Fletcher
	 * (john_fletcher@lccmail.ocf.llnl.gov) for the inspiration. :-)
	 */
	PERM_OP(d, c, t, 4, 0x0f0f0f0fL);
	HPERM_OP(c, t, -2, 0xcccc0000L);
	HPERM_OP(d, t, -2, 0xcccc0000L);
	PERM_OP(d, c, t, 1, 0x55555555L);
	PERM_OP(c, d, t, 8, 0x00ff00ffL);
	PERM_OP(d, c, t, 1, 0x55555555L);
	d					=
		 (((d & 0x000000ffL) << 16L) | (d & 0x0000ff00L) | ((d & 0x00ff0000L) >> 16L) | ((c & 0xf0000000L) >> 4L));
	c					&= 0x0fffffffL;

	for (i = 0; i < ITERATIONS; i++) {
		if (shifts2[i]) {
			c					= ((c >> 2L) | (c << 26L));
			d					= ((d >> 2L) | (d << 26L));
		}
		else {
			c					= ((c >> 1L) | (c << 27L));
			d					= ((d >> 1L) | (d << 27L));
		}

		c					&= 0x0fffffffL;
		d					&= 0x0fffffffL;

		/*
		 * could be a few less shifts but I am to lazy at this point in time
		 * to investigate
		 */
		s					=
			 des_skb[0][(c) & 0x3f] | des_skb[1][((c >> 6L) & 0x03) | ((c >> 7L) & 0x3c)] | des_skb[2][((c >> 13L) & 0x0f) | ((c >> 14L) & 0x30)] | des_skb[3][((c >> 20L) & 0x01) | ((c >> 21L) & 0x06) | ((c >> 22L) & 0x38)];
		t					=
			 des_skb[4][(d) & 0x3f] | des_skb[5][((d >> 7L) & 0x03) | ((d >> 8L) & 0x3c)] | des_skb[6][(d >> 15L) & 0x3f] | des_skb[7][((d >> 21L) & 0x0f) | ((d >> 22L) & 0x30)];

		/* table contained 0213 4657 */
		t2					= ((t << 16L) | (s & 0x0000ffffL)) & 0xffffffffL;
		* (k++) 			= ROTATE(t2, 30) & 0xffffffffL;

		t2					= ((s >> 16L) | (t & 0xffff0000L));
		* (k++) 			= ROTATE(t2, 26) & 0xffffffffL;
	}
}

static
void
DES_encrypt2(DES_LONG * data, DES_key_schedule * ks, int enc)
{
	register DES_LONG l, r, t, u;

	/*
	#ifdef DES_PTR
	register const unsigned char *des_SP = (const unsigned char *)DES_SPtrans;
	#endif
	#ifndef DES_UNROLL
	register int i;
	#endif*/
	register int	i;
	register DES_LONG * s;

	r					= data[0];
	l					= data[1];

	/*
	 * Things have been modified so that the initial rotate is done outside
	 * the loop.  This required the DES_SPtrans values in sp.h to be rotated
	 * 1 bit to the right. One perl script later and things have a 5% speed
	 * up on a sparc2. Thanks to Richard Outerbridge
	 * <71755.204@CompuServe.COM> for pointing this out.
	 */
	/* clear the top bits on machines with 8byte longs */
	r					= ROTATE(r, 29) & 0xffffffffL;
	l					= ROTATE(l, 29) & 0xffffffffL;

	s					= ks->ks->deslong;

	/*
	 * I don't know if it is worth the effort of loop unrolling the inner
	 * loop
	 */
	if (enc) {

		for (i = 0; i < 32; i += 4) {
			D_ENCRYPT(l, r, i + 0); 				/* 1 */
			D_ENCRYPT(r, l, i + 2); 				/* 2 */
		}

	}
	else {

		for (i = 30; i > 0; i -= 4) {
			D_ENCRYPT(l, r, i - 0); 				/* 16 */
			D_ENCRYPT(r, l, i - 2); 				/* 15 */
		}

	}

	/* rotate and clear the top bits on machines with 8byte longs */
	data[0] 			= ROTATE(l, 3) & 0xffffffffL;
	data[1] 			= ROTATE(r, 3) & 0xffffffffL;
	l					= r = t = u = 0;
}

static
void
DES_encrypt3(DES_LONG * data, DES_key_schedule * ks1,
	DES_key_schedule * ks2, DES_key_schedule * ks3)
{
	register DES_LONG l, r;

	l					= data[0];
	r					= data[1];
	IP(l, r);
	data[0] 			= l;
	data[1] 			= r;
	DES_encrypt2((DES_LONG *) data, ks1, DES_ENCRYPT);
	DES_encrypt2((DES_LONG *) data, ks2, DES_DECRYPT);
	DES_encrypt2((DES_LONG *) data, ks3, DES_ENCRYPT);
	l					= data[0];
	r					= data[1];
	FP(r, l);
	data[0] 			= l;
	data[1] 			= r;
}


static
void
DES_decrypt3(DES_LONG * data, DES_key_schedule * ks1,
	DES_key_schedule * ks2, DES_key_schedule * ks3)
{
	register DES_LONG l, r;

	l					= data[0];
	r					= data[1];
	IP(l, r);
	data[0] 			= l;
	data[1] 			= r;
	DES_encrypt2((DES_LONG *) data, ks3, DES_DECRYPT);
	DES_encrypt2((DES_LONG *) data, ks2, DES_ENCRYPT);
	DES_encrypt2((DES_LONG *) data, ks1, DES_DECRYPT);
	l					= data[0];
	r					= data[1];
	FP(r, l);
	data[0] 			= l;
	data[1] 			= r;
}

static
void DES_ecb3_encrypt(const_DES_cblock * input, DES_cblock * output,
	DES_key_schedule * ks1, DES_key_schedule * ks2, DES_key_schedule * ks3, int enc)
{
	register DES_LONG l0, l1;
	DES_LONG		ll[2];
	const unsigned char * in = & (*input)[0];
	unsigned char * out = & (*output)[0];

	c2l(in, l0);
	c2l(in, l1);
	ll[0]				= l0;
	ll[1]				= l1;

	if (enc)
		DES_encrypt3(ll, ks1, ks2, ks3);
	else
		DES_decrypt3(ll, ks1, ks2, ks3);

	l0					= ll[0];
	l1					= ll[1];
	l2c(l0, out);
	l2c(l1, out);
}


/**
 *函数功能：          des3 字符串 加密 / 解密
 *参数：         cmd       DES_ENCRYPT加密                 DES_DECRYPT解密
                src_dat   明文字符串                       暗文字符串
                dat_len   src_dat的长度
                dst       暗文字符串                       明文字符串
 *返回值：	     加密时，无意义
 		解密时，解密后的明文长度
 */
int des3_api(int cmd, unsigned char * src_dat, int dat_len, unsigned char * * dst)
{
	int 			i	= 0;
	int 			len = 0;
	int 			nlen = 0;

	char			ch	= '\0';
	char *			data 	= (char *)src_dat;		/* 获取输入明文 暗文 */
	unsigned char	key[LEN_OF_KEY];
 /*   unsigned char	src[128] = {
		0
	};
	unsigned char	out[128] = {
		0
    };
*/
    unsigned char * src, * out;

	unsigned char	block[8] = {
		0
	};
	DES_key_schedule ks1, ks2, ks3;

    src = (unsigned char *)malloc(4096);
	if (NULL == src) {
		* dst = NULL;
		return 0;
	}
	out = (unsigned char *)malloc(4096);
	if (NULL == out) {
		free(src);
		* dst = NULL;
		return 0;
    }


        memset(block, 0, 8);

	/* 读取大密钥 */
	memcpy(key, DES3_KEY, DES3_KEY_LEN);
	memset(key + DES3_KEY_LEN, 0x00, LEN_OF_KEY - DES3_KEY_LEN);

        /* 将大密钥切成3个小密钥 */
	memcpy(block, key, sizeof(block));
	DES_set_key_unchecked((const_DES_cblock *) block, &ks1);

	memcpy(block, key + 8, sizeof(block));
	DES_set_key_unchecked((const_DES_cblock *) block, &ks2);

	memcpy(block, key + 16, sizeof(block));
	DES_set_key_unchecked((const_DES_cblock *) block, &ks3);



        switch (cmd) {
                case DES_ENCRYPT:                 //加密

                    //nlen				= strlen(data);				//读取输入字符串总长度、读取输入字符串到src缓存
                    nlen = dat_len;

                	len 				= (nlen / 8 + 1) * 8;			//计算输入字符串8为单位的块数

                	ch					= 8 - nlen % 8;			//计算最后一块补齐所需的字节数

                   // memset(src, 0, nlen + (8 - nlen % 8));
                   // memset(out, 0, len);
                    memcpy(src, data, nlen);
                	memset(src + nlen, ch, (8 - nlen % 8));					//src缓存字符串以'\0'补齐

                        //以8byte为单位加密所有块
                        for (i = 0; i < len; i += 8) {
                                DES_ecb3_encrypt((const_DES_cblock *) (src + i), (DES_cblock *) (out + i), &ks1, &ks2, &ks3, DES_ENCRYPT);
                        }

                        * dst = (unsigned char *)strdup((const char *)out);			//拷贝密文到输出参数
                        //len -= ch;
                        break;

                case DES_DECRYPT:                 //解密
			len				= strlen(data);				//读取输入字符串总长度、读取输入字符串到src缓存
                    len = dat_len;
                    //memset(src, 0, len);
                    //memset(out, 0, len);
                	memcpy(src, data, len);

                        //以8byte为单位解密所有块
                        for (i = 0; i < len; i += 8) {
                                DES_ecb3_encrypt((const_DES_cblock *) (src + i), (DES_cblock *) (out + i), &ks1, &ks2, &ks3, DES_DECRYPT);
                        }

                        * dst = (unsigned char *)strdup((const char *)out);			//拷贝密文到输入参数
                        break;

                default:
                        break;
        }

    free(src);
    free(out);

	return len;
}

//3des 测试程序
#if 0
int main00(int argc, char * * argv)
{
        unsigned char * p_anwen, * p_mingwen;
        int 		len = 0, i;

        //加密
        len = des3_api(DES_ENCRYPT, (unsigned char *)"0F8lA48594AC4EC8CDF AABl5E2CFF733DEEl0C8113B82F0384D073F9527Blaaaacl{};;;/?,.&^$#@~``", &p_anwen);

        //解密
        len = des3_api(DES_DECRYPT, p_anwen, &p_mingwen);
        printf("decrypted bin:  ");
        for (i = 0; i < len; i++) {
                printf("%c", * (p_mingwen + i));
        }
        printf("\n");

        free(p_anwen);
        free(p_mingwen);

        return 0;
}
#endif
