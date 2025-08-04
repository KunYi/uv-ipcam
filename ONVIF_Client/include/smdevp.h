#ifndef SMDEVP_H
#define SMDEVP_H

#include "stdsoap2.h"

#ifdef WITH_OPENSSL
#include <openssl/evp.h>
#include <openssl/hmac.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Expose EVP_PKEY in a portable representation */
#define SOAP_SMD_KEY_TYPE	EVP_PKEY

/** Expose EVP_MAX_MD_SIZE in a portable representation */
#define SOAP_SMD_MAX_SIZE	EVP_MAX_MD_SIZE

/** MD5 digest size in octets */
#define SOAP_SMD_MD5_SIZE	(16)

/** SHA1 digest size in octets */
#define SOAP_SMD_SHA1_SIZE	(20)

/** SHA256 digest size in octets */
#define SOAP_SMD_SHA256_SIZE	(32)

/** SHA512 digest size in octets */
#define SOAP_SMD_SHA512_SIZE	(64)

/******************************************************************************\
 *
 * Supported algorithms
 *
\******************************************************************************/

#define SOAP_SMD_NONE	(0x0000)
#define SOAP_SMD_HASH	(0x0003)
#define SOAP_SMD_ALGO	(0x000C)
#define SOAP_SMD_MASK	(0x01FF)

/** MD5 hash */
#define SOAP_SMD_MD5	(0x00)
/** SHA1 hash */
#define SOAP_SMD_SHA1	(0x01)
/** SHA256 hash */
#define SOAP_SMD_SHA256	(0x02)
/** SHA512 hash */
#define SOAP_SMD_SHA512	(0x03)

/** HMAC */
#define SOAP_SMD_HMAC	(0x00)
/** Digest */
#define SOAP_SMD_DGST	(0x04)
/** Sign */
#define SOAP_SMD_SIGN	(0x08)
/** Verify */
#define SOAP_SMD_VRFY	(0x0C)

/** DSA (RSA) signature */
#define SOAP_SMD_DSA	(0x10)

/** HMAC-MD5 shared key signature algorithm */
#define SOAP_SMD_HMAC_MD5	 (SOAP_SMD_HMAC | SOAP_SMD_MD5)
/** HMAC-SHA1 shared key signature algorithm */
#define SOAP_SMD_HMAC_SHA1	 (SOAP_SMD_HMAC | SOAP_SMD_SHA1)
/** HMAC-SHA256 shared key signature algorithm */
#define SOAP_SMD_HMAC_SHA256	 (SOAP_SMD_HMAC | SOAP_SMD_SHA256)
/** HMAC-SHA512 shared key signature algorithm */
#define SOAP_SMD_HMAC_SHA512	 (SOAP_SMD_HMAC | SOAP_SMD_SHA512)

/** DGST-MD5 digest algorithm */
#define SOAP_SMD_DGST_MD5	 (SOAP_SMD_DGST | SOAP_SMD_MD5)
/** DGST-SHA1 digest algorithm */
#define SOAP_SMD_DGST_SHA1	 (SOAP_SMD_DGST | SOAP_SMD_SHA1)
/** DGST-SHA256 digest algorithm */
#define SOAP_SMD_DGST_SHA256	 (SOAP_SMD_DGST | SOAP_SMD_SHA256)
/** DGST-SHA512 digest algorithm */
#define SOAP_SMD_DGST_SHA512	 (SOAP_SMD_DGST | SOAP_SMD_SHA512)

/** DSA-MD5 digest algorithm */
#define SOAP_SMD_DGST_MD5	 (SOAP_SMD_DGST | SOAP_SMD_MD5)
/** DGST-SHA1 digest algorithm */
#define SOAP_SMD_DGST_SHA1	 (SOAP_SMD_DGST | SOAP_SMD_SHA1)
/** DGST-SHA256 digest algorithm */
#define SOAP_SMD_DGST_SHA256	 (SOAP_SMD_DGST | SOAP_SMD_SHA256)
/** DGST-SHA512 digest algorithm */
#define SOAP_SMD_DGST_SHA512	 (SOAP_SMD_DGST | SOAP_SMD_SHA512)

