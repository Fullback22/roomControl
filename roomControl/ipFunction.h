#pragma once
#include <qstring.h>
#include <qstringlist.h>

#include <VimbaCPP.h>

QString convertNumberToMac(const VmbInt64_t macAddress);

QString convertNumberToIpV4(const VmbInt64_t ipAddress);

VmbInt64_t convertMacToNumber(const QString& macAddress);

VmbInt64_t convertIpV4ToNumber(const QString& ipAddress);