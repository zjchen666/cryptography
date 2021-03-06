## RSA

## RSA permutation -  用于加密key

1. 选取 p 和 q 两个质数， p*q = N， φ(N) = （p-1) * (q - 1）
2. 选取 e 和 d， 要求 e * d = k * φ(N) + 1
3. 公钥：pk =（N，e）, 私钥：sk = (N, d)
4. 加密: RSA（x）= x^e
5. 解密：x^e*d = x^(k * φ(n) + 1) = x

## RSA public key encryption
加密：
1. key = HASH（x）
2. y = RSA(x)
3. output = E(y, E(k, m))  

解密：
1. x = y^d
2. key = HASH(x)
3. D(key, E(k,m))

__注意避免 textbook RSA（直接对m进行RSA加解密) 会被meet in the middle 攻击__

## diffe-hellman 

