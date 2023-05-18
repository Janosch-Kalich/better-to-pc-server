#ifndef TO_PC_SERVER_QR_H
#define TO_PC_SERVER_QR_H

#include <qrcodegen.hpp>
#include "Settings.h"
#include <Windows.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdlib.h>
#include <format>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

struct AdapterInfo
{
  std::string name;
  std::string desc;
  std::string addr;

  AdapterInfo()
  {
    this->name = "";
    this->addr = "";
  }
};

std::vector<AdapterInfo> get_ip_addresses();
static std::string to_svg_string(const qrcodegen::QrCode &qr, int border);

std::string gen_qr_code(uint16_t port, std::string password)
{
  Json::Value json;
  json["port"] = port;
  json["pwd"] = password;

  std::vector<AdapterInfo> info = get_ip_addresses();

  for (AdapterInfo el : info)
  {
    json["hosts"].append(el.addr);
  }

  Json::FastWriter writer;
  const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(writer.write(json).c_str(), qrcodegen::QrCode::Ecc::LOW);
  return to_svg_string(qr, 1).c_str();
}

//I DON'T REMOTELY KNOW WHAT ALL THIS DOES BUT IT WORKS
std::vector<AdapterInfo> get_ip_addresses()
{
  std::vector<AdapterInfo> info;

  PIP_ADAPTER_INFO pAdapterInfo;
  PIP_ADAPTER_INFO pAdapter = NULL;
  DWORD dwRetVal = 0;

  ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
  pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
  if (pAdapterInfo == NULL) {
    OutputDebugString("Error allocating memory needed to call GetAdaptersinfo\n");
    return std::vector<AdapterInfo>();
  }

  if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
    FREE(pAdapterInfo);
    pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
    if (pAdapterInfo == NULL) {
      OutputDebugString("Error allocating memory needed to call GetAdaptersinfo\n");
      return std::vector<AdapterInfo>();
    }
  }

  if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
  {
    pAdapter = pAdapterInfo;
    unsigned int j = 0;
    while (pAdapter)
    {
      info.push_back(AdapterInfo{});
    info[j].desc = std::string(pAdapter->Description);

      info[j].addr = std::string(pAdapter->IpAddressList.IpAddress.String);

      j++;
      pAdapter = pAdapter->Next;
    }
  }
  else
  {
    OutputDebugString(std::format("GetAdaptersInfo failed with error: {}\n", dwRetVal).c_str());
  }
  if (pAdapterInfo)
    FREE(pAdapterInfo);

  return info;
}

static std::string to_svg_string(const qrcodegen::QrCode &qr, int border) {
  if (border < 0)
    throw std::domain_error("Border must be non-negative");
  if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
    throw std::overflow_error("Border too large");

  std::ostringstream sb;
  sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
  sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
  sb << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2) << "\" stroke=\"none\">\n";
  sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
  sb << "\t<path d=\"";
  for (int y = 0; y < qr.getSize(); y++) {
    for (int x = 0; x < qr.getSize(); x++) {
      if (qr.getModule(x, y)) {
        if (x != 0 || y != 0)
          sb << " ";
        sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
      }
    }
  }
  sb << "\" fill=\"#000000\"/>\n";
  sb << "</svg>\n";
  return sb.str();
}

#endif //TO_PC_SERVER_QR_H