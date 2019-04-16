/*
 * Copyright (c) 2015-2017 by IT Center, RWTH Aachen University
 *
 * This file is part of EffortLog, a tool for collecting software
 * development effort.
 *
 * EffortLog is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EffortLog is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EffortLog.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "crypt.h"
#include <openssl/aes.h>

#include <QDebug>

static unsigned int salt[] = {12345, 54321};

Crypt::Crypt() {
  encode_ctx_ = EVP_CIPHER_CTX_new();
  decode_ctx_ = EVP_CIPHER_CTX_new();
  EVP_CIPHER_CTX_init(encode_ctx_);
  EVP_CIPHER_CTX_init(decode_ctx_);
}

Crypt::~Crypt() {
  EVP_CIPHER_CTX_cleanup(encode_ctx_);
  EVP_CIPHER_CTX_cleanup(decode_ctx_);
  EVP_CIPHER_CTX_free(encode_ctx_);
  EVP_CIPHER_CTX_free(decode_ctx_);
}

bool Crypt::Encrypt(const QByteArray &data, QByteArray &enc_data) {
  unsigned char key[32], iv[32];
  memset(&iv, '\0', 32);
  int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(), (unsigned char *)&salt,
                         (unsigned char *)pwd_.data(), pwd_.size(), 1, key, iv);
  if (i != 32) {
    qWarning("Encryption: Key has wrong size\n");
    return false;
  }

  if (!EVP_EncryptInit_ex(decode_ctx_, EVP_aes_256_cbc(), NULL, key, iv)) {
    qWarning("Encryption: Couldn't generate key.");
    return false;
  }

  int cipher_len = data.size() + AES_BLOCK_SIZE;
  unsigned char *cipher = (unsigned char *)malloc(cipher_len);

  if (!EVP_EncryptUpdate(decode_ctx_, cipher, &cipher_len,
                         (const unsigned char *)data.data(), data.size() + 1)) {
    qWarning("Encryption: Couldn't generate key.");
    return false;
  }
  int len = 0;
  if (!EVP_EncryptFinal_ex(decode_ctx_, cipher + cipher_len, &len)) {
    qWarning("Encryption: Couldn't generate key.");
    return false;
  }
  QByteArray out = QByteArray((const char *)cipher, cipher_len + len);
  free(cipher);
  enc_data = "Salted__" + QByteArray((const char *)&salt, 8) + out;
  return true;
}

bool Crypt::Decrypt(const QByteArray &data, QByteArray &dec_data) {
  unsigned char key[32], iv[32];
  int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(), (unsigned char *)&salt,
                         (unsigned char *)pwd_.data(), pwd_.size(), 1, key, iv);
  if (i != 32) {
    qWarning("Encryption: Key has wrong size\n");
    return false;
  }

  if (!EVP_DecryptInit_ex(encode_ctx_, EVP_aes_256_cbc(), NULL, key, iv)) {
    qWarning("Decryption: Couldn't generate key.");
    return false;
  }

  QByteArray body = data.mid(16);
  int body_len = body.size();
  unsigned char *plain = (unsigned char *)malloc(body_len);
  if (!EVP_DecryptUpdate(encode_ctx_, plain, &body_len,
                         (unsigned char *)body.data(), body.size())) {
    qWarning("Decryption: Couldn't generate key.");
    return false;
  }
  int len = 0;
  if (!EVP_DecryptFinal_ex(encode_ctx_, plain + body_len, &len)) {
    qWarning("Decryption: Couldn't generate key.");
    return false;
  }
  dec_data = QByteArray((const char *)plain, body_len + len - 1);
  free(plain);
  return true;
}

void Crypt::SetPwd(QByteArray p) { pwd_ = p; }

QByteArray Crypt::GetPwd() const { return pwd_; }
