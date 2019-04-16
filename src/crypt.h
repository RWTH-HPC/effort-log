/*
 * Copyright (c) 2015-2019 by IT Center, RWTH Aachen University
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

#pragma once

#include <QJsonDocument>

#include <openssl/evp.h>

//! Class to handle encryption and decryption of files
/*!
 * This class implements a 256-bit AES encryption and decryption of byte
 * streams.
 * \see Dialog
 */
class Crypt {
 public:
  //! The main constructor.
  Crypt();
  //! The main destructor.
  ~Crypt();
  //! Encrypts a byte stream
  /*!
   * This method encrypts a given byte stream with a 256-bit AES encryption.
   * \param[in] data The plain byte stream to be encrypted.
   * \param[in] enc_data The output byte stream after encryption.
   * \return True on success, false on failure
   */
  bool Encrypt(const QByteArray &data, QByteArray &enc_data);
  //! Decrypts a byte stream
  /*!
   * This method decrypts a given byte stream with a 256-bit AES encryption.
   * \param[in] data The encrypted byte stream to be decrypted.
   * \param[in] dec_data The output byte stream after decryption.
   * \return True on success, false on failure
   */
  bool Decrypt(const QByteArray &data, QByteArray &dec_data);
  //! Setter for pwd_
  /*!
   * \param[in] p The password for encryption/decryption
   */
  void SetPwd(QByteArray p);
  //! Getter for pwd_
  /*!
   * \return The password for encryption/decryption
   */
  QByteArray GetPwd() const;

 private:
  QByteArray pwd_;             /**< Holds the password of the byte stream*/
  EVP_CIPHER_CTX *encode_ctx_; /**< Holds the encryption cipher*/
  EVP_CIPHER_CTX *decode_ctx_; /**< Holds the decryption cipher*/
};