/** RSA-MD5 secret key signature algorithm */
#define SOAP_SMD_SIGN_RSA_MD5	 (SOAP_SMD_SIGN | SOAP_SMD_MD5)
/** RSA-SHA1 secret key signature algorithm */
#define SOAP_SMD_SIGN_RSA_SHA1	 (SOAP_SMD_SIGN | SOAP_SMD_SHA1)
/** RSA-SHA256 secret key signature algorithm */
#define SOAP_SMD_SIGN_RSA_SHA256 (SOAP_SMD_SIGN | SOAP_SMD_SHA256)
/** RSA-SHA512 secret key signature algorithm */
#define SOAP_SMD_SIGN_RSA_SHA512 (SOAP_SMD_SIGN | SOAP_SMD_SHA512)

/** DSA-MD5 secret key signature algorithm */
#define SOAP_SMD_SIGN_DSA_MD5	 (SOAP_SMD_SIGN | SOAP_SMD_DSA | SOAP_SMD_MD5)
/** DSA-SHA1 secret key signature algorithm */
#define SOAP_SMD_SIGN_DSA_SHA1	 (SOAP_SMD_SIGN | SOAP_SMD_DSA | SOAP_SMD_SHA1)
/** DSA-SHA256 secret key signature algorithm */
#define SOAP_SMD_SIGN_DSA_SHA256 (SOAP_SMD_SIGN | SOAP_SMD_DSA | SOAP_SMD_SHA256)
/** DSA-SHA512 secret key signature algorithm */
#define SOAP_SMD_SIGN_DSA_SHA512 (SOAP_SMD_SIGN | SOAP_SMD_DSA | SOAP_SMD_SHA512)

/** RSA-MD5 secret key signature verification algorithm */
#define SOAP_SMD_VRFY_RSA_MD5	 (SOAP_SMD_VRFY | SOAP_SMD_MD5)
/** RSA-SHA1 secret key signature verification algorithm */
#define SOAP_SMD_VRFY_RSA_SHA1	 (SOAP_SMD_VRFY | SOAP_SMD_SHA1)
/** RSA-SHA256 secret key signature verification algorithm */
#define SOAP_SMD_VRFY_RSA_SHA256 (SOAP_SMD_VRFY | SOAP_SMD_SHA256)
/** RSA-SHA512 secret key signature verification algorithm */
#define SOAP_SMD_VRFY_RSA_SHA512 (SOAP_SMD_VRFY | SOAP_SMD_SHA512)

/** DSA-MD5 secret key signature verification algorithm */
#define SOAP_SMD_VRFY_DSA_MD5	 (SOAP_SMD_VRFY | SOAP_SMD_DSA | SOAP_SMD_MD5)
/** DSA-SHA1 secret key signature verification algorithm */
#define SOAP_SMD_VRFY_DSA_SHA1	 (SOAP_SMD_VRFY | SOAP_SMD_DSA | SOAP_SMD_SHA1)
/** DSA-SHA256 secret key signature verification algorithm */
#define SOAP_SMD_VRFY_DSA_SHA256 (SOAP_SMD_VRFY | SOAP_SMD_DSA | SOAP_SMD_SHA256)
/** DSA-SHA512 secret key signature verification algorithm */
#define SOAP_SMD_VRFY_DSA_SHA512 (SOAP_SMD_VRFY | SOAP_SMD_DSA | SOAP_SMD_SHA512)

/** Additional flag: msg sends will pass through digest/signature algorithm */
#define SOAP_SMD_PASSTHRU	 (0x100)

/**
@struct soap_smd_data
@brief The smdevp engine context data, which is hooked up to soap->data[0]
*/
struct soap_smd_data
{ int alg;		/**< The digest or signature algorithm used */
  void *ctx;		/**< EVP_MD_CTX or HMAC_CTX */
  const void *key;	/**< EVP_PKEY */
  int (*fsend)(struct soap*, const char*, size_t);
  size_t (*frecv)(struct soap*, char*, size_t);
  soap_mode mode;	/**< to preserve soap->mode value */
};

/******************************************************************************\
 *
 * soap_smd API functions
 *
\******************************************************************************/

size_t soap_smd_size(int alg, const void *key);

int soap_smd_begin(struct soap *soap, int alg, const void *key, int keylen);
int soap_smd_end(struct soap *soap, char *buf, int *len);

int soap_smd_init(struct soap *soap, struct soap_smd_data *data, int alg, const void *key, int keylen);
int soap_smd_update(struct soap *soap, struct soap_smd_data *data, const char *buf, size_t len);
int soap_smd_final(struct soap *soap, struct soap_smd_data *data, char *buf, int *len);

#ifdef __cplusplus
}
#endif

#endif
