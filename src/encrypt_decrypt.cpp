/* This file was created by Kureii (Tomas Adamek)
 * Date created: 07. 02. 2024
 * This file is under the GPL-3 license
 */
//================================= Includes ===================================
#include "RainText/encrypt_decrypt.hpp"

#include "core/rain_text_core.h"

//================================= Namespace ==================================
namespace rain_text {

//================================= Public method ==============================
RecordItem EncryptDecrypt::DecryptRecordItem(EncryptedRecordItem &eItem,
                                             const std::vector<uint8_t> &key) {
  RecordItem item;
  auto RTCore = std::make_unique<rain_text_core::RainTextCore>(
      static_cast<uint16_t>(eItem.iterations), key, eItem.encryptedHeadline);
  std::vector<uint8_t> output;

  // Decrypt headline
  RTCore->Decrypt(output);
  item.headlineText = QString::fromUtf8(
      reinterpret_cast<const char *>(output.data()), output.size());

  // Decrypt username
  RTCore->SetText(eItem.encryptedUsername);
  RTCore->Decrypt(output);
  item.usernameText = QString::fromUtf8(
      reinterpret_cast<const char *>(output.data()), output.size());

  // Decrypt password
  RTCore->SetText(eItem.encryptedPassword);
  RTCore->Decrypt(output);
  item.passwordText = QString::fromUtf8(
      reinterpret_cast<const char *>(output.data()), output.size());

  return item;
}

EncryptedRecordItem EncryptDecrypt::EncryptRecordItem(
    RecordItem &item, const std::vector<uint8_t> &key, uint16_t iterations) {
  EncryptedRecordItem eItem;
  eItem.iterations = iterations;
  auto RTCore = std::make_unique<rain_text_core::RainTextCore>(
      iterations, key, QStringToUint8Vector(item.headlineText));

  // Encrypt headline
  RTCore->Encrypt(eItem.encryptedHeadline);

  // Encrypt username
  RTCore->SetText(QStringToUint8Vector(item.usernameText));
  RTCore->Encrypt(eItem.encryptedUsername);

  // Encrypt password
  RTCore->SetText(QStringToUint8Vector(item.passwordText));
  RTCore->Encrypt(eItem.encryptedPassword);

  return eItem;
}

std::vector<uint8_t> EncryptDecrypt::QStringToUint8Vector(
    const QString &input) {
  QByteArray utf8Bytes = input.toUtf8();
  std::vector<uint8_t> output(utf8Bytes.begin(), utf8Bytes.end());
  return output;
}
//================================= Testing method =============================
#ifdef ENABLE_TESTS

#endif

//================================= Private method =============================

//================================= End namespace ==============================
}  // namespace rain_text