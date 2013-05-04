#include "stdafx.h"
#include "XmlHelper.h"

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

HRESULT XmlHelper::OpenXmlDocument(const wstring& strFilename, CComPtr<IXMLDOMDocument>& pXmlDocument, CComPtr<IXMLDOMElement>& pRootElement)
{
	HRESULT hr					= S_OK;
	VARIANT_BOOL bLoadSuccess	= 0; // FALSE

	pXmlDocument.Release();
	pRootElement.Release();
	
	hr = pXmlDocument.CoCreateInstance(__uuidof(DOMDocument));
	if (FAILED(hr) || (pXmlDocument.p == NULL)) return E_FAIL;

	hr = pXmlDocument->load(CComVariant(strFilename.c_str()), &bLoadSuccess);
	if (FAILED(hr) || (!bLoadSuccess)) return E_FAIL;

/*
	if (FAILED(hr) || (!bLoadSuccess))
	{
		if (strDefaultFilename.length() == 0) return wstring(L"");

		strXmlFilename = Helpers::GetModulePath(NULL) + strDefaultFilename;

/ *
		wchar_t szModuleFileName[MAX_PATH + 1];
		::ZeroMemory(szModuleFileName, (MAX_PATH+1)*sizeof(wchar_t));
		::GetModuleFileName(NULL, szModuleFileName, MAX_PATH);

		wstring strModuleFileName(szModuleFileName);
		wstring strDefaultOptionsFileName(strModuleFileName.substr(0, strModuleFileName.rfind(L'\\')+1));

		strDefaultOptionsFileName += strDefaultFilename;
* /

		hr = pXmlDocument->load(CComVariant(strXmlFilename.c_str()), &bLoadSuccess);
		if (FAILED(hr) || (!bLoadSuccess)) return wstring(L"");
	}
*/

	hr = pXmlDocument->get_documentElement(&pRootElement);
	if (FAILED(hr)) return E_FAIL;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

HRESULT XmlHelper::GetDomElement(const CComPtr<IXMLDOMElement>& pRootElement, const CComBSTR& bstrPath, CComPtr<IXMLDOMElement>& pElement)
{
	HRESULT					hr = S_OK;
	CComPtr<IXMLDOMNode>	pNode;
	
	if (pRootElement.p == NULL) return E_FAIL;

	hr = pRootElement->selectSingleNode(bstrPath, &pNode);
	if (hr != S_OK) return E_FAIL;

	return pNode.QueryInterface(&pElement);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

HRESULT XmlHelper::CreateDomElement(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, CComPtr<IXMLDOMElement>& pNewElement)
{
  HRESULT hr;
  CComPtr<IXMLDOMNode>     pNode;
  CComPtr<IXMLDOMElement>  pNewElement2;
  CComPtr<IXMLDOMDocument> pXmlDocument;
  
  hr = pElement->get_ownerDocument(&pXmlDocument);
  if( hr != S_OK )
    return hr;
  hr = pXmlDocument->createElement(bstrName, &pNewElement2);
  if( hr != S_OK )
    return hr;
  hr = pElement->appendChild(pNewElement2, &pNode);
  if( hr != S_OK )
    return hr;

  return pNode.QueryInterface(&pNewElement);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

HRESULT XmlHelper::AddTextNode(CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrText)
{
  HRESULT hr;
  CComPtr<IXMLDOMNode>     pNode;
  CComPtr<IXMLDOMText>     pXmlDomText;
  CComPtr<IXMLDOMDocument> pXmlDocument;

  hr = pElement->get_ownerDocument(&pXmlDocument);
  if( hr != S_OK )
    return hr;
  hr = pXmlDocument->createTextNode(bstrText, &pXmlDomText);
  if( hr != S_OK )
    return hr;
  hr = pElement->appendChild(pXmlDomText, &pNode);

  return hr;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

HRESULT XmlHelper::AddDomElementIfNotExist(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, CComPtr<IXMLDOMElement>& pNewElement)
{
  CComPtr<IXMLDOMNode> pNode;
  HRESULT hr = pElement->selectSingleNode(bstrName, &pNode);

  if( hr == S_OK )
    return pNode.QueryInterface(&pNewElement);
  else
    return XmlHelper::CreateDomElement(pElement, bstrName, pNewElement);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void XmlHelper::GetAttribute(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, DWORD& dwValue, DWORD dwDefaultValue)
{
	CComVariant	varValue;

	if (pElement->getAttribute(bstrName, &varValue) == S_OK)
	{
		dwValue = _wtol(varValue.bstrVal);
	}
	else
	{
		dwValue = dwDefaultValue;
	}
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void XmlHelper::GetAttribute(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, int& nValue, int nDefaultValue)
{
	CComVariant	varValue;

	if (pElement->getAttribute(bstrName, &varValue) == S_OK)
	{
		nValue = _wtol(varValue.bstrVal);
	}
	else
	{
		nValue = nDefaultValue;
	}
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void XmlHelper::GetAttribute(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, BYTE& byValue, BYTE byDefaultValue)
{
	CComVariant	varValue;

	if (pElement->getAttribute(bstrName, &varValue) == S_OK)
	{
		byValue = static_cast<BYTE>(_wtoi(varValue.bstrVal));
	}
	else
	{
		byValue = byDefaultValue;
	}
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void XmlHelper::GetAttribute(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, bool& bValue, bool bDefaultValue)
{
	CComVariant	varValue;

	if (pElement->getAttribute(bstrName, &varValue) == S_OK)
	{
		bValue = (_wtol(varValue.bstrVal) > 0);
	}
	else
	{
		bValue = bDefaultValue;
	}
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void XmlHelper::GetAttribute(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, wstring& strValue, const wstring& strDefaultValue)
{
	CComVariant	varValue;

	if (pElement->getAttribute(bstrName, &varValue) == S_OK)
	{
		strValue = varValue.bstrVal;
	}
	else
	{
		strValue = strDefaultValue;
	}
}

//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////

void XmlHelper::GetRGBAttribute(const CComPtr<IXMLDOMElement>& pElement, COLORREF& crValue, COLORREF crDefaultValue)
{
	DWORD r;
	DWORD g;
	DWORD b;

	GetAttribute(pElement, CComBSTR(L"r"), r, GetRValue(crDefaultValue));
	GetAttribute(pElement, CComBSTR(L"g"), g, GetGValue(crDefaultValue));
	GetAttribute(pElement, CComBSTR(L"b"), b, GetBValue(crDefaultValue));

	crValue = RGB(r, g, b);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void XmlHelper::SetAttribute(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, DWORD dwValue)
{
	CComVariant	varValue(boost::str(boost::wformat(L"%1%") % dwValue).c_str());

	pElement->setAttribute(bstrName, varValue);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void XmlHelper::SetAttribute(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, int nValue)
{
	CComVariant	varValue(boost::str(boost::wformat(L"%1%") % nValue).c_str());

	pElement->setAttribute(bstrName, varValue);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void XmlHelper::SetAttribute(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, BYTE byValue)
{
	CComVariant	varValue(boost::str(boost::wformat(L"%1%") % byValue).c_str());

	pElement->setAttribute(bstrName, varValue);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void XmlHelper::SetAttribute(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, bool bValue)
{
	CComVariant	varValue(bValue ? L"1" : L"0");

	pElement->setAttribute(bstrName, varValue);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void XmlHelper::SetAttribute(const CComPtr<IXMLDOMElement>& pElement, const CComBSTR& bstrName, const wstring& strValue)
{
	CComVariant	varValue(strValue.c_str());

	pElement->setAttribute(bstrName, varValue);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void XmlHelper::SetRGBAttribute(const CComPtr<IXMLDOMElement>& pElement, const COLORREF& crValue)
{
	SetAttribute(pElement, CComBSTR(L"r"), GetRValue(crValue));
	SetAttribute(pElement, CComBSTR(L"g"), GetGValue(crValue));
	SetAttribute(pElement, CComBSTR(L"b"), GetBValue(crValue));
}

//////////////////////////////////////////////////////////////////////////////

bool XmlHelper::LoadColors(const CComPtr<IXMLDOMElement>& pElement, COLORREF colors[16])
{
	for (DWORD i = 0; i < 16; ++i)
	{
		CComPtr<IXMLDOMElement>	pFontColorElement;

		if (FAILED(GetDomElement(pElement, CComBSTR(str(boost::wformat(L"colors/color[@id='%1%']") % i).c_str()), pFontColorElement))) return false;

		DWORD id;

		GetAttribute(pFontColorElement, CComBSTR(L"id"), id, i);
		if( id > 15 ) return false;
		GetRGBAttribute(pFontColorElement, colors[id], colors[i]);
	}
	return true;
}

void XmlHelper::SaveColors(CComPtr<IXMLDOMElement>& pElement, const COLORREF colors[16])
{
	CComPtr<IXMLDOMElement>	pFontColorsElement;

	if (FAILED(XmlHelper::AddDomElementIfNotExist(pElement, CComBSTR(L"colors"), pFontColorsElement))) return;

	for (DWORD i = 0; i < 16; ++i)
	{
		CComPtr<IXMLDOMElement>	pFontColorElement;

		if (FAILED(XmlHelper::GetDomElement(pFontColorsElement, CComBSTR(str(boost::wformat(L"color[@id='%1%']") % i).c_str()), pFontColorElement)))
		{
			XmlHelper::AddTextNode(pFontColorsElement, CComBSTR(L"\n\t\t\t\t"));
			if (FAILED(XmlHelper::CreateDomElement(pFontColorsElement, CComBSTR(L"color"), pFontColorElement))) continue;
			if( i == 15 )
				XmlHelper::AddTextNode(pFontColorsElement, CComBSTR(L"\n\t\t\t"));
			SetAttribute(pFontColorElement, CComBSTR(L"id"), i);
		}

		SetRGBAttribute(pFontColorElement, colors[i]);
	}
}
