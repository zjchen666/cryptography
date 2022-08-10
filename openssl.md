### Check a certificate
Check a certificate and return information about it (signing authority, expiration date, etc.):

openssl x509 -in server.crt -text -noout

### Check a key
Check the SSL key and verify the consistency:

openssl rsa -in server.key -check

### Check a CSR
Verify the CSR and print CSR data filled in when generating the CSR:

openssl req -text -noout -verify -in server.csr

### Verify a certificate and key matches
These two commands print out md5 checksums of the certificate and key; the checksums can be compared to verify that the certificate and key match.

openssl x509 -noout -modulus -in server.crt| openssl md5
openssl rsa -noout -modulus -in server.key| openssl md5
