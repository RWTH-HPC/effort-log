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
#include <openssl/evp.h>

#include <QDebug>

static unsigned int salt[] = {12345, 54321};

Crypt::Crypt() {}

bool Crypt::Encrypt(const QByteArray &data, QByteArray &enc_data) {
  QByteArray out;
  unsigned char *key_data = (unsigned char *)pwd_.constData();
  int key_data_len = pwd_.size();

  unsigned char key[32], iv[32];
  memset(&iv, '\0', 32);
  int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(), (unsigned char *)&salt,
                         key_data, key_data_len, 1, key, iv);
  if (i != 32) {
    qWarning("Encryption: Key has wrong size\n");
    return false;
  }

  EVP_CIPHER_CTX ctx;
  EVP_CIPHER_CTX_init(&ctx);
  if (!EVP_EncryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
    qWarning("Encryption: Couldn't generate key.");
    return false;
  }

  int cipher_len = data.size() + AES_BLOCK_SIZE;
  unsigned char *cipher = (unsigned char *)malloc(cipher_len);

  if (!EVP_EncryptUpdate(&ctx, cipher, &cipher_len,
                         (unsigned char *)data.constData(), data.size() + 1)) {
    qWarning("Encryption: Couldn't generate key.");
    return false;
  }
  int len = 0;
  if (!EVP_EncryptFinal_ex(&ctx, cipher + cipher_len, &len)) {
    qWarning("Encryption: Couldn't generate key.");
    return false;
  }
  out = QByteArray((const char *)cipher, cipher_len + len);
  free(cipher);
  if (!EVP_CIPHER_CTX_cleanup(&ctx)) {
    qWarning("Encryption: Couldn't generate key.");
    return false;
  }
  enc_data = "Salted__" + QByteArray((const char *)&salt, 8) + out;
  return true;
}

bool Crypt::Decrypt(const QByteArray &data, QByteArray &dec_data) {
  QByteArray out;
  unsigned char *key_data = (unsigned char *)pwd_.constData();
  int key_data_len = pwd_.size();

  unsigned char key[32], iv[32];
  int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(), (unsigned char *)&salt,
                         key_data, key_data_len, 1, key, iv);
  if (i != 32) {
    qWarning("Encryption: Key has wrong size\n");
    return false;
  }

  EVP_CIPHER_CTX ctx;
  EVP_CIPHER_CTX_init(&ctx);
  if (!EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
    qWarning("Decryption: Couldn't generate key.");
    return false;
  }

  QByteArray body = data.mid(16);
  int body_len = body.size();
  unsigned char *plain = (unsigned char *)malloc(body_len);
  if (!EVP_DecryptUpdate(&ctx, plain, &body_len,
                         (unsigned char *)body.constData(), body.size())) {
    qWarning("Decryption: Couldn't generate key.");
    return false;
  }
  int len = 0;
  if (!EVP_DecryptFinal_ex(&ctx, plain + body_len, &len)) {
    qWarning("Decryption: Couldn't generate key.");
    return false;
  }
  dec_data = QByteArray((const char *)plain, body_len + len - 1);
  free(plain);
  if (!EVP_CIPHER_CTX_cleanup(&ctx)) {
    qWarning("Decryption: Couldn't generate key.");
    return false;
  }
  return true;
}

void Crypt::SetPwd(QByteArray p) { pwd_ = p; }

QByteArray Crypt::GetPwd() const { return pwd_; }
