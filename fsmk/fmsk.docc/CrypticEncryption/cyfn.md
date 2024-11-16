# Cryptonic Cryption Methods

Securely handle encryption and decryption tasks for ScribbleLab using the Cryptonic Cryption Methods API.
 

@Metadata {
    @PageColor(red)
    @TitleHeading("API Collection")
}

## Overview

The Cryptonic Cryption Methods API, implemented in `cyfn`, offers a versatile interface for working with AES encryption in `ECB`, `CBC`, and `CTR` modes. It supports `AES-256` encryption, with customizable initialization vectors (`IVs`) and round keys for secure data handling. These methods form the cryptographic backbone of ScribbleLab's File System Management Kit (`FSMK`).

This API is designed with security and performance in mind, adhering to modern encryption standards to ensure data confidentiality.


## Topics

### Initialization

- <doc:fsmk/cyfn_init_ctx>
- <doc:fsmk/cyfn_init_ctx_iv>
- <doc:fsmk/cyfn_ctx_set_iv>

### Encryption

- <doc:fsmk/cyfn_ctr_xcrypt_buffer>

### Decryption

- <doc:fsmk/cyfn_ctr_xcrypt_buffer>

### Misc

- <doc:fsmk/CYFN_AES256>
- <doc:fsmk/CYFN_AES_BLOCKLEN>
- <doc:fsmk/CYFN_AES_KEYLEN>
- <doc:fsmk/CYFN_AES_keyExpSize>
- <doc:fsmk/CYFN_CBC>
- <doc:fsmk/CYFN_CTR>
- <doc:fsmk/CYFN_ECB>
- <doc:fsmk/cyfn_ctx>

## See Also

- <doc:fsmk>
- [Cryptography Basics (Wikipedia)](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard)

