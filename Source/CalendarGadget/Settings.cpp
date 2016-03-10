/****************************************************************************
**
** Copyright (C) 2010-2016 B.D. Mihai.
**
** This file is part of CalendarGadget.
**
** CalendarGadget is free software: you can redistribute it and/or modify it
** under the terms of the GNU Lesser Public License as published by the Free
** Software Foundation, either version 3 of the License, or (at your option)
** any later version.
**
** CalendarGadget is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
** or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for
** more details.
**
** You should have received a copy of the GNU Lesser Public License along
** with CalendarGadget.  If not, see http://www.gnu.org/licenses/.
**
****************************************************************************/

#include "StdAfx.h"
#include "Settings.h"

//! a private key with exponent of one in a PRIVATEKEYBLOB format
static unsigned char PrivateKeyWithExponentOfOne[] =
{
  0x07, 0x02, 0x00, 0x00, 0x00, 0xA4, 0x00, 0x00,
  0x52, 0x53, 0x41, 0x32, 0x00, 0x02, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0xAB, 0xEF, 0xFA, 0xC6,
  0x7D, 0xE8, 0xDE, 0xFB, 0x68, 0x38, 0x09, 0x92,
  0xD9, 0x42, 0x7E, 0x6B, 0x89, 0x9E, 0x21, 0xD7,
  0x52, 0x1C, 0x99, 0x3C, 0x17, 0x48, 0x4E, 0x3A,
  0x44, 0x02, 0xF2, 0xFA, 0x74, 0x57, 0xDA, 0xE4,
  0xD3, 0xC0, 0x35, 0x67, 0xFA, 0x6E, 0xDF, 0x78,
  0x4C, 0x75, 0x35, 0x1C, 0xA0, 0x74, 0x49, 0xE3,
  0x20, 0x13, 0x71, 0x35, 0x65, 0xDF, 0x12, 0x20,
  0xF5, 0xF5, 0xF5, 0xC1, 0xED, 0x5C, 0x91, 0x36,
  0x75, 0xB0, 0xA9, 0x9C, 0x04, 0xDB, 0x0C, 0x8C,
  0xBF, 0x99, 0x75, 0x13, 0x7E, 0x87, 0x80, 0x4B,
  0x71, 0x94, 0xB8, 0x00, 0xA0, 0x7D, 0xB7, 0x53,
  0xDD, 0x20, 0x63, 0xEE, 0xF7, 0x83, 0x41, 0xFE,
  0x16, 0xA7, 0x6E, 0xDF, 0x21, 0x7D, 0x76, 0xC0,
  0x85, 0xD5, 0x65, 0x7F, 0x00, 0x23, 0x57, 0x45,
  0x52, 0x02, 0x9D, 0xEA, 0x69, 0xAC, 0x1F, 0xFD,
  0x3F, 0x8C, 0x4A, 0xD0,

  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x64, 0xD5, 0xAA, 0xB1,
  0xA6, 0x03, 0x18, 0x92, 0x03, 0xAA, 0x31, 0x2E,
  0x48, 0x4B, 0x65, 0x20, 0x99, 0xCD, 0xC6, 0x0C,
  0x15, 0x0C, 0xBF, 0x3E, 0xFF, 0x78, 0x95, 0x67,
  0xB1, 0x74, 0x5B, 0x60,

  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

//! the RC4 encryption algorithm key in a SIMPLEBLOB format (the key is not encrypted !!!)
static unsigned char RC4KeyBlock[] =
{
  // Header
  0x01, 0x02, 0x00, 0x00,        // BLOBHEADER bType, bVersion, reserved
  0x01, 0x68, 0x00, 0x00,        // BLOBHEADER aiKeyAlg: CALG_RC4
  0x00, 0xA4, 0x00, 0x00,        // algid used to encrypt blob: CALG_RSA_KEYX

  // Rest is a PKCS #1, type 2 encryption block:
  // For MS Base CP this is always 512 bits (64 bytes)

  // Key Material: 16 byte actual RC4 key goes in reverse
  'P', 'O', 'N', 'M', 'L', 'K', 'J', 'I', 'H', 'G', 'F', 'E', 'D', 'C', 'B', 'A',

  // Zero then Random non-zero padding...
  0x00, 0x3D, 0xB5, 0xE1,
  0x5B, 0x27, 0x13, 0x36,
  0x69, 0x9B, 0x56, 0xA9,
  0x52, 0x98, 0x5B, 0xA9,
  0x17, 0x24, 0x1D, 0x1A,
  0x2B, 0x9C, 0xE7, 0x35,
  0x3C, 0xC9, 0xD6, 0xE1,
  0xD7, 0x70, 0xCC, 0x70,
  0x94, 0x6B, 0x90, 0xD0,
  0x7E, 0x92, 0x2E, 0x5C,
  0x80, 0xDB, 0xE5, 0x2D,

  // ...Padding
  // Block type; reserved
  0x60, 0x75,
  0x02, 0x00,
};

//! The client id for google connection
static QString clientId = "";

//! The client secret for google connection
static QString clientSecret = "";

//! The encrypted client id for google connection
static QString encryptedClientId = "";

//! The encrypted client secret for google connection
static QString encryptedClientSecret = "";

/*!
Create a new instance of the Settings class.
*/
Settings::Settings(QString filePath)
  : QSettings(filePath, QSettings::IniFormat, 0)
{
  if (!QFile::exists(filePath))
    setDefault();
}

/*!
Clean up.
*/
Settings::~Settings()
{
}

/*!
This file is called in the constructor of the class in order to provide the
default setting for the application.
*/
void Settings::setDefault()
{
  setValue("Calendar/Position", QPoint(10,10));
  setValue("Calendar/Size", QSize(350,250));
  setValue("Google/RefreshToken", "");
}

void Settings::setPosition(const QPoint &newPosition)
{
  setValue("Calendar/Position", newPosition);
}

QPoint Settings::getPosition()
{
  return value("Calendar/Position").toPoint();
}

void Settings::setSize(const QSize &newSize)
{
  setValue("Calendar/Size", newSize);
}

QSize Settings::getSize()
{
  return value("Calendar/Size").toSize();
}

QString Settings::getRefreshToken()
{
  return decrypt(value("Google/RefreshToken").toString());
}

void Settings::setRefreshToken(const QString &newRefreshToken)
{
  setValue("Google/RefreshToken", encript(newRefreshToken));
}

QString Settings::getClientId()
{
  if (!clientId.isEmpty())
    return clientId;
  if (!encryptedClientId.isEmpty())
    return decrypt(encryptedClientId);

  return "";
}

QString Settings::getClientSecret()
{
  if (!clientSecret.isEmpty())
    return clientSecret;
  if (!encryptedClientSecret.isEmpty())
    return decrypt(encryptedClientSecret);

  return "";
}

/*!
This function encrypts a text using the RC4 algorithm using the Windows CryptoAPI.
For more details about the usage of the service please check the following article:
http://www.phdcc.com/cryptorc4.htm
*/
QString Settings::encript(const QString &text)
{
  unsigned long length = text.length();
  QString helperStr;

  // get a CSP handle
  HCRYPTPROV CryptoProv = NULL;
  if( !CryptAcquireContext(&CryptoProv, NULL, NULL, PROV_RSA_FULL, 0))
    if (!CryptAcquireContext(&CryptoProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
      if (!CryptAcquireContext(&CryptoProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
        return "";

  // import do-nothing exponent-of-one keypair
  HCRYPTKEY ExponentOfOneKey = NULL;
  CryptImportKey(CryptoProv, PrivateKeyWithExponentOfOne, sizeof(PrivateKeyWithExponentOfOne), 0, 0, &ExponentOfOneKey);

  // import known key from RC4 template
  HCRYPTKEY RC4Key = NULL;
  DWORD cbBlob = sizeof(RC4KeyBlock);
  CryptImportKey(CryptoProv, RC4KeyBlock, cbBlob, ExponentOfOneKey, 0, &RC4Key);

  // convert to data
  unsigned char *data = new unsigned char[length];

  for (int i = 0; i < length; i++)
  {
    data[i] = text[i].toLatin1();
  }

  // encrypt the data
  CryptEncrypt(RC4Key, 0, 1, 0, data, &length, length);

  // convert data to text
  for (int i = 0; i < length; i++)
  {
    helperStr += QString("%1").arg(data[i], 2, 16, QChar('0')).toUpper();
  }
  helperStr = helperStr.trimmed();

  delete[] data;

  // clean up
  CryptDestroyKey(RC4Key);
  CryptDestroyKey(ExponentOfOneKey);
  CryptReleaseContext(CryptoProv,0);

  return helperStr;
}

/*!
This function decrypts a text using the RC4 algorithm using the Windows CryptoAPI.
For more details about the usage of the service please check the following article:
http://www.phdcc.com/cryptorc4.htm
*/
QString Settings::decrypt(const QString &text)
{
  unsigned long length;
  QString helperStr;

  // get a CSP handle
  HCRYPTPROV CryptoProv = NULL;
  if( !CryptAcquireContext(&CryptoProv, NULL, NULL, PROV_RSA_FULL, 0))
    if (!CryptAcquireContext(&CryptoProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
      if (!CryptAcquireContext(&CryptoProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
        return "";

  // import do-nothing exponent-of-one keypair
  HCRYPTKEY ExponentOfOneKey = NULL;
  CryptImportKey(CryptoProv, PrivateKeyWithExponentOfOne, sizeof(PrivateKeyWithExponentOfOne), 0, 0, &ExponentOfOneKey);

  // import known key from RC4 template
  HCRYPTKEY RC4Key = NULL;
  DWORD cbBlob = sizeof(RC4KeyBlock);
  CryptImportKey(CryptoProv, RC4KeyBlock, cbBlob, ExponentOfOneKey, 0, &RC4Key);

  // convert to data
  helperStr = text;
  helperStr = helperStr.trimmed();

  length = helperStr.length()/2;
  unsigned char *data = new unsigned char[length];

  for (int i = 0; i < length; i++)
  {
    data[i] = helperStr.mid(i * 2, 2).toUInt(0, 16);
  }

  // decrypt the data
  CryptEncrypt(RC4Key, 0, 1, 0, data, &length, length);

  // convert numbers to text
  helperStr = "";
  for (int i = 0; i < length; i++)
  {
    helperStr += (char)data[i];
  }

  delete[] data;

  // clean up
  CryptDestroyKey(RC4Key);
  CryptDestroyKey(ExponentOfOneKey);
  CryptReleaseContext(CryptoProv,0);

  return helperStr;
}
