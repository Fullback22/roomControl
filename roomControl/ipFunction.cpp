#include"ipFunction.h"

QString convertNumberToMac(const VmbInt64_t macAddress)
{
	QString sIPv4;
	sIPv4 = QString().sprintf("%02x:%02x:%02x:%02x:%02x:%02x",
		static_cast<unsigned int>(0xFF & (macAddress >> 40)),
		static_cast<unsigned int>(0xFF & (macAddress >> 32)),
		static_cast<unsigned int>(0xFF & (macAddress >> 24)),
		static_cast<unsigned int>(0xFF & (macAddress >> 16)),
		static_cast<unsigned int>(0xFF & (macAddress >> 8)),
		static_cast<unsigned int>(0xFF & macAddress));
	return sIPv4;
}

QString convertNumberToIpV4(const VmbInt64_t ipAddress)
{
	QString sIPv4;
	sIPv4 = QString::number(0xFF & (ipAddress >> 24)) + "."
		+ QString::number(0xFF & (ipAddress >> 16)) + "."
		+ QString::number(0xFF & (ipAddress >> 8)) + "."
		+ QString::number(0xFF & ipAddress);
	return sIPv4;
}

VmbInt64_t convertMacToNumber(const QString& macAddress)
{
	VmbInt64_t output = 0;
	QStringList macParts;
	VmbUint32_t temp = 0;
	bool check = false;

	macParts = macAddress.split(":");
	if (true == macParts.empty())
	{
		macParts = macAddress.split("-");
	}

	if (6 == macParts.size())
	{
		for (int i = 0; i < macParts.size(); ++i)
		{
			temp = macParts.at(i).toUInt(&check, 16);
			if (false == check)
			{
				break;
			}
			output = (output << 8) + temp;
		}
	}

	if (false == check)
	{
		output = -1;
	}

	return output;
}

VmbInt64_t convertIpV4ToNumber(const QString& ipAddress)
{
	VmbInt64_t output = 0;
	VmbInt64_t temp = 0;
	QStringList ipParts;
	bool check = false;

	ipParts = ipAddress.split(".");

	if (4 == ipParts.size())
	{
		for (int i = 0; i < ipParts.size(); ++i)
		{
			temp = ipParts.at(i).toUInt(&check, 10);
			if (false == check || temp > 255)
			{
				break;
			}
			output = (output << 8) + temp;
		}
	}

	if (false == check || temp > 255)
	{
		output = -1;
	}

	return output;
}