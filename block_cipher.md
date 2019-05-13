## AES:
 1. Byte Sub
 2. Shift rows
 3. Mix Columns
 4. Xor Key

## CBC
CBC attacks:
1. bit flipping attack
2. padding oracle attack

__prerequists:__
1. A target ciphertext and IV to decrypt.
2. A padding oracle: a function that takes ciphertexts and tells the attacker if the padding was correct  

__steps:__
1. Create a IV and XOR it with a decrypted intermedia data block. 
2. 0 ~ 255 last byte of IV to get 0x01 padding.
3. get the plain text of last bytes. 
    - test IV last byte XOR 0x01 -> intermedia data last byte.
    - intermedia data last byte XOR true TV last byte -> plaintext last byte.
4. repeat 1 ~ 3 as [0x02, 0x02] ...... [0xn ....... 0xn]
